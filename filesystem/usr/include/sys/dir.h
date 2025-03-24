#ifndef SYS_DIR_H
#define SYS_DIR_H

#include <stdint.h>

struct DirectoryEntry{
	char filename[50];
	int name_len;
	uint32_t type;
};

struct DirectoryListing{
	char *directory_path;
	int directory_path_len;
	struct DirectoryEntry *entries;
	int num_entries;
};

struct DirectoryListing getDirectoryListing(char *path);

int changeDirectory(char *change);
char *getDirectory();
void printDirectoryContents(char *path);
void creatdir(char *path);

#endif