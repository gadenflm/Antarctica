#ifndef _TVT_SDK_VIDEO_ROI_H_
#define _TVT_SDK_VIDEO_ROI_H_

#include "tvt_sdk_video.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief 设置ROI位置以及内容
 * @return 0：成功；非0：失败
 */
int tvt_sdk_roi_setPos(int s32VinIDX, int s32ROINum, const TVT_SDK_VIDEO_ROI_INFO_S* pstROIInfo);

/**
 * @brief 显示所有ROI
 * @return 0：成功；非0：失败
 */
int tvt_sdk_roi_startDispALL();

/**
 * @brief 停止显示所有ROI
 * @return 0：成功；非0：失败
 */
int tvt_sdk_roi_stopDispALL();


/**
 * @brief 初始化视频ROI业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_roi_init(int s32VinNum, const int* s32VinDevID);

/**
 * @brief 去初始化视频ROI业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_roi_exit();


#ifdef __cplusplus
}
#endif

#endif