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

struct icmp_queue {
    
} netfs_icmp_queue;

struct SysFS_Inode *netfs_icmp(){
    struct SysFS_Chardev *icmp_dev = sysfs_createCharDevice(NULL, 0, CDEV_READ);
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
    sysfs_addChild(netdir, netfs_icmp());
}