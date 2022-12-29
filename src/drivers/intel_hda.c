#include "intel_hda.h"
#include "intel_hda_enum.h"
//#define WIDGET_DEBUG
uint32_t HDA_BAR;
struct hda_device *HDA;
void hda_reset();
void setup_corb();
void setup_rirb();
void hda_enumerate_codecs();

void initialize_INTEL_HDA(int driverID){
    printk("Intel High Definition Audio Driver Init | %2x\n", driverID);
    //Prepare Base Address and Map Appropriate Memory
    HDA_BAR = pci_drivers[driverID]->BAR[0];
    create_page_entry(HDA_BAR, HDA_BAR, 0x83);
    HDA = malloc(sizeof(struct hda_device));
    HDA->rings = (uint32_t *) ((uint32_t)((uint32_t *) malloc(1024 + 2048 + BDL_BYTES_ROUNDED + 128 + 0x100) + 0x100) & ~0x7F);
    HDA->corb = (uint32_t *) HDA->rings + 0;
    HDA->rirb = (uint64_t *) HDA->rings + 1024;
    HDA->bdl = (struct hda_bdl_entry*) HDA->rings + 3072;
    HDA->dma_pos = (uint32_t *)HDA->rings + 3072+ BDL_BYTES_ROUNDED;

    hda_reset();
}

void hda_reset(){
    printk("[INTEL HDA] Reset\n");
    set_32_offset(HDA_BAR, REG_CORBCTL, 0);
    set_32_offset(HDA_BAR, REG_RIRBCTL, 0);
    while ((get_32_offset(HDA_BAR, REG_CORBCTL) & CORBCTL_CORBRUN)
        || (get_32_offset(HDA_BAR, REG_RIRBCTL) & RIRBCTL_RIRBRUN));
    set_32_offset(HDA_BAR, REG_GCTL, 0);
    while((get_32_offset(HDA_BAR, REG_GCTL) & GCTL_RESET));
    delay(100);
    set_32_offset(HDA_BAR, REG_GCTL, GCTL_RESET);
    while((get_32_offset(HDA_BAR, REG_GCTL) & GCTL_RESET) == 0);

    set_32_offset(HDA_BAR, REG_WAKEEN, 0xffff);
    set_32_offset(HDA_BAR, REG_INTCTL, 0x800000ff);

    setup_corb();
    setup_rirb();
    printk("[INTEL HDA] CORB Size: %3x RIRB Size: %3x\n", HDA->corb_entries, HDA->rirb_entries);

    printk("CORB %x | %x %1b\nRIRB %x | %x %1b\n", 
        get_32_offset(HDA_BAR, REG_CORBLBASE), (uint32_t) HDA->corb, get_32_offset(HDA_BAR, REG_CORBLBASE) == (uint32_t) HDA->corb,
        get_32_offset(HDA_BAR, REG_RIRBLBASE), (uint32_t) HDA->rirb, get_32_offset(HDA_BAR, REG_RIRBLBASE) == (uint32_t) HDA->rirb);

    delay(100);
    hda_enumerate_codecs();
}

void setup_corb(){
    printk("[INTEL HDA] Corb Setup\n");
    uint8_t reg;
    uint32_t corb_base;

    reg = get_8_offset(HDA_BAR, REG_CORBSIZE);

    if(reg & (1 << 6)){
        HDA->corb_entries = 256;
        reg |= 0x2;
    }
    else if(reg & (1 << 5)){
        HDA->corb_entries = 16;
        reg |= 0x1;
    }
    else if(reg & (1 << 4)){
        HDA->corb_entries = 2;
        reg |= 0x0;
    }
    else{
        printk("[INTEL HDA] No supported corb size!\n");
    }
    set_8_offset(HDA_BAR, REG_CORBSIZE, reg);

    corb_base = get_physical((uint32_t) HDA->corb);
    HDA->corb_base = (uint32_t *) corb_base;

    set_32_offset(HDA_BAR, REG_CORBLBASE,(uint32_t) corb_base);
    set_32_offset(HDA_BAR, REG_CORBUBASE, 0);

    set_8_offset(HDA_BAR, REG_CORBCTL, 0x02);
}

void setup_rirb(){
    uint8_t reg;
    uint32_t rirb_base;

    reg = get_8_offset(HDA_BAR, REG_RIRBSIZE);

    if(reg & (1 << 6)){
        HDA->rirb_entries = 256;
        reg |= 0x2;
    }
    else if(reg & (1 << 5)){
        HDA->rirb_entries = 16;
        reg |= 0x1;
    }
    else if(reg & (1 << 4)){
        HDA->rirb_entries = 2;
        reg |= 0x0;
    }
    else{
        printk("[INTEL HDA] No supported rirb size!\n");
    }

    set_8_offset(HDA_BAR, REG_RIRBSIZE, reg);

    rirb_base = get_physical((uint32_t)HDA->rirb);
    HDA->rirb_base = (uint64_t *) rirb_base;
    set_32_offset(HDA_BAR, REG_RIRBLBASE, rirb_base);
    set_32_offset(HDA_BAR, REG_RIRBUBASE, 0);

    set_8_offset(HDA_BAR, REG_RINTCNT, 0x42);

    set_8_offset(HDA_BAR, REG_RIRBCTL, 0x02);
}

void dump_corb_rirb(){
    for(int i = 0; i < 10; i++){
        printk("%x %x\n", HDA->corb[i], HDA->rirb[i]);
    }
}

void corb_write(uint32_t verb){
    uint16_t wp = get_16_offset(HDA_BAR, REG_CORBWP) & 0xFF;
    uint16_t rp;
    uint16_t next;
    next = (wp + 1) % HDA->corb_entries;

    do{
        rp = get_16_offset(HDA_BAR, REG_CORBRP) & 0xff;
    } while(next == rp);
    HDA->corb[next] = verb;
    //printk("CORB:\n%4x %4x\n", get_16_offset(HDA_BAR, REG_CORBWP), get_16_offset(HDA_BAR, REG_CORBRP));
    set_16_offset(HDA_BAR, REG_CORBWP, next);
    //printk("%4x %4x\n", get_16_offset(HDA_BAR, REG_CORBWP), get_16_offset(HDA_BAR, REG_CORBRP));
}

void rirb_read(uint64_t* response){
    uint16_t wp;
    uint16_t rp = HDA->rirbrp;
    //printk("RIRB:\n%4x %4x\n", get_16_offset(HDA_BAR, REG_RIRBWP), rp);
    do{
        wp = get_16_offset(HDA_BAR, REG_RIRBWP) & 0xff;
    } while(wp == rp);
    
    rp = (rp + 1) % HDA->rirb_entries;
    HDA->rirbrp = rp;
    
    *response = HDA->rirb[rp];
    set_8_offset(HDA_BAR, REG_RIRBSTS, 0x5);
    //printk("%4x %4x\n", get_16_offset(HDA_BAR, REG_RIRBWP), rp);
    /*
    for(unsigned int i = 0; i < HDA->rirb_entries; i++){
        uint64_t val = *((uint64_t *) get_32_offset(HDA_BAR, REG_RIRBLBASE) + i);
        if(val != 0){
            printk("%3x -> %x\n", i, val);
        }
    }
    */
}

uint32_t codec_query(int codec, int nid, uint32_t payload){
    uint64_t response;
    uint32_t verb = 
        ((codec & 0xf) << 28) |
        ((nid & 0xff) << 20) |
        (payload & 0xfffff);
    corb_write(verb);
    rirb_read(&response);
    //dump_corb_rirb();
    return response & 0xFFFFFFFF;
}

#ifdef WIDGET_DEBUG
void widget_dump_connections(int codec, int nid){
    uint32_t num_conn;
    uint32_t sel;
    unsigned int i;
    num_conn = codec_query(codec, nid, VERB_GET_PARAMETER | PARAM_CONN_LIST_LEN);
    if(!num_conn){
        return;
    }
    printk("[INTEL HDA] conn:");
    for(i = 0; i < (num_conn & 0x7f); i++){
        uint32_t conn;
        bool range;
        int idx, shift;
        if(num_conn & 0x80){
            idx = i & ~3;
            shift = 8 * (i & 3);
        }else{
            idx = i & ~1;
            shift = 8 * (i & 1);
        }

        conn = codec_query(codec, nid, VERB_GET_CONN_LIST | idx);
        conn >>= shift;

        if(num_conn & 0x80){
            range = conn & 0x8000;
            conn &= 0x7fff;
        }else{
            range = conn & 0x80;
            conn &= 0x7f;
        }

        printk("%c%x", range ? '-' : ' ', conn);
    }
    sel = codec_query(codec, nid, VERB_GET_CONN_SELECT);
    printk(" [current: %x]\n", sel);
}
#endif

void widget_init(int codec, int nid){
    uint32_t widget_cap;
    enum widget_type type;
    uint32_t amp_cap;
    uint32_t eapd_btl;

    widget_cap = codec_query(codec, nid, VERB_GET_PARAMETER | PARAM_AUDIO_WID_CAP);
    if(widget_cap == 0){
        return;
    }
    type = (widget_cap & WIDGET_CAP_TYPE_MASK) >> WIDGET_CAP_TYPE_SHIFT;

    amp_cap = codec_query(codec, nid, VERB_GET_PARAMETER | PARAM_OUT_AMP_CAP);
    eapd_btl = codec_query(codec, nid, VERB_GET_EAPD_BTL);

    //#ifdef WIDGET_DEBUG
    uint32_t amp_gain;
    const char* s;
    switch (type) {
        case 0:     s = "output"; break;
        case 1:     s = "input"; break;
        case 2:     s = "mixer"; break;
        case 3:     s = "selector"; break;
        case 4:     s = "pin complex"; break;
        case 5:     s = "power"; break;
        case 6:     s = "volume knob"; break;
        case 7:     s = "beep generator"; break;
        case 15:    s = "vendor defined"; break;
        default:    s = "unknown"; break;
    }
    amp_gain = codec_query(codec, nid, VERB_GET_AMP_GAIN_MUTE | 0x8000) << 8;

    amp_gain |= codec_query(codec, nid, VERB_GET_AMP_GAIN_MUTE | 0xa000);

    printk("[INTEL HDA] %s at ID %x cap %x eapd %x amp %x/%x\n",
        s, nid, widget_cap, eapd_btl, amp_gain, amp_cap);
    //widget_dump_connections(codec, nid);
    //#endif

    switch(type){
        case WIDGET_PIN:{
            uint32_t pin_cap, ctl;
            #ifdef WIDGET_DEBUG
            uint32_t conf = codec_query(codec, nid, VERB_GET_CONFIG_DEFAULT);
            printk("[INTEL HDA] Pin Config: %x\n", conf);
            #endif
            pin_cap = codec_query(codec, nid, VERB_GET_PARAMETER | PARAM_PIN_CAP);
            if((pin_cap & PIN_CAP_OUTPUT) == 0){
                return;
            }
            ctl = codec_query(codec, nid, VERB_GET_PIN_CONTROL);
            printk("[INTEL HDA] ctl: %x\n", ctl);
            ctl |= PIN_CTL_ENABLE_OUTPUT;
            codec_query(codec, nid, VERB_SET_PIN_CONTROL | ctl);
            codec_query(codec, nid, VERB_SET_EAPD_BTL | eapd_btl | 0x2);
            break;
        }
        case WIDGET_OUTPUT:{
            if(!HDA->output.nid){
                printk("[INTEL HDA] Using output ID: %x!\n", nid);
                HDA->output.codec = codec;
                HDA->output.nid = nid;
                HDA->output.amp_gain_steps = (amp_cap >> 8) & 0x7f;
            }
            codec_query(codec, nid, VERB_SET_EAPD_BTL | eapd_btl | 0x2);
            break;
        }
        default: 
            return;
    }
    if(widget_cap & WIDGET_CAP_POWER_CNTRL){
        codec_query(codec, nid, VERB_SET_POWER_STATE | 0x0);
    }
}

int codec_enumerate_widgets(int codec){
    uint32_t param;
    int num_fg, num_widgets;
    int fg_start, widgets_start;
    int i, j;

    param = codec_query(codec, 0, VERB_GET_PARAMETER | PARAM_NODE_COUNT);
    printk("Param: %x\n", param);
    num_fg = param & 0xff;
    fg_start = (param >> 16) & 0xff;

    printk("[INTEL HDA] %x function groups starting at ID %x\n", num_fg, fg_start);

    for(i = 0; i < num_fg; i++){
        param = codec_query(codec, fg_start + i, VERB_GET_PARAMETER | PARAM_NODE_COUNT);

        num_widgets = param & 0xff;
        widgets_start = (param >> 16) & 0xff;

        printk("[INTEL HDA] %x widgets starting at ID %x\n", num_widgets, widgets_start);

        param = codec_query(codec, fg_start + i, VERB_GET_PARAMETER | PARAM_FN_GROUP_TYPE);
        param &= 0x7f;

        if(param != FN_GROUP_AUDIO){
            printk("[INTEL HDA] Function Group Type %x\n", param);
            continue;
        }
        codec_query(codec, fg_start+i, VERB_GET_POWER_STATE | 0x0);
        for(j = 0; j < num_widgets; j++){
            widget_init(codec, widgets_start+j);
        }
    }
    return HDA->output.nid ? 0 : -1;
}

void hda_enumerate_codecs(){
    uint16_t statests;
    int i;
    printk("[INTEL HDA] Enumerating CODECS\n");

    statests = get_16_offset(HDA_BAR, REG_STATESTS);

    for(i = 0; i < 15; i++){
        if((statests & (1 << i))){
            printk("[INTEL HDA] Codec Found at %x\n", i);
            if(codec_enumerate_widgets(i)){
                return;
            }
        }
    }
}


uint32_t get_bitfield(uint32_t field, uint32_t bit){
    uint32_t val = field & bit;
    while((bit & 0b1) != 0b1){
        val >>= 1;
        bit >>= 1;
    }
    return val;
}

uint8_t get_8_offset(uint32_t BAR, uint32_t offset){
    uint32_t address = BAR + offset;
    return *((uint8_t *) address);
}

uint16_t get_16_offset(uint32_t BAR, uint32_t offset){
    uint32_t address = BAR + offset;
    return *((uint16_t *) address);
}

uint32_t get_32_offset(uint32_t BAR, uint32_t offset){
    uint32_t address = BAR + offset;
    return *((uint32_t *) address);
}

uint32_t get_24_offset(uint32_t BAR, uint32_t offset){
    uint32_t address = BAR + offset;
    return (*((uint32_t *) address) & 0x00FFFFFF);
}

void set_8_offset(uint32_t BAR, uint32_t offset, uint8_t val){
    uint32_t address = BAR + offset;
    uint8_t *reg = (uint8_t *) address;
    *reg = val; 
}

void set_16_offset(uint32_t BAR, uint32_t offset, uint16_t val){
    uint32_t address = BAR + offset;
    uint16_t *reg = (uint16_t *) address;
    *reg = val; 
}

void set_32_offset(uint32_t BAR, uint32_t offset, uint32_t val){
    uint32_t address = BAR + offset;
    uint32_t *reg = (uint32_t *) address;
    *reg = val;
}
