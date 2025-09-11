#include <sys/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <sys/memory.h>
#include <sys/task.h>

#include <gwos/vfs.h>

#include "buffs.h"
#include "hooks.h"

//void *buffs_getLink(void *fs, char *)