int test(int b);
void print(char *str);
char string[] = "Test String!\n\0";

int main(){
    test(5);
    print(string);
    return 2;    
}

int test(int b){
    int a = 2;
    a+=b;
    return a;
}

void print(char *str){
    register int syscall_num asm("eax") = 1;
    register char* arg       asm("ebx") = str;
    asm volatile("int 0x80");
}
