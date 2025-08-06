#include "serial.h"

void ap_startup(int apicid){
    print_serial("[CPU] Core #%d online!\n", apicid);
    while(1);
}