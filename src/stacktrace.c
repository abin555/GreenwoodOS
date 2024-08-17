#include "stacktrace.h"

struct stackframe;
struct stackframe {
  union{
    struct {
      struct stackframe* ebp;
      uint32_t eip;
    } frame;
    uint32_t data[4];
  };
};

void print_stack_trace(uint32_t ebp, unsigned int maxFrames){
    struct stackframe *stk = (struct stackframe *) ebp;
    print_serial("Stack trace:\n");
    for(unsigned int frame = 0; stk && frame < maxFrames; ++frame)
    {
        // Unwind to previous stack frame
        print_serial("%d  0x%x 0:0x%x 1:0x%x 2:0x%x 3:0x%x\n", frame, stk->frame.eip, stk->data[0], stk->data[1], stk->data[2], stk->data[3]);
        stk = stk->frame.ebp;
    }
}