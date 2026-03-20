#include <stdio.h>
#include <sys/io.h>
#include <sys/task.h>
#include <sys/dir.h>
#include <unistd.h>
#include <string.h>
#include <exec/exec.h>

int main(int argc, char **argv){
    char dirBuf[20];
    char *path = getcwd(dirBuf, sizeof(dirBuf));

    printf("%s> ", path);
    char readbuf[10];

    char command_buf[100];
    int cmd_idx = 0;

    while(1){
        int n = read(0, readbuf, 1);
        if(n > 0){
            fwrite(readbuf, 1, 1, stdout);
            command_buf[cmd_idx] = readbuf[0];
            if(readbuf[0] == '\n'){
                command_buf[cmd_idx] = '\0';
                EXEC_CTX ctx = exec_createCTX();
                exec_setProgram(ctx, command_buf);
                exec_setARGC(ctx, 0);
                int pid = exec_child(ctx);
                if(pid > 0){
                    waitpid(pid);
                }
                else{
                    printf("Error, unknown command: %s\n", command_buf);
                }                
                memset(command_buf, 0, sizeof(command_buf));
                cmd_idx = 0;
                printf("%s> ", path);
            }
            else{
                cmd_idx++;
            }
        }
        
        yield();
    }

    return 0;
}