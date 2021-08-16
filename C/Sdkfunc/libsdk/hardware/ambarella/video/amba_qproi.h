#ifndef _AMBA_QPROI_H_
#define _AMBA_QPROI_H_

#include "tvt_sdk_chip.h"

#include "iav_ioctl.h"
#include <basetypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 设置ROI
 * @return 0：成功；非0：失败
 */
int amba_roi_setPos(CHIP_ROI_INFO_S* pstROIInfo);

/**
 * @brief 初始化模块
 * @return 0：成功；非0：失败
 */
int amba_roi_init(int s32IavFD, int s32VinNum);

/**
 * @brief 去初始化模块
 * @return 0：成功；非0：失败
 */
int amba_roi_exit();



#ifdef __cplusplus
}
#endif

#endif