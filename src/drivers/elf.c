#include "elf.h"

bool elf_check_file(int file) {
	int head = vfs_seek(file, 0, 1);
	vfs_seek(file, 0, 0);
	Elf32_Ehdr hdr;
	vfs_read(file, (char *) &hdr, sizeof(Elf32_Ehdr));
	vfs_seek(file, head, 0);
	//if(!hdr) return false;
	if(hdr.e_ident[EI_MAG0] != ELFMAG0) {
		print_serial("ELF Header EI_MAG0 incorrect.\n");
		return false;
	}
	if(hdr.e_ident[EI_MAG1] != ELFMAG1) {
		print_serial("ELF Header EI_MAG1 incorrect.\n");
		return false;
	}
	if(hdr.e_ident[EI_MAG2] != ELFMAG2) {
		print_serial("ELF Header EI_MAG2 incorrect.\n");
		return false;
	}
	if(hdr.e_ident[EI_MAG3] != ELFMAG3) {
		print_serial("ELF Header EI_MAG3 incorrect.\n");
		return false;
	}
	print_serial("[ELF] Successful Header Match!\n");
	return true;
}

bool elf_check_supported(int file) {
	int head = vfs_seek(file, 0, 1);
	vfs_seek(file, 0, 0);
	Elf32_Ehdr hdr;
	vfs_read(file, (char *) &hdr, sizeof(Elf32_Ehdr));
	vfs_seek(file, head, 0);

	if(!elf_check_file(file)) {
		print_serial("[ELF] Invalid ELF File.\n");
		return false;
	}
	if(hdr.e_ident[EI_CLASS] != ELFCLASS32) {
		print_serial("[ELF] Unsupported ELF File Class.\n");
		return false;
	}
	if(hdr.e_ident[EI_DATA] != ELFDATA2LSB) {
		print_serial("[ELF] Unsupported ELF File byte order.\n");
		return false;
	}
	if(hdr.e_machine != EM_386) {
		print_serial("[ELF] Unsupported ELF File target.\n");
		return false;
	}
	if(hdr.e_ident[EI_VERSION] != EV_CURRENT) {
		print_serial("[ELF] Unsupported ELF File version.\n");
		return false;
	}
	if(hdr.e_type != ET_REL && hdr.e_type != ET_EXEC) {
		print_serial("[ELF] Unsupported ELF File type.\n");
		return false;
	}
	return true;
}