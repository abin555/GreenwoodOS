#include "ehci.h"
#include "stddef.h"

void *ehci_initialize(struct PCI_driver *usb){
    if(usb == NULL) return NULL;
    struct ehci_capregs *capregs = (void *) usb->BAR[0];
    print_serial(
        "[EHCI] Init\n"
        "CAPLENGTH: %d\n"
        "HCI Version: %d\n"
        "HCS Params: %d\n"
        "HCC Params: %d\n"
        "HCSP PORTROUTE: %d\n",
        capregs->caplen,
        capregs->hci_version,
        capregs->hcs_params,
        capregs->hcc_params,
        capregs->hcsp_portroute
    );
    return NULL;
}