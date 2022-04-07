#include "io.h"

void software_interrupt(unsigned char interrupt){
	INT_Software_Value = interrupt;
	software_int();
}