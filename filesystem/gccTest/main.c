#include <stdio.h>
#include <stdint.h>
#include <sys/io.h>
#include <sys/vp.h>
#include <sys/console.h>

int main(int argc, char **argv){
	int serial = open("/-/dev/serial", O_WRITE);
	char buf[] = "File I/O Message to Serial\n";
	write(serial, buf, sizeof(buf));
	close(serial);

	struct Viewport *vp = vp_open(300, 100, "GCC GWOS Test\n");

	struct CONSOLE *console = console_get_current();
	console_print(console, "CONSOLE Hello World!\n");

	printf("Hello From Printf!\n");

    return 0;
}
