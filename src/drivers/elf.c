#include "elf.h"

bool elf_check_file(Elf32_Ehdr hdr) {
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
	print_serial("[ELF] Checking Support\n");
	int head = vfs_seek(file, 0, 1);
	vfs_seek(file, 0, 0);
	Elf32_Ehdr hdr;
	vfs_read(file, (char *) &hdr, sizeof(Elf32_Ehdr));
	vfs_seek(file, head, 0);

	if(!elf_check_file(hdr)) {
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
	print_serial("%d\n", hdr.e_type);
	if(hdr.e_type != ET_REL && hdr.e_type != ET_EXEC) {
		print_serial("[ELF] Unsupported ELF File type.\n");
		return false;
	}

	return true;
}

uint32_t elf_get_entry_addr(int file){
	if(file == -1) return 0;
	int head = vfs_seek(file, 0, 1);
	vfs_seek(file, 0, 0);
	Elf32_Ehdr hdr;
	vfs_read(file, (char *) &hdr, sizeof(Elf32_Ehdr));
	vfs_seek(file, head, 0);
	print_serial("Entry Point Address: 0x%x\n", hdr.e_entry);
	return hdr.e_entry;
}

bool elf_load_section(int file, Elf32_Ehdr hdr, int sectionIdx, void *buffer){
	if(file == -1) return false;
	vfs_seek(file, 0, 0);

	Elf32_Shdr shdr;
	vfs_seek(file, hdr.e_shoff+hdr.e_shentsize*sectionIdx, 0);
	vfs_read(file, (char *) &shdr, hdr.e_shentsize);	

	if(shdr.sh_type == 1){	
		print_serial("[ELF] Section Load %d\n Name Idx: %d\n Type: %d\n Flags: %d\n Addr: %d\n Offset: %d\n Size: %d\n",
			sectionIdx,
			shdr.sh_name,
			shdr.sh_type,
			shdr.sh_flags,
			shdr.sh_addr,
			shdr.sh_offset,
			shdr.sh_size
		);
		vfs_seek(file, shdr.sh_offset, 0);
		vfs_read(file, ((char *) buffer) + shdr.sh_addr, shdr.sh_size);
		vfs_seek(file, 0, 0);
		return true;
	}
	else{
		print_serial("[ELF] Section %d is not PROGBITS\n", sectionIdx);
		return false;
	}
	return false;
}

bool elf_load(int file, void *buffer){
	if(file == -1) return false;

	vfs_seek(file, 0, 0);
	Elf32_Ehdr hdr;
	vfs_read(file, (char *) &hdr, sizeof(Elf32_Ehdr));

	for(int i = 0; i < hdr.e_shnum; i++){
		elf_load_section(file, hdr, i, buffer);
	}
	return true;
}