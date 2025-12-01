#ifndef NETFS_H
#define NETFS_H

#include "stdint.h"

struct NetFS_Connection {
    int cid;

    int active;
    int pending;

    int owner_pid;
    char *buffer;
    size_t buf_size;
    char *read_head;
    char *write_head;

    enum {
        NETFS_Connection_None,
        NETFS_Connection_HTTP
    } type;
};
#define NETFS_MAXCONNECTIONS 10

void netfs_init();
int netfs_freeConnection(struct NetFS_Connection *conn);
struct NetFS_Connection *netfs_allocConnection(int type, int requestor_pid);

#endif