/**
 *  Edited by wkl
 *  /kernel/liteos_a/kernel/base/misc/wkldebug_shellcmd.c
 */
#include "wkl_debug.h"
#include <stdio.h>
#include <stdlib.h>
#include "i2c_if.h"
#include "gpio_if.h"
#include "osal_io.h"
#include "osal_time.h"
#include "hdf_log.h"

#define HDF_LOG_TAG WKL_Debug
#ifdef LOSCFG_SHELL
#include "shcmd.h"
#include "shell.h"
#endif

UINT32 ShellWklDebug(INT32 argc, const CHAR **argv){
    if (argc == 0) {
        print_use_guide();
        return  WKL_DEBUG_WRONG;
    } 
    if (!strcmp(argv[0], "-h") || !strcmp(argv[0], "--help")) {
        print_use_guide();
        return  WKL_DEBUG_RIGHT;
    }
    if (!strcmp(argv[0], "-ti") ) {
        i2c_test();
        return  WKL_DEBUG_RIGHT;
    }
    print_use_guide();
    return WKL_DEBUG_WRONG;
}

static void i2c_test(void){
    static struct I2cDevice i2c;
    int ret;
    // int ret_gpio;
    unsigned char bufRead[2] = {0};

    
    PRINTK("\r\n    [I2C TEST]\n");
    
    // I2C Init:
    OSAL_WRITEL(I2C0_REF_CFG,IO_DEVICE_ADDR(I2C0_SDA_ADDR));
    OSAL_WRITEL(I2C0_REF_CFG,IO_DEVICE_ADDR(I2C0_SCL_ADDR));

    PRINTK("[WKL Debug]I2C0_SCL:0x%x\n\n",OSAL_READL(IO_DEVICE_ADDR(I2C0_SCL_ADDR)));
    // GPIO RST:
    
/*    ret_gpio = GpioSetDir (33, GPIO_DIR_OUT);
    if(ret_gpio!= HDF_SUCCESS) PRINTK ("[WKL Debug] GPIO Set Direction failed \n");
    
    ret_gpio = GpioWrite (33, GPIO_VAL_HIGH);
    if (ret_gpio != HDF_SUCCESS) PRINTK("[WKL Debug] GPIO Write failed \n");
    OsalMSleep (20);
    ret_gpio = GpioWrite (33, GPIO_VAL_LOW);
    if (ret_gpio != HDF_SUCCESS) PRINTK("[WKL Debug] GPIO Write failed \n");
    OsalMSleep (50);
    ret_gpio = GpioWrite (33, GPIO_VAL_HIGH);
    if (ret_gpio != HDF_SUCCESS) PRINTK("[WKL Debug] GPIO Write failed \n");
    OsalMSleep (20);*/

    // I2C Read:
    
    i2c.busId=I2C_BUSID;
    i2c.addr= (OS05A_I2C_ADDR);
    i2c.handle = I2cOpen(i2c.busId);
    if (i2c.handle == NULL) {
        PRINTK ("I2C (0) open failed.\n");
        PRINTK("\r\n    [I2C TEST END]\n");
        return ;
    }

    PRINTK("[WKL Debug] I2C handle get,I2C handle:0x%p\n\n",i2c.handle);

/*    // I2C WRITE:
    ret=i2c_readwrite(&i2c,bufWrite,4,WKL_DB_I2C_WRITE);
    if (ret == WKL_DEBUG_WRONG) {
        PRINTK ("I2C Write failed.\n"); 
        PRINTK("\r\n    [I2C TEST END]\n");
        return ;
    }*/

    // I2C Read:
    ret=i2c_readwrite(&i2c,bufRead,1,WKL_DB_I2C_READ);
    if (ret == WKL_DEBUG_WRONG) {
        PRINTK ("[WKL Debug] I2C Read failed.\n"); 
        I2cClose(i2c.handle);
        PRINTK("\r\n    [I2C TEST END]\n");
        return ;
    }

    PRINTK ("[WKL Debug] bufRead[%d] = %x",0,bufRead[0]);
    // PRINTK ("[WKL Debug] bufRead[%d] = %x",1,bufRead[1]);

    // I2C Close:
    I2cClose(i2c.handle);
    PRINTK("\r\n    [I2C TEST END]\n");
    return ;
}

static int i2c_readwrite(struct I2cDevice* i2c, unsigned char* regData, unsigned int dataLen, uint8_t flag){
    int32_t ret;
    int index = 0;
    struct I2cMsg msgs[2]={0};
    unsigned int regAddr=0x3010;

    if (flag== WKL_DB_I2C_READ) {

        unsigned char bufWrite[2] ={0};

        bufWrite[index++] = ( regAddr >> 8 ) & 0xFF;
        bufWrite[index++] = ( regAddr ) & 0xFF;

        msgs[0].buf = bufWrite;
        msgs[0].len = 2;
        msgs[0].addr = (i2c-> addr); 
        msgs[0].flags = 0;

        msgs[1].buf = regData;
        msgs[1].len = 2;
        msgs[1].addr = (i2c-> addr); 
        msgs[1].flags = I2C_FLAG_READ;

        ret = I2cTransfer(i2c->handle, msgs, 2);
        if (ret == 2 ){
            return WKL_DEBUG_RIGHT;
        }
        PRINTK ("[WKL Debug] regData:%s\n",regData);
        PRINTK ("[WKL Debug] dataLen:%u\n",dataLen);
        PRINTK ("[WKL Debug] I2C_Addr:0x%x\n",i2c->addr);

        PRINTK ("[WKL Debug] I2cTransfer returns: %ld\n",ret);
        return WKL_DEBUG_WRONG;
    } else {

        return WKL_DEBUG_RIGHT;
    }
    
}

void print_use_guide(void){
    PRINTK("usage:\n");
    PRINTK("      wkldebug [-h || --help] for help\n"
           "      wkldebug [-Selection Mode]\n");
    PRINTK("\r\nSelection Mode:\n"
           "  -ti  test_i2c\n");
}

#ifdef LOSCFG_SHELL
SHELLCMD_ENTRY(wkldebug_shellcmd , CMD_TYPE_EX, "wkldebug",0,(CMD_CBK_FUNC)ShellWklDebug);
#endif
