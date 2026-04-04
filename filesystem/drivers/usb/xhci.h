#ifndef XHCI_H
#define XHCI_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    uint8_t  cap_length;       // offset to operational regs
    uint8_t  reserved;
    uint16_t hci_version;
    uint32_t hcs_params1;      // #slots, #intrs, #ports
    uint32_t hcs_params2;
    uint32_t hcs_params3;
    uint32_t hcc_params1;
    uint32_t db_offset;        // doorbell array offset
    uint32_t rts_offset;       // runtime registers offset
    uint32_t hcc_params2;
} __attribute__((packed)) xhci_cap_regs_t;

typedef struct {
    uint32_t usb_cmd;
    uint32_t usb_sts;
    uint32_t page_size;
    uint8_t  reserved1[8];
    uint32_t dn_ctrl;
    uint64_t crcr;             // command ring control
    uint8_t  reserved2[16];
    uint64_t dcbaap;           // device context base address array pointer
    uint32_t config;           // max slots enabled
    // ... port registers follow at offset 0x400
} __attribute__((packed)) xhci_op_regs_t;

typedef struct {
    uint32_t portsc;    // Port Status and Control
    uint32_t portpmsc;
    uint32_t portli;
    uint32_t porthlpmc;
} xhci_port_regs_t;

/* ── PORTSC bit definitions ─────────────────────────────────────────── */

#define PORTSC_CCS      (1u <<  0)  /* Current Connect Status           */
#define PORTSC_PED      (1u <<  1)  /* Port Enabled/Disabled            */
#define PORTSC_OCA      (1u <<  3)  /* Over-Current Active              */
#define PORTSC_PR       (1u <<  4)  /* Port Reset                       */
#define PORTSC_PLS_MASK (0xFu << 5) /* Port Link State [8:5]            */
#define PORTSC_PP       (1u <<  9)  /* Port Power                       */
#define PORTSC_SPD_MASK (0xFu << 10)/* Port Speed [13:10]               */
#define PORTSC_PIC_MASK (0x3u << 14)/* Port Indicator Control [15:14]   */
#define PORTSC_LWS      (1u << 16)  /* Link Write Strobe                */
#define PORTSC_CSC      (1u << 17)  /* Connect Status Change (RW1C)     */
#define PORTSC_PEC      (1u << 18)  /* Port Enable/Disable Change       */
#define PORTSC_WRC      (1u << 19)  /* Warm Port Reset Change           */
#define PORTSC_OCC      (1u << 20)  /* Over-Current Change              */
#define PORTSC_PRC      (1u << 21)  /* Port Reset Change                */
#define PORTSC_PLC      (1u << 22)  /* Port Link State Change           */
#define PORTSC_CEC      (1u << 23)  /* Config Error Change              */
#define PORTSC_CAS      (1u << 24)  /* Cold Attach Status (USB3)        */
#define PORTSC_WCE      (1u << 25)  /* Wake on Connect Enable           */
#define PORTSC_WDE      (1u << 26)  /* Wake on Disconnect Enable        */
#define PORTSC_WOE      (1u << 27)  /* Wake on Over-Current Enable      */
#define PORTSC_DR       (1u << 30)  /* Device Removable (USB3)          */
#define PORTSC_WPR      (1u << 31)  /* Warm Port Reset (USB3)           */

/*
 * PORTSC change bits that must be cleared by writing 1 (RW1C).
 * Writing these bits back preserves other state while clearing
 * only the status-change flags.
 */
#define PORTSC_RW1C_BITS (PORTSC_CSC | PORTSC_PEC | PORTSC_WRC | \
                          PORTSC_OCC | PORTSC_PRC | PORTSC_PLC | PORTSC_CEC)

/* ── Port link states ───────────────────────────────────────────────── */

#define PLS_U0          0x0     /* Active (USB3) / On (USB2) */
#define PLS_U1          0x1
#define PLS_U2          0x2
#define PLS_U3          0x3     /* Suspended              */
#define PLS_DISABLED    0x4
#define PLS_RXDETECT    0x5     /* Awaiting device (USB3) */
#define PLS_INACTIVE    0x6
#define PLS_POLLING     0x7
#define PLS_RECOVERY    0x8
#define PLS_HOT_RESET   0x9
#define PLS_COMPLIANCE  0xA
#define PLS_TEST_MODE   0xB
#define PLS_RESUME      0xF

/* ── Port speed codes ───────────────────────────────────────────────── */

typedef enum {
    USB_SPEED_INVALID    = 0,
    USB_SPEED_FULL       = 1,   /*  12 Mb/s  (USB 1.1) */
    USB_SPEED_LOW        = 2,   /* 1.5 Mb/s  (USB 1.1) */
    USB_SPEED_HIGH       = 3,   /* 480 Mb/s  (USB 2.0) */
    USB_SPEED_SUPER      = 4,   /*   5 Gb/s  (USB 3.0) */
    USB_SPEED_SUPER_PLUS = 5,   /*  10 Gb/s  (USB 3.1) */
} usb_speed_t;

static const char *const speed_names[] = {
    [USB_SPEED_INVALID]    = "Invalid",
    [USB_SPEED_FULL]       = "Full-Speed (12 Mb/s)",
    [USB_SPEED_LOW]        = "Low-Speed (1.5 Mb/s)",
    [USB_SPEED_HIGH]       = "High-Speed (480 Mb/s)",
    [USB_SPEED_SUPER]      = "SuperSpeed (5 Gb/s)",
    [USB_SPEED_SUPER_PLUS] = "SuperSpeed+ (10 Gb/s)",
};

/* ── Device descriptor populated per port ───────────────────────────── */

typedef struct {
    uint8_t    port_number;     /* 1-based port index                    */
    bool       connected;       /* device physically present             */
    bool       enabled;         /* port is enabled by the controller     */
    bool       powered;         /* port has power                        */
    bool       overcurrent;     /* over-current condition active         */
    usb_speed_t speed;          /* negotiated speed                      */
    uint8_t    link_state;      /* raw PLS value                         */
    bool       is_usb3;         /* true if SuperSpeed port               */
} xhci_port_info_t;

void xhci_init(void *mmio_base);

uint8_t xhci_read_port_registers(void *mmio_base,
                                 xhci_port_info_t *ports_out,
                                 uint8_t           max_entries);

void xhci_print_port_info(const xhci_port_info_t *ports, uint8_t num_ports);

#endif