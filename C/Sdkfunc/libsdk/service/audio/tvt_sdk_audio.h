#ifndef _TVT_SDK_AUDIO_H_
#define _TVT_SDK_ADUIO_H_

#include "tvt_sdk_product_param.h"
#include "sdk_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 控制命令枚举
 */
typedef enum tvt_sdk_audio_cmd_e
{
    TVT_SDK_AUDIO_CFG_CAPTURE,      ///< PARAM: TVT_SDK_AUDIO_CFG_S
    TVT_SDK_AUDIO_CFG_PLAYBACK,     ///< PARAM: TVT_SDK_AUDIO_CFG_S
    TVT_SDK_AUDIO_SET_CAP_VOLUME,   ///< PARAM: int [0,100]
    TVT_SDK_AUDIO_SET_PLAY_VOLUME,  ///< PARAM: int [0,100]
    TVT_SDK_AUDIO_CMD_BUTT,
} TVT_SDK_AUDIO_CMD_E;


typedef struct tvt_sdk_audio_cfg_s
{
    SDK_AUDIO_ENCODE_TYPE   enCodeType;     ///< 编码类型
    int                     s32Rate;        ///< 采样率 8000/16000...
    int                     s32BitWidth;    ///< 位宽 8/16/24
    int                     s32FrameSize;   ///< 单帧数据量
} TVT_SDK_AUDIO_CFG_S;


/**
 * @brief 初始化音频业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_audio_init(TVT_SDK_PROD_DEV_INFO* pstParamDevInfo, TvtSdkProdCb pParamCb);

/**
 * @brief 去初始化音频业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_audio_exit();

/**
 * @brief 音频业务控制接口
 * @param [in] s32Cmd:命令字
 * @param [in] pParam:参数指针
 * @param [in] s32Length:参数长度
 * @return 0：成功；非0：失败
 */
int tvt_sdk_audio_ctrl(int s32Cmd, void* pParam, int s32Length);

/**
 * @brief 获取音频编码流
 * @return 0：成功；非0：失败
 */
int tvt_sdk_audio_getCaptureStream(IPCAMERA_AV_FRAME_INFO* pstFrameInfo);

/**
 * @brief 推送音频编码流
 * @return 0：成功；非0：失败
 */
int tvt_sdk_audio_sendPlayStream(IPCAMERA_AV_FRAME_INFO* pstFrameInfo);

/**
 * @brief 释放音频编码流
 * @return 0：成功；非0：失败
 */
int tvt_sdk_audio_releaseStream(IPCAMERA_AV_FRAME_INFO* pstFrameInfo);

#ifdef __cplusplus
}
#endif

#endif