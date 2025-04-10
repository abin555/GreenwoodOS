#ifndef NETFS_H
#define NETFS_H

#include "stdint.h"

typedef enum {
    NETFS_ICMP,
    NETFS_HTTP
} NetFS_Connection_Type;

struct NetFS_File {
    char *name;
    int len;
    char *body;
};

struct NetFS_Connection_HTTP {
    uint16_t port;
    uint8_t dst_ip[4];

    struct NetFS_File url;
    struct NetFS_File header;
    struct NetFS_File body;
};

struct NetFS_Connection {
    NetFS_Connection_Type type;
    int cid;
    int origin_pid;
    union {
        struct NetFS_Connection_HTTP http;
    };
};

struct NetFS_CTRL {
    int (*write_callback)(void *ctrl, void *buf, int woffset, int nbytes, int *head);
    int (*read_callback)(void *ctrl, void *buf, int roffset, int nbytes, int *head);
};

#define NETFS_MAX_CONNECTIONS 10

struct NetFS_System {
    int numConnections;
    struct {
        char isFree;
        struct NetFS_Connection conn;
    } connections[NETFS_MAX_CONNECTIONS];
    struct NetFS_CTRL ctrl_file;
};

struct NetFS_Inode {
    enum {
        NETFS_Inode_Type_ROOT,
        NETFS_Inode_Type_FILE,
        NETFS_Inode_Type_CTRL
    } type;
    union {
        struct NetFS_System *root;
        struct NetFS_File *file;
        struct NetFS_CTRL *ctrl;
    };
};

void netfs_init();

struct NetFS_Inode *netfs_createRoot();

struct NetFS_Inode *netfs_find(struct NetFS_Inode *root, char *path);

struct VFS_File;
int netfs_read(struct VFS_File *file, void *buf, int nbytes);
int netfs_write(struct VFS_File *file, void *buf, int nbytes);
struct DirectoryListing netfs_advListDirectory(struct NetFS_Inode *sysfs, char *path);

#endif