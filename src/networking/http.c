#include "http.h"
#include "utils.h"
#include "netproc.h"

int http_reqno = 0;

uint16_t http_send_request(struct ethernet_driver *driver __attribute__((unused)), uint8_t destination_ip[4], uint16_t destination_port, const char *method, const char *path, const char *host) {
    print_serial("http_send_request\n");
    char *request = malloc(1024 * sizeof(char));
    uint16_t source_port = 10101 + http_reqno++;
    sprintf(request, "%s %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", method, path, host);
    print_serial("%s\n", request);
    tcp_install_listener(source_port, http_receive_request);
    uint32_t ack;
    if (tcp_syn(driver, destination_ip, destination_port, source_port, 10000, &ack)) {
        tcp_send_packet(driver, driver->ipv4.ip, source_port, destination_ip, destination_port, 0, ack, false, true, false, true, request, strlen(request));
        // tcp_close_connection(driver, destination_ip, destination_port, source_port);
    }
    return source_port;

    //free(request);
}

bool http_receive_request(struct ethernet_driver *driver __attribute__((unused)), uint16_t port, void *data, size_t data_size) {
    print_serial("[HTTP] Recieve on port %d %d bytes\n", port, data_size);
    if (data_size == 0) {
        return true;
    }

    //print_serial("http_receive_request\n");
    //hexdump(data, data_size);
    char *request = malloc(data_size + 1);
    memcpy(request, data, data_size);
    request[data_size] = 0;
    print_serial("Recieved %d bytes\n\n", data_size);
    print_serial("%s\n", request);

    netproc_checkPending_http_response(port, data, data_size);

    //free(request);
    return true;
}