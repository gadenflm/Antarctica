#include <iostream>
#include <stdio.h>
#include "wkl_debug.h"

#include "hdf_log.h"

#define HDF_LOG_TAG WKL_Debug_Func
using namespace std;


void i2c_test(unsigned int reg_addr) {
    static struct I2cDevice i2c;
    int ret;

    unsigned char bufRead[2] = {0};
    unsigned char bufWrite[2] = {0xFF};
    printf("\r\n    [I2C TEST]\n");

    i2c.busId = I2C_BUSID;
    i2c.addr = (OS05A_I2C_ADDR >> 1);
    i2c.handle = I2cOpen(i2c.busId);
    if (i2c.handle == NULL) {
        printf ("I2C (0) open failed.\n");
        printf("\r\n    [I2C TEST END]\n");
        return ;
    }
    ret = i2c_readwrite(&i2c, bufWrite, 1, WKL_DB_I2C_WRITE, reg_addr);

    if (ret == WKL_DEBUG_WRONG) {
        printf ("[WKL Debug] I2C Write failed.\n");
        I2cClose(i2c.handle);
        printf("\r\n    [I2C TEST END]\n");
        return ;
    }

    OsalMSleep(10);

    ret = i2c_readwrite(&i2c, bufRead, 1, WKL_DB_I2C_READ, reg_addr);
    if (ret == WKL_DEBUG_WRONG) {
        printf ("[WKL Debug] I2C Read failed.\n");
        I2cClose(i2c.handle);
        printf("\r\n    [I2C TEST END]\n");
        return ;
    }

    // I2C Close:
    I2cClose(i2c.handle);
    printf("\r\n    [I2C TEST END]\n");
    return ;
}


static int i2c_readwrite(struct I2cDevice* i2c, unsigned char* regData, unsigned int dataLen, uint8_t flag, unsigned int reg_addr) {
    int32_t ret;
    int index = 0;
    struct I2cMsg msgs[2] = {0};
    unsigned int regAddr = reg_addr;
    printf ("RegAddr = 0x%x\n", regAddr);

    unsigned char bufWrite[4] = {0};

    bufWrite[index++] = ( regAddr >> 8 ) & 0xFF;
    bufWrite[index++] = ( regAddr ) & 0xFF;

    msgs[0].buf = bufWrite;
    msgs[0].len = 2;
    msgs[0].addr = (i2c-> addr);
    msgs[0].flags = 0;

    msgs[1].buf = regData;
    msgs[1].len = dataLen; // 1
    msgs[1].addr = (i2c-> addr);
    msgs[1].flags = (flag == WKL_DB_I2C_READ) ? I2C_FLAG_READ : 0 ;
    if (flag == WKL_DB_I2C_READ ) {
        ret = I2cTransfer(i2c->handle, msgs, 2);
    } else {
        bufWrite[index++] =  regData[0] & 0xFF;
        msgs[0].buf = bufWrite;
        msgs[0].len = 3;
        msgs[0].addr = (i2c-> addr);
        msgs[0].flags = 0;
        ret = I2cTransfer(i2c->handle, msgs, 1);
        printf ("flags:%d\n", msgs[0].flags);
        printf ("regAddr:0x%x%x\n", bufWrite[0], bufWrite[1]);
        printf ("data:0x%x\n", bufWrite[2]);

        if (ret == 1) {
            return WKL_DEBUG_RIGHT;
        }
        return WKL_DEBUG_WRONG;
    }


    printf ("flags:%d\n", msgs[1].flags);
    printf ("regAddr:0x%x%x\n", bufWrite[0], bufWrite[1]);
    printf ("data:0x%x\n", regData[0]);

    if (ret == 2 ) {
        return WKL_DEBUG_RIGHT;
    }
    return WKL_DEBUG_WRONG;


}

