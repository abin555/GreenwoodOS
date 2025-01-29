#include "grub.h"

struct multiboot_tag_framebuffer *GRUB_tagfb = NULL;
struct multiboot_tag_apm *GRUB_APM = NULL;
struct multiboot_tag_mmap *GRUB_MMAP = NULL;
struct multiboot_tag_new_acpi *GRUB_ACPI_NEW = NULL;
struct multiboot_tag_old_acpi *GRUB_ACPI_OLD = NULL;

struct multiboot_tag* GRUB_multiboot_table;


void parse_multiboot2(unsigned long magic_addr){
	print_serial("[MULTIBOOT] Parsing Multiboot Boot Structure!\n");
	GRUB_multiboot_table = (struct multiboot_tag *)(magic_addr + 8);
	struct multiboot_tag *tag;
	for (
        tag = (struct multiboot_tag *)(magic_addr + 8);
        tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag + ((tag->size + 7) & ~7)))
    {
        switch (tag->type)
        {
			case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
			{
				GRUB_tagfb = (struct multiboot_tag_framebuffer *)tag;
				print_serial("Multiboot Address: 0x%x \nFramebuffer Address: 0x%x\n", (unsigned int) magic_addr, GRUB_tagfb->common.framebuffer_addr);
				//init_framebuffer(tagfb);
				break;
			}
			case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
			{
				struct multiboot_tag_string *bootloader = (struct multiboot_tag_string *) tag;
				print_serial("Greenwood OS Booted by %s\n", bootloader->string);
				break;
			}
			case MULTIBOOT_TAG_TYPE_CMDLINE:
			{
				struct multiboot_tag_string *cmdline = (struct multiboot_tag_string *) tag;
				print_serial("Greenwood OS Args are \"%s\"\n", cmdline->string);
				break;
			}
			case MULTIBOOT_TAG_TYPE_APM:
			{
				GRUB_APM = (struct multiboot_tag_apm*) tag;
				print_serial("APM Table Identified\n");
				break;
			}
			case MULTIBOOT_TAG_TYPE_MMAP:
			{
				GRUB_MMAP = (struct multiboot_tag_mmap*) tag;
				print_serial("MMAP Table Identified\n");
				break;
			}
			case MULTIBOOT_TAG_TYPE_ACPI_NEW:
			{
				GRUB_ACPI_NEW = (struct multiboot_tag_new_acpi *) tag;
				print_serial("ACPI NEW Table Identified @ 0x%x\n", &((struct multiboot_tag_new_acpi *) tag)->rsdp);
				break;
			}
			case MULTIBOOT_TAG_TYPE_ACPI_OLD:
			{
				GRUB_ACPI_OLD = (struct multiboot_tag_old_acpi *) tag;
				print_serial("ACPI OLD Table Identified @ 0x%x\n", &((struct multiboot_tag_old_acpi *) tag)->rsdp);
				break;
			}
			case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
			{
				struct multiboot_tag_basic_meminfo *meminfo = (struct multiboot_tag_basic_meminfo *) tag;
				print_serial("MEMORY INFO:\n");
				print_serial("KB Lower: 0x%x\n", meminfo->mem_lower);
				print_serial("KB Upper: 0x%x\n", meminfo->mem_upper);
				break;
			}
			default:
				print_serial("Tag ID: %d\n", tag->type);
				break;
        }
    }
	print_serial("[MULTIBOOT] Parsing Finished!\n");
}
