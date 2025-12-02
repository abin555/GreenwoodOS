#include "netfs.h"
#include "vfs.h"
#include "sysfs.h"
#include "ethernet.h"
#include "ip.h"
#include "tcp.h"
#include "udp.h"
#include "http.h"
#include "arp.h"
#include "dhcp.h"
#include "icmp.h"
#include "multitasking.h"
#include "netproc.h"
#include "utils.h"

struct NetFS_Connection NetFS_Connections[NETFS_MAXCONNECTIONS];

void split_ip(char *ip, char *ip_parts[4]) {
    int part = 0;
    char *walker = ip;

    ip_parts[part++] = walker;

    while (*walker != '\0' && part < 4) {
        if (*walker == '.') {
            *walker = '\0';             // terminate the current part
            ip_parts[part++] = walker + 1; // next part starts after dot
        }
        walker++;
    }
}

struct http_request {
    http_reply_callback callback;
    int str_len;
    char req[];
};

int netfs_http_write_spec(void *cdev, void *buf, int woffset, int nbytes, int *head){
    print_serial("[NETFS] HTTP Write Handler\n");
    struct SysFS_Chardev *sdev = cdev;
    if(sdev == NULL || buf == NULL) return 0;
    if(woffset != 0){
        print_serial("Size / offset error %d %d\n", sizeof(struct netproc_request), nbytes);
        return 0;
    }
    struct http_request *hreq = buf;

    print_serial("Request payload: %d %s\n", hreq->str_len, (char *) hreq->req);
    char *walker = (char *) hreq->req;
    char *ip = NULL;
    char *port = NULL;
    char *method = NULL;
    char *path = NULL;
    char *host = NULL;

    int part = 0;
    while(*walker != '\0'){
        if(part == 0 && *walker != ' '){
            ip = walker;
            part++;
        }
        if(part == 1 && *walker == ' '){
            *walker = '\0';
            walker++;
            part++;
            port = walker;
        }
        if(part == 2 && *walker == ' '){
            *walker = '\0';
            walker++;
            part++;
            method = walker;
        }
        if(part == 3 && *walker == ' '){
            *walker = '\0';
            walker++;
            part++;
            path = walker;
        }
        if(part == 4 && *walker == ' '){
            *walker = '\0';
            walker++;
            part++;
            host = walker;
        }

        walker++;
    }
    if(part != 5){
        print_serial("[NETFS] Incomplete HTTP request or parse error!\n");
        return 0;
    }
    print_serial("[NETFS] Parsed HTTP Request:\n%s:%s\n%s\n%s\n%s\n", ip, port, method, path, host);
    
    char *ip_parts[4];
    split_ip(ip, ip_parts);
    

    struct netproc_request request;
    request.type = NETPROC_HTTP_REQUEST;
    for(int i = 0; i < 4; i++){
        print_serial("%d - %s\n", i, ip_parts[i]);
        request.request.http_request.dst_ip[i] = atoi(ip_parts[i]);
    }
    request.request.http_request.dst_port = atoi(port);
    request.request.http_request.callback = hreq->callback;
    request.request.http_request.method = malloc(strlen(method)+1);
    memcpy(request.request.http_request.method, method, strlen(method));
    request.request.http_request.path = malloc(strlen(path)+1);
    memcpy(request.request.http_request.path, path, strlen(path));
    request.request.http_request.host = malloc(strlen(host)+1);
    memcpy(request.request.http_request.host, host, strlen(host));

    print_serial("[NETFS] Parse 2 HTTP: %d.%d.%d.%d:%d %s %s %s\n",
        request.request.http_request.dst_ip[0],
        request.request.http_request.dst_ip[1],
        request.request.http_request.dst_ip[2],
        request.request.http_request.dst_ip[3],
        request.request.http_request.dst_port,
        request.request.http_request.method,
        request.request.http_request.path,
        request.request.http_request.host
    );
    
    char *request_str;
    switch(request.type){
        case NETPROC_ICMP_ECHO_REQUEST:
            request_str = "ICMP ECHO REQUEST";
            break;
        case NETPROC_HTTP_REQUEST:
            request_str = "HTTP REQUEST";
            break;
    }
    print_serial("[NETFS] HTTP Request Made - \"%s\" from PID %d, TIDX %d\n", request_str, task_getCurrent()->pid, task_running_idx);
    print_console(task_getCurrent()->console, "WTF");
    netproc_addToQueue(task_getCurrent()->pid, request);
    
    *head = 0;

    return nbytes;
}

struct SysFS_Inode *netfs_http(){
    struct SysFS_Chardev *http_dev = sysfs_createCharDevice(NULL, 0, CDEV_READ | CDEV_WRITE);
    sysfs_setCallbacks(
        http_dev,
        NULL,
        NULL,
        netfs_http_write_spec,
        NULL
    );
    struct SysFS_Inode *http_inode = sysfs_mkcdev("http", http_dev);
    return http_inode;
}

struct NETFS_concstat {
    uint8_t ip[4];
    uint8_t subnet[4];
    uint8_t gateway[4];
    uint8_t dns[4];
};

int netfs_net_read_spec(void *cdev, void *buf, int roffset, int nbytes, int *head){
    struct SysFS_Chardev *sdev = cdev;
    if(sdev == NULL) return 0;
    struct NETFS_concstat cstat;
    struct ethernet_driver *ether = ethernet_getDriver();
    memcpy(cstat.ip, ether->ipv4.ip, sizeof(cstat.ip));
    memcpy(cstat.subnet, ether->ipv4.netmask, sizeof(cstat.subnet));
    memcpy(cstat.gateway, ether->ipv4.gateway, sizeof(cstat.gateway));
    memcpy(cstat.dns, ether->ipv4.dns, sizeof(cstat.dns));
    char *wbuf = buf;
    char *rbuf = ((char *) &cstat)+roffset;
    int i;
    for(i = 0; i < nbytes && i+roffset < (int) sizeof(cstat); i++){
        wbuf[i] = rbuf[i];
    }
    *head = 0;
    return i;
}

struct SysFS_Inode *netfs_net(){
    struct SysFS_Chardev *conn_dev = sysfs_createCharDevice(NULL, 0, CDEV_READ);
    sysfs_setCallbacks(conn_dev,
        NULL,
        NULL,
        NULL,
        netfs_net_read_spec
    );
    struct SysFS_Inode *conn_inode = sysfs_mkcdev("conn", conn_dev);
    return conn_inode;
}


int netfs_icmp_write_spec(void *cdev, void *buf, int woffset, int nbytes, int *head){
    print_serial("[NETFS] ICMP Write Handler\n");
    struct SysFS_Chardev *sdev = cdev;
    if(sdev == NULL || buf == NULL) return 0;
    if(nbytes != sizeof(struct netproc_request) || woffset != 0){
        print_serial("Size / offset error %d %d\n", sizeof(struct netproc_request), nbytes);
    }
    struct netproc_request *request = buf;
    char *request_str;
    switch(request->type){
        case NETPROC_ICMP_ECHO_REQUEST:
            request_str = "ICMP ECHO REQUEST";
            break;
        case NETPROC_HTTP_REQUEST:
            request_str = "HTTP REQUEST";
            break;
    }
    print_serial("[NETFS] ICMP Request Made - \"%s\" from PID %d\n", request_str, task_running_idx);
    netproc_addToQueue(task_running_idx, *request);
    *head = 0;

    return nbytes;
}

struct SysFS_Inode *netfs_icmp(){
    struct SysFS_Chardev *icmp_dev = sysfs_createCharDevice(NULL, 0, CDEV_READ | CDEV_WRITE);
    sysfs_setCallbacks(
        icmp_dev,
        NULL,
        NULL,
        netfs_icmp_write_spec,
        NULL
    );
    struct SysFS_Inode *icmp_inode = sysfs_mkcdev("icmp", icmp_dev);
    return icmp_inode;
}

int netfs_freeConnection(struct NetFS_Connection *conn){
    if(conn == NULL) return 0;
    conn->active = 0;
    conn->pending = 0;
    conn->owner_pid = -1;
    if(conn->buffer != NULL){
        MEM_freeRegionBlock((uint32_t) conn->buffer, conn->buf_size);
        conn->buffer = NULL;
    }
    conn->buf_size = 0;
    conn->read_head = NULL;
    conn->write_head = NULL;
    conn->type = NETFS_Connection_None;
    return 1;
}

struct NetFS_Connection *netfs_allocConnection(int type, int requestor_pid){
    for(int i = 0; i < NETFS_MAXCONNECTIONS; i++){
        if(NetFS_Connections[i].active == 0){
            struct NetFS_Connection *conn_dev = &NetFS_Connections[i];
            conn_dev->active = 1;
            conn_dev->owner_pid = requestor_pid;
            conn_dev->buf_size = PAGE_SIZE;
            conn_dev->buffer = (char *) MEM_reserveRegionBlock(MEM_findRegionIdx(conn_dev->buf_size), conn_dev->buf_size, 0, OTHER);
            conn_dev->read_head = conn_dev->buffer;
            conn_dev->write_head = conn_dev->buffer;
            conn_dev->type = type;
            print_serial("[NETFS] Allocated Connection %d, buffer @ 0x%x\n", i, conn_dev->buffer);
            return conn_dev;
        }
    }
    return NULL;
}

int netfs_connectionFill(struct NetFS_Connection *conn, char *buf, size_t bufsize){
    if(conn == NULL || buf == NULL) return -1;
    print_serial("[NETFS] Filling connection %d buffer!\nBuffer Base @ 0x%x\nBuffer Write @ 0x%x\n", conn->cid, conn->buffer, conn->write_head);
    memcpy(conn->write_head, buf, bufsize);
    conn->write_head += bufsize;
    return bufsize;
}

int netfs_conn_write_spec(void *cdev, void *buf, int woffset, int nbytes, int *head){
    struct SysFS_Chardev *conndev = cdev;
    struct NetFS_Connection *conn = (struct NetFS_Connection *) conndev->buf;
    print_serial("[NETFS] Write to connection %d from 0x%x:%d of %d @ 0x%x\n", conn->cid, buf, woffset, nbytes, *head);
    return 0;
}

int netfs_conn_read_spec(void *cdev, void *buf, int roffset, int nbytes, int *head){
    struct SysFS_Chardev *conndev = cdev;
    struct NetFS_Connection *conn = (struct NetFS_Connection *) conndev->buf;
    print_serial("[NETFS] Read from connection %d from 0x%x:%d of %d @ 0x%x\n", conn->cid, buf, roffset, nbytes, *head);
    if(buf == NULL) return 0;
    size_t avail = conn->write_head - conn->buffer;
    if((size_t) (roffset + nbytes) > avail) nbytes = avail - roffset;
    memcpy(buf, conn->buffer + roffset, nbytes);
    *head += nbytes;
    return nbytes;
}

int netfs_conn_seek(void *vfs_file, void *cdev, int offset, int whence){
    if(cdev == NULL || vfs_file == NULL) return -1;
    struct VFS_File *file_idx = vfs_file;
    struct SysFS_Chardev *conndev = cdev;
    struct NetFS_Connection *conn = (struct NetFS_Connection *) conndev->buf;

    print_serial("[NETFS] Connection seek in connection %d\n", conn->cid);

    if(whence == 0){//SEEK_SET
        file_idx->head = offset;
    }
    else if(whence == 1){//SEEK_CUR
        file_idx->head += offset;
    }
    else if(whence == 2){//SEEK_END
        file_idx->head = conn->write_head - conn->buffer + offset;
        if(file_idx->head > conn->write_head - conn->buffer)
            file_idx->head = conn->write_head - conn->buffer;
    }
    else{
        return -1;
    }
	return file_idx->head;
}

int netfs_conn_stat(void *cdev, void *statbuf){
    if(cdev == NULL) return -1;
    if(statbuf == NULL) return -1;
    struct SysFS_Chardev *conndev = cdev;
    struct NetFS_Connection *conn = (struct NetFS_Connection *) conndev->buf;
    struct VFS_stat *stat = statbuf;

    
    memcpy(stat->fs_ownerIden, "NETFS", 6);
    stat->open_stat = conn->active;
    if(stat->open_stat)
        stat->size = conn->write_head - conn->buffer;
    else
        stat->size = 0;
    return 0;
};

struct SysFS_Inode *netfs_makeConnections(){
    struct SysFS_Inode *conn_folder = sysfs_mkdir("C");
    for(int i = 0; i < NETFS_MAXCONNECTIONS; i++){
        char conn_filename[10];
        memset(conn_filename, 0, sizeof(conn_filename));
        snprintf(conn_filename, sizeof(conn_filename), "conn%d", i);
        struct NetFS_Connection *conn_dev = &NetFS_Connections[i];
        netfs_freeConnection(conn_dev);
        conn_dev->cid = i;
        struct SysFS_Chardev *cdev = sysfs_createCharDevice((char *) conn_dev, sizeof(NetFS_Connections[0]), CDEV_READ | CDEV_WRITE);
        sysfs_setCallbacks(
            cdev,
            NULL,
            NULL,
            netfs_conn_write_spec,
            netfs_conn_read_spec
        );
        sysfs_setCallbacksExtra(
            cdev,
            netfs_conn_seek,
            netfs_conn_stat
        );
        struct SysFS_Inode *cdev_inode = sysfs_mkcdev(conn_filename, cdev);
        sysfs_addChild(conn_folder, cdev_inode);
    }
    return conn_folder;
}

void netfs_init(){
    print_serial("[NETFS] Init\n");
    struct VFS_Inode *vfs_sysroot = vfs_findRoot('-');
    struct SysFS_Inode *sysfs = vfs_sysroot->root->interface->root;

    struct SysFS_Inode *netdir = sysfs_mkdir("net");
    sysfs_addChild(sysfs, netdir);
    sysfs_addChild(netdir, netfs_http());
    sysfs_addChild(netdir, netfs_net());
    sysfs_addChild(netdir, netfs_icmp());
    sysfs_addChild(netdir, netfs_makeConnections());
}