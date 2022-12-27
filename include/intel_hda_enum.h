#ifndef INTEL_HDA_ENUM_H
#define INTEL_HDA_ENUM_H

#define REG_SDnCTL(stream)   ((stream)->offset + 0x00)
#define REG_SDnCBL(stream)   ((stream)->offset + 0x08)
#define REG_SDnLVI(stream)   ((stream)->offset + 0x0c)
#define REG_SDnFMT(stream)   ((stream)->offset + 0x12)
#define REG_SDnBDLPL(stream) ((stream)->offset + 0x18)
#define REG_SDnBDLPU(stream) ((stream)->offset + 0x1c)

#define VERB_GET_PARAM          0xf00
#define VERB_GET_CONLIST        0xf02
#define VERB_SET_STREAM_FORMAT  0x200
#define VERB_SET_CH_STREAM_ID   0x706
#define VERB_SET_PIN_WIDGET_CTL 0x707
#define VERB_GET_AMP_GAIN_MUTE  0xb00
#define VERB_SET_AMP_GAIN_MUTE  0x300
#define VERB_SET_POWER_STATE    0x705

#define PARAM_NODE_CNT          0x04
#define PARAM_AUDIO_WIDGET_CAPS 0x09
#define PARAM_CON_LIST_LEN      0x0e

#define NID_ROOT 0
#define WIDGET_TYPE_PIN 4

enum HDA_REG_OFFSET{
    GCAP = 0x00,//2B
    VMIN = 0x02,//1B
    VMAJ = 0x03,//1B
    OUTPAY = 0x04,//2B
    INPAY = 0x06,//2B
    GCTL = 0x08,//4B
    WAKEEN = 0x0C,//2B
    STATESTS = 0x0E,//2B
    GSTS = 0x10,//2B
    OUTSTRMPAY = 0x18,//2B
    INSTRMPAY = 0x1A,//2B
    INTCTL = 0x20,//4B
    INTSTS = 0x24,//4B
    WALLCLOCK = 0x30,//4B
    SSYNC = 0x38,//4B
    CORB_LOW = 0x40,//4B
    CORB_HIGH = 0x44,//4B
    CORBWP = 0x48,//2B
    CORBRP = 0x4A,//2B
    CORBCTL = 0x4C,//1B
    CORBSTS = 0x4D,//1B
    CORBSIZE = 0x4E,//1B
    RIRBLBASE = 0x50,//4B
    RIRBUBASE = 0x54,//4B
    RIRBWP = 0x58,//2B
    RINTCNT = 0x5A,//2B
    RIRBCTL = 0x5C,//1B
    RIRBSTS = 0x5D,//1B
    RIRBSIZE = 0x5E,//1B
    DPLBASE = 0x70,//4B
    DPUBASE = 0x74,//4B
    SDnCTL = 0x80,//3B *** IS THREE BYTES
    SD0STS = 0x83,//1B
    SDnLPIB = 0x84,//4B
    SDnCBL = 0x88,//4B
    ISDnLVI = 0x8C,//2B
    SDnFIFOS = 0x90,//2B
    SDnFMT = 0x92,//2B
    SDnBDPL = 0x98,//4B
    SDnBDPU = 0x9C,//4B
    WALCLKA = 0x2030,//4B

    //IOBI Stream Descriptor N Link Position in Buffer Alias (4B each)
    SDnLICBA = 0x2084,

    Immediate_Command_Output_Interface = 0x60,//4B
    Immediate_Response_Input_Interface = 0x64,//4B
    Immediate_Command_Status = 0x68//2B
};

enum HDA_GCAP{//Global Capabilities
    OSS = 0xF000,
    ISS = 0x0F00,
    BSS = 0x00F8,
    NDSO = 0x0006,
    OK64 = 0x0001
};

enum HDA_GCTL{//Global Control
    FCNTRL = 0b10,
    UNSOL = 0x100,
    CRST = 0b1
};

enum HDA_INTCTL{//Interrupt Control
    GIE = 0x80000000,
    CIE = 0x40000000,
    SIE = 0x3FFFFFFF
};

enum HDA_INTSTS{//Interrupt Status
    GIS = 0x80000000,
    CIS = 0x40000000,
    SIS = 0x3FFFFFFF
};

enum HDA_CORB_RIRB_CTL{
    CORBRUN = 0b10,
    RIRBDMAEN = 0b10
};


#endif