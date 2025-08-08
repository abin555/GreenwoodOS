#include <stdio.h>
#include <stddef.h>
#include <sys/memory.h>
#include <sys/task.h>
#include <stdint.h>
#include <sys/io.h>

typedef uint16_t Elf32_Half;	// Unsigned half int
typedef uint32_t Elf32_Off;	// Unsigned offset
typedef uint32_t Elf32_Addr;	// Unsigned address
typedef uint32_t Elf32_Word;	// Unsigned int
typedef int32_t  Elf32_Sword;	// Signed int

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

char elf_load(int file, void *buffer){
	if(file == -1) return 0;

	lseek(file, 0, 0);
	Elf32_Ehdr hdr;
	read(file, (char *) &hdr, sizeof(Elf32_Ehdr));

	for(int i = 1; i < hdr.e_shnum; i++){
		if(!elf_load_section(file, hdr, i, buffer)){
			break;
		}
	}
	return 1;
}

int main(int argc, char **argv){
    printf("Hello from Loader!\n");
    int service_fd = open("/A/demos/service/service.elf", O_READ);
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
        manual_task(service_address + entry, "Service");
    }
    else{
        printf("Not valid ELF file!\n");
        close(service_fd);
        return 1;
    }
    return 0;
    //manual_task(service_address + 0x84, NULL);
}