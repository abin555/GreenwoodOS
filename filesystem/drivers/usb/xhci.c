#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "xhci.h"
#include "pci.h"
#include "utils.h"

/* ── Helpers ────────────────────────────────────────────────────────── */

/*
 * Extract the Port Link State from a PORTSC value.
 */
static inline uint8_t portsc_link_state(uint32_t portsc)
{
    return (uint8_t)((portsc & PORTSC_PLS_MASK) >> 5);
}

/*
 * Extract the speed code from a PORTSC value.
 *
 * The speed field is only meaningful when CCS is set; on an empty port
 * the bits are undefined.
 */
static inline usb_speed_t portsc_speed(uint32_t portsc)
{
    uint8_t raw = (uint8_t)((portsc & PORTSC_SPD_MASK) >> 10);
    if (raw > USB_SPEED_SUPER_PLUS)
        return USB_SPEED_INVALID;
    return (usb_speed_t)raw;
}

/*
 * A SuperSpeed port carries USB 3.x devices.
 * Full/Low/High-Speed ports carry USB 1.x / 2.0 devices.
 */
static inline bool speed_is_usb3(usb_speed_t s)
{
    return (s == USB_SPEED_SUPER || s == USB_SPEED_SUPER_PLUS);
}

/*
 * Clear only the RW1C status-change bits in PORTSC without disturbing
 * the remaining readable/writable state.
 *
 * The PORTSC register contains a mix of field types:
 *   - Read-only bits  (CCS, PED, OCA, …)
 *   - RW bits         (PP, PIC, WCE, …)
 *   - RW1C bits       (CSC, PEC, OCC, PRC, PLC, CEC)
 *   - RW1S bits       (PR, WPR)
 *
 * Writing 1 to an RW1C bit clears it; writing 0 leaves it unchanged.
 * Writing 1 to an RW1S bit (PR/WPR) asserts the reset — so we must
 * never set those bits here.
 */
static void xhci_port_clear_change_bits(volatile xhci_port_regs_t *port)
{
    uint32_t portsc = port->portsc;

    /* Preserve everything, set only the RW1C bits that are currently
     * asserted.  Mask out PR and WPR so we don't accidentally trigger
     * a port reset. */
    portsc &= ~(PORTSC_PR | PORTSC_WPR);
    portsc |=  PORTSC_RW1C_BITS;

    port->portsc = portsc;
}

/* ── Core detection function ────────────────────────────────────────── */

/*
 * xhci_read_port_registers
 *
 * Reads the port status registers for every port exposed by the xHCI
 * controller and fills in @ports_out[0..num_ports-1].  The caller must
 * provide a buffer with at least (max_ports_in_cap) entries; typically
 * you allocate this after reading hcs_params1 at controller init time.
 *
 * @mmio_base   - Virtual address of the xHCI MMIO region (BAR0 mapped).
 * @ports_out   - Caller-allocated array of xhci_port_info_t.
 * @max_entries - Number of entries in @ports_out.
 *
 * Returns the number of ports found (≤ max_entries), or 0 on error.
 */
uint8_t xhci_read_port_registers(void *mmio_base,
                                 xhci_port_info_t *ports_out,
                                 uint8_t           max_entries)
{
    if (!mmio_base || !ports_out || max_entries == 0)
        return 0;

    volatile xhci_cap_regs_t *cap =
        (volatile xhci_cap_regs_t *)mmio_base;

    /* Port register array begins at operational base + 0x400. */
    void * op_base = mmio_base + cap->cap_length;
    volatile xhci_port_regs_t *ports =
        (volatile xhci_port_regs_t *)(op_base + 0x400);

    /* Number of ports is in bits [31:24] of HCSPARAMS1. */
    uint8_t num_ports = (uint8_t)((cap->hcs_params1 >> 24) & 0xFF);
    if (num_ports > max_entries)
        num_ports = max_entries;

    for (uint8_t i = 0; i < num_ports; i++) {
        /*
         * Take a single snapshot of PORTSC.  Reading the register
         * multiple times risks observing torn state if the hardware
         * updates it concurrently (e.g., a device hot-plug event).
         */
        uint32_t portsc = ports[i].portsc;

        xhci_port_info_t *info = &ports_out[i];
        info->port_number = (uint8_t)(i + 1);   /* 1-based */
        info->connected   = (portsc & PORTSC_CCS) != 0;
        info->enabled     = (portsc & PORTSC_PED) != 0;
        info->powered     = (portsc & PORTSC_PP)  != 0;
        info->overcurrent = (portsc & PORTSC_OCA) != 0;
        info->link_state  = portsc_link_state(portsc);

        /*
         * Speed and USB generation.
         *
         * Only decode speed when a device is actually connected;
         * the speed field is undefined on empty ports.
         */
        if (info->connected) {
            info->speed   = portsc_speed(portsc);
            info->is_usb3 = speed_is_usb3(info->speed);
        } else {
            info->speed   = USB_SPEED_INVALID;
            info->is_usb3 = false;
        }

        /*
         * Clear status-change bits so they don't linger and cause
         * the controller to fire stale events later.  This is a
         * write-back of the snapshot with RW1C bits set — it does
         * NOT affect CCS or PED.
         */
        xhci_port_clear_change_bits(&ports[i]);
    }

    return num_ports;
}

/* ── Pretty-printer ─────────────────────────────────────────────────── */

/*
 * Converts the raw PLS value to a short human-readable string for
 * diagnostics/logging.
 */
static const char *link_state_name(uint8_t pls)
{
    switch (pls) {
        case PLS_U0:         return "U0 (active)";
        case PLS_U1:         return "U1";
        case PLS_U2:         return "U2";
        case PLS_U3:         return "U3 (suspended)";
        case PLS_DISABLED:   return "Disabled";
        case PLS_RXDETECT:   return "RxDetect";
        case PLS_INACTIVE:   return "Inactive";
        case PLS_POLLING:    return "Polling";
        case PLS_RECOVERY:   return "Recovery";
        case PLS_HOT_RESET:  return "Hot Reset";
        case PLS_COMPLIANCE: return "Compliance";
        case PLS_TEST_MODE:  return "Test Mode";
        case PLS_RESUME:     return "Resume";
        default:             return "Unknown";
    }
}

void xhci_print_port_info(const xhci_port_info_t *ports, uint8_t num_ports)
{
    printf("xHCI port scan: %d port(s) found\n", num_ports);
    printf("%s %s %s %s %s %s %s\n",
           "Port", "Connected", "Enabled", "Powered",
           "Link State", "Speed", "USB Gen");

    for (uint8_t i = 0; i < num_ports; i++) {
        const xhci_port_info_t *p = &ports[i];
        const char *speed = (p->speed <= USB_SPEED_SUPER_PLUS)
                            ? speed_names[p->speed] : "Unknown";
        printf("%d %s %s %s %s %s %s\n",
               p->port_number,
               p->connected   ? "YES" : "no",
               p->enabled     ? "yes" : "no",
               p->powered     ? "yes" : "no",
               link_state_name(p->link_state),
               p->connected   ? speed : "-",
               p->connected   ? (p->is_usb3 ? "USB 3.x" : "USB 2.0/1.x") : "-");
    }
}

void xhci_init(void *mmio_base) {
    xhci_cap_regs_t *cap = (xhci_cap_regs_t *)mmio_base;
    xhci_op_regs_t  *op  = (xhci_op_regs_t *)(mmio_base + cap->cap_length);

    // 1. Stop the controller
    op->usb_cmd &= ~(1 << 0); // clear Run/Stop
    while (!(op->usb_sts & (1 << 0))); // wait for HCHalted

    // 2. Reset
    op->usb_cmd |= (1 << 1); // set HCRST
    while (op->usb_cmd & (1 << 1)); // wait for reset to complete

    // 3. Configure max slots
    uint8_t max_slots = cap->hcs_params1 & 0xFF;
    op->config = max_slots;

    // 4. Set up Device Context Base Address Array (DCBAA)
    uint64_t *dcbaa = allocate_aligned(sizeof(uint64_t) * (max_slots + 1), 64);
    memset(dcbaa, 0, sizeof(uint64_t) * (max_slots + 1));
    op->dcbaap = (uint32_t)dcbaa;

    // 5. Set up Command Ring (CRCR) — 64-byte aligned
    // ... (allocate a ring buffer, set CRCR)

    // 6. Start the controller
    op->usb_cmd |= (1 << 0); // Run/Stop = 1
    while (op->usb_sts & (1 << 0)); // wait until no longer halted
}