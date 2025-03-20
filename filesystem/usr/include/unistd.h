#ifndef _UNISTD_H
#define _UNISTD_H

int execve(const char *pathname, char *const argv[], char *const envp[]);
int execv(const char *pathname, char *const argv[]);
int execvp(const char *file, char *const argv[]);

void exit(int status);

#ifndef _PID_T
#define _PID_T
typedef int pid_t;
#endif

pid_t getpid(void);
pid_t fork(void);

#endif