#ifndef GRUB_H
#define GRUB_H

#include "stdint.h"
#include "serial.h"
#include "multiboot.h"

extern struct multiboot_tag_framebuffer *GRUB_tagfb;
extern struct multiboot_tag_apm *GRUB_APM;
extern struct multiboot_tag_mmap *GRUB_MMAP;
extern struct multiboot_tag_new_acpi *GRUB_ACPI_NEW;
extern struct multiboot_tag_old_acpi *GRUB_ACPI_OLD;

void parse_multiboot2(unsigned long magic_addr);

#endif