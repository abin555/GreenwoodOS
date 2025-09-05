#include <stdio.h>
#include <stddef.h>
#include <sys/memory.h>
#include <sys/task.h>
#include <stdint.h>
#include <sys/io.h>
#include <string.h>

typedef uint16_t Elf32_Half;	// Unsigned half int
typedef uint32_t Elf32_Off;	// Unsigned offset
typedef uint32_t Elf32_Addr;	// Unsigned address
typedef uint32_t Elf32_Word;	// Unsigned int
typedef int32_t  Elf32_Sword;	// Signed int
typedef uint16_t Elf32_Section;

#define ELF_NIDENT	16
 
typedef struct {
	uint8_t		e_ident[ELF_NIDENT];
	Elf32_Half	e_type;
	Elf32_Half	e_machine;
	Elf32_Word	e_version;
	Elf32_Addr	e_entry;
	Elf32_Off	e_phoff;
	Elf32_Off	e_shoff;
	Elf32_Word	e_flags;
	Elf32_Half	e_ehsize;
	Elf32_Half	e_phentsize;
	Elf32_Half	e_phnum;
	Elf32_Half	e_shentsize;
	Elf32_Half	e_shnum;
	Elf32_Half	e_shstrndx;
} Elf32_Ehdr;

enum Elf_Ident {
	EI_MAG0		= 0, // 0x7F
	EI_MAG1		= 1, // 'E'
	EI_MAG2		= 2, // 'L'
	EI_MAG3		= 3, // 'F'
	EI_CLASS	= 4, // Architecture (32/64)
	EI_DATA		= 5, // Byte Order
	EI_VERSION	= 6, // ELF Version
	EI_OSABI	= 7, // OS Specific
	EI_ABIVERSION	= 8, // OS Specific
	EI_PAD		= 9  // Padding
};
 
#define ELFMAG0	0x7F // e_ident[EI_MAG0]
#define ELFMAG1	'E'  // e_ident[EI_MAG1]
#define ELFMAG2	'L'  // e_ident[EI_MAG2]
#define ELFMAG3	'F'  // e_ident[EI_MAG3]
 
#define ELFDATA2LSB	(1)  // Little Endian
#define ELFCLASS32	(1)  // 32-bit Architecture

enum Elf_Type {
	ET_NONE		= 0, // Unkown Type
	ET_REL		= 1, // Relocatable File
	ET_EXEC		= 2, // Executable File
	ET_DYN		= 3, // Shared Object File
	ET_CORE 	= 4  // Core File
};

#define EM_386		(3)  // x86 Machine Type
#define EV_CURRENT	(1)  // ELF Current Version

typedef struct {
	Elf32_Word sh_name;
	Elf32_Word sh_type;
	Elf32_Word sh_flags;
	Elf32_Addr sh_addr;
	Elf32_Off sh_offset;
	Elf32_Word sh_size;
	Elf32_Word sh_link;
	Elf32_Word sh_info;
	Elf32_Word sh_addralign;
	Elf32_Word sh_entsize;
} Elf32_Shdr;

/* Legal values for sh_type (section type).  */

#define SHT_NULL          0                /* Section header table entry unused */
#define SHT_PROGBITS          1                /* Program data */
#define SHT_SYMTAB          2                /* Symbol table */
#define SHT_STRTAB          3                /* String table */
#define SHT_RELA          4                /* Relocation entries with addends */
#define SHT_HASH          5                /* Symbol hash table */
#define SHT_DYNAMIC          6                /* Dynamic linking information */
#define SHT_NOTE          7                /* Notes */
#define SHT_NOBITS          8                /* Program space with no data (bss) */
#define SHT_REL                  9                /* Relocation entries, no addends */
#define SHT_SHLIB          10                /* Reserved */
#define SHT_DYNSYM          11                /* Dynamic linker symbol table */
#define SHT_INIT_ARRAY          14                /* Array of constructors */
#define SHT_FINI_ARRAY          15                /* Array of destructors */
#define SHT_PREINIT_ARRAY 16                /* Array of pre-constructors */
#define SHT_GROUP          17                /* Section group */
#define SHT_SYMTAB_SHNDX  18                /* Extended section indeces */
#define        SHT_NUM                  19                /* Number of defined types.  */
#define SHT_LOOS          0x60000000        /* Start OS-specific.  */
#define SHT_GNU_ATTRIBUTES 0x6ffffff5        /* Object attributes.  */
#define SHT_GNU_HASH          0x6ffffff6        /* GNU-style hash table.  */
#define SHT_GNU_LIBLIST          0x6ffffff7        /* Prelink library list */
#define SHT_CHECKSUM          0x6ffffff8        /* Checksum for DSO content.  */
#define SHT_LOSUNW          0x6ffffffa        /* Sun-specific low bound.  */
#define SHT_SUNW_move          0x6ffffffa
#define SHT_SUNW_COMDAT   0x6ffffffb
#define SHT_SUNW_syminfo  0x6ffffffc
#define SHT_GNU_verdef          0x6ffffffd        /* Version definition section.  */
#define SHT_GNU_verneed          0x6ffffffe        /* Version needs section.  */
#define SHT_GNU_versym          0x6fffffff        /* Version symbol table.  */
#define SHT_HISUNW          0x6fffffff        /* Sun-specific high bound.  */
#define SHT_HIOS          0x6fffffff        /* End OS-specific type */
#define SHT_LOPROC          0x70000000        /* Start of processor-specific */
#define SHT_HIPROC          0x7fffffff        /* End of processor-specific */
#define SHT_LOUSER          0x80000000        /* Start of application-specific */
#define SHT_HIUSER          0x8fffffff        /* End of application-specific */

#define SHN_UNDEF	(0x00) // Undefined/Not Present
#define SHN_ABS    0xFFF1 // Absolute symbol

/* Program segment header.  */

typedef struct
{
  Elf32_Word        p_type;                        /* Segment type */
  Elf32_Off        p_offset;                /* Segment file offset */
  Elf32_Addr        p_vaddr;                /* Segment virtual address */
  Elf32_Addr        p_paddr;                /* Segment physical address */
  Elf32_Word        p_filesz;                /* Segment size in file */
  Elf32_Word        p_memsz;                /* Segment size in memory */
  Elf32_Word        p_flags;                /* Segment flags */
  Elf32_Word        p_align;                /* Segment alignment */
} Elf32_Phdr;

/* Legal values for p_type (segment type).  */

#define        PT_NULL                0                /* Program header table entry unused */
#define PT_LOAD                1                /* Loadable program segment */
#define PT_DYNAMIC        2                /* Dynamic linking information */
#define PT_INTERP        3                /* Program interpreter */
#define PT_NOTE                4                /* Auxiliary information */
#define PT_SHLIB        5                /* Reserved */
#define PT_PHDR                6                /* Entry for header table itself */
#define PT_TLS                7                /* Thread-local storage segment */
#define        PT_NUM                8                /* Number of defined types */

typedef struct
{
  Elf32_Addr        r_offset;                /* Address */
  Elf32_Word        r_info;                        /* Relocation type and symbol index */
} Elf32_Rel;

# define ELF32_R_SYM(INFO)	((INFO) >> 8)
# define ELF32_R_TYPE(INFO)	((uint8_t)(INFO))

/* Symbol table entry.  */

typedef struct
{
  Elf32_Word        st_name;                /* Symbol name (string tbl index) */
  Elf32_Addr        st_value;                /* Symbol value */
  Elf32_Word        st_size;                /* Symbol size */
  unsigned char        st_info;                /* Symbol type and binding */
  unsigned char        st_other;                /* Symbol visibility */
  Elf32_Section        st_shndx;                /* Section index */
} Elf32_Sym;

/* i386 relocs.  */

#define R_386_NONE           0                /* No reloc */
#define R_386_32           1                /* Direct 32 bit  */
#define R_386_PC32           2                /* PC relative 32 bit */
#define R_386_GOT32           3                /* 32 bit GOT entry */
#define R_386_PLT32           4                /* 32 bit PLT address */
#define R_386_COPY           5                /* Copy symbol at runtime */
#define R_386_GLOB_DAT           6                /* Create GOT entry */
#define R_386_JMP_SLOT           7                /* Create PLT entry */
#define R_386_RELATIVE           8                /* Adjust by program base */
#define R_386_GOTOFF           9                /* 32 bit offset to GOT */
#define R_386_GOTPC           10                /* 32 bit PC relative offset to GOT */
#define R_386_32PLT           11
#define R_386_TLS_TPOFF           14                /* Offset in static TLS block */
#define R_386_TLS_IE           15                /* Address of GOT entry for static TLS
                                           block offset */
#define R_386_TLS_GOTIE           16                /* GOT entry for static TLS block
                                           offset */
#define R_386_TLS_LE           17                /* Offset relative to static TLS
                                           block */
#define R_386_TLS_GD           18                /* Direct 32 bit for GNU version of
                                           general dynamic thread local data */
#define R_386_TLS_LDM           19                /* Direct 32 bit for GNU version of
                                           local dynamic thread local data
                                           in LE code */
#define R_386_16           20
#define R_386_PC16           21
#define R_386_8                   22
#define R_386_PC8           23
#define R_386_TLS_GD_32           24                /* Direct 32 bit for general dynamic
                                           thread local data */
#define R_386_TLS_GD_PUSH  25                /* Tag for pushl in GD TLS code */
#define R_386_TLS_GD_CALL  26                /* Relocation for call to
                                           __tls_get_addr() */
#define R_386_TLS_GD_POP   27                /* Tag for popl in GD TLS code */
#define R_386_TLS_LDM_32   28                /* Direct 32 bit for local dynamic
                                           thread local data in LE code */
#define R_386_TLS_LDM_PUSH 29                /* Tag for pushl in LDM TLS code */
#define R_386_TLS_LDM_CALL 30                /* Relocation for call to
                                           __tls_get_addr() in LDM code */
#define R_386_TLS_LDM_POP  31                /* Tag for popl in LDM TLS code */
#define R_386_TLS_LDO_32   32                /* Offset relative to TLS block */
#define R_386_TLS_IE_32           33                /* GOT entry for negated static TLS
                                           block offset */
#define R_386_TLS_LE_32           34                /* Negated offset relative to static
                                           TLS block */
#define R_386_TLS_DTPMOD32 35                /* ID of module containing symbol */
#define R_386_TLS_DTPOFF32 36                /* Offset in TLS block */
#define R_386_TLS_TPOFF32  37                /* Negated offset in static TLS block */
/* 38? */
#define R_386_TLS_GOTDESC  39                /* GOT offset for TLS descriptor.  */
#define R_386_TLS_DESC_CALL 40                /* Marker of call through TLS
                                           descriptor for
                                           relaxation.  */
#define R_386_TLS_DESC     41                /* TLS descriptor containing
                                           pointer to code and to
                                           argument, returning the TLS
                                           offset for the symbol.  */
#define R_386_IRELATIVE           42                /* Adjust indirectly by program base */
/* Keep this the last entry.  */
#define R_386_NUM           43

char elf_check_file(Elf32_Ehdr hdr) {
	//if(!hdr) return 0;
	if(hdr.e_ident[EI_MAG0] != ELFMAG0) {
		printf("ELF Header EI_MAG0 incorrect.\n");
		return 0;
	}
	if(hdr.e_ident[EI_MAG1] != ELFMAG1) {
		printf("ELF Header EI_MAG1 incorrect.\n");
		return 0;
	}
	if(hdr.e_ident[EI_MAG2] != ELFMAG2) {
		printf("ELF Header EI_MAG2 incorrect.\n");
		return 0;
	}
	if(hdr.e_ident[EI_MAG3] != ELFMAG3) {
		printf("ELF Header EI_MAG3 incorrect.\n");
		return 0;
	}
	//printf("[ELF] Successful Header Match!\n");
	return 1;
}

char elf_check_supported(int file) {
	//printf("[ELF] Checking Support\n");
	int head = lseek(file, 0, 1);
	lseek(file, 0, 0);
	Elf32_Ehdr hdr;
	read(file, (char *) &hdr, sizeof(Elf32_Ehdr));
	lseek(file, head, 0);

	if(!elf_check_file(hdr)) {
		printf("[ELF] Invalid ELF File.\n");
		return 0;
	}
	if(hdr.e_ident[EI_CLASS] != ELFCLASS32) {
		printf("[ELF] Unsupported ELF File Class.\n");
		return 0;
	}
	if(hdr.e_ident[EI_DATA] != ELFDATA2LSB) {
		printf("[ELF] Unsupported ELF File byte order.\n");
		return 0;
	}
	if(hdr.e_machine != EM_386) {
		printf("[ELF] Unsupported ELF File target.\n");
		return 0;
	}
	if(hdr.e_ident[EI_VERSION] != EV_CURRENT) {
		printf("[ELF] Unsupported ELF File version.\n");
		return 0;
	}
	//printf("%d\n", hdr.e_type);
	if(hdr.e_type != ET_REL && hdr.e_type != ET_EXEC) {
		printf("[ELF] Unsupported ELF File type.\n");
		return 0;
	}

	return 1;
}

uint32_t elf_get_entry_addr(int file){
	if(file == -1) return 0;
	int head = lseek(file, 0, 1);
	lseek(file, 0, 0);
	Elf32_Ehdr hdr;
	read(file, (char *) &hdr, sizeof(Elf32_Ehdr));
	lseek(file, head, 0);
	//printf("Entry Point Address: 0x%x\n", hdr.e_entry);
	return hdr.e_entry;
}

char elf_load_section(int file, Elf32_Ehdr hdr, int sectionIdx, void *buffer){
	if(file == -1) return 0;
	lseek(file, 0, 0);

	Elf32_Shdr shdr;
	lseek(file, hdr.e_shoff+hdr.e_shentsize*sectionIdx, 0);
	read(file, (char *) &shdr, hdr.e_shentsize);	

	if(shdr.sh_type == 1){	
		/*
		printf("[ELF] Section Load %d\n Name Idx: %d\n Type: %d\n Flags: %d\n Addr: %d\n Offset: %d\n Size: %d\n",
			sectionIdx,
			shdr.sh_name,
			shdr.sh_type,
			shdr.sh_flags,
			shdr.sh_addr,
			shdr.sh_offset,
			shdr.sh_size
		);
		*/
		lseek(file, shdr.sh_offset, 0);
		read(file, ((char *) buffer) + shdr.sh_addr, shdr.sh_size);
		lseek(file, 0, 0);
		return 1;
	}
	else{
		//printf("[ELF] Section %d is not PROGBITS\n", sectionIdx);
		return 0;
	}
	return 0;
}

char elf_load_program_header(int file, Elf32_Ehdr hdr, int headerIdx, void *buffer){
	if(file == -1) return 0;
	lseek(file, 0, 0);

	Elf32_Phdr phdr;
	lseek(file, hdr.e_phoff+hdr.e_phentsize*headerIdx, 0);
	read(file, (char *) &phdr, hdr.e_phentsize);

	printf("Header #%d Type: %d 0x%x 0x%x 0x%x 0x%x\n",
		headerIdx,
		phdr.p_type,
		phdr.p_offset,
		phdr.p_vaddr,
		phdr.p_paddr,
		phdr.p_filesz
	);

	if(phdr.p_type == PT_LOAD){
		lseek(file, phdr.p_offset, 0);
		read(file, ((char *) buffer) + phdr.p_vaddr, phdr.p_filesz);
		lseek(file, 0, 0);
		return 1;
	}
	return 0;
}

uint32_t elf_get_sym_val(int file, Elf32_Ehdr hdr, int table, uint32_t idx){
	if(table == SHN_UNDEF || idx == SHN_UNDEF) return 0;
	Elf32_Shdr shdr;
	lseek(file, hdr.e_shoff+hdr.e_shentsize*table, 0);
	read(file, (char *) &shdr, hdr.e_shentsize);

	uint32_t symtab_entries = shdr.sh_size / shdr.sh_entsize;
	if(idx >= symtab_entries) {
		printf("ERROR: Symbol index out of range! (%d,%d)\n", idx, symtab_entries);
		return 0;
	}
	Elf32_Sym symbol;
	lseek(file, shdr.sh_offset+(sizeof(symbol)*idx), 0);
	read(file, (char *) &symbol, sizeof(symbol));

	if(symbol.st_shndx == SHN_UNDEF){
		//printf("Error: External Symbol %d %d\n", table, idx);
		return 0;
	}
	else{
		return symbol.st_value;
	}

}

# define DO_386_32(S, A)	((S) + (A))
# define DO_386_PC32(S, A, P)	((S) + (A) - (P))

char elf_do_reloc(int file, Elf32_Ehdr hdr, Elf32_Shdr shdr, void *buffer){
	if(file == -1) return 0;
	int num_rels = shdr.sh_size / sizeof(Elf32_Rel);
	for(int i = 0; i < num_rels; i++){
		Elf32_Rel rel;
		lseek(file, shdr.sh_offset + (sizeof(Elf32_Rel) * i), 0);
		read(file, (char *) &rel, sizeof(Elf32_Rel));
		
		Elf32_Shdr target_sh;
		lseek(file, hdr.e_shoff+hdr.e_shentsize*shdr.sh_info, 0);
		read(file, (char *) &target_sh, sizeof(target_sh));
		
		uint32_t symval = 0;
		if(ELF32_R_SYM(rel.r_info) != SHN_UNDEF){
			symval = elf_get_sym_val(file, hdr, shdr.sh_link, ELF32_R_SYM(rel.r_info));
		}
		
		//printf("Reloc #%d - 0x%x 0x%x Symval: 0x%x\n", i, rel.r_offset, rel.r_info, symval);

		uint32_t offset;
		uint32_t *ref = buffer + rel.r_offset;

		switch(ELF32_R_TYPE(rel.r_info)){
			case R_386_NONE:
				break;
			case R_386_32:
				offset = DO_386_32(symval, (uint32_t) buffer);
				//printf("Reloc 0x%x @ 0x%x from 0x%x (0x%x) to 0x%x\n", rel.r_offset, ref, symval, *ref, offset);
				*ref = offset;
				break;
			default:
				break;
		}
	}
	return 1;
}

char elf_handle_section(int file, Elf32_Ehdr hdr, int sectionIdx, void *buffer){
	if(file == -1) return 0;
	lseek(file, 0, 0);

	Elf32_Shdr shdr;
	lseek(file, hdr.e_shoff+hdr.e_shentsize*sectionIdx, 0);
	read(file, (char *) &shdr, hdr.e_shentsize);

	printf("Section #%d Type: %d 0x%x 0x%x 0x%x\n", sectionIdx, shdr.sh_type, shdr.sh_addr, shdr.sh_offset, shdr.sh_size);

	switch(shdr.sh_type){
		case SHT_REL:
			return elf_do_reloc(file, hdr, shdr, buffer);
			break;
		default:
			return 0;
	}
}

char elf_load(int file, void *buffer){
	if(file == -1) return 0;

	lseek(file, 0, 0);
	Elf32_Ehdr hdr;
	read(file, (char *) &hdr, sizeof(Elf32_Ehdr));

	for(int i = 0; i < hdr.e_phnum; i++){
		elf_load_program_header(file, hdr, i, buffer);
	}

	for(int i = 0; i < hdr.e_shnum; i++){
		elf_handle_section(file, hdr, i, buffer);
	}

	return 1;
}

extern struct FILE **fileListing;

int main(int argc, char **argv){
	if(argc < 2){
		printf("Loader Error! No provided binary!\n");
		return 1;
	}	
	char wait = 0;
	if(argc == 3){
		if(!strcmp(argv[2], "-w")){
			wait = 1;
		}
	}
    printf("[Loader] Loading \"%s\"\n", argv[1]);
    int service_fd = open(argv[1], O_READ);
    if(service_fd == -1){
        printf("Service file not found!\n");
        return 1;
    }
    int service_size = lseek(service_fd, 0, SEEK_END);
    lseek(service_fd, 0, SEEK_SET);
    void *service_address = memory_requestRegion(service_size);
    printf("Loading to 0x%x %d bytes\n", service_address, service_size);
    if(elf_check_supported(service_fd)){
        uint32_t entry = elf_get_entry_addr(service_fd);
		elf_load(service_fd, service_address);
        close(service_fd);
        //*((uint32_t *) (service_address + 0x365c)) = (uint32_t) service_address;

        int pid = manual_task(service_address + entry, "Service");
		if(wait){
			waitpid(pid);
		}
    }
    else{
        printf("Not valid ELF file!\n");
        close(service_fd);
        return 1;
    }
    return 0;
    //manual_task(service_address + 0x84, NULL);
}