#ifndef _AMBA_DEVICE_H_
#define _AMBA_DEVICE_H_

#include "tvt_sdk_chip.h"

#include "iav_ioctl.h"
#include <basetypes.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief 初始化模块
 * @return 0：成功；非0：失败
 */
int amba_device_init();

/**
 * @brief 去初始化模块
 * @return 0：成功；非0：失败
 */
int amba_device_exit();



#ifdef __cplusplus
}
#endif

#endif