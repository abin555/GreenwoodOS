/*
TODO: Switch to using variable references rather than global definitions for the HDA device.
Too lazy to do tonight.
*/


#include "intel_hda.h"
#include "intel_hda_enum.h"
#define WIDGET_DEBUG
//uint32_t HDA_BAR;
#define HDA_BAR HDA->mmio
//struct hda_device *HDA;
void hda_reset(struct hda_device *HDA);
void setup_corb(struct hda_device *HDA);
void setup_rirb(struct hda_device *HDA);
void hda_enumerate_codecs(struct hda_device *HDA);
void init_output_widget(struct hda_device *HDA);
void init_stream_descriptor(struct hda_device *HDA);
uint32_t codec_query(struct hda_device *HDA, int codec, int nid, uint32_t payload);

void HDA_set_volume(struct audio_stream* stream, uint8_t volume);
int HDA_set_sample_rate(struct audio_stream* stream, int sample_rate);
int HDA_set_number_of_channels(struct audio_device* device, int channels);
int HDA_transfer_data(struct audio_stream* stream, uint32_t size, uint32_t* memory, uint32_t offset);
audio_status_t HDA_change_device_status(struct audio_device* device, audio_status_t status);
void HDA_get_position(struct audio_stream* stream, audio_position_t* position);

void HDA_interrupt_handler(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
    printk("[INTEL HDA] Interrupt!\n");
}

void initialize_INTEL_HDA(int driverID){
    printk("Intel High Definition Audio Driver Init | %2x\n", driverID);
    //Prepare Base Address and Map Appropriate Memory
    struct hda_device *HDA;
    uint32_t bar = pci_drivers[driverID]->BAR[0];
    create_page_entry(bar, bar, 0x83);
    HDA = malloc(sizeof(struct hda_device));
    HDA->mmio = bar;
    HDA->rings = (uint32_t *) ((uint32_t)((uint32_t *) malloc(1024 + 2048 + BDL_BYTES_ROUNDED + 128 + 0x100) + 0x100) & ~0x7F);
    HDA->corb = (uint32_t *) HDA->rings + 0;
    HDA->rirb = (uint64_t *) HDA->rings + 1024;
    HDA->bdl = (struct hda_bdl_entry*) HDA->rings + 3072;
    HDA->dma_pos = (uint32_t *)HDA->rings + 3072+ BDL_BYTES_ROUNDED;
    HDA->buffer = (uint32_t *) malloc(BDL_SIZE * BUFFER_SIZE);

    interrupt_add_handle(pci_drivers[driverID]->interrupt, HDA_interrupt_handler);

    hda_reset(HDA);
    if(!HDA->output.nid){
        printk("[INTEL HDA] No OUTPUT!\n");
        free(HDA->buffer);
        free(HDA->rings);
        free(HDA);
        return;
    }
    init_output_widget(HDA);
    init_stream_descriptor(HDA);
    HDA->audio.device = (void*) HDA;
    HDA->audio.record = 0;
    HDA->audio.stream = &HDA->output.stream;

    struct audio_device* audio_dev = (struct audio_device*) malloc(sizeof(struct audio_device));
    audio_dev->type = INTEL_HDA;
    audio_dev->hardware = &HDA->audio;
    audio_dev->set_volume = HDA_set_volume;
    audio_dev->set_sample_rate = HDA_set_sample_rate;
    audio_dev->set_number_of_channels = HDA_set_number_of_channels;
    audio_dev->transfer_data = HDA_transfer_data;
    audio_dev->change_device_status = HDA_change_device_status;
    audio_dev->get_position = HDA_get_position;
    add_audio_device(audio_dev);
}

void hda_reset(struct hda_device *HDA){
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

    setup_corb(HDA);
    setup_rirb(HDA);
    printk("[INTEL HDA] CORB Size: %3x RIRB Size: %3x\n", HDA->corb_entries, HDA->rirb_entries);

    printk("CORB %x | %x %1b\nRIRB %x | %x %1b\n", 
        get_32_offset(HDA_BAR, REG_CORBLBASE), (uint32_t) HDA->corb, get_32_offset(HDA_BAR, REG_CORBLBASE) == (uint32_t) HDA->corb,
        get_32_offset(HDA_BAR, REG_RIRBLBASE), (uint32_t) HDA->rirb, get_32_offset(HDA_BAR, REG_RIRBLBASE) == (uint32_t) HDA->rirb);

    delay(100);
    hda_enumerate_codecs(HDA);
}

void setup_corb(struct hda_device *HDA){
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

void setup_rirb(struct hda_device *HDA){
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

void dump_corb_rirb(struct hda_device *HDA){
    for(int i = 0; i < 10; i++){
        printk("%x %x\n", HDA->corb[i], HDA->rirb[i]);
    }
}

void corb_write(struct hda_device *HDA, uint32_t verb){
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

void rirb_read(struct hda_device *HDA, uint64_t* response){
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

uint32_t codec_query(struct hda_device *HDA, int codec, int nid, uint32_t payload){
    uint64_t response;
    uint32_t verb = 
        ((codec & 0xf) << 28) |
        ((nid & 0xff) << 20) |
        (payload & 0xfffff);
    corb_write(HDA, verb);
    rirb_read(HDA, &response);
    return response & 0xFFFFFFFF;
}

#ifdef WIDGET_DEBUG
void widget_dump_connections(struct hda_device *HDA, int codec, int nid){
    uint32_t num_conn;
    uint32_t sel;
    unsigned int i;
    num_conn = codec_query(HDA, codec, nid, VERB_GET_PARAMETER | PARAM_CONN_LIST_LEN);
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

        conn = codec_query(HDA, codec, nid, VERB_GET_CONN_LIST | idx);
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
    sel = codec_query(HDA, codec, nid, VERB_GET_CONN_SELECT);
    printk(" [current: %x]\n", sel);
}
#endif

void widget_init(struct hda_device *HDA, int codec, int nid){
    uint32_t widget_cap;
    enum widget_type type;
    uint32_t amp_cap;
    uint32_t eapd_btl;

    widget_cap = codec_query(HDA, codec, nid, VERB_GET_PARAMETER | PARAM_AUDIO_WID_CAP);
    if(widget_cap == 0){
        return;
    }
    type = (widget_cap & WIDGET_CAP_TYPE_MASK) >> WIDGET_CAP_TYPE_SHIFT;

    amp_cap = codec_query(HDA, codec, nid, VERB_GET_PARAMETER | PARAM_OUT_AMP_CAP);
    eapd_btl = codec_query(HDA, codec, nid, VERB_GET_EAPD_BTL);

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
    amp_gain = codec_query(HDA, codec, nid, VERB_GET_AMP_GAIN_MUTE | 0x8000) << 8;

    amp_gain |= codec_query(HDA, codec, nid, VERB_GET_AMP_GAIN_MUTE | 0xa000);

    printk("[INTEL HDA] %s at ID %x cap %x eapd %x amp %x/%x\n",
        s, nid, widget_cap, eapd_btl, amp_gain, amp_cap);
    //widget_dump_connections(codec, nid);
    //#endif

    switch(type){
        case WIDGET_PIN:{
            uint32_t pin_cap, ctl;
            #ifdef WIDGET_DEBUG
            uint32_t conf = codec_query(HDA, codec, nid, VERB_GET_CONFIG_DEFAULT);
            printk("[INTEL HDA] Pin Config: %x\n", conf);
            #endif
            pin_cap = codec_query(HDA, codec, nid, VERB_GET_PARAMETER | PARAM_PIN_CAP);
            if((pin_cap & PIN_CAP_OUTPUT) == 0){
                return;
            }
            ctl = codec_query(HDA, codec, nid, VERB_GET_PIN_CONTROL);
            printk("[INTEL HDA] ctl: %x\n", ctl);
            ctl |= PIN_CTL_ENABLE_OUTPUT;
            codec_query(HDA, codec, nid, VERB_SET_PIN_CONTROL | ctl);
            codec_query(HDA, codec, nid, VERB_SET_EAPD_BTL | eapd_btl | 0x2);
            break;
        }
        //case WIDGET_OUTPUT:{
        case WIDGET_BEEP_GEN:{
            if(!HDA->output.nid){
                printk("[INTEL HDA] Using output ID: %x!\n", nid);
                HDA->output.codec = codec;
                HDA->output.nid = nid;
                HDA->output.amp_gain_steps = (amp_cap >> 8) & 0x7f;
            }
            codec_query(HDA, codec, nid, VERB_SET_EAPD_BTL | eapd_btl | 0x2);
            break;
        }
        default: 
            return;
    }
    if(widget_cap & WIDGET_CAP_POWER_CNTRL){
        codec_query(HDA, codec, nid, VERB_SET_POWER_STATE | 0x0);
    }
}

int codec_enumerate_widgets(struct hda_device *HDA, int codec){
    uint32_t param;
    int num_fg, num_widgets;
    int fg_start, widgets_start;
    int i, j;

    param = codec_query(HDA, codec, 0, VERB_GET_PARAMETER | PARAM_NODE_COUNT);
    printk("Param: %x\n", param);
    num_fg = param & 0xff;
    fg_start = (param >> 16) & 0xff;

    printk("[INTEL HDA] %x function groups starting at ID %x\n", num_fg, fg_start);

    for(i = 0; i < num_fg; i++){
        param = codec_query(HDA, codec, fg_start + i, VERB_GET_PARAMETER | PARAM_NODE_COUNT);

        num_widgets = param & 0xff;
        widgets_start = (param >> 16) & 0xff;

        printk("[INTEL HDA] %x widgets starting at ID %x\n", num_widgets, widgets_start);

        param = codec_query(HDA, codec, fg_start + i, VERB_GET_PARAMETER | PARAM_FN_GROUP_TYPE);
        param &= 0x7f;

        if(param != FN_GROUP_AUDIO){
            printk("[INTEL HDA] Function Group Type %x\n", param);
            continue;
        }
        codec_query(HDA, codec, fg_start+i, VERB_GET_POWER_STATE | 0x0);
        for(j = 0; j < num_widgets; j++){
            widget_init(HDA, codec, widgets_start+j);
        }
    }
    return HDA->output.nid ? 0 : -1;
}

void hda_enumerate_codecs(struct hda_device *HDA){
    uint16_t statests;
    int i;
    printk("[INTEL HDA] Enumerating CODECS\n");

    statests = get_16_offset(HDA_BAR, REG_STATESTS);
    printk("[INTEL HDA] STATESTS: %4x\n", statests);
    for(i = 0; i < 15; i++){
        if((statests & (1 << i))){
            printk("[INTEL HDA] Codec Found at %x\n", i);
            if(codec_enumerate_widgets(HDA, i)){
                return;
            }
        }
    }
}

void configure_output_widget(struct hda_device *HDA){
    uint16_t format = BITS_16 | HDA->output.sample_rate | (HDA->output.num_channels - 1);
    codec_query(HDA, HDA->output.codec, HDA->output.nid, VERB_SET_FORMAT | format);
    set_32_offset(HDA_BAR, REG_O0_FMT, format);
}

void init_output_widget(struct hda_device *HDA){
    HDA->output.stream.device = &HDA->audio;
    HDA->output.stream.num_buffers = BDL_SIZE;
    HDA->output.stream.buffer_size = BUFFER_SIZE / 2;
    HDA->output.stream.sample_format = CDI_AUDIO_16SI;

    codec_query(HDA, HDA->output.codec, HDA->output.nid, VERB_SET_STREAM_CHANNEL | 0x10);

    HDA->output.sample_rate = SR_48_KHZ;
    HDA->output.num_channels = 2;
    configure_output_widget(HDA);
}

void init_stream_descriptor(struct hda_device *HDA){
    uint32_t bdl_base, dma_pos_base;
    int i;

    set_8_offset(HDA_BAR, REG_O0_CTLU, 0x10);
    set_32_offset(HDA_BAR, REG_O0_CBL, BDL_SIZE * BUFFER_SIZE);
    set_16_offset(HDA_BAR, REG_O0_STLVI, BDL_SIZE - 1);

    bdl_base = HDA->bdl->paddr;
    set_32_offset(HDA_BAR, REG_O0_BDLPL, bdl_base & 0xffffffff);
    set_32_offset(HDA_BAR, REG_O0_BDLPU, 0);

    for(i = 0; i < BDL_SIZE; i++){
        HDA->bdl[i].paddr = (uint64_t) get_physical((uint32_t)HDA->buffer + (i*BUFFER_SIZE));
        HDA->bdl[i].length = BUFFER_SIZE;
        HDA->bdl[i].flags = 1;
    }

    memset(HDA->buffer, 0, BDL_SIZE * BUFFER_SIZE);
    for(i = 0; i < 8; i++){
        HDA->dma_pos[i] = 0;
    }

    dma_pos_base = get_physical((uint32_t) HDA->dma_pos);
    set_32_offset(HDA_BAR, REG_DPLBASE, dma_pos_base & 0xffffffff);
    set_32_offset(HDA_BAR, REG_DPUBASE, 0);

}


void HDA_set_volume(struct audio_stream* stream, uint8_t volume){
    struct hda_device* hda = (struct hda_device*) stream->device->device;
    int meta = 0xb000;
    printk("Setting Volume to %2x\n", volume);
    if(volume == 0){
        volume = 0x80;
    }
    else{
        volume = volume * hda->output.amp_gain_steps / 255;
    }
    codec_query(hda, hda->output.codec, hda->output.nid, VERB_SET_AMP_GAIN_MUTE | meta | volume);
}

int HDA_set_sample_rate(struct audio_stream* stream, int sample_rate){
    struct hda_device* hda = (struct hda_device*) stream->device->device;
    switch(sample_rate){
        case 44100:
            hda->output.sample_rate = SR_44_KHZ;
            break;
        default:
            sample_rate = 48000;
            hda->output.sample_rate = SR_48_KHZ;
            break;
    }
    configure_output_widget(hda);
    return sample_rate;
}

int HDA_set_number_of_channels(struct audio_device* device, int channels){
    struct hda_device* hda = (struct hda_device*) device->hardware->device;
    if(channels < 1 || channels > 2){
        channels = 2;
    }
    hda->output.num_channels = channels;
    configure_output_widget(hda);
    return channels;
}

int HDA_transfer_data(struct audio_stream* stream, uint32_t size, uint32_t* memory, uint32_t offset){
    struct hda_device* hda = (struct hda_device*) stream->device->device;
    printk("[Intel HDA] Transfer Data: size %x, off %x\n", size, offset);
    if(size > BDL_SIZE*BUFFER_SIZE || offset >= SAMPLES_PER_BUFFER){
        return -1;
    }
    //memcpy(memory, hda->buffer + size * BUFFER_SIZE + (offset * 2), (SAMPLES_PER_BUFFER - offset) * 2);
    memcpy(memory, hda->buffer, size);
    return 0;
}

audio_status_t HDA_change_device_status(struct audio_device* device, audio_status_t status){
    struct hda_device* hda = (struct hda_device*) device->hardware->device;
    uint16_t ctl;
    printk("[INTEL HDA] Change Status %x\n", status);
    if(status == CDI_AUDIO_PLAY){
        ctl = SDCTL_RUN | SDCTL_IOCE;
    }
    else{
        ctl = 0;
    }
    set_16_offset(hda->mmio, REG_O0_CTLL, ctl);
    return status;
}
void HDA_get_position(struct audio_stream* stream, audio_position_t* position){
    struct hda_device* hda = (struct hda_device*) stream->device->device;
    uint32_t pos = hda->dma_pos[4] & 0xffffffff;
    position->buffer = pos / BUFFER_SIZE;
    position->frame = (pos % BUFFER_SIZE) / 2;
    printk("[INTEL HDA] Position: %x/%x\n", position->buffer, position->frame);
}
