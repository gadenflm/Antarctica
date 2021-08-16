#ifndef _AMBA_VIN_H_
#define _AMBA_VIN_H_

#include "tvt_sdk_chip.h"

#include "iav_ioctl.h"
#include "iav_ucode_ioctl.h"
#include "vp_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 设置镜像，通过配置Sensor寄存器实现
 * @return 0：成功；非0：失败
 */
int amba_vin_dev_setMirror(int s32VinDevID, int s32Enable);

/**
 * @brief 设置翻转，通过配置Sensor寄存器实现
 * @return 0：成功；非0：失败
 */
int amba_vin_dev_setFlip(int s32VinDevID, int s32Enable);

/**
 * @brief Step1：设置dev属性，告诉Sensor当前设置的模式
 * @param [in] pstVideoCfg：Dev属性
 * @return 0：成功；非0：失败
 */
int amba_vin_dev_start(const CHIP_VIDEO_CFG_S* pstVideoCfg);


/**
 * @brief 初始化模块
 * @return 0：成功；非0：失败
 */
int amba_vin_init(int s32IavFD);

/**
 * @brief 去初始化模块
 * @return 0：成功；非0：失败
 */
int amba_vin_exit();



#ifdef __cplusplus
}
#endif

#endif