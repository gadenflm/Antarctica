#ifndef _TVT_SDK_VIDEO_MASK_H_
#define _TVT_SDK_VIDEO_MASK_H_

#include "tvt_sdk_video.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 设置MASK位置以及内容
 * @return 0：成功；非0：失败
 */
int tvt_sdk_mask_setPos(int s32VinIDX, int s32Enable, const SDK_POSITION* pstPos);

/**
 * @brief 显示所有MASK
 * @return 0：成功；非0：失败
 */
int tvt_sdk_mask_startDispALL();

/**
 * @brief 停止显示所有MASK
 * @return 0：成功；非0：失败
 */
int tvt_sdk_mask_stopDispALL();

/**
 * @brief 初始化视频MASK业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_mask_init(int s32VinNum, const int* s32VinDevID);

/**
 * @brief 去初始化视频MASK业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_mask_exit();


#ifdef __cplusplus
}
#endif

#endif