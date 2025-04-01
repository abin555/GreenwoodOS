#include "http.h"
#include "utils.h"

void http_send_request(struct ethernet_driver *driver __attribute__((unused)), uint8_t destination_ip[4], uint16_t destination_port, const char *method, const char *path, const char *host) {
    print_serial("http_send_request\n");
    char *request = malloc(1024 * sizeof(char));
    uint16_t source_port = 10101;
    sprintf(request, "%s %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", method, path, host);
    print_serial("%s\n", request);
    tcp_install_listener(source_port, http_receive_request);
    uint32_t ack;
    if (tcp_syn(driver, destination_ip, destination_port, source_port, 10000, &ack)) {
        tcp_send_packet(driver, driver->ipv4.ip, source_port, destination_ip, destination_port, 0, ack, false, true, false, true, request, strlen(request));
        // tcp_close_connection(driver, destination_ip, destination_port, source_port);
    }

    //free(request);
}

bool http_receive_request(struct ethernet_driver *driver __attribute__((unused)), void *data, size_t data_size) {
    if (data_size == 0) {
        return true;
    }

    //print_serial("http_receive_request\n");
    //hexdump(data, data_size);
    char *request = malloc(data_size + 1);
    memcpy(request, data, data_size);

    print_serial("%s\n", request);

    //free(request);
    return true;
}