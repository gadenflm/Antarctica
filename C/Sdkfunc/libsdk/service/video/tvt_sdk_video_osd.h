#ifndef _TVT_SDK_VIDEO_OSD_H_
#define _TVT_SDK_VIDEO_OSD_H_

#include "tvt_sdk_video.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief 设置时间OSD格式位置
 * @return 0：成功；非0：失败
 */
int tvt_sdk_osd_setTimeOSDCfg(int s32VinIDX, const TVT_SDK_OSD_TIME_FMT_S* pstTimeFmt, const SDK_POSITION* pstPos);

/**
 * @brief 设置Comm OSD位置以及内容
 * @return 0：成功；非0：失败
 */
int tvt_sdk_osd_setCommOSD(int s32VinIDX, int s32OSDIDX, const TVT_SDK_OSD_COMM_INFO_S* pstCfg, const SDK_POSITION* pstPos);

/**
 * @brief 刷新Comm OSD显示数据
 * @return 0：成功；非0：失败
 */
int tvt_sdk_osd_updateCommOSD(int s32VinIDX, int s32OSDIDX);

/**
 * @brief 刷新时间OSD内容
 * @return 0：成功；非0：失败
 */
int tvt_sdk_osd_updateTimeOSD(int s32VinIDX);

/**
 * @brief 显示所有OSD
 * @return 0：成功；非0：失败
 */
int tvt_sdk_osd_startDispALLOSD();

/**
 * @brief 停止显示所有OSD
 * @return 0：成功；非0：失败
 */
int tvt_sdk_osd_stopDispALLOSD();


/**
 * @brief 初始化视频OSD业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_osd_init(int s32VinNum, const int* s32VinDevID);

/**
 * @brief 去初始化视频OSD业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_osd_exit();


#ifdef __cplusplus
}
#endif

#endif