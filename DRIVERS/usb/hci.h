#ifndef HCI_H
#define HCI_H

#include "drivers.h"
#include "console.h"
#include "pci.h"
#include "memory.h"

typedef struct __uhci_cmd_reg{
    uint16_t Run : 1;
    uint16_t HostConReset : 1;
    uint16_t GlobalReset : 1;
    uint16_t GlobalSuspent : 1;
    uint16_t GlobalResume : 1;
    uint16_t SoftwareDebug : 1;
    uint16_t Configure : 1;
    uint16_t Max_Packet : 1;
}uhci_cmd_reg;
typedef struct __uhci_status_reg{
    uint16_t interrupt : 1;
    uint16_t error_interrupt : 1;
    uint16_t resume_detected : 1;
    uint16_t system_error : 1;
    uint16_t process_error : 1;
    uint16_t halted : 1;
}uhci_status_reg;
typedef struct __uhci_intenb_reg{
    uint16_t timeout : 1;
    uint16_t resume : 1;
    uint16_t complete : 1;
    uint16_t short_packet : 1;
}uhci_intenb_reg;


struct __uhci_struct;
typedef struct __uhci_struct{
    uhci_cmd_reg USBCMD;
    uhci_status_reg USBSTS;
    uhci_intenb_reg USBINTR;
    unsigned short FRNUM;
    uint32_t *FRBASEADD;
    uint8_t SOFMOD;
    uint16_t PORTSC1;
    uint16_t PORTSC2;
}uhci_struct;

void init_uhci(int pciDriverID);
void init_ohci(int pciDriverID);
void init_ehci(int pciDriverID);
void init_xhci(int pciDriverID);

#endif