#ifndef LIBEXEC_H
#define LIBEXEC_H

typedef struct exec_spec_ctx* EXEC_CTX;

EXEC_CTX exec_createCTX();
EXEC_CTX exec_setProgram(EXEC_CTX ctx, char *program_file);
EXEC_CTX exec_setARGC(EXEC_CTX ctx, int argc);
EXEC_CTX exec_setARGV(EXEC_CTX ctx, int argNum, char *arg);
EXEC_CTX exec_setFDCount(EXEC_CTX ctx, int fdCount);
EXEC_CTX exec_setFD(EXEC_CTX ctx, int fdNum, int parentFD, int childFD);

int exec_simple(EXEC_CTX ctx);
int exec_child(EXEC_CTX ctx);

#endif