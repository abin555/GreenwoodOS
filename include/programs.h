#ifndef PROGRAMS_H
#define PROGRAMS_H

#include "processes.h"
#include "paging.h"
#include "io.h"

extern void program();
extern void program_end();

void exec_user_program();

#endif