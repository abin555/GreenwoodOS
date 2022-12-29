#ifndef INTEL_HDA_ENUM_H
#define INTEL_HDA_ENUM_H
#include "intel_hda.h"
#include "stdint.h"
typedef unsigned int size_t;

#define BDL_SIZE 4
#define BUFFER_SIZE 0x10000

#define BDL_BYTES_ROUNDED ((BDL_SIZE * sizeof(struct hda_bdl_entry) + 127) & ~127)

#define SAMPLES_PER_BUFFER (BUFFER_SIZE / 2)

/** HD Audio Memory Mapped Registers */
enum hda_reg {
    REG_GCTL        = 0x08, ///< GCTL - Global Control
    REG_WAKEEN      = 0x0c, ///< WAKEEN - Wake Enable
    REG_STATESTS    = 0x0e, ///< STATESTS - State Change Status
    REG_INTCTL      = 0x20, ///< INTCTL - Interrupt Control
    REG_INTSTS      = 0x24, ///< INTSTS - Interrupt Status

    REG_CORBLBASE   = 0x40, ///< CORBLBASE - CORB Lower Base Address
    REG_CORBUBASE   = 0x44, ///< CORBUBASE - CORB Upper Base Address
    REG_CORBWP      = 0x48, ///< CORBWP - CORB Write Pointer
    REG_CORBRP      = 0x4a, ///< CORBRP - CORB Read Pointer
    REG_CORBCTL     = 0x4c, ///< CORBCTL - CORB Control
    REG_CORBSIZE    = 0x4e, ///< CORBSIZE - CORB size

    REG_RIRBLBASE   = 0x50, ///< RIRBLBASE - RIRB Lower Base Address
    REG_RIRBUBASE   = 0x54, ///< RIRBUBASE - RIRB Upper Base Address
    REG_RIRBWP      = 0x58, ///< RIRBWP - RIRB Write Pointer
    REG_RINTCNT     = 0x5a, ///< RINTCNT - Respone Interrupt Count
    REG_RIRBCTL     = 0x5c, ///< RIRBCTL - RIRB Control
    REG_RIRBSTS     = 0x5d, ///< RIRBSTS - RIRB Interrupt Status
    REG_RIRBSIZE    = 0x5e, ///< RIRBSIZE - RIRB size

    REG_DPLBASE     = 0x70, ///< DPLBASE - DMA Position Lower Base Address
    REG_DPUBASE     = 0x74, ///< DPUBASE - DMA Posiition Upper Base Address

    REG_O0_CTLL     = 0x100,    ///< Output 0 - Control Lower
    REG_O0_CTLU     = 0x102,    ///< Output 0 - Control Upper
    REG_O0_STS      = 0x103,    ///< Output 0 - Status
    REG_O0_CBL      = 0x108,    ///< Output 0 - Cyclic Buffer Length
    REG_O0_STLVI    = 0x10c,    ///< Output 0 - Last Valid Index
    REG_O0_FMT      = 0x112,    ///< Output 0 - Format
    REG_O0_BDLPL    = 0x118,    ///< Output 0 - BDL Pointer Lower
    REG_O0_BDLPU    = 0x11c,    ///< Output 0 - BDL Pointer Upper
};

/* GCTL bits */
enum hda_reg_gctl {
    GCTL_RESET  =   (1 << 0),
};

/* CORBCTL bits */
enum hda_reg_corbctl {
    CORBCTL_CORBRUN = (1 << 1),
};

/* RIRBCTL bits */
enum hda_reg_rirbctl {
    RIRBCTL_RIRBRUN = (1 << 1),
};

/* Stream Descriptor Control Register bits */
enum hda_reg_sdctl {
    SDCTL_RUN   =   0x2, ///< Enable DMA Engine
    SDCTL_IOCE  =   0x4, ///< Enable Interrupt on Complete
};

/** Represents an address of an output widget */
struct hda_output {
    struct audio_stream stream;

    uint8_t     codec;
    uint16_t    nid;

    uint32_t    sample_rate;
    int         amp_gain_steps;
    int         num_channels;
};

/** Buffer Descriptor List Entry */
struct hda_bdl_entry {
    uint64_t paddr;
    uint32_t length;
    uint32_t flags;
} __attribute__((packed));

/** Represents the state of an HD Audio PCI card */
struct hda_device {
    struct audio_device_hw audio;

    struct hda_output output;
    uint32_t mmio;

    uint32_t *rings;
    uint32_t *corb_base;
    uint64_t *rirb_base;
    uint32_t*               corb;           ///< Command Outbound Ring Buffer
    volatile uint64_t*      rirb;           ///< Response Inbound Ring Buffer
    struct hda_bdl_entry*   bdl;            ///< Buffer Descriptor List
    volatile uint32_t*      dma_pos;        ///< DMA Position in Current Buffer

    size_t                  corb_entries;   ///< Number of CORB entries
    size_t                  rirb_entries;   ///< Number of RIRB entries
    uint16_t                rirbrp;         ///< RIRB Read Pointer

    uint32_t*    buffer;
    int                     buffers_completed;
};

enum codec_verbs {
    VERB_GET_PARAMETER      = 0xf0000,
    VERB_SET_STREAM_CHANNEL = 0x70600,
    VERB_SET_FORMAT         = 0x20000,
    VERB_GET_AMP_GAIN_MUTE  = 0xb0000,
    VERB_SET_AMP_GAIN_MUTE  = 0x30000,
    VERB_GET_CONFIG_DEFAULT = 0xf1c00,
    VERB_GET_CONN_LIST      = 0xf0200,
    VERB_GET_CONN_SELECT    = 0xf0100,
    VERB_GET_PIN_CONTROL    = 0xf0700,
    VERB_SET_PIN_CONTROL    = 0x70700,
    VERB_GET_EAPD_BTL       = 0xf0c00,
    VERB_SET_EAPD_BTL       = 0x70c00,
    VERB_GET_POWER_STATE    = 0xf0500,
    VERB_SET_POWER_STATE    = 0x70500,
};

enum codec_parameters {
    PARAM_NODE_COUNT        = 0x04,
    PARAM_FN_GROUP_TYPE     = 0x05,
    PARAM_AUDIO_WID_CAP     = 0x09,
    PARAM_PIN_CAP           = 0x0c,
    PARAM_CONN_LIST_LEN     = 0x0e,
    PARAM_OUT_AMP_CAP       = 0x12,
};

enum fn_group_type {
    FN_GROUP_AUDIO          = 0x01,
};

enum widget_type {
    WIDGET_OUTPUT           = 0x0,
    WIDGET_INPUT            = 0x1,
    WIDGET_MIXER            = 0x2,
    WIDGET_SELECTOR         = 0x3,
    WIDGET_PIN              = 0x4,
    WIDGET_POWER            = 0x5,
    WIDGET_VOLUME_KNOB      = 0x6,
    WIDGET_BEEP_GEN         = 0x7,
    WIDGET_VENDOR_DEFINED   = 0xf,
};

enum widget_capabilities {
    WIDGET_CAP_POWER_CNTRL  = (1 << 10),

    WIDGET_CAP_TYPE_SHIFT   = 20,
    WIDGET_CAP_TYPE_MASK    = (0xf << 20),
};

enum pin_capabilities {
    PIN_CAP_OUTPUT          = (1 << 4),
    PIN_CAP_INPUT           = (1 << 5),
};

enum pin_ctl_flags {
    PIN_CTL_ENABLE_OUTPUT   = (1 << 6),
};

#endif