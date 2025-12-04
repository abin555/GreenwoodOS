#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>
#include <sys/task.h>
#include <sys/stat.h>
#include <string.h>

typedef int *http_conn_ref;

struct http_request {
    http_conn_ref conn_ref;
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
    int conn_fd;
    int http_fd = open("/-/net/http", O_WRITE);
    if(http_fd == -1){
        printf("Unable to open HTTP file\n");
        return 1;
    }

    char *request = "10.0.1.2 80 GET / localhost";
    struct http_request *hreq = malloc(sizeof(struct http_request) + strlen(request));
    hreq->conn_ref = &conn_fd;
    hreq->str_len = strlen(request);
    memcpy(hreq->req, request, hreq->str_len);

    has_reply = 0;

    printf("HTTP %s\n",
        hreq->req
    );

    write(http_fd, hreq, sizeof(struct http_request) + strlen(request));
    close(http_fd);

    while(conn_fd == -1){

    }

    struct stat conn_stat;
    fstat(conn_fd, &conn_stat);
    printf("Connection is owned by %s\n", conn_stat.fs_ownerIden);
    while(conn_stat.open_stat){
        fstat(conn_fd, &conn_stat);
        yield();
    }
    printf("Buffer is %d bytes\n", conn_stat.size);
    char *read_buf = malloc(conn_stat.size+1);
    memset(read_buf, 0, conn_stat.size+1);
    int n = read(conn_fd, read_buf, conn_stat.size);
    printf("Read %d bytes\n", n);
    for(int i = 0; i < n; i++){
        putc(read_buf[i], stdout);
    }

    close(conn_fd);

    printf("Done!\n");
}