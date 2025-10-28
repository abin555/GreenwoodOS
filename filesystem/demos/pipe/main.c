#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/io.h>
#include <string.h>
#include <sys/task.h>
#include <sys/stat.h>

int main(int argc, char **argv){
    printf("Pipe test program!\n");
    int pipedescs[2];
    struct stat pipe_stat;
    char msg[] = "This is a test message!";
    int status = pipe(pipedescs);
    printf("Pipe created with status %d\n writer fd: %d, reader fd: %d\n", status, pipedescs[0], pipedescs[1]);

    fstat(pipedescs[0], &pipe_stat);

    
    int nwrite = write(pipedescs[0], msg, sizeof(msg));
    printf("Wrote %d bytes into pipe!\n", nwrite);

    char readbuf[100];
    memset(readbuf, 0, sizeof(readbuf));
    int nread = read(pipedescs[1], readbuf, sizeof(readbuf));
    printf("Read %d bytes from pipe\n\"%s\"\n", nread, readbuf);
    

    struct exec_spec_ctx *proc_ctx = malloc(sizeof(struct exec_spec_ctx));
    proc_ctx->filename = "/A/demos/pipe/child.elf";
    proc_ctx->argc = 0;
    proc_ctx->argv = NULL;
    proc_ctx->file_ctx = malloc(sizeof(struct task_file_ctx) + 2*sizeof(struct task_file_association));
    proc_ctx->file_ctx->num_fds = 2;
    proc_ctx->file_ctx->fds[0] = (struct task_file_association) {0, 0};
    proc_ctx->file_ctx->fds[1] = (struct task_file_association) {pipedescs[0], 1};
    int child_pid = exec_spec(proc_ctx);
    //waitpid(child_pid);

    printf("Pipe stat owner: %s\n", pipe_stat.fs_ownerIden);
    printf("Pipe open stat: %d\n", pipe_stat.open_stat);
    while(pipe_stat.open_stat != 0){
        fstat(pipedescs[0], &pipe_stat);
        int n = read(pipedescs[1], readbuf, 10);
        if(n > 0){
            readbuf[n+1] = '\0';
            printf("%s", readbuf);
        }
        if(pipe_stat.open_stat == 0){
            printf("Pipe closed!");
            memset(readbuf, 0, sizeof(readbuf));
            int nread = read(pipedescs[1], readbuf, sizeof(readbuf));
            printf("Read %d bytes from pipe\n\"%s\"\n", nread, readbuf);
        }
    }
    printf("Pipe Program Exit!\n");
    close(pipedescs[0]);
    close(pipedescs[1]);
    return 0;
}