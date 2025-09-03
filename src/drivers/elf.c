#include "elf.h"

#ifndef ELF32_R_SYM
#define ELF32_R_SYM(info) ((info) >> 8)
#define ELF32_R_TYPE(info) ((uint8_t)(info))
#endif

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
	//print_serial("[ELF] Successful Header Match!\n");
	return true;
}

bool elf_check_supported(int file) {
	//print_serial("[ELF] Checking Support\n");
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
	//print_serial("%d\n", hdr.e_type);
	if(hdr.e_type != ET_REL && hdr.e_type != ET_EXEC && hdr.e_type != ET_DYN) {
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
	//print_serial("Entry Point Address: 0x%x\n", hdr.e_entry);
	return hdr.e_entry;
}

bool elf_load_section(int file, Elf32_Ehdr hdr, int sectionIdx, void *buffer){
	if(file == -1) return false;
	vfs_seek(file, 0, 0);
	//print_serial("[ELF] Loading section %d\n", sectionIdx);

	Elf32_Shdr shdr;
	vfs_seek(file, hdr.e_shoff+hdr.e_shentsize*sectionIdx, 0);
	vfs_read(file, (char *) &shdr, hdr.e_shentsize);	

	if(shdr.sh_type == 1){	
		/*
		print_serial("[ELF] Section Load %d\n Name Idx: %d\n Type: %d\n Flags: %d\n Addr: %d\n Offset: %d\n Size: %d\n",
			sectionIdx,
			shdr.sh_name,
			shdr.sh_type,
			shdr.sh_flags,
			shdr.sh_addr,
			shdr.sh_offset,
			shdr.sh_size
		);
		*/
		vfs_seek(file, shdr.sh_offset, 0);
		vfs_read(file, ((char *) buffer) + shdr.sh_addr, shdr.sh_size);
		vfs_seek(file, 0, 0);
		return true;
	}
	else{
		//print_serial("[ELF] Section %d is not PROGBITS\n", sectionIdx);
		return false;
	}
	return false;
}

static inline Elf32_Shdr *elf_sheader(Elf32_Ehdr *hdr) {
	return (Elf32_Shdr *)((int)hdr + hdr->e_shoff);
}

static inline Elf32_Shdr *elf_section(Elf32_Ehdr *hdr, int idx) {
	return &elf_sheader(hdr)[idx];
}

static inline char *elf_str_table(Elf32_Ehdr *hdr) {
	if(hdr->e_shstrndx == SHN_UNDEF) return NULL;
	return (char *)hdr + elf_section(hdr, hdr->e_shstrndx)->sh_offset;
}

static inline char *elf_lookup_string(Elf32_Ehdr *hdr, int offset) {
	char *strtab = elf_str_table(hdr);
	if(strtab == NULL) return NULL;
	return strtab + offset;
}

bool elf_load(int file, void *buffer){
	if(file == -1) return false;
	//print_serial("[ELF] Loading elf file\n");
	vfs_seek(file, 0, 0);
	Elf32_Ehdr hdr;
	vfs_read(file, (char *) &hdr, sizeof(Elf32_Ehdr));

	for(int i = 1; i < hdr.e_shnum; i++){
		if(!elf_load_section(file, hdr, i, buffer)){
			break;
		}
	}
	return true;
}

bool elf_is_dyn(int file){
	//print_serial("[ELF] Checking Support\n");
	int head = vfs_seek(file, 0, 1);
	vfs_seek(file, 0, 0);
	Elf32_Ehdr hdr;
	vfs_read(file, (char *) &hdr, sizeof(Elf32_Ehdr));
	vfs_seek(file, head, 0);

	if(hdr.e_type == ET_DYN) return true;
	return false;
}

bool elf_load_dyn(int file, void *base){
	print_serial("[ELF] Loading Dynamic File (%d) to 0x%x\n", file, base);
	if(file == -1) return false;

	int fsize = vfs_seek(file, 0, 2);
	vfs_seek(file, 0, 0);


	void *file_block = (void *) MEM_reserveRegionBlock(MEM_findRegionIdx(fsize), fsize, 0, SYSTEM);
	print_serial("[ELF] File is %d bytes\n");
	vfs_seek(file, 0, 0);
	vfs_read(file, (char *) file_block, fsize);
	vfs_seek(file, 0, 0);

	Elf32_Ehdr *ehdr = file_block;
	if(!elf_check_file(*ehdr)){
		print_serial("[ELF] Error while loading the file\n");
		goto ELF_DYN_LOAD_FAIL;
	}

	Elf32_Phdr *phdrs = (Elf32_Phdr *)(file_block + ehdr->e_phoff);
    Elf32_Dyn *dynamic = NULL;
    for (int i = 0; i < ehdr->e_phnum; ++i) {
        if (phdrs[i].p_type == PT_DYNAMIC) {
            dynamic = (Elf32_Dyn *)(file_block + phdrs[i].p_offset);
            break;
        }
    }

    if (!dynamic) {
        // No dynamic section; possibly static ET_EXEC with relocations in sections only.
        print_serial("[ELF] apply_elf_relocations: no PT_DYNAMIC\n");
        goto ELF_DYN_LOAD_FAIL;
    }

	// dynamic table entries
    Elf32_Rel *rel = NULL;
    size_t relsz = 0;
    size_t relent = sizeof(Elf32_Rel);

    Elf32_Rela *rela = NULL;
    size_t relasz = 0;
    size_t relaent = sizeof(Elf32_Rela);

    Elf32_Rel *jmprel = NULL;
    size_t jmprel_sz = 0;

    Elf32_Sym *symtab = NULL;
    const char *strtab = NULL;

    // Walk dynamic entries
    for (Elf32_Dyn *d = dynamic; d->d_tag != DT_NULL; ++d) {
		if(d->d_tag < 100){
			print_serial("[ELF] DT @ 0x%x type: %d\n", d, d->d_tag);
		}
		else{
			print_serial("[ELF] DT @ 0x%x type: NA\n", d);
		}
        switch (d->d_tag) {
        case DT_REL:
            rel = (Elf32_Rel *)(file_block + d->d_un.d_ptr);
            break;
        case DT_RELSZ:
            relsz = (size_t)d->d_un.d_val;
            break;
        case DT_RELENT:
            relent = d->d_un.d_val ? (size_t)d->d_un.d_val : sizeof(Elf32_Rel);
            break;
        case DT_RELA:
            rela = (Elf32_Rela *)(file_block + d->d_un.d_ptr);
            break;
        case DT_RELASZ:
            relasz = (size_t)d->d_un.d_val;
            break;
        case DT_RELAENT:
            relaent = d->d_un.d_val ? (size_t)d->d_un.d_val : sizeof(Elf32_Rela);
            break;
        case DT_JMPREL:
            jmprel = (Elf32_Rel *)(file_block + d->d_un.d_ptr);
            break;
        case DT_PLTRELSZ:
            jmprel_sz = (size_t)d->d_un.d_val;
            break;
        case DT_SYMTAB:
            symtab = (Elf32_Sym *)(file_block + d->d_un.d_ptr);
            break;
        case DT_STRTAB:
            strtab = (const char *)(file_block + d->d_un.d_ptr);
            break;
        default:
            break;
        }
    }

	print_serial("[ELF] Sections: 0x%x 0x%x 0x%x 0x%x 0x%x\n",
		rel,
		rela,
		jmprel,
		symtab,
		strtab
	);
	print_serial("[ELF] Sections: 0x%x 0x%x 0x%x 0x%x 0x%x\n",
		jmprel_sz,
		relaent,
		relasz,
		relent,
		relsz
	);

	for(int i = 1; i < ehdr->e_shnum; i++){
		Elf32_Shdr *section = elf_section(ehdr, i);
		char *name = elf_lookup_string(ehdr,section->sh_name);
		print_serial("[ELF] Section #%d \"%s\": Type: %d, Addr: 0x%x, Offs: 0x%x Size: %d\n", i, name, section->sh_type, section->sh_addr, section->sh_offset, section->sh_size);
		if(section->sh_type == PT_LOAD){
			memcpy(base + section->sh_addr, file_block + section->sh_offset, section->sh_size);
		}

	}	

	MEM_freeRegionBlock((uint32_t) file_block, fsize);
	return true;
	ELF_DYN_LOAD_FAIL:;
	MEM_freeRegionBlock((uint32_t) file_block, fsize);
	return false;
}