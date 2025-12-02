#ifndef HTTP_H
#define HTTP_H
#include "stddef.h"
#include "stdint.h"
#include "ip.h"
#include "tcp.h"

//Sends http request, returns source port
uint16_t http_send_request(struct ethernet_driver *driver, uint8_t destination_ip[4], uint16_t destination_port, const char *method, const char *path, const char *host);

bool http_receive_request(struct ethernet_driver *driver, uint16_t port, void *data, size_t data_size);

#endif