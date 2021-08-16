#ifndef _AMBA_OSD_H_
#define _AMBA_OSD_H_

#include "tvt_sdk_chip.h"

#include "iav_ioctl.h"
#include <basetypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 设置OSD
 * @return 0：成功；非0：失败
 */
int amba_osd_setOverLay(CHIP_OSD_INFO_S* pstOSDInfo);

/**
 * @brief 初始化模块
 * @return 0：成功；非0：失败
 */
int amba_osd_init(int s32IavFD, int s32VinNum);

/**
 * @brief 去初始化模块
 * @return 0：成功；非0：失败
 */
int amba_osd_exit();



#ifdef __cplusplus
}
#endif

#endif