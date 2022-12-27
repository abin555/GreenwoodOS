#include "intel_hda.h"
#include "intel_hda_enum.h"

uint32_t HDA_BAR;
uint8_t gcap_iss;
uint8_t gcap_oss;
uint32_t corb_num_entries;
uint32_t rirb_num_entries;
uint32_t corb_dma;
uint32_t rirb_dma;
uint32_t corb_wp = 1;
int speaker_node = -1;
int pin_node = -1;
struct hdaudio_stream out_stream;


void init_HDA_corb();
void init_HDA_rirb();
void init_speaker(uint16_t stream_format);
uint32_t corb_enqueue(int cad, uint32_t nid, uint32_t verb, uint32_t payload);
int look_for_node(uint8_t parent_nid, uint8_t target_widget_type);
uint32_t rirb_data(uint32_t index);

void initialize_INTEL_HDA(int driverID){
    printk("Intel High Definition Audio Driver Init | %x\n", driverID);
    //Prepare Base Address and Map Appropriate Memory
    HDA_BAR = pci_drivers[driverID]->BAR[0];
    create_page_entry(HDA_BAR, HDA_BAR, 0x83);
    //Reset the Device
    uint32_t GCTL_reg = get_32_offset(HDA_BAR, GCTL);
    GCTL_reg = GCTL_reg | CRST;
    set_32_offset(HDA_BAR, GCTL, GCTL_reg);
    //Wait for device to be ready
    while(get_bitfield(get_32_offset(HDA_BAR, GCTL), CRST) == 0){}
    printk("[INTEL HDA] HW Initialized\n");

    //Get Capabilities
    //uint16_t GCAP_reg = get_16_offset(HDA_BAR, GCAP);
    gcap_iss = get_bitfield(get_16_offset(HDA_BAR, GCAP), ISS);
    gcap_oss = get_bitfield(get_16_offset(HDA_BAR, GCAP), OSS);
    //printk("[INTEL HDA] GCAP: %4x\n", GCAP_reg);
    printk("[INTEL HDA] OSS: %1x ISS: %1x\n", gcap_oss, gcap_iss);
    init_HDA_corb();
    init_HDA_rirb();
    init_speaker(1);
}

void init_HDA_corb(){
    uint32_t corb_len = 0x10100;
    corb_dma = get_physical(((uint32_t) malloc(corb_len) + 0x100) & (~0b111111));
    printk("[INTEL HDA] CORB DMA: %x\n", corb_dma);

    set_8_offset(HDA_BAR, CORBCTL, (get_8_offset(HDA_BAR, CORBCTL) & ~CORBRUN));

    corb_num_entries = 256;

    set_32_offset(HDA_BAR, CORB_LOW, corb_dma);
    set_32_offset(HDA_BAR, CORB_HIGH, 0);
    set_16_offset(HDA_BAR, CORBWP, 0);

    set_16_offset(HDA_BAR, CORBRP, 1 << 15);
    set_16_offset(HDA_BAR, CORBRP, 0);

    set_8_offset(HDA_BAR, CORBCTL, (get_8_offset(HDA_BAR, CORBCTL) | CORBRUN));
}

void init_HDA_rirb(){
    uint32_t rirb_len = 0x10100;
    rirb_dma = get_physical(((uint32_t) malloc(rirb_len) + 0x100) & (~0b111111));
    printk("[INTEL HDA] RIRB DMA: %x\n", rirb_dma);

    set_8_offset(HDA_BAR, RIRBCTL, get_8_offset(HDA_BAR, RIRBCTL) & ~RIRBDMAEN);

    rirb_num_entries = 256;
    set_32_offset(HDA_BAR, RIRBLBASE, rirb_dma);
    set_32_offset(HDA_BAR, RIRBUBASE, 0);

    set_16_offset(HDA_BAR, RINTCNT, 1);
    set_16_offset(HDA_BAR, RIRBWP, 1 << 15);

    set_8_offset(HDA_BAR, RIRBCTL, get_8_offset(HDA_BAR, RIRBCTL) | RIRBDMAEN | 1);
}

void init_out_stream(struct hdaudio_stream *stream, int id){
    printk("[INTEL HDA] Out Stream Init\n");
    stream->id = id;
    stream->offset = SDnCTL + gcap_iss * 0x20 + (id-1) * 0x20;
    stream->buffer_size = 512 * 1024;
    stream->buffer_dma = get_physical(((uint32_t) malloc(stream->buffer_size) + 0x100) & (~0b111111));

    set_32_offset(HDA_BAR, REG_SDnCTL(stream),
        (get_32_offset(HDA_BAR, REG_SDnCTL(stream)) & ~(0xf << 20)) | (stream->id << 20));
    
    stream->bdl_dma = ((uint32_t) malloc(stream->buffer_size) + 0x100) & (~0b111111);
    struct hdaudio_buffer_desc *bd_list = (struct hdaudio_buffer_desc *) get_physical(stream->bdl_dma);
    
    uint32_t num_bds = 2;
    bd_list[0].buffer_addr = stream->buffer_dma;
    bd_list[0].buffer_len = stream->buffer_size;
    bd_list[0].ioc = 0;
    bd_list[1].buffer_addr = 0;
    bd_list[1].buffer_len = 0;
    bd_list[1].ioc = 1;

    set_32_offset(HDA_BAR, REG_SDnCTL(stream), stream->buffer_size);
    set_32_offset(HDA_BAR, REG_SDnLVI(stream), num_bds-1);
    set_32_offset(HDA_BAR, REG_SDnBDLPL(stream), stream->bdl_dma);
    set_32_offset(HDA_BAR, REG_SDnBDLPU(stream), 0);
}

uint32_t corb_enqueue(int cad, uint32_t nid, uint32_t verb, uint32_t payload){
    uint32_t *queue = (uint32_t *) get_physical(corb_dma);
    uint32_t current = corb_wp % corb_num_entries;
    uint32_t data = (cad << 28) | (nid << 20) | (verb << 8) | payload;
    queue[current] = data;
    set_16_offset(HDA_BAR, CORBWP, current);
    corb_wp++;
    return current;
}

uint32_t rirb_data(uint32_t index){
    uint32_t *resps = (uint32_t *) get_physical(rirb_dma);
    return resps[index * 2];
}

uint32_t run_command(uint32_t nid, uint32_t verb, uint32_t payload){
    int cad = 0;
    uint32_t idx = corb_enqueue(cad, nid, verb, payload);
    printk("[INTEL HDA] RIRBSTS: %x\n", get_8_offset(HDA_BAR, RIRBSTS));
    while ((get_8_offset(HDA_BAR, RIRBSTS) & 1) == 0){}
    set_8_offset(HDA_BAR, RIRBSTS, 0x5);
    return rirb_data(idx);
}

int look_for_node(uint8_t parent_nid, uint8_t target_widget_type){
    printk("[INTEL HDA] Finding node\n");
    uint32_t value = run_command(parent_nid, VERB_GET_PARAM, PARAM_NODE_CNT);
    uint32_t nid_base = (value >> 16) & 0xff;
    uint32_t num_nodes = value & 0xff;
    if(!num_nodes){
        printk("[INTEL HDA] No nodes!\n");
        return -1;
    }

    for(uint32_t i = 0; i < num_nodes; i++){
        uint32_t value = run_command(nid_base + i, VERB_GET_PARAM, PARAM_AUDIO_WIDGET_CAPS);
        uint8_t widget_type = (value >> 20) & 0xf;
        if(widget_type == target_widget_type) {
            return nid_base + i;
        }
        int nid = look_for_node(nid_base + 1, target_widget_type);
        if(nid >= 0){
            return nid;
        }
    }
    return -1;
}

void init_speaker(uint16_t stream_format){
    //printk("[INTEL HDA] Stream Format: %4x\n", stream_format);
    printk("[INTEL HDA] Init Speaker\n");
    init_out_stream(&out_stream, 1);

    pin_node = look_for_node(NID_ROOT, WIDGET_TYPE_PIN);
    printk("[INTEL HDA] Found pin widget: %x\n", pin_node);

    uint32_t value = run_command(pin_node, VERB_GET_PARAM, PARAM_CON_LIST_LEN);
    int con_list_len = value & 0x07;
    bool is_long_form = (value & 0x80) != 0;
    if(!is_long_form){
        printk("[INTEL HDA] Long form is not supported!\n");
    }

    speaker_node = -1;
    for(int i = 0; i * 4 < con_list_len; i++){
        uint32_t value = run_command(pin_node, VERB_GET_CONLIST, i);
        for(int j = 0; i + j < con_list_len && j < 4; j++){
            int nid = (value >> (8 * j)) & 0xff;
            printk("[INTEL HDA] connection #%x entry=%x\n", i * 4 + j, nid);
            if(nid > 0){
                speaker_node = nid;
                break;
            }
        }
    }

    printk("[INTEL HDA] Found a speaker widget: nid=%x\n", speaker_node);
    run_command(speaker_node, VERB_SET_STREAM_FORMAT, stream_format);
    run_command(speaker_node, VERB_SET_CH_STREAM_ID, out_stream.id << 4);
    run_command(pin_node, VERB_SET_PIN_WIDGET_CTL, 0xc0);

    set_32_offset(HDA_BAR, SSYNC, 1);
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
