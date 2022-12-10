#ifndef EDITOR_H
#define EDITOR_H

#include "processes.h"
#include "console.h"
#include "keyboard.h"
#include "filesystem.h"
#include "framebuffer.h"

void Editor(uint8_t process, uint32_t args[10]);

#endif