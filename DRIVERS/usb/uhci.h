#ifndef UHCI_H
#define UHCI_H

#include "pci.h"
#include "usb.h"
#include "drivers.h"
#include "console.h"
#include "memory.h"

struct uhci_cmd{ __attribute__((packed))
    uint16_t RunStop : 1;
    uint16_t HCRESET : 1;//Host Controller Reset
    uint16_t GRESET : 1;//Global Reset
    uint16_t EGSM : 1;//Enter Global Suspend Mode
    uint16_t FGR : 1;//Force Global Resume
    uint16_t SWDBG : 1;//Software Debug
    uint16_t ConfigFlag : 1;
    uint16_t MAXP : 1;//Max Packet
    uint16_t Reserved : 8;
};

struct uhci_status{ __attribute__((packed))
    uint16_t USBINT : 1; //USB Interrupt
    uint16_t USBErrINT : 1; //USb Error interrupt
    uint16_t ResumeDetect : 1;
    uint16_t HostSysErr : 1;//Host System Error
    uint16_t HostControlProcessError : 1;
    uint16_t Controller_Halted : 1;
};

struct uhci_interrupt_enable{ __attribute__((packed))
    uint16_t TimeoutCRCINTEnable : 1;
    uint16_t ResumeInterruptEnable : 1;
    uint16_t InterruptOnCompleteEnable : 1;
    uint16_t ShortPacketInterruptEnable : 1;
};

struct uhci_frame_number{ __attribute__((packed))
    uint16_t FrameListCurrentIndex : 11;
};

struct uhci_frame_base{ __attribute__((packed))
    uint32_t reserved : 12;
    uint32_t BaseAddress : 20;
};
struct uhci_start_of_frame{ __attribute__((packed))
    uint8_t SOF_TimingValue : 7;
};

struct uhci_port_status_control{ __attribute__((packed))
    uint16_t CurrentConnectStatus : 1;
    uint16_t ConnectStatusChange : 1;
    uint16_t PortEnableDisable : 1;
    uint16_t PortEnableDisableChange : 1;
    uint16_t LineStatus : 1;
    uint16_t ResumeDetect : 1;
    uint16_t Reserved1 : 1;
    uint16_t LowSpeedDeviceAttached : 1;
    uint16_t PortReset : 1;
    uint16_t Reserved2 : 2;
    uint16_t Suspend : 1;
};

struct uhci_io_reg{ __attribute__((packed))
    struct uhci_cmd USBCMD;//Command Reg
    struct uhci_status USBSTS;//Status Reg
    struct uhci_interrupt_enable USBINTR;//Interrupt Enable Reg
    struct uhci_frame_number FRNUM;//Frame Number
    struct uhci_frame_base FRBASEADD;//Frame Base Address
    struct uhci_start_of_frame SOFMOD;//Start of Frame Modify
    uint8_t Reserved1;
    uint16_t Reserved2;
    struct uhci_port_status_control PortSC1;
    struct uhci_port_status_control PortSC2;
};


bool uhci_detect_root(uint32_t base, bool io_space);
#endif