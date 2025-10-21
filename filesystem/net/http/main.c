#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>
#include <sys/task.h>
#include <string.h>

typedef int (*http_reply_callback)(uint16_t port, void *data, size_t data_size);

struct http_request {
    http_reply_callback callback;
    int str_len;
    char req[];
};

int has_reply;

int callback(uint16_t port, void *data, size_t data_size) {
    printf("HTTP callback! - %d %d\n", port, data_size);
    for(int i = 0; i < data_size; i++){
        putc(((char *) data)[i], stdout);
    }
    return 0;
}

int main(int argc, char **argv){
    int http_fd = open("/-/net/http", O_WRITE);
    if(http_fd == -1){
        printf("Unable to open HTTP file\n");
        return 1;
    }

    char *request = "10.0.1.2 5000 GET / localhost";
    struct http_request *hreq = malloc(sizeof(struct http_request) + strlen(request));
    hreq->callback = callback;
    hreq->str_len = strlen(request);
    memcpy(hreq->req, request, hreq->str_len);

    has_reply = 0;

    printf("HTTP %s\n",
        hreq->req
    );

    write(http_fd, hreq, sizeof(struct http_request) + strlen(request));
    close(http_fd);

    while(1){

    }

    printf("Done!\n");
}