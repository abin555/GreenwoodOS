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

struct SysFS_Inode *netfs_http(){
    struct SysFS_Chardev *http_dev = sysfs_createCharDevice(NULL, 0, CDEV_READ | CDEV_WRITE);
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
    if(nbytes != sizeof(struct netproc_request) || woffset != 0) return 0;
    struct netproc_request *request = buf;
    char *request_str;
    switch(request->type){
        case NETPROC_ICMP_ECHO_REQUEST:
            request_str = "ICMP ECHO REQUEST";
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
    struct SysFS_Inode *sysfs;
    if(vfs_sysroot->type == VFS_SYS){
        sysfs = vfs_sysroot->fs.sysfs;
    }
    else{
        return;
    }

    struct SysFS_Inode *netdir = sysfs_mkdir("net");
    sysfs_addChild(sysfs, netdir);
    sysfs_addChild(netdir, netfs_http());
    sysfs_addChild(netdir, netfs_conn());
    //sysfs_addChild(netdir, netfs_icmp());
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

    if(!strcmp(path, "ctrl") && root == vfs_findRoot('@')->fs.netfs){
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