#ifndef EHCI_H
#define EHCI_H

#include "USB.h"
#include "memory.h"
#include "console.h"
#include "paging.h"
#include "utilities.h"
#include "interrupts.h"
#include "pci.h"

#define CMD_FRAMELIST_1024             0x0
#define CMD_FRAMELIST_512              0x4
#define CMD_FRAMELIST_256              0x8


struct pci_device;
struct pci_driver;

struct EHCI_controller{
    struct pci_device* pci_device;
    struct pci_driver* pci_driver;
    uint32_t mmio;
    uint8_t operationalRegs_offset;
    uint16_t version;
    uint8_t N_PORTS;
    uint8_t N_CC;
    uint8_t N_PTT;
    uint8_t N_TT;
    uint8_t ecap_offset;
    uint8_t IsochronousSchedThreshold;
    uint32_t PeriodicBase;
    uint32_t AsyncList;
    uint16_t framesize;
};

#define EHCI_HCSPARAMS_OFFSET 0x04
typedef enum{
    N_PORTS = 0xF,//Number of Ports
    PPC = 0x10,//Port Power Control
    PortRoutingRules = 0x80,//Port Routing RUles
    N_PCC = 0xF00,//Number of Ports per Companion Controller
    N_CC = 0xF000,//Number of Companion Controllers
    P_INDICATOR = 0x10000,//Port Indicators
    DebugPortN = 0xF00000,
    N_PTT = 0xF00000,//Number of Ports per Transaction Table
    N_TT = 0xF000000//Number of Transaction Tables
} EHCI_HCSPARAMS;

uint32_t EHCI_get_HCSPARAMS(struct EHCI_controller* controller, EHCI_HCSPARAMS field);

#define EHCI_HCCPARAMS_OFFSET 0x08
typedef enum{
    Bit64Addressing = 0x1,
    ProgrammableFrameListFlag = 0x2,
    AsyncScheduleParkCap = 0x4,
    IsochronousSchedThreshold = 0xF0,
    EECP = 0xFF00 //EHCI Extended Capabilities Pointer
} EHCI_HCCPARAMS;

uint32_t EHCI_get_HCCPARAMS(struct EHCI_controller* controller, EHCI_HCCPARAMS field);

#define EHCI_USBCMD_OFFSET 0
typedef enum{
    RS = 0x1,//Run/Stop
    HCRESET = 0x2,//Host Controller Reset
    FrameListSize = 0xC,//Frame List Size
    PeriodicScheduleEnable = 0x10,//Periodic Schedule Enable
    AsyncScheduleEnable = 0x20,//Asyncronous Schedule Enable
    IntOnAsyncAdvDoorbell = 0x40,//Interrupt on Async Advance Doorbell
    AsyncSchedParkModeCnt = 0x300,//Asyncronous Schedule Park Mode Count
    AsyncSchedParkModeEn = 0x800,//Asyncronous Schedule Park Mode Enable
    IntThresholdCtrl = 0xFF0000//Interrupt Threshold Control
} EHCI_USBCMD;

uint32_t EHCI_get_USBCMD(struct EHCI_controller* controller, EHCI_USBCMD field);
uint32_t EHCI_set_USBCMD(struct EHCI_controller* controller, EHCI_USBCMD field, uint32_t value);

#define EHCI_USBSTS_OFFSET 4
typedef enum{
    USBINT = 0x1,//USB Interrupt
    USBERRINT = 0x2,//USB Error Interrupt
    PortChangeDetect = 0x4,
    FrameListRollover = 0x8,
    HostSystemError = 0x10,
    IntOnAsyncAdv = 0x20,//Interrupt on Async Advance
    HCHalted = 0x1000,
    Reclamation = 0x2000,
    PerSchedSts = 0x4000,//Periodic Schedule Status
    AsyncSchedSts = 0x8000//Asyncronous Schedule Status
} EHCI_USBSTS;

uint32_t EHCI_get_USBSTS(struct EHCI_controller* controller, EHCI_USBCMD field);

#define EHCI_USBINTR_OFFSET 0x08

typedef enum{
    USBIntREn = 0x01,//USB Interrupt Enable
    USBErrIntEn = 0x02,//USB Error Interrupt Enable
    PortCngIntEn = 0x04,//Port Change Interrupt Enable
    FrameListRolloverEnable = 0x08,
    HostSysErrEn = 0x10,//Host System Error Enable
    IntOnAsyncAdvEn = 0x20//Interrupt on Async Advance Enable
} EHCI_USBINTR;

uint32_t EHCI_get_USBINTR(struct EHCI_controller* controller, EHCI_USBINTR field);
uint32_t EHCI_set_USBINTR(struct EHCI_controller* controller, EHCI_USBINTR field, uint32_t value);
#define EHCI_FRINDEX_OFFSET 0x0C
uint32_t EHCI_get_FRINDEX(struct EHCI_controller* controller);
uint32_t EHCI_set_FRINDEX(struct EHCI_controller* controller, uint32_t value);

typedef enum{
    USBCMD = 0x00,
    USBSTS = 0x04,
    USBINTR = 0x08,
    FRINDEX = 0x0C,//Frame Index
    CTRLDSSSegment = 0x10,//Control Data Structure Segment Register
    PeriodicListBase = 0x14,//Frame List Base Address
    AsyncListAddr = 0x18,//Next Asyncronous List Address
    ConfigFlag = 0x40,//Configured Flag Register
    PORTSC_BASE = 0x44//Port Status/Control
} EHCI_OperationalRegister;

uint32_t EHCI_get_operReg(struct EHCI_controller* controller, EHCI_OperationalRegister offset);
uint32_t EHCI_set_operReg(struct EHCI_controller* controller, EHCI_OperationalRegister offset, uint32_t value);

typedef enum{
    CurrentConnectStatus = 0x01,
    ConnectStatusChange = 0x02,
    PortEn = 0x04,
    PortEnChange = 0x08,
    OverCurrentActive = 0x10,
    OverCurrentChange = 0x20,
    ForcePortResume = 0x40,
    Suspend = 0x80,
    PortReset = 0x100,
    LineStatus = 0xC00,
    PortPower = 0x1000,
    PortOwner = 0x2000,
    PortIndicatorControl = 0xC000,
    PortTestControl = 0xF0000,
    WakeOnConnectEnable = 0x100000,
    WakeOnDisconnectEnable = 0x200000,
    WakeOnOverCurrentEnable = 0x400000,
    TT_speed = 0xC000000//Translation Table Speed Check
} EHCI_PORTSC;

uint32_t EHCI_get_portSC(struct EHCI_controller* controller, uint32_t port, EHCI_PORTSC field);
uint32_t EHCI_set_portSC(struct EHCI_controller* controller, uint32_t port, EHCI_PORTSC field, uint32_t value);

void init_EHCI(struct pci_device* device, struct pci_driver* driver);
void ehci_start(struct EHCI_controller* e);
void ehci_analyze(struct EHCI_controller* e);
void ehci_initHC(struct EHCI_controller* e);
void ehci_resetHC(struct EHCI_controller* e);
void ehci_startHC(struct EHCI_controller* e);
void ehci_initializePeriodicList(struct EHCI_controller* e);
void ehci_deactivateLegacySupport(struct EHCI_controller* e);
bool ehci_detect_root(struct EHCI_controller* controller);
struct cpu_state ehci_handler(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused)));


#endif