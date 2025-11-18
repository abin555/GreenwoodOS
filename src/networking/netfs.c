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

struct NetFS_System netfs_system;

#define MAX_QUEUE_ENTRIES 10

struct NETFS_programCallback {
    int pid;
    void *callback;
};

struct NETFS_http_request {
    
};

struct NETFS_http_response {

};

int atoi(const char *arr){
    int val = 0;
    char neg = 0;
    if(*arr == '-'){
      arr++;
      neg = 1;
    }
    while(*arr != 0 && *arr != ' ' && *arr != '\n'){
      val = val * 10 + (*arr - '0');
      arr++;
    }
    //print_arg("ATOI: %d\n", val);
    if(neg) return -val;
    return val;
}

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
    print_serial("[NETFS] HTTP Request Made - \"%s\" from PID %d\n", request_str, task_running_idx);
    netproc_addToQueue(task_running_idx, request);
    
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

int netfs_conn_read_spec(void *cdev, void *buf, int roffset, int nbytes, int *head){
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



struct SysFS_Inode *netfs_conn(){
    struct SysFS_Chardev *conn_dev = sysfs_createCharDevice(NULL, 0, CDEV_READ);
    sysfs_setCallbacks(conn_dev,
        NULL,
        NULL,
        NULL,
        netfs_conn_read_spec
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


void netfs_init(){
    print_serial("[NETFS] Init\n");
    struct VFS_Inode *vfs_sysroot = vfs_findRoot('-');
    struct SysFS_Inode *sysfs = vfs_sysroot->root->interface->root;

    struct SysFS_Inode *netdir = sysfs_mkdir("net");
    sysfs_addChild(sysfs, netdir);
    sysfs_addChild(netdir, netfs_http());
    sysfs_addChild(netdir, netfs_conn());
    sysfs_addChild(netdir, netfs_icmp());
    sysfs_addChild(netdir, sysfs_mkdir("conns"));
}


struct NetFS_Inode *netfs_createRoot(){
    struct NetFS_Inode *root = malloc(sizeof(struct NetFS_Inode));
    root->type = NETFS_Inode_Type_ROOT;
    root->root = &netfs_system;
    root->root->numConnections = 0;
    memset(root->root->connections, 0, sizeof(root->root->connections));
    return root;
}

struct NetFS_Inode *netfs_find(struct NetFS_Inode *root, char *path){
    print_serial("[NETFS] Looking for %s\n", path);
    if(!strcmp(path, ".")) return root;

    if(!strcmp(path, "ctrl") && root == vfs_findRoot('@')->fs.fs){
        print_serial("[NETFS] Looking for CTRL file\n");
        //return root->ctrl;
    }
    return root;
}

struct DirectoryListing netfs_advListDirectory(struct NetFS_Inode *netfs, char *path){
    struct DirectoryListing listing = {0};
    if(netfs == NULL || path == NULL) return listing;
    struct NetFS_Inode *target = netfs_find(netfs, path);
    if(target == NULL) return listing;

    //if(target->type != NETFS_Inode_Type_FILE) return listing;

    listing.directory_path_len = strlen(path);
	listing.directory_path = strdup(path);
	listing.num_entries = 3;
	listing.entries = malloc(sizeof(struct DirectoryEntry) * listing.num_entries);
	memset(listing.entries, 0, sizeof(struct DirectoryEntry) * listing.num_entries);

    const char *dotfiles[3] = {
        ".",
        "..",
        "ctrl"
    };
    int dotfileTypes[3] = {
        ENTRY_DIRECTORY,
        ENTRY_DIRECTORY,
        ENTRY_FILE
    };
    for(int i = 0; i < 3; i++){
        memset(listing.entries[i].filename, 0, 50);
        memcpy(listing.entries[i].filename, (void *) dotfiles[i], sizeof(dotfiles[i])); 
        listing.entries[i].name_len = sizeof(dotfiles[i]);       
        listing.entries[i].type = dotfileTypes[i];
        print_serial("%s\n", listing.entries[i].filename);
    }

    return listing;
}