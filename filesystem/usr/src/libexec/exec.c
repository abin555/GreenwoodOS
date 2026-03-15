#include <exec/exec.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/task.h>
#include <sys/memory.h>
#include <sys/io.h>

//typedef struct exec_spec_ctx* EXEC_CTX;

EXEC_CTX exec_createCTX(){
    EXEC_CTX ctx = malloc(sizeof(struct exec_spec_ctx));
    ctx->filename = NULL;
    ctx->argc = 0;
    ctx->argv = NULL;
    ctx->file_ctx = NULL;
    return ctx;
}
EXEC_CTX exec_setProgram(EXEC_CTX ctx, char *program_file){
    ctx->filename = strdup(program_file);
    return ctx;   
}
EXEC_CTX exec_setARGC(EXEC_CTX ctx, int argc){
    ctx->argc = argc;
    ctx->argv = malloc(sizeof(char *) * ctx->argc);
    return ctx;
}
EXEC_CTX exec_setARGV(EXEC_CTX ctx, int argNum, char *arg){
    ctx->argv[argNum] = arg;
    return ctx;
}
EXEC_CTX exec_setFDCount(EXEC_CTX ctx, int fdCount){
    ctx->file_ctx = malloc(sizeof(struct task_file_ctx) + sizeof(struct task_file_association) * fdCount);
    ctx->file_ctx->num_fds = fdCount;
    return ctx;
}
EXEC_CTX exec_setFD(EXEC_CTX ctx, int fdNum, int parentFD, int childFD){
    ctx->file_ctx->fds[fdNum].creator_fd = parentFD;
    ctx->file_ctx->fds[fdNum].new_fd = childFD;
    return ctx;
}

int exec_simple(EXEC_CTX ctx){
    return exec_spec((struct exec_spec_ctx *) ctx);
}

int exec_child(EXEC_CTX ctx){
    if(ctx->file_ctx == NULL){
        exec_setFDCount(ctx, 2);
    }
    else{
        if(ctx->file_ctx->num_fds < 2){
            exec_setFDCount(ctx, 2);
        }
    }
    exec_setFD(ctx, 0, dup(0), 0);
    exec_setFD(ctx, 1, dup(1), 1);
    return exec_simple(ctx);
}