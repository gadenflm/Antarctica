#ifndef _SDK_PRIVATE_H_
#define _SDK_PRIVATE_H_

#include "tvt_sdk_product_param.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tvt_sdk_cfg_s
{
    unsigned char                       u8Init;
    int                                 s32SDKVersion;      ///< 版本号
    unsigned long long                  u64DevFeature;      ///< 设备能力集
    TVT_SDK_PROD_DEV_INFO               stDevModelInfo;     ///< 机型
    SDK_DAYNIGHT_STATE                  enDayNightState;    ///< 日夜状态
    int                                 s32WorkMode;        ///< 线性/HDR模式
    int                                 s32VinCnt;          ///< Sensor个数
    pthread_t                           s32SdkThread;
    int                                 s32SdkThreadRun;
}TVT_SDK_CFG_S;


typedef struct sdk_zoomcb_param_s
{
    SDK_LENS_DIRECTION enDirection;
    SDK_LENS_STATUS    enStatus;
    SDK_LENS_MODE      enMode;
}SDK_ZOOM_CB_PARAM_S;

/**
 * @brief 初始化视频业务
 * @return 0：成功；非0：失败
 */
int SDKInitVideo(SDK_VIDEO_FORMAT enInputFormat);



#ifdef __cplusplus
}
#endif

#endif