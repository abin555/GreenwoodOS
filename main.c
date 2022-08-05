
#include "multiboot.h"
#include "screen.h"

void kmain(unsigned long magic_num, unsigned long magic_addr){
    struct multiboot_tag *tag;

    if(magic != MULTIBOOT2_BOOTLOADER_MAGIC) return 0xFF;

    for(
        tag = (struct multiboot_tag *) (magic_addr + 8);
        tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7))
    ){
        switch(tag->type){
        case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
            {
                struct multiboot_tag_framebuffer *tagfb
                = (struct multiboot_tag_framebuffer *) tag;
                init_fb(tagfb);
            }
        }
    }
}