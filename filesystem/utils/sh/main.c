#include <stdio.h>
#include <sys/io.h>
#include <sys/task.h>
#include <sys/dir.h>
#include <unistd.h>
#include <string.h>
#include <exec/exec.h>
#include <unistd.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <stdlib.h>

char **args;

struct Alias {
	char *match;
	char *result;
};

struct Alias alias[] = {
	{"nasm", "/A/utils/nasm.elf"},
	{"lisp", "/A/lisp/lisp.elf"},
	{"tmgr", "/A/utils/task/taskmgr.elf"},
	{"edit", "/A/utils/editor/editor.elf"},
	{"netstat", "/A/net/netstat/netstat.elf"},
	{"ping", "/A/net/ping/ping.elf"},
	{"setbg", "/A/utils/setbg_scale/setbg_scale.elf"},
	{"tga", "/A/utils/image/image.elf"},
	{"gif", "/A/utils/gif/gif.elf"}
};

int running;

void run_command(char *cmd){
	int fullsize = 0;
	while(cmd[fullsize] != 0){
		fullsize++;
	}
	if(fullsize == 0){
		return;
	}
	fullsize++;


	int cancel_wait = 0;
	if(cmd[0] == '&'){
		fullsize--;
		cmd++;
		cancel_wait = 1;
	}
	
	char *command;
	int argc = 1;
	for(int i = 0; cmd[i] != 0 && i < 50; i++){
		if(cmd[i] == ' ') argc++;
	}
	args = (char **) malloc(sizeof(char *) * argc);
	int idx = 0;
	for(int i = 0; i < argc && cmd[idx] != 0; i++){
		int size = 0;
		for(int j = 0; cmd[idx+j] != ' ' && cmd[idx+j] != 0; j++){
			size++;
		}
		args[i] = (char *) malloc(size+1);
		memset(args[i], 0, size+1);
		memcpy(args[i], cmd+idx, size);
		idx = idx + size + 1;
	}
	for(int i = 0; i < fullsize; i++){
		if(cmd[i] == ' ') cmd[i] = 0;
	}
	if(!strcmp(args[0], "cd")){
		chdir(args[1]);
	}
	else if(!strcmp(args[0], "ls")){
		char *path;
		if(argc == 1){
			path = ".";
		}
		else{
			path = args[1];
		}
		struct DirectoryListing dir = getDirectoryListing(path);
		for(int i = 0; i < dir.num_entries; i++){
			puts(dir.entries[i].filename);
		}
	}
	else if(!strcmp(args[0], "mkdir")){
		printf("Making dir %s\n", (uint32_t) args[1]);
		if(argc >= 1){
			mkdir(args[1], 0);
		}
	}
	else if(!strcmp(args[0], "mkfile")){
		printf("Making file %s\n", (uint32_t) args[1]);
		if(argc >= 1){
			creat(args[1]);
		}
	}
    else if(!strcmp(args[0], "clear")){
        printf("\033[2J\033[H");
    }
	else if(!strcmp(args[0], "exit")){
		running = 0;
	}
	else{
		for(int i = 0; i < sizeof(alias) / sizeof(struct Alias); i++){
			if(!strcmp(cmd, alias[i].match)){
				cmd = alias[i].result;
				break;
			}
		}
		
		EXEC_CTX ctx = exec_createCTX();
        exec_setProgram(ctx, cmd);
		exec_setFDCount(ctx, 2);
		exec_setFD(ctx, 0, 0, 0);
		exec_setFD(ctx, 1, 1, 1);
        exec_setARGC(ctx, argc);
        for(int i = 0; i < argc; i++){
            exec_setARGV(ctx, i, args[i]);
        }
        int pid = exec_child(ctx);
        if(pid > 0){
            if(!cancel_wait) waitpid(pid);
        }
        else{
            printf("Error, unknown command: %s\n", cmd);
        }
	}
}

int main(int argc, char **argv){
    char dirBuf[20];
    char *path = getcwd(dirBuf, sizeof(dirBuf));

    printf("\033[32m%s\033[0m> ", path);
    char readbuf[10];

    char command_buf[100];
    int cmd_idx = 0;

	running = 1;
    while(running){
        int n = read(0, readbuf, 1);
        if(n > 0){
            if(readbuf[0] == '\n'){
                command_buf[cmd_idx] = '\0';
                
				printf("\n");
                run_command(command_buf);
				printf("\n");

    			path = getcwd(dirBuf, sizeof(dirBuf));

                memset(command_buf, 0, sizeof(command_buf));
                cmd_idx = 0;
                printf("\033[32m%s\033[0m> ", path);
            }
            else if(readbuf[0] == 8 && cmd_idx > 0){
                char *backspace_msg = "\b";
                fwrite(backspace_msg, 1, 1, stdout);
                command_buf[--cmd_idx] = '\0';
            }
            else{
                fwrite(readbuf, 1, 1, stdout);
                command_buf[cmd_idx] = readbuf[0];
                cmd_idx++;
            }
        }
        
        yield();
    }

    return 0;
}