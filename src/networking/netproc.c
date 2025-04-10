#include "netproc.h"
#include "serial.h"
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

#define NETACTION_QUEUE_LEN 100

int netproc_queue_needs_attention;

struct NETPROC_QUEUE {
    struct netproc_req_entry {
        char pending;
        char free;
        int pid;
        struct netproc_request request;
    } entries[NETACTION_QUEUE_LEN];
} netproc_queue;

int netproc_pend_needs_attention;

struct NETPROC_PENDING {
    struct netproc_pend_entry {
        char free;
        int pid;
        struct netproc_request request;
        char has_reply;
        union {
            struct netproc_icmp_reply{
                uint8_t source_ip[4];
                unsigned int packet_size;
                struct icmp_packet *packet;
            } icmp_reply;
        } reply;
    } entries[NETACTION_QUEUE_LEN];
} netproc_pending;

int netproc_addToQueue(int caller_pid, struct netproc_request request){
    for(int i = 0; i < NETACTION_QUEUE_LEN; i++){
        struct netproc_req_entry *req = &netproc_queue.entries[i];
        if(req->free && !req->pending){
            req->free = 0;
            req->pending = 1;
            req->request = request;
            req->pid = caller_pid;
            netproc_queue_needs_attention = 1;
            print_serial("[NETPROC] OS added to queue idx %d, PID %d, callback 0x%x\n", i, req->pid, req->request.request.icmp_echo_request.callback);
            return 0;
        }
    }
    print_serial("[NETPROC] Failed to add to queue\n");
    return 1;
}

int netproc_addToPending(int pid, struct netproc_request request){
    for(int i = 0; i < NETACTION_QUEUE_LEN; i++){
        struct netproc_pend_entry *pend = &netproc_pending.entries[i];
        if(pend->free){
            pend->free = 0;
            pend->pid = pid;
            pend->request = request;
            return 0;
        }
    }
    return 1;
}

void netprocess_yield(){
	register uint32_t eax asm("eax") __attribute__((unused)) ;
	eax = 0x34;
	asm("int 0x80");
}

int netprocess(int argc __attribute__((unused)), char **argv __attribute__((unused))){
    print_serial("[NETPROC] Starting Process\n");
    set_schedule(ALWAYS);

    while(1){
        if(!netproc_queue_needs_attention && !netproc_pend_needs_attention){
            netprocess_yield();
        }
        if(netproc_queue_needs_attention){
            print_serial("[NETPROC] Addressing Queue\n");
            for(int i = 0; i < NETACTION_QUEUE_LEN; i++){
                struct netproc_req_entry *req = &netproc_queue.entries[i];
                if(req->pending && !req->free){
                    switch(req->request.type){
                        case NETPROC_ICMP_ECHO_REQUEST:
                            print_serial("[NETPROC] Addressing ICMP Request\n");
                            netproc_addToPending(req->pid, req->request);
                            icmp_echoRequest(
                                ethernet_getDriver(),
                                req->request.request.icmp_echo_request.dst_ip,
                                1, 1, 0, 0
                            );
                            break;
                    }
                }
            }
            netproc_queue_needs_attention = 0;
        }

        if(netproc_pend_needs_attention){
            print_serial("[NETPROC] Addressing Pending\n");
            
            task_lock = 1;
            for(int i = 0; i < NETACTION_QUEUE_LEN; i++){
                struct netproc_pend_entry *pend = &netproc_pending.entries[i];
                if(pend->free) continue;
                if(!pend->has_reply) continue;

                if(pend->request.type == NETPROC_ICMP_ECHO_REQUEST){
                    print_serial("0x%x\n", get_physical((uint32_t) pend->request.request.icmp_echo_request.callback));
                    list_tasks();
                    int current_task_id = task_running_idx;
                    task_running_idx = pend->pid;
                    int slot = task_get_slot(task_running_idx);
                    print_serial("New PID Running: %d Old: %d\n", task_running_idx, current_task_id);
                    if(tasks[task_running_idx].program_slot != -1){
                        print_serial("PID %d Selecting Program Slot %d\n", task_running_idx, slot);
                        select_program(slot);
                    }

                    print_serial("0x%x\n", get_physical((uint32_t) pend->request.request.icmp_echo_request.callback));
                    
                    print_serial("[NETPROC] Processing ICMP Callback to 0x%x on PID %d\n", pend->request.request.icmp_echo_request.callback, task_running_idx);
                    //viewport->event_handler(viewport, event);
                    int response = pend->request.request.icmp_echo_request.callback(pend->reply.icmp_reply.packet_size, pend->reply.icmp_reply.source_ip, pend->reply.icmp_reply.packet);
                    print_serial("[NETPROC] Result: %d\n", response);
                    task_running_idx = current_task_id;
                    if(tasks[task_running_idx].program_slot != -1){
                        print_serial("Selecting Program Slot %d\n", tasks[task_running_idx].program_slot);
                        select_program(task_get_slot(task_running_idx));
                    }
                }
                pend->free = 1;
                pend->has_reply = 0;
            }
            netproc_pend_needs_attention = 0;
            task_lock = 0;
        }
    }

    return 0;
}

void netproc_init(){

    for(int i = 0; i < NETACTION_QUEUE_LEN; i++){
        netproc_queue.entries[i].free = 1;
        netproc_queue.entries[i].pending = 0;
        netproc_pending.entries[i].free = 1;
        netproc_pending.entries[i].has_reply = 0;
    }
    netproc_queue_needs_attention = 0;
    netproc_pend_needs_attention = 0;

    start_task(
        netprocess,
        -1,
        0,
        NULL,
        "Network Process"
    );
}

int netproc_checkPending_icmp_reply(uint8_t source_ip[4], unsigned int packet_size, struct icmp_packet *packet){
    for(int i = 0; i < NETACTION_QUEUE_LEN; i++){
        struct netproc_pend_entry *pend = &netproc_pending.entries[i];
        if(pend->free) continue;
        if(pend->request.type != NETPROC_ICMP_ECHO_REQUEST) continue;

        if(
            pend->request.request.icmp_echo_request.dst_ip[0] == source_ip[0] &&
            pend->request.request.icmp_echo_request.dst_ip[1] == source_ip[1] &&
            pend->request.request.icmp_echo_request.dst_ip[2] == source_ip[2] &&
            pend->request.request.icmp_echo_request.dst_ip[3] == source_ip[3]
        ){
            pend->has_reply = 1;
            pend->reply.icmp_reply.packet = packet;
            pend->reply.icmp_reply.packet_size = packet_size;
            pend->reply.icmp_reply.source_ip[0] = source_ip[0];
            pend->reply.icmp_reply.source_ip[1] = source_ip[1];
            pend->reply.icmp_reply.source_ip[2] = source_ip[2];
            pend->reply.icmp_reply.source_ip[3] = source_ip[3];
            netproc_pend_needs_attention = 1;       
            print_serial("[NETPROC interrupt backend] ICMP reply set in queue, awaiting addressing\n");     
            return 0;
        }
    }
    return 1;
}