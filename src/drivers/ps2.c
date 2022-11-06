#include "ps2.h"

char ps2_controllers[] = {1, 1};

void init_ps2(){
    printk("[PS2] Init PS/2 Devices\n");
    char dual_channel = 1;

    IRQ_OFF;

    ps2_write(PS2_CMD, PS2_DISABLE_FIRST);
    ps2_write(PS2_CMD, PS2_DISABLE_SECOND);

    inb(PS2_DATA);

    ps2_write(PS2_CMD, PS2_READ_CONFIG);
    

    uint8_t config = ps2_read(PS2_DATA);

    config |= PS2_CFG_SYSTEM_FLAG;

    if(config & PS2_CFG_MUST_BE_ZERO){
        printk("[PS2] Invalid bit set in config\n");
    }

    config &= ~(PS2_CFG_FIRST_PORT | PS2_CFG_SECOND_PORT | PS2_CFG_TRANSLATION);

    ps2_write(PS2_CMD, PS2_WRITE_CONFIG);
    ps2_write(PS2_DATA, config);

    ps2_write(PS2_CMD, PS2_SELF_TEST);
    if(ps2_read(PS2_DATA) != PS2_SELF_TEST_OK){
        printk("[PS2] Controller failed self-test\n");
        ps2_controllers[0] = 0;
        ps2_controllers[1] = 0;
        return;
    }

    

    ps2_write(PS2_CMD, PS2_WRITE_CONFIG);
    ps2_write(PS2_DATA, config);

    ps2_write(PS2_CMD, PS2_ENABLE_SECOND);
    ps2_write(PS2_CMD, PS2_READ_CONFIG);
    config = ps2_read(PS2_DATA);

    

    if(config & PS2_CFG_SECOND_CLOCK){
        printk("[PS2] Only one PS/2 controller\n");
        dual_channel = 0;
        ps2_controllers[1] = 0;
    }
    else{
        ps2_write(PS2_CMD, PS2_DISABLE_SECOND);
    }

    ps2_write(PS2_CMD, PS2_TEST_FIRST);

    if(ps2_read(PS2_DATA) != PS2_TEST_OK){
        printk("[PS2] PS/2 Port 1 failed test\n");
        ps2_controllers[0] = 0;
    }

    if(dual_channel){
        ps2_write(PS2_CMD, PS2_TEST_SECOND);

        if(ps2_read(PS2_DATA) != PS2_TEST_OK){
            printk("[PS2] PS/2 Port 2 failed test\n");
            ps2_controllers[1] = 0;
        }
    }

    

    if(ps2_controllers[0]){
        ps2_write(PS2_CMD, PS2_ENABLE_FIRST);
        config |= PS2_CFG_FIRST_PORT;
        config &= ~PS2_CFG_FIRST_CLOCK;
    }

    if(ps2_controllers[1]){
        ps2_write(PS2_CMD, PS2_ENABLE_SECOND);
        config |= PS2_CFG_SECOND_PORT;
        config &= ~PS2_CFG_SECOND_CLOCK;
    }

    ps2_write(PS2_CMD, PS2_WRITE_CONFIG);
    ps2_write(PS2_DATA, config);

    

    for(uint32_t i = 0; i < 2; i++){
        if(!ps2_controllers[i]) continue;

        ps2_write_device(i, PS2_DEV_RESET);
        uint8_t ret = ps2_read(PS2_DATA);

        if(ret != PS2_DEV_ACK || ps2_read(PS2_DATA) != PS2_DEV_RESET_ACK){
            printk("[PS2] Failure to reset dev %1x\n", i);
            ps2_controllers[i] = 0;
            config &= ~(i == 0 ? PS2_CFG_FIRST_PORT : PS2_CFG_SECOND_PORT);

            ps2_write(PS2_CMD, PS2_WRITE_CONFIG);
            ps2_write(PS2_DATA, config);
        }
    }


    IRQ_RES;
    printk("[PS2] Initialized\n");
}

char ps2_wait_write(){
    int timer = PS2_TIMEOUT;
    while((inb(0x64) & 2) && timer-- > 0){
        asm("pause");
    }
    return timer != 0;
}

char ps2_wait_read(){
    int timer = PS2_TIMEOUT;
    while(!(inb(0x64) & 1) && timer-- >= 0){
        asm("pause");
    }
    return timer != 0;
}

uint8_t ps2_read(uint32_t port){
    if(ps2_wait_read()){
        return inb(port);
    }
    printk("[PS2] Read failure\n");
    return -1;
}

char ps2_write(uint32_t port, uint8_t b){
    if(ps2_wait_write()){
        outb(port, b);
        return 1;
    }
    printk("[PS2] Write failure\n");
    return 0;
}

char ps2_write_device(uint32_t dev, uint8_t b){
    if(dev != 0){
        if(!ps2_write(PS2_CMD, PS2_WRITE_SECOND)){
            return 0;
        }
    }
    return ps2_write(PS2_DATA, b);
}

char ps2_expect_ack(){
    uint8_t ret = ps2_read(PS2_DATA);
    if(ret != PS2_DEV_ACK){
        printk("[PS2] Device failed to ack cmd\n");
        return 0;
    }
    return 1;
}