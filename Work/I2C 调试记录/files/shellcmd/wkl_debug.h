/**
 *  Edited by wkl
 *  /kernel/liteos_a/kernel/include/wkldebug.h
 */
#ifndef _WKL_DEBUG
#define _WKL_DEBUG

#define WKL_DEBUG_RIGHT 1
#define WKL_DEBUG_WRONG 0
#define WKL_DB_I2C_READ 0
#define WKL_DB_I2C_WRITE 1

#define I2C_BUSID 0
#define OS05A_I2C_ADDR 0x6c
#define I2C0_SDA_ADDR 0x114f0078
#define I2C0_SCL_ADDR 0x114f0074
#define I2C0_REF_CFG 0x0402

#include "i2c_if.h"
#include "gpio_if.h"
#include "osal_io.h"
#include "osal_time.h"
#include <stdlib.h>

struct I2cDevice
{
	uint16_t busId;
	uint16_t addr;	
	DevHandle handle;
};

void print_use_guide(void);
static void i2c_test(void);
static int i2c_readwrite(struct I2cDevice* i2c, unsigned char* regData, unsigned int dataLen, uint8_t flag);

#endif