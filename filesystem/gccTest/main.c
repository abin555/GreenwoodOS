#include <stdio.h>
#include <stdint.h>
#include <sys/io.h>
#include <sys/vp.h>

int main(int argc, char **argv){
    puts("Test Message\nIs this working???\n");
	int serial = open("/-/dev/serial", O_WRITE);
	char buf[] = "File I/O Message to Serial\n";
	write(serial, buf, sizeof(buf));
	close(serial);

	vp_open(100, 100, "GCC GWOS Test\n");
    return 0;
}
