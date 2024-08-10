#include "stacktrace.h"

struct stackframe;
struct stackframe {
  struct stackframe* ebp;
  uint32_t eip;
};

void print_stack_trace(uint32_t ebp, unsigned int maxFrames){
    struct stackframe *stk = (struct stackframe *) ebp;
    print_serial("Stack trace:\n");
    for(unsigned int frame = 0; stk && frame < maxFrames; ++frame)
    {
        // Unwind to previous stack frame
        print_serial("%d  0x%x\n", frame, stk->eip);
        stk = stk->ebp;
    }
}