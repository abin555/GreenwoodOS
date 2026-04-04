#include <stdio.h>
#include "pci.h"
#include "xhci.h"

int main(int argc, char **argv){
    FILE *pci_file = pci_open_devices();
    struct PCI_device *usbdev = pci_getDev(pci_file, 0x0C03, 0x30);//XHCI controller
    if(usbdev == NULL) return 1;
    printf("USB XHCI PCI dev @ 0x%x\n", usbdev);
    for(int i = 0; i < 6; i++){
        printf("BAR[%d] -> 0x%x\n", i, usbdev->BAR[i]);
    }

    void *mmio_base = (void *) usbdev->BAR[0];

    volatile xhci_cap_regs_t *cap = (volatile xhci_cap_regs_t *) mmio_base;
    uint8_t max_ports = (cap->hcs_params1 >> 24) & 0xFF;
    
    xhci_port_info_t ports[max_ports];   // or heap-allocate
    uint8_t found = xhci_read_port_registers(mmio_base, ports, max_ports);
    xhci_print_port_info(ports, found);

    return 0;
}