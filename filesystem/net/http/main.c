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

int main(int argc, char **argv){
    int conn_fd;
    int http_fd = open("/-/net/http", O_WRITE);
    if(http_fd == -1){
        printf("Unable to open HTTP file\n");
        return 1;
    }
    if(argc == 1) argv[1] = "10.0.1.2";
    //char *request = "10.0.1.2 80 GET / localhost";
    char request_buf[100];
    snprintf(request_buf, sizeof(request_buf), "%s 80 GET / hostname\0", argv[1]);
    size_t hreq_size = sizeof(struct http_request) + strlen(request_buf) + 1;
    struct http_request *hreq = malloc(hreq_size);
    memset(hreq, 0, hreq_size);
    hreq->conn_ref = &conn_fd;
    hreq->str_len = strlen(request_buf);
    memcpy(hreq->req, request_buf, hreq->str_len);

    has_reply = 0;

    printf("HTTP %s\n",
        hreq->req
    );

    write(http_fd, hreq, sizeof(struct http_request) + strlen(request_buf));
    close(http_fd);

    while(conn_fd == -1){
        yield();
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
    printf("Read %d bytes, len is %d\n", n, strlen(read_buf));
    for(int i = 0; i < n; i++){
        putc(read_buf[i], stdout);
    }

    close(conn_fd);

    printf("Done!\n");
}