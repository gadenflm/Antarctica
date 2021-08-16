#ifndef _AMBA_VOUT_H_
#define _AMBA_VOUT_H_

#include "tvt_sdk_chip.h"

#include "iav_ioctl.h"
#include "iav_ucode_ioctl.h"
#include "vp_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AMBA_VOUT_ID 0

/**
 * @brief 初始化模块
 * @return 0：成功；非0：失败
 */
int amba_vout_init(int s32IavFD, CHIP_VIDEO_FORMAT_E enFormat);

/**
 * @brief 去初始化模块
 * @return 0：成功；非0：失败
 */
int amba_vout_exit();

#ifdef __cplusplus
}
#endif

#endif