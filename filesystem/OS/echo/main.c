#include "libc.h"

int main(int argc, char **argv){
	if(argc > 1){
		print(argv[1]);
	}
	else{
		print("Not enough Args!\n");
	}
}