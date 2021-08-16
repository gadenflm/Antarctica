#ifndef _AMBA_MASK_H_
#define _AMBA_MASK_H_

#include "tvt_sdk_chip.h"

#include "iav_ioctl.h"
#include <basetypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 设置MASK
 * @return 0：成功；非0：失败
 */
int amba_mask_setPos(CHIP_MASK_INFO_S* pstMaskInfo);

/**
 * @brief 初始化模块
 * @return 0：成功；非0：失败
 */
int amba_mask_init(int s32IavFD, int s32VinNum);

/**
 * @brief 去初始化模块
 * @return 0：成功；非0：失败
 */
int amba_mask_exit();


#ifdef __cplusplus
}
#endif

#endif