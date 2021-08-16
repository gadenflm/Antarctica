#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <fcntl.h>
#include "tvt_sdk_chip.h"
#include <alsa/asoundlib.h>

#ifndef AM_IOCTL
#define AM_IOCTL(_filp, _cmd, _arg) \
        do {                        \
            if (ioctl(_filp, _cmd, _arg) < 0) { \
                perror(#_cmd);      \
                TIPC_PRINTF_ERR("%s err [%d]", #_cmd, errno); \
                return -1;      \
            }                       \
        } while (0)
#endif

// #define AMBA_PCM_DEV_NAME "default"
#define AMBA_PCM_DEV_NAME "hw:0,0"
#define AMBA_PCM_DEV_CAP_VOLUME_NAME "Capture Volume"
#define AMBA_PCM_DEV_PLAY_VOLUME_NAME "Playback Volume"
#define AMBA_AUDIO_MAX_BUFFER_TIME 500000

typedef struct amba_audio_dev_info_s
{
    unsigned char       u8Init;
    int                 s32Rate;
    int                 s32ChannelNum;
    snd_pcm_format_t    enFormat;
    int                 s32BitsPerFrame;
    int                 s32BitsPerSample;
    int                 s32ChunkSize;
    int                 s32ChunkBytes;
} AMBA_AUDIO_DEV_INFO_S;

typedef struct amba_audio_cfg_s
{
    snd_pcm_t*              pCapDevHandle;
    snd_pcm_t*              pPlayDevHandle;
    AMBA_AUDIO_DEV_INFO_S   stCapInfo;
    AMBA_AUDIO_DEV_INFO_S   stPlayInfo;
} AMBA_AUDIO_CFG_S;

AMBA_AUDIO_CFG_S g_stAudioCfg = {0};


/**
 * @brief  Wkl Audio Debug
 */
static void show_pcm_paras(const snd_pcm_hw_params_t* HwParams) {


    snd_pcm_access_t access = 0;
    snd_pcm_hw_params_get_access(HwParams, &access);

    snd_pcm_format_t format = 0;
    snd_pcm_hw_params_get_format(HwParams, &format);

    unsigned int channel_num = 0;
    snd_pcm_hw_params_get_channels(HwParams, &channel_num);

    TIPC_PRINTF_CHK("[ !!!!!! ] Access: %s", snd_pcm_access_name(access));
    TIPC_PRINTF_CHK("[ !!!!!! ] Format: %s", snd_pcm_format_name(format));
    TIPC_PRINTF_CHK("[ !!!!!! ] Channel Num = %u", channel_num);

    return;
}

/**
 * @brief I/O error handler
 */
static int xrun(snd_pcm_t* pHandle, snd_pcm_stream_t stream)
{
    snd_pcm_status_t* pStatus;
    int s32Ret;

    snd_pcm_status_alloca(&pStatus);
    if ((s32Ret = snd_pcm_status(pHandle, pStatus)) < 0)
    {
        TIPC_PRINTF_ERR("status error: %s", snd_strerror(s32Ret));
        return -1;
    }
    if (snd_pcm_status_get_state(pStatus) == SND_PCM_STATE_XRUN)
    {
        struct timeval now, diff, tstamp;
        gettimeofday(&now, 0);
        snd_pcm_status_get_trigger_tstamp(pStatus, &tstamp);
        timersub(&now, &tstamp, &diff);
        // fprintf(stderr, "%s!!! (at least %.3f ms long)\n",
        //     stream == SND_PCM_STREAM_PLAYBACK ? "underrun" : "overrun",
        //     diff.tv_sec * 1000 + diff.tv_usec / 1000.0);
        TIPC_PRINTF_ERR("%s!!! (at least %.3f ms long)",
                        stream == SND_PCM_STREAM_PLAYBACK ? "underrun" : "overrun",
                        diff.tv_sec * 1000 + diff.tv_usec / 1000.0);

        if ((s32Ret = snd_pcm_prepare(pHandle)) < 0)
        {
            TIPC_PRINTF_ERR("xrun: prepare error: %s", snd_strerror(s32Ret));
            return -1;
        }
        return 0; /* ok, data should be accepted again */
    }
    if (snd_pcm_status_get_state(pStatus) == SND_PCM_STATE_DRAINING)
    {
        TIPC_PRINTF_ERR("draining!!!");
        if (stream == SND_PCM_STREAM_CAPTURE)
        {
            // fprintf(stderr, "capture stream format change? attempting recover...\n");
            TIPC_PRINTF_ERR("capture stream format change? attempting recover...");
            if ((s32Ret = snd_pcm_prepare(pHandle)) < 0)
            {
                TIPC_PRINTF_ERR("xrun(DRAINING): prepare error: %s", snd_strerror(s32Ret));
                return -1;
            }
            return 0;
        }
    }

    return 0;
}

/**
 * @brief 计算每帧的间隔时间
 * @return 0：成功；非0：失败
 */
static int amba_audio_getPeriodTime(int s32Rate, int s32FrameSize, int* s32PeriodTime)
{
    if ((0 == s32Rate) || (0 == s32FrameSize))
    {
        TIPC_PRINTF_ERR("err input");
        return -1;
    }
    int s32Time = 1000000 * s32FrameSize / s32Rate;
    *s32PeriodTime = s32Time;
    return 0;
}


/**
 * @brief 获取平台对应的格式
 * @return 0：成功；非0：失败
 */
static int amba_audio_getFormat(CHIP_AUDIO_CODE_TYPE_E enCodeType, snd_pcm_format_t* enFormat)
{
    if (NULL == enFormat)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    snd_pcm_format_t enPcmFormat = 0;
    switch (enCodeType)
    {
    case CHIP_AUDIO_CODE_G711A:
        enPcmFormat = SND_PCM_FORMAT_A_LAW;
        break;
    case CHIP_AUDIO_CODE_G711U:
        enPcmFormat = SND_PCM_FORMAT_MU_LAW;
        break;
    case CHIP_AUDIO_CODE_PCM:
        enPcmFormat = SND_PCM_FORMAT_S16_LE;
        break;
    default:
        TIPC_PRINTF_ERR("unsupport audio code type[%d]", enCodeType);
        return -1;
        break;
    }
    *enFormat = enPcmFormat;
    return 0;
}


/**
 * @brief 芯片音频模块参数设置接口，参考 test_audio.cpp
 * @return 0：成功；非0：失败
 */
int chip_audio_setParam(const CHIP_AUDIO_PARAM_S* pstAudioParam)
{
    /// TODO: 切换linein/micin
    int s32Ret = 0;
    if (NULL == pstAudioParam)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    snd_pcm_t* pHandle = (CHIP_AUDIO_STREAM_CAPTURE == pstAudioParam->enStreamType) ? g_stAudioCfg.pCapDevHandle : g_stAudioCfg.pPlayDevHandle;
    if (NULL == pHandle)
    {
        TIPC_PRINTF_ERR("audio dev[%s] handle is NULL", (CHIP_AUDIO_STREAM_CAPTURE == pstAudioParam->enStreamType) ? "capture" : "playback");
        return -1;
    }
    snd_pcm_hw_params_t* pstHwParams = NULL;
    snd_pcm_sw_params_t* pstSwParams = NULL;
    snd_pcm_uframes_t u32BufferSize = 0;
    snd_pcm_uframes_t u32ChunkSize = 0;
    snd_pcm_uframes_t u32StartThreshold = 0;
    unsigned int u32PeriodTime = 0;
    unsigned int u32BufferTime = 0;
    size_t s32ChunkBytes = 0;

    snd_pcm_hw_params_alloca(&pstHwParams);
    snd_pcm_sw_params_alloca(&pstSwParams);
    // snd_pcm_hw_params_malloc(&pstHwParams);
    // snd_pcm_hw_params_malloc(&pstSwParams);

    if ((NULL == pstHwParams) || (NULL == pstSwParams))
    {
        TIPC_PRINTF_ERR("snd alloca failed");
        return -1;
    }

    s32Ret = snd_pcm_hw_params_any(pHandle, pstHwParams);
    if (s32Ret < 0)
    {
        TIPC_PRINTF_ERR("Broken configuration for this PCM: no configurations available");
        return -1;
    }
    // show_pcm_paras(pstHwParams);
    s32Ret = snd_pcm_hw_params_set_access(pHandle, pstHwParams, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (s32Ret < 0)
    {
        TIPC_PRINTF_ERR("Access type not available");
        return -1;
    }

    snd_pcm_format_t enCodeFormat = 0;
    s32Ret = amba_audio_getFormat(pstAudioParam->en32CodeType, &enCodeFormat);
    if (s32Ret != 0)
    {
        return -1;
    }
    enCodeFormat = SND_PCM_FORMAT_S16_LE;
    s32Ret = snd_pcm_hw_params_set_format(pHandle, pstHwParams, enCodeFormat);
    if (s32Ret < 0)
    {
        TIPC_PRINTF_ERR("Sample format non available");
        return -1;
    }

    s32Ret = snd_pcm_hw_params_set_channels(pHandle, pstHwParams, pstAudioParam->s32ChannelNum + 1);
    if (s32Ret < 0)
    {
        TIPC_PRINTF_ERR("Channels Num[%d] non available", pstAudioParam->s32ChannelNum);
        return -1;
    }

    s32Ret = snd_pcm_hw_params_set_rate(pHandle, pstHwParams, pstAudioParam->s32SampleRate, 0);
    if (s32Ret < 0)
    {
        TIPC_PRINTF_ERR("Rate [%d] non available", pstAudioParam->s32SampleRate);
        return -1;
    }

    s32Ret = snd_pcm_hw_params_get_buffer_time_max(pstHwParams, &u32BufferTime, 0);
    if (s32Ret < 0)
    {
        TIPC_PRINTF_ERR("get max Buffer Time failed");
        return -1;
    }

    if (u32BufferTime > AMBA_AUDIO_MAX_BUFFER_TIME)
    {
        u32BufferTime = AMBA_AUDIO_MAX_BUFFER_TIME;
    }

    s32Ret = amba_audio_getPeriodTime(pstAudioParam->s32SampleRate, pstAudioParam->s32FrameSize, (int*)&u32PeriodTime);
    if (s32Ret != 0)
    {
        return -1;
    }

    if (u32PeriodTime * 4 > u32BufferTime)
    {
        TIPC_PRINTF_ERR("Period Time[%d] should be less than [%d]", u32PeriodTime, (u32BufferTime / 4));
        return -1;
    }

    // u32BufferTime = u32PeriodTime * 4;

    s32Ret = snd_pcm_hw_params_set_period_time_near(pHandle, pstHwParams, &u32PeriodTime, 0);
    if (s32Ret < 0)
    {
        TIPC_PRINTF_ERR("set Period Time failed");
        return -1;
    }

    s32Ret = snd_pcm_hw_params_set_buffer_time_near(pHandle, pstHwParams, &u32BufferTime, 0);
    if (s32Ret < 0)
    {
        TIPC_PRINTF_ERR("set Period Time failed");
        return -1;
    }
    s32Ret = snd_pcm_hw_params(pHandle, pstHwParams);
    if (s32Ret < 0)
    {
        TIPC_PRINTF_ERR("Unable to install hw params:");
        return -1;
    }

    snd_pcm_hw_params_get_period_size(pstHwParams, &u32ChunkSize, 0);
    snd_pcm_hw_params_get_buffer_size(pstHwParams, &u32BufferSize);
    if (u32ChunkSize == u32BufferSize)
    {
        TIPC_PRINTF_ERR("Can't use period equal to buffer size (%lu == %lu)",
                        u32ChunkSize, u32BufferSize);
        return -1;
    }

    if (0 == u32ChunkSize)
    {
        TIPC_PRINTF_ERR("chunk size err");
        return -1;
    }

    snd_pcm_sw_params_current(pHandle, pstSwParams);
    snd_pcm_sw_params_set_avail_min(pHandle, pstSwParams, u32ChunkSize);
    if (CHIP_AUDIO_STREAM_PLAYBACK == pstAudioParam->enStreamType)
    {
        u32StartThreshold = (u32BufferSize / u32ChunkSize) * u32ChunkSize;
    }
    else
    {
        u32StartThreshold = 1;
    }
    s32Ret = snd_pcm_sw_params_set_start_threshold(pHandle, pstSwParams, u32StartThreshold);
    if (s32Ret < 0)
    {
        TIPC_PRINTF_ERR("set start threshold failed\n");
        return -1;
    }

    s32Ret = snd_pcm_sw_params_set_stop_threshold(pHandle, pstSwParams, u32BufferSize);
    if (s32Ret < 0)
    {
        TIPC_PRINTF_ERR("set stop threshold failed\n");
        return -1;
    }

    s32Ret = snd_pcm_sw_params(pHandle, pstSwParams);
    if (s32Ret < 0)
    {
        TIPC_PRINTF_ERR("unable to install sw params:\n");
        return -1;
    }

    int s32BitsPerSample = snd_pcm_format_physical_width(enCodeFormat);
    int s32BitsPerFrame = s32BitsPerSample * pstAudioParam->s32ChannelNum;

    s32ChunkBytes = u32ChunkSize * s32BitsPerFrame / 8;

    AMBA_AUDIO_DEV_INFO_S* pstDevInfo = NULL;

    if (CHIP_AUDIO_STREAM_PLAYBACK == pstAudioParam->enStreamType)
    {
        pstDevInfo = &g_stAudioCfg.stPlayInfo;
    }
    else
    {
        pstDevInfo = &g_stAudioCfg.stCapInfo;
    }

    if (pstDevInfo != NULL)
    {
        pstDevInfo->s32Rate = pstAudioParam->s32SampleRate;
        pstDevInfo->s32ChannelNum = pstAudioParam->s32ChannelNum;
        pstDevInfo->enFormat = enCodeFormat;
        pstDevInfo->s32BitsPerFrame = s32BitsPerFrame;
        pstDevInfo->s32BitsPerSample = s32BitsPerSample;
        pstDevInfo->s32ChunkSize = (int)u32ChunkSize;
        pstDevInfo->s32ChunkBytes = s32ChunkBytes;
        pstDevInfo->u8Init = 1;
    }

    TIPC_PRINTF_CHK("chunk_size = %d,chunk_bytes = %d,buffer_size = %d\n\n", (int)u32ChunkSize, (int)s32ChunkBytes, (int)u32BufferSize);

    show_pcm_paras(pstHwParams);

    // snd_pcm_sw_params_free (pstSwParams);
    // snd_pcm_sw_params_free (pstHwParams);
    return 0;
}

/**
 * @brief 芯片音频模块获取音频数据（音频输入）
 * @return 0：成功；非0：失败
 */
int chip_audio_getCapBuffer(char* pszBuffer, int* ps32BuffSize)
{
    int s32Ret = 0;
    int s32InputSize = *(int*)ps32BuffSize;
    snd_pcm_t* pHandle = g_stAudioCfg.pCapDevHandle;
    if (NULL == pHandle || NULL == pszBuffer)
    {
        TIPC_PRINTF_ERR("NULL Pointer\n");
        return -1;
    }
    if (0 == g_stAudioCfg.stCapInfo.u8Init)
    {
        TIPC_PRINTF_ERR("Audio Capture dev should config first\n");
        return -1;
    }
    if (s32InputSize < g_stAudioCfg.stCapInfo.s32ChunkBytes)
    {
        TIPC_PRINTF_ERR("Audio Capture buffer size[%d] should larger than [%d]\n", s32InputSize, g_stAudioCfg.stCapInfo.s32ChunkBytes);
        return -1;
    }
    ssize_t s32ReadSize = 0;
    size_t s32ChunkSize = g_stAudioCfg.stCapInfo.s32ChunkSize;
    size_t s32BitsPerFrame = g_stAudioCfg.stCapInfo.s32BitsPerFrame;
    unsigned char* pszData = (unsigned char*)pszBuffer;
    while (s32ChunkSize > 0)
    {
        s32ReadSize = snd_pcm_readi(pHandle, pszBuffer, s32ChunkSize);
        if (s32ReadSize == -EAGAIN || (s32ReadSize >= 0 && (size_t)s32ReadSize < s32ChunkSize))
        {
            snd_pcm_wait(pHandle, 1000);
        }
        else if (s32ReadSize == -EPIPE)
        {
            s32Ret = xrun(pHandle, SND_PCM_STREAM_CAPTURE);
            if (s32Ret != 0)
            {
                return -1;
            }
        }
        else if (s32ReadSize < 0)
        {
            TIPC_PRINTF_ERR("read error: %s", snd_strerror(s32ReadSize));
            return -1;
        }

        if (s32ReadSize > 0)
        {
            s32ChunkSize -= s32ReadSize;
            pszData += s32ReadSize * s32BitsPerFrame / 8;
        }
    }

    /// TODO: AAC 格式还需要进行编码操作

    *ps32BuffSize = g_stAudioCfg.stCapInfo.s32ChunkBytes;

    return 0;
}

/**
 * @brief 芯片音频模块播放音频数据（音频输出）
 * @return 0：成功；非0：失败
 */
int chip_audio_sendPlayBuffer(char* pszBuffer, int s32BuffSize)
{
    int s32Ret = 0;
    snd_pcm_t* pHandle = g_stAudioCfg.pPlayDevHandle;
    if (NULL == pHandle || NULL == pszBuffer)
    {
        TIPC_PRINTF_ERR("NULL Pointer\n");
        return -1;
    }
    if (0 == g_stAudioCfg.stPlayInfo.u8Init)
    {
        TIPC_PRINTF_ERR("Audio Playback dev should config first\n");
        return -1;
    }
    if (s32BuffSize != g_stAudioCfg.stPlayInfo.s32ChunkBytes)
    {
        TIPC_PRINTF_CHK("Audio Playback buffer size[%d] should equal to [%d]\n", s32BuffSize, g_stAudioCfg.stPlayInfo.s32ChunkBytes);
    }

    /// TODO: AAC 格式还需要进行解码操作

    size_t s32BitsPerFrame = g_stAudioCfg.stPlayInfo.s32BitsPerFrame;
    size_t s32ChunkSize = s32BuffSize * 8 / s32BitsPerFrame;
    ssize_t s32WriteSize = 0;
    unsigned char* pszData = (unsigned char*)pszBuffer;
    while (s32ChunkSize > 0)
    {
        s32WriteSize = snd_pcm_writei(pHandle, pszData, s32ChunkSize);
        if (s32WriteSize == -EAGAIN || (s32WriteSize >= 0 && (size_t)s32WriteSize < s32ChunkSize))
        {
            snd_pcm_wait(pHandle, 1000);
        }
        else if (s32WriteSize == -EPIPE)
        {
            s32Ret = snd_pcm_prepare(pHandle);
            if (s32Ret < 0)
            {
                TIPC_PRINTF_ERR("Failed to recover from under run: %s", snd_strerror(s32Ret));
            }
            // s32Ret = xrun(pHandle, SND_PCM_STREAM_PLAYBACK);
            // if(s32Ret != 0)
            // {
            //     return -1;
            // }
        }
        else if (s32WriteSize < 0)
        {
            if (s32WriteSize == -EIO)
            {
                TIPC_PRINTF_ERR("write error: %s -EIO error!", snd_strerror(s32WriteSize));
            }
            else if (s32WriteSize == -EINVAL)
            {
                TIPC_PRINTF_ERR("write error: %s -EINVAL error!", snd_strerror(s32WriteSize));
            }
            else
            {
                TIPC_PRINTF_ERR("write error: %s unknown error!", snd_strerror(s32WriteSize));
            }
            return -1;
        }
        if (s32WriteSize > 0)
        {
            s32ChunkSize -= s32WriteSize;
            pszData += s32WriteSize * s32BitsPerFrame / 8;
        }
    }
    return 0;
}

/**
 * @brief 芯片音频模块设置音频输入音量
 * @return 0：成功；非0：失败
 */
int chip_audio_setVolume(CHIP_AUDIO_STREAM_TYPE_E enStreamType, int s32Volume)
{
    int s32Ret = 0;
    snd_hctl_t* pHctl = NULL;
    snd_ctl_elem_id_t* pID = NULL;
    snd_ctl_elem_value_t* pControl = NULL;
    snd_ctl_elem_info_t* pInfo = NULL;
    int s32MaxVolume = 0, s32MinVolume = 0;
    // snd_hctl_open(&pHctl, AMBA_PCM_DEV_NAME, 0);
    snd_hctl_open(&pHctl, "hw:0", 0);
    if (NULL == pHctl)
    {
        TIPC_PRINTF_ERR("snd_hctl_open error!");
        return -1;
    }
    if (s32Volume < 0 || s32Volume > 100)
    {
        TIPC_PRINTF_ERR("Volume should less than 100 and larger than 0");
        return -1;
    }
    s32Ret = snd_hctl_load(pHctl);
    if (s32Ret < 0)
    {
        TIPC_PRINTF_ERR("snd_hctl_load error!");
        return -1;
    }
    snd_ctl_elem_id_alloca(&pID);
    if (NULL == pID)
    {
        TIPC_PRINTF_ERR("snd_ctl_elem_id_alloca error!");
        return -1;
    }
    snd_ctl_elem_id_set_interface(pID, SND_CTL_ELEM_IFACE_MIXER);
    char* pElemName = (CHIP_AUDIO_STREAM_CAPTURE == enStreamType) ? AMBA_PCM_DEV_CAP_VOLUME_NAME : AMBA_PCM_DEV_PLAY_VOLUME_NAME;
    snd_ctl_elem_id_set_name(pID, pElemName);
    snd_hctl_elem_t* pElem = snd_hctl_find_elem(pHctl, pID);
    if (NULL == pElem)
    {
        TIPC_PRINTF_ERR("snd_hctl_find_elem error!");
        return -1;
    }
    snd_ctl_elem_info_alloca(&pInfo);
    if (NULL == pInfo)
    {
        TIPC_PRINTF_ERR("snd_ctl_elem_info_alloca error!");
        return -1;
    }
    s32Ret = snd_hctl_elem_info(pElem, pInfo);
    if (s32Ret < 0)
    {
        TIPC_PRINTF_ERR("snd_hctl_elem_info error!");
        return -1;
    }

    s32MinVolume = snd_ctl_elem_info_get_min(pInfo);
    s32MaxVolume = snd_ctl_elem_info_get_max(pInfo);

    int s32TargetVolume = (s32MaxVolume - s32MinVolume) * s32Volume / 100 + s32MinVolume;

    snd_ctl_elem_value_alloca(&pControl);
    if (NULL == pControl)
    {
        TIPC_PRINTF_ERR("snd_ctl_elem_id_alloca error!");
        return -1;
    }
    snd_ctl_elem_value_set_id(pControl, pID);
    snd_ctl_elem_value_set_integer(pControl, 0, s32TargetVolume);
    s32Ret = snd_hctl_elem_write(pElem, pControl);
    if (s32Ret < 0)
    {
        TIPC_PRINTF_ERR("snd_hctl_elem_write error!");
        return -1;
    }
    return 0;
}

/**
 * @brief 芯片音频模块初始化接口
 * @return 0：成功；非0：失败
 */
int chip_audio_init()
{
    int s32Ret = 0;

    // 打开Capture设备以及playback设备
    if (NULL == g_stAudioCfg.pCapDevHandle)
    {
        s32Ret = snd_pcm_open(&g_stAudioCfg.pCapDevHandle, AMBA_PCM_DEV_NAME, SND_PCM_STREAM_CAPTURE, 0);
        if (s32Ret < 0)
        {
            TIPC_PRINTF_ERR("amba audio capture dev open failed: %s", snd_strerror(s32Ret));
            g_stAudioCfg.pCapDevHandle = NULL;
            return -1;
        }
    }
    if (NULL == g_stAudioCfg.pPlayDevHandle)
    {
        s32Ret = snd_pcm_open(&g_stAudioCfg.pPlayDevHandle, AMBA_PCM_DEV_NAME, SND_PCM_STREAM_PLAYBACK, 0);
        if (s32Ret < 0)
        {
            TIPC_PRINTF_ERR("amba audio capture dev open failed: %s", snd_strerror(s32Ret));
            g_stAudioCfg.pPlayDevHandle = NULL;
            return -1;
        }
    }
    return 0;
}


/**
 * @brief 芯片音频模块去初始化接口
 * @return 0：成功；非0：失败
 */
int chip_audio_exit()
{
    if (g_stAudioCfg.pCapDevHandle != NULL)
    {
        snd_pcm_close(g_stAudioCfg.pCapDevHandle);
    }

    if (g_stAudioCfg.pPlayDevHandle != NULL)
    {
        snd_pcm_close(g_stAudioCfg.pPlayDevHandle);
    }

    memset(&g_stAudioCfg, 0, sizeof(g_stAudioCfg));

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

static long unsigned int g_chunk_size = 0;
static long unsigned int g_chunk_bytes = 0;
static long unsigned int g_buffer_size = 0;
static long unsigned int g_bits_per_sample = 0;
static long unsigned int g_bits_per_frame = 0;

static int wkl_set_hw_param(snd_pcm_t* pcm_handle)
{
    snd_pcm_hw_params_t* hw_params = NULL;
    snd_pcm_hw_params_alloca(&hw_params);

    int ret = 0;
    ret = snd_pcm_hw_params_any(pcm_handle, hw_params);
    if (ret < 0) {
        TIPC_PRINTF_ERR("Error! snd_pcm_hw_params_any ");
        return -1;
    }

    unsigned int rate = 8000;

    ret += snd_pcm_hw_params_set_access (pcm_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    ret += snd_pcm_hw_params_set_format (pcm_handle, hw_params, SND_PCM_FORMAT_S16_LE);
    ret += snd_pcm_hw_params_set_rate_near (pcm_handle, hw_params, &rate, 0);
    ret += snd_pcm_hw_params_set_channels (pcm_handle, hw_params, 2);

    if (ret < 0) {
        TIPC_PRINTF_ERR("Set access, format, rate, channels, Error!")
        return -1;
    }

    unsigned int buffer_time = 0;
    ret += snd_pcm_hw_params_get_buffer_time_max(hw_params, &buffer_time, NULL);
    unsigned int period_time = 0;
    ret += snd_pcm_hw_params_get_period_time_max(hw_params, &period_time, NULL);
    TIPC_PRINTF_CHK("Get Buffer Time Max = %u, Period Time Max = %u.", buffer_time, period_time);

    period_time = buffer_time / 4;

    ret += snd_pcm_hw_params_set_period_time_near(pcm_handle, hw_params, &period_time, 0);
    ret += snd_pcm_hw_params_set_buffer_time_near(pcm_handle, hw_params, &buffer_time, 0);

    ret += snd_pcm_hw_params(pcm_handle, hw_params);
    if (ret < 0) {
        TIPC_PRINTF_CHK("Set Hardwere Parameters Error!");
        return -1;
    }

    ret += snd_pcm_hw_params_get_period_size(hw_params, &g_chunk_size, 0);
    ret += snd_pcm_hw_params_get_buffer_size(hw_params, &g_buffer_size);

    g_bits_per_sample = snd_pcm_format_physical_width(SND_PCM_FORMAT_S16_LE);
    g_bits_per_frame = g_bits_per_sample * 2;
    g_chunk_bytes = g_chunk_size * g_bits_per_frame / 8;

    TIPC_PRINTF_CHK("chunk_size = %u  chunk_bytes = %u  buffer size = %u ", g_chunk_size, g_chunk_bytes, g_buffer_size);
    TIPC_PRINTF_CHK("g_bits_per_sample = %u  g_bits_per_frame = %u", g_bits_per_sample, g_bits_per_frame);
    return 0;
}

static int wkl_set_sw_param(snd_pcm_t* pcm_handle)
{
    snd_pcm_sw_params_t* sw_params = NULL;

    snd_pcm_sw_params_alloca(&sw_params);

    snd_pcm_sw_params_current(pcm_handle, sw_params);

    int ret = 0;

    ret += snd_pcm_sw_params_set_avail_min(pcm_handle, sw_params, g_chunk_size);

    snd_pcm_uframes_t start_threshold = 1;
    ret += snd_pcm_sw_params_set_start_threshold(pcm_handle, sw_params, start_threshold);
    ret += snd_pcm_sw_params_set_stop_threshold(pcm_handle, sw_params, g_buffer_size);

    if (ret < 0 ) {
        TIPC_PRINTF_ERR("Prepare sw_params Set Error!");
        return -1;
    }

    if (snd_pcm_sw_params(pcm_handle, sw_params) < 0) {
        TIPC_PRINTF_ERR("Set Softwere Parameters Error!");
        return -1;
    }

    return 0;
}

static int wkl_capture(snd_pcm_t* pcm_handle) {
    const unsigned int size = g_chunk_bytes;
    char* buffer = (char*) malloc (size);

    const unsigned int half_size = g_chunk_bytes >> 1;
    char* left_buff = (char*) malloc (half_size);
    char* right_buff = (char*) malloc (half_size);

    int run_times = 20 ;
    int ret = 0;

    const char* filename = "audio_test.snd";
    FILE* pFile = fopen(filename, "wb");

    if (!pFile) {
        TIPC_PRINTF_ERR("Open File Failed");
        return -1;
    }

    while (run_times--) {
        ret = snd_pcm_readi(pcm_handle, buffer, g_chunk_size);
        if (ret == -EPIPE) {
            /* EPIPE means overrun */
            TIPC_PRINTF_ERR(" Over Run !");
            snd_pcm_prepare(pcm_handle);
        } else if (ret < 0) {
            TIPC_PRINTF_ERR("Error read: %s\n", snd_strerror(ret));
        } else if (ret != (int)g_chunk_size) {
            TIPC_PRINTF_ERR("Error, Short read, read %d frames\n", ret);
        }

        memset(left_buff, 0, half_size);
        memset(right_buff, 0, half_size);
        // TIPC_PRINTF_CHK("read: %d frames", ret)
        for (int i = 0; i < (half_size / 2); i++) {
            *(left_buff + i * 2) = *(buffer + i * 4 );
            *(left_buff + i * 2 + 1) = *(buffer + i * 4 + 1);

            *(right_buff + i * 2) = *(buffer + i * 4 + 2);
            *(right_buff + i * 2 + 1) = *(buffer + i * 4 + 3);
        }

        ret = fwrite(buffer, size, 1, pFile);
        fflush(pFile);
        memset(buffer, 0, size);
    }
    free (buffer);
    free (left_buff);
    free (right_buff);
    fclose(pFile);
    return 0;
}

static int wkl_play(snd_pcm_t* pcm_handle)
{
    const unsigned int size = g_chunk_bytes;
    char* buffer = (char*) malloc (size);

    const unsigned int half_size = g_chunk_bytes >> 1;
    char* left_buff = (char*) malloc (half_size);
    char* right_buff = (char*) malloc (half_size);

    int run_times = 20 ;
    int ret = 0;

    const char* filename = "8k_16bits_mono.pcm";
    FILE* pFile = fopen(filename, "rb");

    if (!pFile) {
        TIPC_PRINTF_ERR("Open File Failed");
        return -1;
    }

    while (run_times--) {
        memset(left_buff, 0, half_size);
        memset(right_buff, 0, half_size);
        ret = fread(left_buff, half_size, 1, pFile);
        memcpy(right_buff, left_buff, half_size);

        memset(buffer, 0, size);
        for (int i = 0; i < (half_size / 2); i++) {
            *(buffer + 4 * i) = * (left_buff + i * 2);
            *(buffer + 4 * i + 1 ) = * (left_buff + i * 2 + 1);
            *(buffer + 4 * i + 2 ) = * (right_buff + i * 2 ) ;
            *(buffer + 4 * i + 3 ) = * (right_buff + i * 2 + 1);
        }
        ret = snd_pcm_writei(pcm_handle, buffer, g_chunk_size);
        if (ret == -EPIPE) {
            /* EPIPE means overrun */
            TIPC_PRINTF_ERR(" Over Run !");
            snd_pcm_prepare(pcm_handle);
        } else if (ret < 0) {
            TIPC_PRINTF_ERR("Error write: %s\n", snd_strerror(ret));
        } else if (ret != (int)g_chunk_size) {
            TIPC_PRINTF_ERR("Error, Short write, write %d frames\n", ret);
        }

    }
    free (buffer);
    free (left_buff);
    free (right_buff);
    fclose(pFile);
    return 0;
}

int amba_audio_wkldebug(void)
{
    if (!g_stAudioCfg.pCapDevHandle || !g_stAudioCfg.pPlayDevHandle)
    {
        TIPC_PRINTF_ERR("Not Init");
        return -1;
    }
    TIPC_PRINTF_CHK("__________________________________________________________________________________________________________");

    snd_pcm_t* pcm_handle = g_stAudioCfg.pPlayDevHandle;

    wkl_set_hw_param(pcm_handle);
    wkl_set_sw_param(pcm_handle);
    wkl_play(pcm_handle);

    TIPC_PRINTF_CHK("__________________________________________________________________________________________________________");
    return 0;
}