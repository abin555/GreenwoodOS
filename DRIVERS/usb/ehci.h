#ifndef USB_EHCI_H
#define USB_EHCI_H

#include "pci.h"
#include "usb.h"
#include "console.h"
#include "memory.h"

struct ehci_HCSPARAMS{
    uint32_t NPORTS : 4;//Number of Ports
    uint32_t PPC : 1;//Port Power Control
    uint32_t Reserved1 : 2;
    uint32_t Port_Routing_Rules : 1;
    uint32_t N_PCC : 4;//Number Ports per Companion Controller
    uint32_t N_CC : 4;//Number of Companion Controllers
    uint32_t P_INDICATOR : 1; //Port Indicators
    uint32_t Reserved2 : 3;
    uint32_t Debug_Port_Number : 4;
    uint32_t Reserved3 : 8;
};

struct ehci_HCCPARAMS{
    uint32_t Addr_Cap : 1;//64 bit addressing capability
    uint32_t Prog_Frame_LST_FG : 1;//Programmable Frame list Flag
    uint32_t Async_Sch_Park_Capability : 1;//Asynchronous Schedule Park Capability
    uint32_t Reserved1 : 1;
    uint32_t Isochronous_Scheduling_Threshold : 4;//Isochronous Scheduling Threshold
    uint32_t EECP : 8;//EHCI Extended Capabilities Pointer
    uint32_t Reserved : 16;
};

struct ehci_HCSP_PORTROUTE{
    uint64_t Port0_Route : 4;
    uint64_t Port1_Route : 4;
    uint64_t Port2_Route : 4;
    uint64_t Port3_Route : 4;
    uint64_t Port4_Route : 4;
    uint64_t Port5_Route : 4;
    uint64_t Port6_Route : 4;
    uint64_t Port7_Route : 4;
    uint64_t Port8_Route : 4;
    uint64_t Port9_Route : 4;
    uint64_t Port10_Route : 4;
    uint64_t Port11_Route : 4;
    uint64_t Port12_Route : 4;
    uint64_t Port13_Route : 4;
    uint64_t Port14_Route : 4;
    uint64_t Port15_Route : 4;
    uint64_t Reserved : 4;
};

struct ehci_capability_registers{
    uint8_t CAPLENGTH; //Offset to the Operational Registers
    uint8_t Reserved; 
    uint16_t HCIVERSION;//Interface Version Number
    struct ehci_HCSPARAMS HCSPARAMS;//Structural Parameters
    struct ehci_HCCPARAMS HCCPARAMS;//Capability Parameters
    struct ehci_HCSP_PORTROUTE HCSP_PORTROUTE;//Companion port route description
};

struct ehci_USBCMD{//USB Command
    uint32_t RS : 1; //Run / Stop
    uint32_t HCRESET : 1;//Host Controller Reset
    /*
    00b 1024 elements at 4096 bytes, default
    01b 512 elements at 2048 bytes
    10b 256 elements at 1024 bytes
    11b Reserved (32 elements in EHCI ver 1.1)
    */
    uint32_t Frame_List_Size : 2;
    uint32_t Periodic_Schedule_Enable : 1;
    uint32_t Asynch_Schedule_Enable : 1;
    uint32_t Interrupt_on_Async_Advance_Doorbell : 1;
    uint32_t Light_Host_Controller_Reset : 1;
    uint32_t Async_Schedule_Park_Mode_Count : 2;
    uint32_t Reserved1 : 1;
    uint32_t Async_Schedule_Park_Mode_Enable : 1;
    uint32_t Reserved2 : 4;
    /*
    0x00 Reserved
    0x01 1 micro-frame
    0x02 2 micro-frames
    0x04 4 micro-frames
    0x08 8 micro-frames (default) (1mS)
    0x10 16 micro-frames (2mS)
    0x20 32 micro-frames (4ms)
    0x40 64 micro-frames (8ms)
    */
    uint32_t Interrupt_Threshold_Control : 8;
    uint32_t Reserved3 : 8;
};

struct ehci_USBSTS{//USB Status
    uint32_t USBINT : 1; //USB Interrupt
    uint32_t USBERRINT : 1;//USB Error Interrupt
    uint32_t Port_Change_Detect : 1;
    uint32_t Frame_List_Rollover : 1;
    uint32_t Host_System_Error : 1;
    uint32_t Interrupt_on_Async_Advance : 1;
    uint32_t Reserved1 : 6;
    uint32_t HCHalted : 1;
    uint32_t Reclamation : 1;
    uint32_t Periodic_Schedule_Status : 1;
    uint32_t Async_Schedule_Status : 1;
    uint32_t Reserved2 : 16;
};

struct ehci_USBINTR{//USB Interrupt Enable
    uint32_t USB_Interrupt_Enable : 1;
    uint32_t USB_Err_Interrupt_Enable : 1;
    uint32_t Port_Change_Interrupt_Enable : 1;
    uint32_t Frame_List_Rollover_Enable : 1;
    uint32_t Host_System_Error_Enable : 1;
    uint32_t Interrupt_on_Async_Advance_Enable : 1;
    uint32_t Reserved : 26;
};

struct ehci_FRINDEX{//USB Frame Index
    uint32_t Frame_Index : 14;
    uint32_t Reserved : 18;
};

struct ehci_CTRLDSSEGMENT{//4G Segment Selector
    uint32_t Control_Data_Structure_Segment;
};

struct ehci_PERIODICLISTBASE{//Frame List Base Address
    uint32_t Reserved : 12;
    uint32_t Base_Addr : 20;
};

struct ehci_ASYNCLISTADDR{//Next Asynchronous List Address
    uint32_t Reserved : 5;
    uint32_t Base_Addr : 27;
};

struct ehci_CONFIGFLAG{//Configured Flag Register
    uint32_t Configure_Flag : 1;
    uint32_t Reserved : 31;
};

struct ehci_PORTSC{//Port Status / Control
    uint32_t Current_Connect_Status : 1;
    uint32_t Connect_Status_Change : 1;
    uint32_t Port_Enabled_Disabled : 1;
    uint32_t Port_Enable_Disable_Change : 1;
    uint32_t OverCurrent_Active : 1;
    uint32_t OverCurrent_Change : 1;
    uint32_t Force_Port_Resume : 1;
    uint32_t Suspend : 1;
    uint32_t Port_Reset : 1;
    uint32_t Reserved : 1;
    uint32_t Line_Status : 2;
    uint32_t Port_Power : 1;
    uint32_t Port_Owner : 1;
    uint32_t Port_Indicator_Control : 2;
    uint32_t Port_Test_Control : 4;
    uint32_t Wake_on_Connect_Enable : 1;
    uint32_t Wake_on_Disconnect_Enable : 1;
    uint32_t Wake_on_Over_Current_Enable : 1;
    uint32_t Reserved2 : 9;
};

enum ehci_OPER_Offsets{
    EHCI_OPS_USBCMD = 0,
    EHCI_OPS_USBSTS = 4,
    EHCI_OPS_USBINTR = 8,
    EHCI_OPS_FRINDEX = 12,
    EHCI_OPS_CTRLDSSEGMENT = 16,
    EHCI_OPS_PERIODICLISTBASE = 20,
    EHCI_OPS_ASYNCLISTADDR = 24,
    EHCI_OPS_CONFIGFLAG = (sizeof(uint32_t) * 9) + 24 + 4
};

struct ehci_operational_registers{
    struct ehci_USBCMD USBCMD;//USB Command
    struct ehci_USBSTS USBSTS;//USB Status
    struct ehci_USBINTR USBINTR;//Usb Interrupt
    struct ehci_FRINDEX FRINDEX;//Frame Index
    struct ehci_CTRLDSSEGMENT CTRLDSSEGMENT;//Control DS Segment
    struct ehci_PERIODICLISTBASE PERIODICLISTBASE;
    struct ehci_ASYNCLISTADDR ASYNCLISTADDR;
    uint32_t Reserved[9];
    struct ehci_CONFIGFLAG CONFIGFLAG;
    struct ehci_PORTSC PORTSC[];
};

void ehci_init(int PCI_Driver_ID);
struct ehci_PORTSC ehci_read_port(int PCI_Driver_ID, int portnum);
bool ehci_detect_root(int PCI_Driver_ID);
void ehci_reset_port(int PCI_Driver_ID, int port);
void ehci_write_oper_reg(uint32_t base, uint32_t offset, uint32_t value);
uint32_t ehci_read_oper_reg(uint32_t base, uint32_t offset);
void ehci_write_cap_reg(uint32_t base, uint32_t offset, uint32_t value);
uint32_t ehci_read_cap_reg(uint32_t base, uint32_t offset);
#endif