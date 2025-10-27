#ifndef SYS_STAT_H
#define SYS_STAT_H

int mkdir(const char *pathname, int mode);

struct stat {
    char fs_ownerIden[10];
    int open_stat;
    uint32_t size;
};

#endif