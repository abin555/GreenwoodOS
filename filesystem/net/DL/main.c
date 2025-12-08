#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>
#include <sys/task.h>
#include <sys/stat.h>
#include <string.h>
#include <internal/stdio.h>

typedef int *http_conn_ref;

struct http_request {
    http_conn_ref conn_ref;
    int str_len;
    char req[];
};

int main(int argc, char **argv){
    int conn_fd;
    int http_fd = open("/-/net/http", O_WRITE);
    if(http_fd == -1){
        printf("Unable to open HTTP file\n");
        return 1;
    }
    char *hostname;
    char *file;
    char *outfile;
    if(argc < 4){
        printf("Usage: %s [hostname] [uri] [outfile]", argv[0]);
        return 1;
    }
    else{
        hostname = argv[1];
        file = argv[2];
        outfile = argv[3];
    }
    char request_buf[100];
    memset(request_buf, 0, sizeof(request_buf));
    snprintf(request_buf, sizeof(request_buf), "%s 80 GET %s %s\0", hostname, file, hostname);
    printf("Sending Request: %s\n", request_buf);
    
    size_t hreq_size = sizeof(struct http_request) + strlen(request_buf) + 1;
    struct http_request *hreq = malloc(hreq_size);
    memset(hreq, 0, hreq_size);
    hreq->conn_ref = &conn_fd;
    hreq->str_len = strlen(request_buf);
    memcpy(hreq->req, request_buf, hreq->str_len);

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
    close(conn_fd);

    printf("Read %d bytes, len is %d\n", n, strlen(read_buf));
    char *msg_body;
    for(int i = 0; i < n; i++){
        if(read_buf[i] == '\r' && read_buf[i+1] == '\n' && read_buf[i+2] == '\r' && read_buf[i+3] == '\n'){
            msg_body = read_buf+i+4;
            break;
        }
    }
    size_t len = n - (msg_body - read_buf);
    printf("Message Starts at offset %d and is %d bytes\n", msg_body - read_buf, len);

    FILE *fout = fopen(outfile, "w");
    if(fout == NULL){
        printf("Creating output \"%s\"\n", fout);
        if(creat(outfile) == -1){
            printf("Error creating file!\n");
            fclose(fout);
            return 1;
        }
        fout = fopen(outfile, "w");
        if(fout == NULL){
            printf("output creation failed. aborting\n");
            fclose(fout);
            return 1;
        }
    }
    ftruncate(fout->fd, len);

    fwrite(msg_body, len, 1, fout);
    fclose(fout);

    printf("Done!\n");
}