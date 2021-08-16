#include <stdlib.h>
#include <string.h>

#include "sdk_func.h"
#include "sdk_define.h"
#include "sdk_private.h"

#include "tvt_sdk_param.h"
#include "tvt_sdk_print.h"
#include "tvt_sdk_audio.h"

extern TVT_SDK_CFG_S g_stSdkCfg;


/**
 * @brief 设置音频参数
 * @return 0：成功；非0：失败
 */
int SDKSetAudioPara(SDK_AUDIO_RATE eRate, SDK_AUDIO_BIT_WIDTH eBitWidth,
                    SDK_AUDIO_ENCODE_TYPE eEncType, unsigned int uPtPerFrame)
{
    int s32Ret = 0;
    // 音频输入和输出均采用同一套配置
    TVT_SDK_AUDIO_CFG_S stAudioCfg = {0};
    stAudioCfg.s32Rate = eRate;
    stAudioCfg.s32FrameSize = (int)uPtPerFrame;
    switch (eBitWidth)
    {
    case SDK_AUDIO_BIT_WIDTH_8:
        stAudioCfg.s32BitWidth = 8;
        break;
    case SDK_AUDIO_BIT_WIDTH_16:
        stAudioCfg.s32BitWidth = 16;
        break;
    case SDK_AUDIO_BIT_WIDTH_32:
        stAudioCfg.s32BitWidth = 32;
        break;
    default:
        stAudioCfg.s32BitWidth = 8;
        break;
    }
    stAudioCfg.enCodeType = eEncType;
    s32Ret = tvt_sdk_audio_ctrl(TVT_SDK_AUDIO_CFG_CAPTURE, &stAudioCfg, sizeof(stAudioCfg));
    // if (s32Ret != 0)
    // {
    //     TIPC_PRINTF_ERR("TVT_SDK_AUDIO_CFG_CAPTURE failed[%d]!!", s32Ret);
    // }
    // s32Ret = tvt_sdk_audio_ctrl(TVT_SDK_AUDIO_CFG_PLAYBACK, &stAudioCfg, sizeof(stAudioCfg));
    // if (s32Ret != 0)
    // {
    //     TIPC_PRINTF_ERR("TVT_SDK_AUDIO_CFG_PLAYBACK failed[%d]!!", s32Ret);
    // }
    return 0;
}

/**
 * @brief 获取音频帧
 * @return 0：成功；非0：失败
 */
int SDKGetAudioFrame(int iStream, IPCAMERA_AV_FRAME_INFO *pFrameInfo)
{
    int s32Ret = 0;
    /// TODO: iStream
    if (NULL == pFrameInfo)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    pFrameInfo->ucStreamIndex = iStream;
    s32Ret = tvt_sdk_audio_getCaptureStream(pFrameInfo);
    if (s32Ret != 0)
    {
        return -1;
    }
    return 0;
}

/**
 * @brief 推送音频帧
 * @return 0：成功；非0：失败
 */
int SDKPushAudioFrame(int iStream, IPCAMERA_AV_FRAME_INFO *pFrameInfo)
{
    int s32Ret = 0;
    /// TODO: iStream
    if (NULL == pFrameInfo)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    pFrameInfo->ucStreamIndex = iStream;
    s32Ret = tvt_sdk_audio_sendPlayStream(pFrameInfo);
    if (s32Ret != 0)
    {
        return -1;
    }
    return 0;
}

/**
 * @brief 开启音频
 * @return 0：成功；非0：失败
 */
int SDKStartAudio(int iStream)
{
    ///TODO:
    return 0;
}

/**
 * @brief 关闭音频
 * @return 0：成功；非0：失败
 */
int SDKStopAudio(int iStream)
{
    ///TODO:
    return 0;
}