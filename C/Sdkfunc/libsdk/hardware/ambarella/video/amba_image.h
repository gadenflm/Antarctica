#ifndef _AMBA_IMAGE_H_
#define _AMBA_IMAGE_H_

#include "tvt_sdk_chip.h"

#include "iav_ioctl.h"
#include "iav_ucode_ioctl.h"
#include "vp_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化模块
 * @return 0：成功；非0：失败
 */
int amba_image_init(int s32IavFD);

/**
 * @brief 去初始化模块
 * @return 0：成功；非0：失败
 */
int amba_image_exit();



#ifdef __cplusplus
}
#endif

#endif