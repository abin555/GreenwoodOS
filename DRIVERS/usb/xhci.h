#ifndef XHCI_CONT_H
#define XHCI_CONT_H

#include "pci.h"
#include "usb.h"
#include "console.h"
#include "memory.h"

typedef struct xhci_HCSPARAMS1{
    uint32_t MaxSlots : 8; //Number of device slots
    uint32_t MaxIntrs : 11;//Number of interrupters
    uint32_t Reserved : 5;
    uint32_t MaxPorts : 8;//Number of Ports
}xhci_HCSPARAMS1;

typedef struct xhci_HCSPARAMS2{
    uint32_t IST : 4;//Isochronous Scheduling Threshold
    uint32_t ERST_Max : 4;//Event Ring Segment Table Max
    uint32_t Reserved : 13;
    uint32_t Max_Scratchpad_Buffs_H5 : 5;//Max Scratchpad Buffer (High 5)
    uint32_t Scratchpad_Restore : 1;//Scratchpad Restore
    uint32_t Max_Scratchpad_Buffs_L5 : 5;//Max Scrachpad Buffer (Low 5)
} xhci_HCSPARAMS2;

typedef struct xhci_HCSPARAMS3{
    uint32_t U1_Device_Exit_Latency : 8;//U1 Device Exit Latency
    uint32_t Reserved : 8;
    uint32_t U2_Device_Exit_Latency : 16;//U2 Device Exit Latency
} xhci_HCSPARAMS3;

typedef struct xhci_HCCPARAMS1{
    uint32_t AC64 : 1;//64 Bit addressing capability
    uint32_t BNC : 1;//BW Negotiation Capability
    uint32_t CSZ : 1;//Context Size
    uint32_t PPC : 1;//Port Power Control
    uint32_t PIND : 1;//Port Indicator
    uint32_t LHRC : 1;//Port Indicators
    uint32_t LTC : 1;//Light HC Reset Capability
    uint32_t NSS : 1;//Latency Tolerance Messaging Capability
    uint32_t PAE : 1;//No Secondary SID Support
    uint32_t SPC : 1;//Parse All Event Data
    uint32_t SEC : 1;//Stopped - Short Packet Capability
    uint32_t CFC : 1;//Continuous Frame ID Capability
    uint32_t MaxPSASize : 4;//Maximum Primary Stream Array Size
    uint32_t xECP : 16;//xHCI Extended Capabilities Pointer
} xhci_HCCPARAMS1;

typedef struct xhci_DBOFF{
    uint32_t Reserved : 2;
    uint32_t DBOFF : 30; //Doorbell Array Offset
} xhci_DBOFF;

typedef struct xhci_RTSOFF{
    uint32_t Reserved : 4;
    uint32_t RTSOFF : 28; //Runtime Register Space Offset
} xhci_RTSOFF;

typedef struct xhci_HCCPARAMS2{
    uint32_t U3C : 1; //U3 Entry Capability
    uint32_t CMC : 1; //ConfigEP Command Max Exit Latency too Large
    uint32_t FSC : 1; //Force Save Context Capability
    uint32_t CTC : 1; //Compliance Transition Capability
    uint32_t LEC : 1; //Large ESIT Payload Capability
    uint32_t CIC : 1; //Configuration Information Capability
    uint32_t Reserved : 26;
} xhci_HCCPARAMS2;

typedef struct xhci_capability_regs{
    uint8_t CAPLENGTH;//Capability Register Length
    uint8_t Reserved;
    uint16_t HCIVersion;//Version
    xhci_HCSPARAMS1 HCSPARAMS1;
    xhci_HCSPARAMS2 HCSPARAMS2;
    xhci_HCSPARAMS3 HCSPARAMS3;
    xhci_HCCPARAMS1 HCCPARAMS1;
    xhci_DBOFF DBOFF;
    xhci_RTSOFF RTSOFF;
    xhci_HCCPARAMS2 HCCPARAMS2;
} xhci_capability_regs;

typedef struct xhci_extended_capabilities{
    uint8_t ID;
    uint8_t Offset;
}xhci_extended_capabilities;

typedef struct xhci_usb_command{
    uint32_t RS : 1; //Run/Stop
    uint32_t HCRST : 1; //Host Controller Reset
    uint32_t INTE : 1; //Interrupter Enable
    uint32_t HSEE : 1; //Host System Error Enable
    uint32_t Reserve1 : 3;
    uint32_t LHCRST : 1; //Light Host Controller Reset
    uint32_t CSS : 1; //Controller Save State
    uint32_t CRS : 1; //Controller Restore State
    uint32_t EWE : 1; //Enable Wrap Event
    uint32_t UE3S : 1; //Enable U3 MFINDEX Stop
    uint32_t SPE : 1; //Stopped Short Packet Enable
    uint32_t CME : 1; //CEM Enable
    uint32_t Reserve2 : 18;
}xhci_usb_command;

typedef struct xhci_usb_status{
    uint32_t HCHalted : 1; //HCHalted
    uint32_t Reserved1 : 1;
    uint32_t HSE : 2; //Host System Error
    uint32_t EINT : 1; //Event Interrupt
    uint32_t PCD : 1; //Port Change Detected
    uint32_t Reserved2 : 3;
    uint32_t SSS : 1; //Save State Status
    uint32_t RSS : 1; //Restore State Status
    uint32_t SRE : 1; //Save/Restore Error
    uint32_t CNR : 1; //Controller Not Ready
    uint32_t HCE : 1; //Host Controller Error
    uint32_t Reserved3 : 19;
} xhci_usb_status;

typedef struct xhci_pagesize{
    uint32_t Reserved : 16;
    uint32_t PageSize : 16;
} xhci_pagesize;

typedef struct xhci_DNCTRL{
    uint16_t Notification_Enable;
    uint16_t Reserved; 
} xhci_DNCTRL;

typedef struct xhci_CRCR{
    uint64_t RCS : 1; //Ring Cycle State
    uint64_t CS : 1; //Command Stop
    uint64_t CA : 1; //Command Abort
    uint64_t  CRR : 1; //Command Ring Running
    uint64_t Reserved : 2;
    uint64_t Command_Ring_Pointer : 58; //Command Ring Pointer
} xhci_CRCR;

typedef struct xhci_DCBAAP{
    uint64_t Reserved : 6;
    uint64_t BCBAAR : 58;//Device Context Base Address Array Pointer
} xhci_DCBAAP;

typedef struct xhci_CONFIG{
    uint32_t MaxSlotsEn : 8; //MAx Device Slots Enable
    uint32_t U3E : 1; //U3 Entry Enable
    uint32_t CIE : 1; //Configuration Information Enable
    uint32_t Reserved : 22;
} xhci_CONFIG;

typedef struct xhci_operational_regs{
    xhci_usb_command usb_cmd;
    xhci_usb_status usb_status;
    xhci_pagesize pagesize;
    uint8_t Reserved1;
    xhci_DNCTRL Device_Notification_Control;
    xhci_CRCR Command_Ring_Control;
    uint16_t Reserved2;
    xhci_DCBAAP DCBAAR;
    xhci_CONFIG Configure;
}xhci_operational_regs;

typedef struct xhci_PORTSC{
    uint32_t Current_Connect_Status : 1;
    uint32_t Port_Enable_Disable : 1;
    uint32_t Reserved1 : 1;
    uint32_t Over_Current_Active : 1;
    uint32_t Port_Reset : 1;
    uint32_t Port_Link_State : 4;
    uint32_t Port_Power : 1;
    uint32_t Port_Speed : 4;
    uint32_t Port_Indicator_Control : 2;
    uint32_t Port_Link_State_write_Strobe : 1;
    uint32_t Connect_Status_Change : 1;
    uint32_t Port_Enable_Disable_Change : 1;
    uint32_t Warm_Port_Reset_Change : 1;
    uint32_t Over_Current_Change : 1;
    uint32_t Port_Reset_Change : 1;
    uint32_t Port_Line_State_Change : 1;
    uint32_t Port_Config_Error_Change : 1;
    uint32_t Cold_Attach_Status : 1;
    uint32_t Wake_on_Connect_Enable : 1;
    uint32_t Wake_on_Disconnect_Enable : 1;
    uint32_t Wake_on_Over_current_Enable : 1;
    uint32_t Reserved2 : 2;
    uint32_t Device_Removable : 1;
    uint32_t Warm_Port_Reset : 1;
} xhci_PORTSC;

typedef struct xhci_USB2_PORTPMSC{
    uint32_t L1_Status : 3;
    uint32_t Remote_Wake_Enable : 1;
    uint32_t Host_Initiated_Resume_Duration : 4;
    uint32_t L1_Device_Slot : 8;
    uint32_t Hardware_LPM_Enable : 1;
    uint32_t Reserved : 11;
    uint32_t Port_Test_Control : 4;
} xhci_USB2_PORTPMSC;

typedef struct xhci_USB3_PORTPMSC{
    uint32_t U1_Timeout : 8;
    uint32_t U2_Timeout : 8;
    uint32_t Force_Link_PM_Accept : 1;
    uint32_t Reserved : 16;
} xhci_USB3_PORTPMSC;

typedef struct xhci_USB3_PORTLI{
    uint32_t Link_Error_Count : 16;
    uint32_t Rx_Lane_Count : 4;
    uint32_t Tx_Lane_Count : 4;
    uint32_t Reserved : 8;
} xhci_USB3_PORTLI;

typedef struct xhci_USB2_PORTHLPMC{
    uint32_t HIRDM : 2;
    uint32_t L1_Timeout : 8;
    uint32_t BESLD : 4;
    uint32_t Reserved : 18;
} xhci_USB2_PORTHLPMC;

typedef struct xhci_Port_Register_Set{
    xhci_PORTSC PORTSC;
    uint32_t PORTPMSC;
    uint32_t PORTLI;
    uint32_t PORTHLPMC;
} xhci_Port_Register_Set;

typedef struct xhci_interrupter_management_register{
    uint32_t Interrupt_Pending : 1;
    uint32_t Interrupt_Enable : 1;
    uint32_t Reserved : 30;
} xhci_interrupter_management_register;

typedef struct xhci_interrupter_moderation_register{
    uint32_t Interrupt_Interval : 16;
    uint32_t Interrupt_Counter : 16;
} xhci_interrupter_moderation_register;

typedef struct xhci_event_ring_seg_tbl_sz{
    uint32_t Seg_Tbl_Size : 16;
    uint32_t Reserved : 16;
} xhci_event_ring_seg_tbl_sz;

typedef struct xhci_event_ring_seg_tbl_BA{
    uint64_t Reserved : 6;
    uint64_t Seg_tbl_BA : 58;
} xhci_event_ring_seg_tbl_BA;

typedef struct xhci_event_ring_dequeue_ptr{
    uint64_t Dequeue_ERST_Seg_index : 3;
    uint64_t Event_Handler_Busy : 1;
    uint64_t Event_Ring_Dequeue_PTR : 60;
} xhci_event_ring_dequeue_ptr;

typedef struct xhci_interrupter_register_set{
    xhci_interrupter_management_register INT_MNG_REG;
    xhci_interrupter_moderation_register INT_MOD;
    xhci_event_ring_seg_tbl_sz EVNT_RING_TBL_SZ;
    uint32_t Reserved;
    xhci_event_ring_seg_tbl_BA EVNT_RING_TBL_BA;
    xhci_event_ring_dequeue_ptr EVNT_RING_DQ_PTR;
} xhci_interrupter_register_set;

typedef struct xhci_HOST_Runtime_Reg{
    uint32_t Microframe_Index_Register;
    uint32_t reserve1;
    uint32_t reserve2;
    uint32_t reserve3;
    uint32_t reserve4;
    uint32_t reserve5;
    uint32_t reserve6;
    uint32_t reserve7;
    xhci_interrupter_register_set interrupter_register_sets[];
} xhci_HOST_Runtime_Reg;

typedef struct xhci_command_doorbell_reg{
    uint32_t Target : 8;
    uint32_t Reserved : 24;
} xhci_command_doorbell_reg;

typedef struct xhci_doorbell_reg{
    uint32_t Target : 8;
    uint32_t Reserved : 8;
    uint32_t Doorbell_Stream_ID : 16;
} xhci_doorbell_reg;

enum Capability_Reg_Members{
    CAPLENGTH = 1,
    HCIVERSION = 2,
    HCSPARAMS1 = 3,
    HCSPARAMS2 = 4,
    HCSPARAMS3 = 5,
    HCCPARAMS1 = 6,
    DBOFF = 7,
    RTSOFF = 8,
    HCCPARAMS2 = 9,
    VTIOSOFF = 10
};

uint32_t xhci_get_capability_register(uint32_t BaseAddr, int component);
uint32_t xhci_get_operational_register(uint32_t BaseAddr, int offset);
void xhci_write_operational_register(uint32_t BaseAddr, int offset, uint32_t value);
void xhci_init(uint32_t base_address, uint8_t irq);
bool det_xhci_root(uint32_t BaseAddr);
#endif