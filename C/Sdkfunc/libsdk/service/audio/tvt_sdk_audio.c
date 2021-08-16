
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "tvt_sdk_audio.h"
#include "tvt_sdk_print.h"
#include "tvt_sdk_chip.h"

#define TVT_SDK_AUDIO_DEBUG 0

#define TVT_SDK_AUDIO_CNT 2
#define TVT_SDK_AUDIO_CAP 0
#define TVT_SDK_AUDIO_PLAY 1

#define TVT_SDK_AUDIO_BUFFER_SIZE (1024576) // 1MB

typedef struct tvt_sdk_audio_buffer_s
{
    pthread_mutex_t                      s32BufMutex;
    char                                 szBufMutexName[32];
    unsigned int                         u32LockCnt;
    unsigned int                         u32FrameCnt;
    char*                                pszBuf;
} TVT_SDK_AUDIO_BUFFER_S;


typedef struct tvt_sdk_audio_dev_cfg_s
{
    unsigned char               u8Init;
    TVT_SDK_AUDIO_CFG_S         stAudioParam;
} TVT_SDK_AUDIO_DEV_CFG_S;


/**
 * @brief ҵ��ṹ��
 */
typedef struct tvt_sdk_audio_info_s
{
    char                                 u8Init;                            ///< ��ʼ����־λ
    char                                 u8Exit;                            ///< ȥ��ʼ����־λ���߳��˳���־
    pthread_mutex_t                      s32Mutex;                          ///< ҵ����
    char                                 szMutexName[32];                   ///< ҵ��������
    unsigned int                         u32MutexLockCnt;                   ///< ҵ������������
    pthread_t                            s32ThreadID;                       ///< ҵ���߳�ID
    TVT_SDK_AUDIO_DEV_CFG_S              stAudioCfg[TVT_SDK_AUDIO_CNT];     ///< ��Ƶ�豸����
    TVT_SDK_AUDIO_BUFFER_S               stAudioBuffer;
    TVT_SDK_PROD_DEV_INFO                stParamDevInfo;                    ///<
    TvtSdkProdCb                         pSdkParamCbFunc;
    int                                  s32ReserveArr[4];
} TVT_SDK_AUDIO_INFO_S;

static TVT_SDK_AUDIO_INFO_S* g_pstSdkAudioInfo = NULL;

typedef int (*AudioImplFunc)(TVT_SDK_AUDIO_INFO_S* pstHandle, void* pParam, int s32ParamSize);
typedef struct tvt_sdk_audio_ctrl_impl_s
{
    TVT_SDK_AUDIO_CMD_E         enCmd;          ///< ������
    AudioImplFunc               pImplFunc;      ///< ʵ�ֽӿ�
} TVT_SDK_AUDIO_CTRL_IMPL_S;

/**
 * @brief ��ȡģ��ṹ��ָ��
 */
static TVT_SDK_AUDIO_INFO_S* tvt_sdk_audio_getHandle(void)
{
    return g_pstSdkAudioInfo;
}

/**
 * @brief ������Ƶ��������
 * @return 0���ɹ�����0��ʧ��
 */
static int tvt_sdk_audio_setCapVolume(TVT_SDK_AUDIO_INFO_S* pstHandle, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int s32Volume = *(int*)pParam;
    s32Ret = chip_audio_setVolume(CHIP_AUDIO_STREAM_CAPTURE, s32Volume);
    if (s32Ret != 0)
    {
        return -1;
    }
    return 0;
}

/**
 * @brief ������Ƶ�������
 * @return 0���ɹ�����0��ʧ��
 */
static int tvt_sdk_audio_setPlayVolume(TVT_SDK_AUDIO_INFO_S* pstHandle, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int s32Volume = *(int*)pParam;
    s32Ret = chip_audio_setVolume(CHIP_AUDIO_STREAM_PLAYBACK, s32Volume);
    if (s32Ret != 0)
    {
        return -1;
    }
    return 0;
}

/**
 * @brief ������Ƶ����
 * @return 0���ɹ�����0��ʧ��
 */
static int tvt_sdk_audio_cfgCapture(TVT_SDK_AUDIO_INFO_S* pstHandle, void* pParam, int s32ParamSize)
{

    amba_audio_wkldebug();
    return 0;

    int s32Ret = 0;
    TVT_SDK_AUDIO_CFG_S* pstCfg = (TVT_SDK_AUDIO_CFG_S*)pParam;
    if (NULL == pstHandle->pSdkParamCbFunc)
    {
        TIPC_PRINTF_ERR("param callback should not be NULL\n");
        return -1;
    }
    // ��ȡ�豸����
    TVT_SDK_PROD_CTRL_INFO stParamCtrl = {0};
    memcpy(&stParamCtrl.stDevInfo, &pstHandle->stParamDevInfo, sizeof(TVT_SDK_PROD_DEV_INFO));
    s32Ret = pstHandle->pSdkParamCbFunc(TVT_SDK_PARAM_GET_AUDIO_IN_CFG, &stParamCtrl, sizeof(stParamCtrl));

    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get param cfg failed[%d]!!", s32Ret);
        return s32Ret;
    }
    TVT_SDK_PROD_AUDIO_S* pstAudioInCfg = stParamCtrl.pParam;
    if (NULL == pstAudioInCfg)
    {
        TIPC_PRINTF_ERR("audio in prod cfg is NULL!!");
        return -1;
    }

    if (1 == pstHandle->stAudioCfg[TVT_SDK_AUDIO_CAP].u8Init)
    {
        TVT_SDK_AUDIO_CFG_S* pstCurrParam = &pstHandle->stAudioCfg[TVT_SDK_AUDIO_CAP].stAudioParam;
        if (0 == memcmp(pstCurrParam, pstCfg, sizeof(TVT_SDK_AUDIO_CFG_S)))
        {
            TIPC_PRINTF_CHK("audio same config");
            return 0;
        }
    }
    /// TODO: �Ƿ���Ҫ�ȹرգ�
    // ���ò���
    CHIP_AUDIO_PARAM_S stAudioParam = {0};
    stAudioParam.s32ChannelNum = (TVT_SDK_PROD_AUDIO_MODE_STEREO == pstAudioInCfg->enAudioMode) ? 2 : 1;
    switch (pstCfg->enCodeType)
    {
    case SDK_AENC_TYPE_G711A:
        stAudioParam.en32CodeType = CHIP_AUDIO_CODE_G711A;
        break;
    case SDK_AENC_TYPE_G711U:
        stAudioParam.en32CodeType = CHIP_AUDIO_CODE_G711U;
        break;
    default:
        TIPC_PRINTF_ERR("audio not support format[%d]", pstCfg->enCodeType);
        return -1;
        break;
    }
    stAudioParam.s32BitWidth = pstCfg->s32BitWidth;
    stAudioParam.s32ChnID = (TVT_SDK_PROD_AUDIO_MODE_LEFT == pstAudioInCfg->enAudioMode) ? 0 : 1;
    stAudioParam.s32FrameSize = pstCfg->s32FrameSize;
    stAudioParam.s32SampleRate = pstCfg->s32Rate;
    stAudioParam.enStreamType = CHIP_AUDIO_STREAM_CAPTURE;

    s32Ret = chip_audio_setParam(&stAudioParam);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("audio config failed");
        return -1;
    }
    // ��������
    memcpy(&pstHandle->stAudioCfg[TVT_SDK_AUDIO_CAP].stAudioParam, pstCfg, sizeof(TVT_SDK_AUDIO_CFG_S));
    pstHandle->stAudioCfg[TVT_SDK_AUDIO_CAP].u8Init = 1;

    chip_audio_setVolume(CHIP_AUDIO_STREAM_CAPTURE, 100);
    return 0;
}

/**
 * @brief ������Ƶ���
 * @return 0���ɹ�����0��ʧ��
 */
static int tvt_sdk_audio_cfgPlayBack(TVT_SDK_AUDIO_INFO_S* pstHandle, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    TVT_SDK_AUDIO_CFG_S* pstCfg = (TVT_SDK_AUDIO_CFG_S*)pParam;
    if (NULL == pstHandle->pSdkParamCbFunc)
    {
        TIPC_PRINTF_ERR("param callback should not be NULL\n");
        return -1;
    }
    // ��ȡ�豸����
    TVT_SDK_PROD_CTRL_INFO stParamCtrl = {0};
    memcpy(&stParamCtrl.stDevInfo, &pstHandle->stParamDevInfo, sizeof(TVT_SDK_PROD_DEV_INFO));
    s32Ret = pstHandle->pSdkParamCbFunc(TVT_SDK_PARAM_GET_AUDIO_OUT_CFG, &stParamCtrl, sizeof(stParamCtrl));
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get param cfg failed[%d]!!", s32Ret);
        return s32Ret;
    }
    TVT_SDK_PROD_AUDIO_S* pstAudioInCfg = stParamCtrl.pParam;
    if (NULL == pstAudioInCfg)
    {
        TIPC_PRINTF_ERR("audio in prod cfg is NULL!!");
        return -1;
    }

    if (1 == pstHandle->stAudioCfg[TVT_SDK_AUDIO_PLAY].u8Init)
    {
        TVT_SDK_AUDIO_CFG_S* pstCurrParam = &pstHandle->stAudioCfg[TVT_SDK_AUDIO_PLAY].stAudioParam;
        if (0 == memcmp(pstCurrParam, pstCfg, sizeof(TVT_SDK_AUDIO_CFG_S)))
        {
            TIPC_PRINTF_CHK("audio same config");
            return 0;
        }
    }
    /// TODO: �Ƿ���Ҫ�ȹرգ�
    // ���ò���
    CHIP_AUDIO_PARAM_S stAudioParam = {0};
    stAudioParam.s32ChannelNum = (TVT_SDK_PROD_AUDIO_MODE_STEREO == pstAudioInCfg->enAudioMode) ? 2 : 1;
    switch (pstCfg->enCodeType)
    {
    case SDK_AENC_TYPE_G711A:
        stAudioParam.en32CodeType = CHIP_AUDIO_CODE_G711A;
        break;
    case SDK_AENC_TYPE_G711U:
        stAudioParam.en32CodeType = CHIP_AUDIO_CODE_G711U;
        break;
    default:
        TIPC_PRINTF_ERR("audio not support format[%d]", pstCfg->enCodeType);
        return -1;
        break;
    }
    stAudioParam.s32BitWidth = pstCfg->s32BitWidth;
    stAudioParam.s32ChnID = (TVT_SDK_PROD_AUDIO_MODE_LEFT == pstAudioInCfg->enAudioMode) ? 0 : 1;
    stAudioParam.s32FrameSize = pstCfg->s32FrameSize;
    stAudioParam.s32SampleRate = pstCfg->s32Rate;
    stAudioParam.enStreamType = CHIP_AUDIO_STREAM_PLAYBACK;

    // TIPC_PRINTF_CHK(" [****** Audio Debug ******] chip_audio_setParam by cfg Playback");
    s32Ret = chip_audio_setParam(&stAudioParam);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("audio config failed");
        return -1;
    }
    // ��������
    memcpy(&pstHandle->stAudioCfg[TVT_SDK_AUDIO_PLAY].stAudioParam, pstCfg, sizeof(TVT_SDK_AUDIO_CFG_S));
    pstHandle->stAudioCfg[TVT_SDK_AUDIO_PLAY].u8Init = 1;
    chip_audio_setVolume(CHIP_AUDIO_STREAM_PLAYBACK, 100);
    return 0;
}

/**
 * @brief ��ȡ��Ƶ������
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_audio_getCaptureStream(IPCAMERA_AV_FRAME_INFO* pstFrameInfo)
{
    int s32Ret = 0;
    TVT_SDK_AUDIO_INFO_S* pstHandle = tvt_sdk_audio_getHandle();
    if (NULL == pstHandle || NULL == pstFrameInfo)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }

    if (pstHandle->stAudioCfg[TVT_SDK_AUDIO_CAP].u8Init != 1)
    {
        TIPC_PRINTF_ERR("audio cap is not configured");
        return -1;
    }

    char* pszBuffer = pstHandle->stAudioBuffer.pszBuf;
    if (NULL == pszBuffer)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }

    // ����
    TIPC_MUTEX_LOCK(&g_pstSdkAudioInfo->stAudioBuffer.s32BufMutex,
                    g_pstSdkAudioInfo->stAudioBuffer.szBufMutexName,
                    &g_pstSdkAudioInfo->stAudioBuffer.u32LockCnt,
                    TVT_SDK_AUDIO_DEBUG);
    // ��ͷ
    /// TODO: ��AACӦ�ò���Ҫ��������Ҫ���ֿ�
    pszBuffer[0] = 0x0;
    pszBuffer[1] = 0x1;
    pszBuffer[2] = 0xa0;
    pszBuffer[3] = 0x0;

    int s32Size = TVT_SDK_AUDIO_BUFFER_SIZE;

    s32Ret = chip_audio_getCapBuffer((pszBuffer + 4), &s32Size);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("audio cap get stream failed");
        TIPC_MUTEX_UNLOCK(&g_pstSdkAudioInfo->stAudioBuffer.s32BufMutex,
                          g_pstSdkAudioInfo->stAudioBuffer.szBufMutexName,
                          &g_pstSdkAudioInfo->stAudioBuffer.u32LockCnt,
                          TVT_SDK_AUDIO_DEBUG);
        return -1;
    }
    // ��ֵ
    pstFrameInfo->relativeTime = 0; /// TODO:������
    pstFrameInfo->frameIndex = g_pstSdkAudioInfo->stAudioBuffer.u32FrameCnt++;
    pstFrameInfo->pData = (unsigned char*)pszBuffer;
    pstFrameInfo->length = s32Size + 4;
    pstFrameInfo->ucFrameType = IPC_FRAME_TYPE_AUDIO;
    switch (g_pstSdkAudioInfo->stAudioCfg[TVT_SDK_AUDIO_CAP].stAudioParam.enCodeType)
    {
    case SDK_AENC_TYPE_G711A:
        pstFrameInfo->ucEncodeType = IPC_AUDIO_ENCODE_TYPE_G711;
        break;
    case SDK_AENC_TYPE_G711U:
        pstFrameInfo->ucEncodeType = IPC_AUDIO_ENCODE_TYPE_G711U;
        break;
    case SDK_AENC_TYPE_G726:
        pstFrameInfo->ucEncodeType = IPC_AUDIO_ENCODE_TYPE_G726;
        break;
    case SDK_AENC_TYPE_ADPCM:
        pstFrameInfo->ucEncodeType = IPC_AUDIO_ENCODE_TYPE_ADPCM;
        break;
    case SDK_AENC_TYPE_AAC:
        pstFrameInfo->ucEncodeType = IPC_AUDIO_ENCODE_TYPE_AAC;
        break;
    default:
        break;
    }

    // �ȵ���Release�������н���

    return 0;
}

/**
 * @brief �ͷ���Ƶ������
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_audio_releaseStream(IPCAMERA_AV_FRAME_INFO* pstFrameInfo)
{
    TVT_SDK_AUDIO_INFO_S* pstHandle = tvt_sdk_audio_getHandle();
    if (NULL == pstHandle || NULL == pstFrameInfo)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }

    if (pstHandle->stAudioCfg[TVT_SDK_AUDIO_CAP].u8Init != 1)
    {
        TIPC_PRINTF_ERR("audio cap is not configured");
        return -1;
    }

    char* pszBuffer = pstHandle->stAudioBuffer.pszBuf;
    if (NULL == pszBuffer)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }

    int s32FrameSize = pstFrameInfo->length;
    memset(pszBuffer, 0, s32FrameSize);

    TIPC_MUTEX_UNLOCK(&g_pstSdkAudioInfo->stAudioBuffer.s32BufMutex,
                      g_pstSdkAudioInfo->stAudioBuffer.szBufMutexName,
                      &g_pstSdkAudioInfo->stAudioBuffer.u32LockCnt,
                      TVT_SDK_AUDIO_DEBUG);
    return 0;
}

/**
 * @brief ������Ƶ������
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_audio_sendPlayStream(IPCAMERA_AV_FRAME_INFO* pstFrameInfo)
{
    int s32Ret = 0;
    TVT_SDK_AUDIO_INFO_S* pstHandle = tvt_sdk_audio_getHandle();
    if (NULL == pstHandle || NULL == pstFrameInfo)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    if (NULL == pstFrameInfo->pData || 0 == pstFrameInfo->length)
    {
        TIPC_PRINTF_ERR("Err: frame size");
        return -1;
    }

    /// TODO:����ʽ�����仯��ʱ����Ҫ��������

    if (pstHandle->stAudioCfg[TVT_SDK_AUDIO_PLAY].u8Init != 1)
    {
        TIPC_PRINTF_ERR("audio cap is not configured");
        return -1;
    }

    char* pszBuffer = (char*)pstFrameInfo->pData + 4; // ȥͷ
    /// TODO: AAC����Ҫȥͷ
    int s32BuffSize = pstFrameInfo->length - 4;
    s32Ret = chip_audio_sendPlayBuffer(pszBuffer, s32BuffSize);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("audio send buffer failed");
        return -1;
    }

    return 0;
}

static TVT_SDK_AUDIO_CTRL_IMPL_S g_stFunList[] =
{
    {TVT_SDK_AUDIO_CFG_CAPTURE,      tvt_sdk_audio_cfgCapture},
    {TVT_SDK_AUDIO_CFG_PLAYBACK,     tvt_sdk_audio_cfgPlayBack},
    {TVT_SDK_AUDIO_SET_CAP_VOLUME,   tvt_sdk_audio_setCapVolume},
    {TVT_SDK_AUDIO_SET_PLAY_VOLUME,  tvt_sdk_audio_setPlayVolume},
};

/**
 * @brief ��Ƶҵ����ƽӿ�
 * @param [in] s32Cmd:������
 * @param [in] pParam:����ָ��
 * @param [in] s32Length:��������
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_audio_ctrl(int s32Cmd, void* pParam, int s32Length)
{
    int s32Ret = 0;
    int i = 0;

    if (NULL == pParam || 0 == s32Length)
    {
        TIPC_PRINTF_ERR("err input param");
        return -1;
    }

    TVT_SDK_AUDIO_INFO_S* pstHandle = tvt_sdk_audio_getHandle();
    if (NULL == pstHandle)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }

    if (pstHandle->u8Init != 1)
    {
        TIPC_PRINTF_ERR("current module is not running");
        return -1;
    }

    TIPC_MUTEX_LOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_AUDIO_DEBUG);

    int s32SuppFuncSize = sizeof(g_stFunList) / sizeof(TVT_SDK_AUDIO_CTRL_IMPL_S);
    for (i = 0; i < s32SuppFuncSize; i++)
    {
        if ((s32Cmd == (int)g_stFunList[i].enCmd) && (g_stFunList[i].pImplFunc != NULL))
        {
            s32Ret = g_stFunList[i].pImplFunc(pstHandle, pParam, s32Length);
            break;
        }
    }

    if (i >= s32SuppFuncSize)
    {
        TIPC_PRINTF_ERR("unknow cmd[%d]", s32Cmd);
    }

    TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_AUDIO_DEBUG);
    return s32Ret;
}

/**
 * @brief ��ʼ����Ƶҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_audio_init(TVT_SDK_PROD_DEV_INFO* pstParamDevInfo, TvtSdkProdCb pParamCb)
{
    int s32Ret = -1;
    if (NULL == pParamCb || NULL == pstParamDevInfo)
    {
        TIPC_PRINTF_ERR("device service input param err");
        return s32Ret;
    }

    if (g_pstSdkAudioInfo != NULL)
    {
        if (g_pstSdkAudioInfo->u8Init != 0)
        {
            // �ѳ�ʼ��
            TIPC_PRINTF_ERR("audio service is running");
            return 0;
        }
    }
    else
    {
        // �����ṹ��
        g_pstSdkAudioInfo = (TVT_SDK_AUDIO_INFO_S* )malloc(sizeof(TVT_SDK_AUDIO_INFO_S));
        if (NULL == g_pstSdkAudioInfo)
        {
            TIPC_PRINTF_ERR("audio service malloc failed");
            return s32Ret;
        }
        memset(g_pstSdkAudioInfo, 0, sizeof(TVT_SDK_AUDIO_INFO_S));
    }

    // ��ʼ����Ƶģ��
    s32Ret = chip_audio_init();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("audio init failed");
        return s32Ret;
    }

    // ��ʼ��Buffer
    g_pstSdkAudioInfo->stAudioBuffer.pszBuf = malloc(TVT_SDK_AUDIO_BUFFER_SIZE);
    if (NULL == g_pstSdkAudioInfo->stAudioBuffer.pszBuf)
    {
        TIPC_PRINTF_ERR("malloc failed");
        return -1;
    }
    // ��ʼ��Buffer��
    pthread_mutex_init(&g_pstSdkAudioInfo->stAudioBuffer.s32BufMutex, NULL);

    // �������
    g_pstSdkAudioInfo->pSdkParamCbFunc = pParamCb;
    memcpy(&g_pstSdkAudioInfo->stParamDevInfo, pstParamDevInfo, sizeof(TVT_SDK_PROD_DEV_INFO));

    // ��ʼ��ģ����
    pthread_mutex_init(&g_pstSdkAudioInfo->s32Mutex, NULL);
    sprintf(g_pstSdkAudioInfo->szMutexName, "tvt_sdk_audio_lock");
    g_pstSdkAudioInfo->u8Init = 1;

    return s32Ret;
}

/**
 * @brief ȥ��ʼ����Ƶҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_audio_exit()
{
    int s32Ret = 0;
    if (NULL == g_pstSdkAudioInfo)
    {
        // δ��ʼ��
        TIPC_PRINTF_ERR("video service is not running");
        return -1;
    }

    if (g_pstSdkAudioInfo->u8Init != 1)
    {
        // δ��ʼ��
        TIPC_PRINTF_ERR("audio service is not running");
        free((void*)g_pstSdkAudioInfo);
        g_pstSdkAudioInfo = NULL;
        return -1;
    }

    TIPC_MUTEX_LOCK(&g_pstSdkAudioInfo->s32Mutex, g_pstSdkAudioInfo->szMutexName, &g_pstSdkAudioInfo->u32MutexLockCnt, TVT_SDK_AUDIO_DEBUG);

    // ����
    if (g_pstSdkAudioInfo->stAudioBuffer.pszBuf != NULL)
    {
        TIPC_MUTEX_LOCK(&g_pstSdkAudioInfo->stAudioBuffer.s32BufMutex,
                        g_pstSdkAudioInfo->stAudioBuffer.szBufMutexName,
                        &g_pstSdkAudioInfo->stAudioBuffer.u32LockCnt,
                        TVT_SDK_AUDIO_DEBUG);
    }

    // �ر�ģ��
    s32Ret = chip_audio_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("chip_audio_exit failed[%d]", s32Ret);
    }

    // �ͷ�������Դ
    if (g_pstSdkAudioInfo->stAudioBuffer.pszBuf != NULL)
    {
        free(g_pstSdkAudioInfo->stAudioBuffer.pszBuf);
        g_pstSdkAudioInfo->stAudioBuffer.pszBuf = NULL;
        TIPC_MUTEX_UNLOCK(&g_pstSdkAudioInfo->stAudioBuffer.s32BufMutex,
                          g_pstSdkAudioInfo->stAudioBuffer.szBufMutexName,
                          &g_pstSdkAudioInfo->stAudioBuffer.u32LockCnt,
                          TVT_SDK_AUDIO_DEBUG);
        pthread_mutex_destroy(&g_pstSdkAudioInfo->stAudioBuffer.s32BufMutex);
    }

    TIPC_MUTEX_UNLOCK(&g_pstSdkAudioInfo->s32Mutex, g_pstSdkAudioInfo->szMutexName, &g_pstSdkAudioInfo->u32MutexLockCnt, TVT_SDK_AUDIO_DEBUG);
    pthread_mutex_destroy(&g_pstSdkAudioInfo->s32Mutex);

    free((void*)g_pstSdkAudioInfo);
    g_pstSdkAudioInfo = NULL;

    return 0;
}