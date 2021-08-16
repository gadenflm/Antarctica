
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "tvt_sdk_video.h"
#include "tvt_sdk_video_osd.h"
#include "tvt_sdk_video_mask.h"
#include "tvt_sdk_video_roi.h"
#include "tvt_sdk_video_facedt.h"

#include "tvt_sdk_print.h"
#include "tvt_sdk_chip.h"

#define ENCODE_STREAM_BUFF_SIZE (128 * 1024 * 15) ///< 15Mbps

#define TVT_SDK_VIDEO_DEBUG 0

#define MAX_SDK_WATERMARK_LEN (8)
#define MAX_WATERMARK_LEN  (160)
#define MAX_SLICE_NUM  (16)

typedef struct tvt_sdk_video_buffer_s
{
    pthread_mutex_t                      s32BufMutex;
    char                                 szBufMutexName[32];
    unsigned int                         u32LockCnt;
    char*                                pszBuf;
    int                                  s32BufSize;
    int                                  s32EncBufSize;
    unsigned long long                   u64TimeStampUS;
} TVT_SDK_VIDEO_BUFFER_S;

typedef struct tvt_sdk_vin_info_s
{
    int                                  s32VinDevID;                       ///< 视频输入设备ID
    SDK_VIDEO_FORMAT                     enFormat;                          ///< 当前的制式
    SDK_BLC_MODE_E                       enWorkMode;                        ///< 视频工作模式
    int                                  s32SrcWidth;                       ///< 视频输入分辨率宽
    int                                  s32SrcHeight;                      ///< 视频输入分辨率高
} TVT_SDK_VIN_INFO_S;

typedef struct tvt_sdk_video_watermark_s
{
    unsigned char       u8Enable;
    pthread_mutex_t     s32BufMutex;
    char                szBufMutexName[32];
    unsigned int        u32LockCnt;
    char                szData[MAX_WATERMARK_LEN]; ///< SDK data + user data
    int                 s32UserDataLen;
    int                 s32SDKDataLen;
} TVT_SDK_VIDEO_WATERMARK_S;


/**
 * @brief 业务结构体
 */
typedef struct tvt_sdk_video_info_s
{
    char                                 u8Init;                            ///< 初始化标志位
    char                                 u8Exit;                            ///< 去初始化标志位，线程退出标志
    pthread_mutex_t                      s32Mutex;                          ///< 业务锁
    char                                 szMutexName[32];                   ///< 业务锁名称
    unsigned int                         u32MutexLockCnt;                   ///< 业务锁锁定次数
    pthread_t                            s32ThreadID;                       ///< 业务线程ID
    int                                  s32VinCnt;                         ///< sensor个数
    TVT_SDK_VIN_INFO_S                   stVinInfo[CHIP_MAX_VI_NUM];        ///< 视频输入信息
    TVT_SDK_VIDEO_BUFFER_S               stStreamBuff[CHIP_MAX_VI_NUM];     ///< 各路编码流缓存数据
    TVT_SDK_VIDEO_BUFFER_S               stJpegBuff[CHIP_MAX_VI_NUM][CHIP_VENC_JPEG_NUM];   ///< 各路JPEG流缓存数据
    TVT_SDK_VIDEO_ENC_CFG_S              stEncChnInfo[CHIP_MAX_VENC_NUM];   ///< 编码通道信息
    TVT_SDK_VIDEO_WATERMARK_S            stWaterMark;                       ///< 编码水印信息
    TvtSdkProdCb                         pSdkParamCbFunc;
    int                                  s32ReserveArr[4];
} TVT_SDK_VIDEO_INFO_S;

static TVT_SDK_VIDEO_INFO_S* g_pstSdkVideoInfo = NULL;

typedef int (*VideoImplFunc)(TVT_SDK_VIDEO_INFO_S* pstHandle, void* pParam, int s32ParamSize);
typedef struct tvt_sdk_video_ctrl_impl_s
{
    TVT_SDK_VIDEO_CMD_E         enCmd;          ///< 命令字
    VideoImplFunc               pImplFunc;      ///< 实现接口
} TVT_SDK_VIDEO_CTRL_IMPL_S;

/**
 * @brief 获取模块结构体指针
 */
static TVT_SDK_VIDEO_INFO_S* tvt_sdk_video_getHandle(void)
{
    return g_pstSdkVideoInfo;
}

/**
 * @brief 获取设备号以及编码索引
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_video_getVinIDAndEncIDX(const TVT_SDK_VIDEO_INFO_S* pstHandle, const int s32StreamID, int* ps32VinDevID, int* ps32EncIDX)
{
    int s32VinIDX = 0;
    int s32VinDevID = 0;
    int s32EncIDX = 0;
    if (NULL == pstHandle)
    {
        // Sanity Check
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    int s32VinCnt = pstHandle->s32VinCnt;
    if (s32VinCnt <= 0)
    {
        TIPC_PRINTF_ERR("err vin count[%d]", s32VinCnt);
        return -1;
    }
    int s32MaxH26xChnNum = s32VinCnt * CHIP_VENC_H26X_NUM;
    if (s32StreamID < s32MaxH26xChnNum)
    {
        // h26x
        s32VinIDX = s32StreamID / CHIP_VENC_H26X_NUM;
        s32EncIDX = s32StreamID % CHIP_VENC_H26X_NUM;
    }
    else
    {
        // mjpeg
        int s32MinJpegChnNum = s32MaxH26xChnNum;
        s32VinIDX = (s32StreamID - s32MinJpegChnNum) / CHIP_VENC_JPEG_NUM;
        s32EncIDX = (s32StreamID - s32MinJpegChnNum) % CHIP_VENC_JPEG_NUM;
        s32EncIDX += s32MinJpegChnNum;
    }

    s32VinDevID = pstHandle->stVinInfo[s32VinIDX].s32VinDevID;
    *ps32VinDevID = s32VinDevID;
    *ps32EncIDX = s32EncIDX;
    return 0;
}


/**
 * @brief 获取编码通道信息
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_video_getEncInfo(TVT_SDK_VIDEO_INFO_S* pstHandle, void* pParam, int s32ParamSize)
{
    TVT_SDK_VIDEO_ENC_CFG_S* pstEncCfg = (TVT_SDK_VIDEO_ENC_CFG_S*)pParam;
    if (pstEncCfg->s32StreamID >= CHIP_MAX_VENC_NUM)
    {
        TIPC_PRINTF_ERR("err stream id[%d]", pstEncCfg->s32StreamID);
        return -1;
    }
    TVT_SDK_VIDEO_ENC_CFG_S* pstCurrEncCfg = &pstHandle->stEncChnInfo[pstEncCfg->s32StreamID];
    pstEncCfg->enBitRateMode = pstCurrEncCfg->enBitRateMode;
    pstEncCfg->enEncType = pstCurrEncCfg->enEncType;
    pstEncCfg->u32BitRate = pstCurrEncCfg->u32BitRate;
    pstEncCfg->u32FrameRate = pstCurrEncCfg->u32FrameRate;
    pstEncCfg->u32GOP = pstCurrEncCfg->u32GOP;
    pstEncCfg->u32Height = pstCurrEncCfg->u32Height;
    pstEncCfg->u32Profile = pstCurrEncCfg->u32Profile;
    pstEncCfg->u32Width = pstCurrEncCfg->u32Width;
    pstEncCfg->u8RequestIDR = pstCurrEncCfg->u8RequestIDR;
    return 0;
}

/**
 * @brief 创建编码通道
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_video_createEncStream(TVT_SDK_VIDEO_INFO_S* pstHandle, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;

    TVT_SDK_VIDEO_ENC_CFG_S* pstEncCfg = (TVT_SDK_VIDEO_ENC_CFG_S*)pParam;

    if (pstEncCfg->s32StreamID >= CHIP_MAX_VENC_NUM)
    {
        TIPC_PRINTF_ERR("err stream id[%d]", pstEncCfg->s32StreamID);
        return -1;
    }

    int s32VinDevID = -1;
    int s32EncIDX = -1;
    s32Ret = tvt_sdk_video_getVinIDAndEncIDX(pstHandle, pstEncCfg->s32StreamID, &s32VinDevID, &s32EncIDX);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get VinDevID or EncIDX failed");
        return s32Ret;
    }
    int s32Reset = 0;
    // 检查参数是否更新
    TVT_SDK_VIDEO_ENC_CFG_S* pstCurrEncCfg = &pstHandle->stEncChnInfo[pstEncCfg->s32StreamID];
    if ((pstEncCfg->enBitRateMode != pstCurrEncCfg->enBitRateMode) ||
            (pstEncCfg->enEncType != pstCurrEncCfg->enEncType) ||
            (pstEncCfg->u32BitRate != pstCurrEncCfg->u32BitRate) ||
            (pstEncCfg->u32FrameRate != pstCurrEncCfg->u32FrameRate) ||
            (pstEncCfg->u32GOP != pstCurrEncCfg->u32GOP) ||
            (pstEncCfg->u32Height != pstCurrEncCfg->u32Height) ||
            (pstEncCfg->u32Width != pstCurrEncCfg->u32Width) ||
            (pstEncCfg->u32Profile != pstCurrEncCfg->u32Profile))
    {
        s32Reset = 1;
    }

    if (0 == s32Reset)
    {
        return s32Ret;
    }

    // 需要先关闭OSD/ROI
    tvt_sdk_osd_stopDispALLOSD();
    tvt_sdk_roi_stopDispALL();

    CHIP_VIDEO_ENC_CFG_S stChipEncChnCfg = {0};
    stChipEncChnCfg.s32VinDevID = s32VinDevID;
    stChipEncChnCfg.s32StreamIDX = s32EncIDX;
    stChipEncChnCfg.enBitRateMode = pstEncCfg->enBitRateMode;
    stChipEncChnCfg.enEncType = pstEncCfg->enEncType;
    stChipEncChnCfg.u32BitRate = pstEncCfg->u32BitRate;
    stChipEncChnCfg.u32FrameRate = pstEncCfg->u32FrameRate;
    stChipEncChnCfg.u32GOP = pstEncCfg->u32GOP;
    stChipEncChnCfg.u32Height = pstEncCfg->u32Height;
    stChipEncChnCfg.u32Profile = pstEncCfg->u32Profile;
    stChipEncChnCfg.u32Width = pstEncCfg->u32Width;

    s32Ret = chip_video_createChn(&stChipEncChnCfg);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("cheate encode channel failed[%d]", s32Ret);
        // 重新开启OSD
        tvt_sdk_osd_startDispALLOSD();
        return s32Ret;
    }

    // 保存新参数
    memcpy(pstCurrEncCfg, pstEncCfg, sizeof(TVT_SDK_VIDEO_ENC_CFG_S));

    // 设置请求I帧标识
    // pstCurrEncCfg->u8RequestIDR = 1;
    // wkl update: for temp debug
    pstCurrEncCfg->u8RequestIDR = 0;

    // 重新开启OSD/ROI
    tvt_sdk_osd_startDispALLOSD();
    tvt_sdk_roi_startDispALL();
    return s32Ret;
}


/**
 * @brief 销毁编码通道
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_video_destroyEncStream(TVT_SDK_VIDEO_INFO_S* pstHandle, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;

    int s32StreamID = *(int* )pParam;

    if (s32StreamID >= CHIP_MAX_VENC_NUM)
    {
        TIPC_PRINTF_ERR("err stream id[%d]", s32StreamID);
        return s32Ret;
    }

    int s32VinDevID = -1;
    int s32EncIDX = -1;
    s32Ret = tvt_sdk_video_getVinIDAndEncIDX(pstHandle, s32StreamID, &s32VinDevID, &s32EncIDX);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get VinDevID or EncIDX failed");
        return s32Ret;
    }

    // 需要先关闭OSD
    tvt_sdk_osd_stopDispALLOSD();

    s32Ret = chip_video_destroyChn(s32VinDevID, s32EncIDX);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("destroy encode channel failed[%d]", s32Ret);
        // 重新开启OSD
        tvt_sdk_osd_startDispALLOSD();
        return s32Ret;
    }

    // 清空保存参数
    TVT_SDK_VIDEO_ENC_CFG_S* pstCurrEncCfg = &pstHandle->stEncChnInfo[s32StreamID];
    memset(pstCurrEncCfg, 0, sizeof(TVT_SDK_VIDEO_ENC_CFG_S));

    // 重新开启OSD
    tvt_sdk_osd_startDispALLOSD();
    return s32Ret;
}


/**
 * @brief 编码通道请求I帧
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_video_reqIDRStream(TVT_SDK_VIDEO_INFO_S* pstHandle, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int s32StreamID = *(int* )pParam;

    if (s32StreamID >= CHIP_MAX_VENC_NUM)
    {
        TIPC_PRINTF_ERR("err stream id[%d]", s32StreamID);
        return s32Ret;
    }

    int s32VinDevID = -1;
    int s32EncIDX = -1;
    s32Ret = tvt_sdk_video_getVinIDAndEncIDX(pstHandle, s32StreamID, &s32VinDevID, &s32EncIDX);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get VinDevID or EncIDX failed");
        return s32Ret;
    }

    s32Ret = chip_video_requestIDRStream(s32VinDevID, s32EncIDX);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("chip_video_requestIDRStream failed[%d]", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}


/**
 * @brief 启动编码通道
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_video_startStream(TVT_SDK_VIDEO_INFO_S* pstHandle, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int s32StreamID = *(int* )pParam;

    if (s32StreamID >= CHIP_MAX_VENC_NUM)
    {
        TIPC_PRINTF_ERR("err stream id[%d]", s32StreamID);
        return s32Ret;
    }

    int s32VinDevID = -1;
    int s32EncIDX = -1;
    s32Ret = tvt_sdk_video_getVinIDAndEncIDX(pstHandle, s32StreamID, &s32VinDevID, &s32EncIDX);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get VinDevID or EncIDX failed");
        return s32Ret;
    }

    // 检查有无通道信息
    TVT_SDK_VIDEO_ENC_CFG_S* pstEncCfg = &pstHandle->stEncChnInfo[s32StreamID];
    if (0 == pstEncCfg->u32Width || 0 == pstEncCfg->u32Height)
    {
        // 没有此通道信息，返回错误
        TIPC_PRINTF_ERR("stream id[%d] is not created", s32StreamID);
        return -1;
    }

    s32Ret = chip_video_startStream(s32VinDevID, s32EncIDX);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("chip_video_requestIDRStream failed[%d]", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

/**
 * @brief 设置视频水印
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_video_setWaterMark(TVT_SDK_VIDEO_INFO_S* pstHandle, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    TVT_SDK_VIDEO_WATERMARK_CFG_S* pstWaterMarkCfg = (TVT_SDK_VIDEO_WATERMARK_CFG_S*)pParam;
    // 加锁
    pthread_mutex_t* ps32Mutex = &pstHandle->stWaterMark.s32BufMutex;
    char* pszMutexName = pstHandle->stWaterMark.szBufMutexName;
    unsigned int* pu32MutexLockCnt = &pstHandle->stWaterMark.u32LockCnt;
    TIPC_MUTEX_LOCK(ps32Mutex, pszMutexName, pu32MutexLockCnt, TVT_SDK_VIDEO_DEBUG);

    int s32DataLens = pstWaterMarkCfg->s32DataLens;
    if (1 == pstWaterMarkCfg->u8Enable)
    {
        if (pstWaterMarkCfg->s32DataLens > (MAX_WATERMARK_LEN - MAX_SDK_WATERMARK_LEN))
        {
            s32DataLens = (MAX_WATERMARK_LEN - MAX_SDK_WATERMARK_LEN);
        }
        if (NULL == pstWaterMarkCfg->pszUserData)
        {
            TIPC_PRINTF_ERR("watermark data is NULL");
            TIPC_MUTEX_LOCK(ps32Mutex, pszMutexName, pu32MutexLockCnt, TVT_SDK_VIDEO_DEBUG);
            return -1;
        }
        memcpy(pstHandle->stWaterMark.szData, pstWaterMarkCfg->pszUserData, s32DataLens);
    }

    pstHandle->stWaterMark.u8Enable = pstWaterMarkCfg->u8Enable;

    // 解锁
    TIPC_MUTEX_LOCK(ps32Mutex, pszMutexName, pu32MutexLockCnt, TVT_SDK_VIDEO_DEBUG);
    return s32Ret;
}

/**
 * @brief 设置编码Slice分片
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_video_setEncodeSlice(TVT_SDK_VIDEO_INFO_S* pstHandle, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    // int s32Enable = *(int* )pParam;
    /// TODO:
    return s32Ret;
}


/**
 * @brief 设置时间OSD格式
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_video_setTimeOSDCfg(TVT_SDK_VIDEO_INFO_S* pstHandle, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    TVT_SDK_OSD_TIME_CFG_S* pstTimeOSDCfg = (TVT_SDK_OSD_TIME_CFG_S*)pParam;
    s32Ret = tvt_sdk_osd_setTimeOSDCfg(pstTimeOSDCfg->s32VinIDX, &pstTimeOSDCfg->stTimeFmt, &pstTimeOSDCfg->stPosition);
    return s32Ret;
}

/**
 * @brief 更新时间OSD数据
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_video_updateTimeOSD(TVT_SDK_VIDEO_INFO_S* pstHandle, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int s32VinIDX = *(int* )pParam;
    s32Ret = tvt_sdk_osd_updateTimeOSD(s32VinIDX);
    return s32Ret;
}

/**
 * @brief 设置Comm OSD数据
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_video_setCommOSD(TVT_SDK_VIDEO_INFO_S* pstHandle, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    TVT_SDK_OSD_COMM_CFG_S* pstCommOSDCfg = (TVT_SDK_OSD_COMM_CFG_S*)pParam;
    // TIPC_PRINTF_CHK("[****** WKL_DEBUG ******]     tvt_sdk_video_setCommOSD_enter");
    s32Ret = tvt_sdk_osd_setCommOSD(pstCommOSDCfg->s32VinIDX, pstCommOSDCfg->u32OSDIDX, &pstCommOSDCfg->stOSDInfo, &pstCommOSDCfg->stPosition);
    return s32Ret;
}

/**
 * @brief 更新时间OSD数据
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_video_updateCommOSD(TVT_SDK_VIDEO_INFO_S* pstHandle, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int s32VinIDX = *(int* )pParam;
    int s32OSDIDX = 0;
    // TIPC_PRINTF_CHK("[****** WKL_DEBUG ******]      TVT_SDK_OSD_DEV = %d", TVT_SDK_OSD_DEV);
    // TIPC_PRINTF_CHK("[****** WKL_DEBUG ******]      TVT_SDK_OSD_INDEX_BUTT = %d", TVT_SDK_OSD_INDEX_BUTT);
    for (s32OSDIDX = TVT_SDK_OSD_DEV; s32OSDIDX < TVT_SDK_OSD_INDEX_BUTT; s32OSDIDX++)
    {
        s32Ret = tvt_sdk_osd_updateCommOSD(s32VinIDX, s32OSDIDX);
    }
    return s32Ret;
}

/**
 * @brief 设置Mask
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_video_setMask(TVT_SDK_VIDEO_INFO_S* pstHandle, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    TVT_SDK_VIDEO_MASK_CFG_S* pstMaskCfg = (TVT_SDK_VIDEO_MASK_CFG_S*)pParam;
    s32Ret = tvt_sdk_mask_setPos(pstMaskCfg->s32VinIDX, pstMaskCfg->u32Enable, &pstMaskCfg->stPosition);
    return s32Ret;
}

/**
 * @brief 设置ROI
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_video_setROI(TVT_SDK_VIDEO_INFO_S* pstHandle, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    TVT_SDK_VIDEO_ROI_CFG_S* pstROICfg = (TVT_SDK_VIDEO_ROI_CFG_S*)pParam;
    s32Ret = tvt_sdk_roi_setPos(pstROICfg->s32VinIDX, pstROICfg->s32ROINum, pstROICfg->pstROIInfo);
    return s32Ret;
}

/**
 * @brief 获取编码流
 * @return 0：成功；非0：失败
 */
int tvt_sdk_video_getEncodeStream(int s32StreamID, IPCAMERA_AV_FRAME_INFO* pstFrameInfo, int* ps32SLice, int* ps32SliceBuf, int s32SliceBufLen)
{
    int s32Ret = 0;
    int i = 0;
    TVT_SDK_VIDEO_INFO_S* pstHandle = tvt_sdk_video_getHandle();
    if (NULL == pstHandle || NULL == pstFrameInfo)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }

    if (pstHandle->u8Init != 1)
    {
        TIPC_PRINTF_ERR("current module is not running");
        return -1;
    }

    int s32VinDevID = -1;
    int s32EncIDX = -1;
    s32Ret = tvt_sdk_video_getVinIDAndEncIDX(pstHandle, s32StreamID, &s32VinDevID, &s32EncIDX);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get VinDevID or EncIDX failed");
        return s32Ret;
    }

    int s32VinIDX = -1;
    for (i = 0; i < CHIP_MAX_VI_NUM; i++)
    {
        if (s32VinDevID == pstHandle->stVinInfo[i].s32VinDevID)
        {
            s32VinIDX = i;
            break;
        }
    }
    if (s32VinIDX < 0)
    {
        TIPC_PRINTF_ERR("check vin index failed");
        return -1;
    }

    // 为保证码流获取的实时性暂时不加锁，模块释放时通过缓存锁来保证安全性
    // Update:还是得加个锁，性能问题后面再观察
    // TIPC_MUTEX_LOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt);
    int s32MallocBufferSize = 0;
    CHIP_VIDEO_ENC_STREAM_S stEncStream = {0};
    stEncStream.s32VinDevID = s32VinDevID;
    stEncStream.pstFrameInfo = pstFrameInfo;
    stEncStream.s32StreamIDX = s32EncIDX;

    TVT_SDK_VIDEO_ENC_CFG_S* pstCurrEncCfg = &pstHandle->stEncChnInfo[s32StreamID];
    TVT_SDK_VIDEO_BUFFER_S* pstVideoBuffer = NULL;
    if (s32EncIDX < TVT_SDK_VIDEO_ENC_JPEG_STREAM)
    {
        // h26x
        pstVideoBuffer = &pstHandle->stStreamBuff[s32VinIDX];
        if (1 == pstCurrEncCfg->u8RequestIDR)
        {
            // 请求I帧
            stEncStream.u8RequestIDR = 1;
        }
    }
    else
    {
        // mjpeg
        int s32JpegIDX = s32EncIDX - TVT_SDK_VIDEO_ENC_JPEG_STREAM;
        pstVideoBuffer = &pstHandle->stJpegBuff[s32VinIDX][s32JpegIDX];
    }

    if (NULL == pstVideoBuffer)
    {
        TIPC_PRINTF_ERR("NULL Pointer of Buffer");
        return -1;
    }

    stEncStream.pstFrameInfo->pData = (unsigned char*)pstVideoBuffer->pszBuf;
    stEncStream.pstFrameInfo->length = pstVideoBuffer->s32BufSize;
    s32MallocBufferSize = stEncStream.pstFrameInfo->length;
    stEncStream.ps32Slice = ps32SLice;
    stEncStream.ps32SliceBuf = ps32SliceBuf;
    stEncStream.s32SliceBufLen = s32SliceBufLen;

    // 缓存加锁，用户需要通过tvt_sdk_video_releaseEncodeStream解锁
    pthread_mutex_t* ps32Mutex = &pstVideoBuffer->s32BufMutex;
    char* pszMutexName = pstVideoBuffer->szBufMutexName;
    unsigned int* pu32MutexLockCnt = &pstVideoBuffer->u32LockCnt;

    TIPC_MUTEX_LOCK(ps32Mutex, pszMutexName, pu32MutexLockCnt, TVT_SDK_VIDEO_DEBUG);

    s32Ret = chip_video_getChnStream(&stEncStream);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get chn stream failed[%d]", s32Ret);
        TIPC_MUTEX_UNLOCK(ps32Mutex, pszMutexName, pu32MutexLockCnt, TVT_SDK_VIDEO_DEBUG);
        // TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt);
        return -1;
    }
    pstFrameInfo->ucStreamIndex = (unsigned int)s32StreamID;

    pstVideoBuffer->s32EncBufSize = stEncStream.pstFrameInfo->length; // 记住这一帧真实长度
    pstVideoBuffer->u64TimeStampUS = stEncStream.pstFrameInfo->relativeTime;

    if ((1 == pstCurrEncCfg->u8RequestIDR) && (1 == pstFrameInfo->ucKeyFrame))
    {
        // 检查当前获取到的帧是否为需求的I帧
        pstCurrEncCfg->u8RequestIDR = 0;
    }

    // 加水印
    if ((1 == pstHandle->stWaterMark.u8Enable) &&
            (1 == pstFrameInfo->ucKeyFrame) &&
            ((IPC_VIDEO_ENCODE_TYPE_H264 == pstFrameInfo->ucEncodeType) || (IPC_VIDEO_ENCODE_TYPE_H265 == pstFrameInfo->ucEncodeType)))
    {
        // 按照一定规则处理
        int i = 0;
        int s32UserDataLens = pstHandle->stWaterMark.s32UserDataLen;
        int s32SDKDataLens = pstHandle->stWaterMark.s32SDKDataLen;

        // 判断长度
        int s32StreamTotalLens = s32UserDataLens + s32SDKDataLens + pstFrameInfo->length;
        if (s32StreamTotalLens >= s32MallocBufferSize)
        {
            TIPC_PRINTF_ERR("encode size with watermark is larger than malloc size");
            // 放弃这帧数据，请求下个I帧
            pstCurrEncCfg->u8RequestIDR = 1;
            TIPC_MUTEX_UNLOCK(ps32Mutex, pszMutexName, pu32MutexLockCnt, TVT_SDK_VIDEO_DEBUG);
            return -1;
        }

        int s32Spec = pstFrameInfo->length / (s32UserDataLens + 1);
        char* pszMarkPos = (char*)pstFrameInfo->pData + pstFrameInfo->length;
        char* pszUserData = pstHandle->stWaterMark.szData;
        memcpy(pszMarkPos, pszUserData, s32SDKDataLens);
        pszMarkPos += s32SDKDataLens;
        pszUserData += s32SDKDataLens;
        static char* pszSpecial = "#timestamp#";
        char szUserTmpBuff[64] = {0};
        if (0 == strncmp(pszUserData, pszSpecial, strlen(pszSpecial)))
        {
            // 添加时间水印
            time_t stNow;
            struct tm stNowTmtime ;
            struct tm* pstNowTime = &stNowTmtime;

            time(&stNow);
            localtime_r(&stNow, &stNowTmtime);

            pstNowTime->tm_year += 1900;
            pstNowTime->tm_mon += 1;

            snprintf(szUserTmpBuff, 64, "%.4d%.2d%.2d %.2d%.2d%.2d", pstNowTime->tm_year,
                     pstNowTime->tm_mon, pstNowTime->tm_mday, pstNowTime->tm_hour, pstNowTime->tm_min, pstNowTime->tm_sec);

            pszUserData = szUserTmpBuff;
        }

        for (i = 0; i < s32UserDataLens; i++ )
        {
            pszMarkPos[i] = pszUserData[i] - pstFrameInfo->pData[i * s32Spec];
        }
        pstFrameInfo->length += s32UserDataLens;

        if (ps32SliceBuf != NULL)
        {
            /// TODO: 这里需要优化
            for (i = 0; i < MAX_SLICE_NUM; i++)
            {
                if (ps32SliceBuf[i] == 0 && i > 0)
                {
                    ps32SliceBuf[i - 1] += s32UserDataLens;
                }
            }
        }
    }

    // TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt);
    return s32Ret;
}

/**
 * @brief 释放编码流
 * @return 0：成功；非0：失败
 */
int tvt_sdk_video_releaseEncodeStream(IPCAMERA_AV_FRAME_INFO* pstFrameInfo)
{
    int s32Ret = 0;
    int i = 0;

    int debug_flag = 0;

    TVT_SDK_VIDEO_INFO_S* pstHandle = tvt_sdk_video_getHandle();
    if (NULL == pstHandle || NULL == pstFrameInfo)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }

    if (pstHandle->u8Init != 1)
    {
        TIPC_PRINTF_ERR("current module is not running");
        return -1;
    }

    int s32StreamID = pstFrameInfo->ucStreamIndex;
    int s32VinDevID = -1;
    int s32EncIDX = -1;
    s32Ret = tvt_sdk_video_getVinIDAndEncIDX(pstHandle, s32StreamID, &s32VinDevID, &s32EncIDX);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get VinDevID or EncIDX failed");
        return s32Ret;
    }

    int s32VinIDX = -1;
    for (i = 0; i < CHIP_MAX_VI_NUM; i++)
    {
        if (s32VinDevID == pstHandle->stVinInfo[i].s32VinDevID)
        {
            s32VinIDX = i;
            break;
        }
    }
    if (s32VinIDX < 0)
    {
        TIPC_PRINTF_ERR("check vin index failed");
        return -1;
    }

    TVT_SDK_VIDEO_BUFFER_S* pstVideoBuffer = NULL;
    switch (pstFrameInfo->ucFrameType)
    {
    case IPC_FRAME_TYPE_VIDEO:
    {
        pstVideoBuffer = &pstHandle->stStreamBuff[s32VinIDX];
    }
    break;
    case IPC_FRAME_TYPE_PIC:
    {
        // mjpeg
        int s32JpegIDX = s32EncIDX - TVT_SDK_VIDEO_ENC_JPEG_STREAM;
        pstVideoBuffer = &pstHandle->stJpegBuff[s32VinIDX][s32JpegIDX];
        TIPC_PRINTF_CHK(" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  s32StreamID [%d]", s32StreamID);
        debug_flag = 1;
    }
    break;
    default:
    {
        TIPC_PRINTF_ERR("unknown frame type[%d]", pstFrameInfo->ucFrameType);
        return -1;
    }
    break;
    }

    if (NULL == pstVideoBuffer)
    {
        TIPC_PRINTF_ERR("NULL Pointer of Video Buffer");
        return -1;
    }

    pthread_mutex_t* ps32Mutex = &pstVideoBuffer->s32BufMutex;
    char* pszMutexName = pstVideoBuffer->szBufMutexName;
    unsigned int* pu32MutexLockCnt = &pstVideoBuffer->u32LockCnt;

    // 缓存解锁
    TIPC_MUTEX_UNLOCK(ps32Mutex, pszMutexName, pu32MutexLockCnt, TVT_SDK_VIDEO_DEBUG);
    if (debug_flag) {
        TIPC_PRINTF_CHK(" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  Unlock the [%s]", pszMutexName);
    }
    return 0;
}


/**
 * @brief 获取Jpeg缓存数据
 * @return 0：成功；非0：失败
 */
int tvt_sdk_video_getJpegBuffer(int s32StreamID, IPCAMERA_AV_FRAME_INFO* pstFrameInfo)
{
    int s32Ret = 0;
    int i = 0;
    TVT_SDK_VIDEO_INFO_S* pstHandle = tvt_sdk_video_getHandle();
    if (NULL == pstHandle || NULL == pstFrameInfo)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }

    if (pstHandle->u8Init != 1)
    {
        TIPC_PRINTF_ERR("current module is not running");
        return -1;
    }

    int s32VinDevID = -1;
    int s32EncIDX = -1;
    s32Ret = tvt_sdk_video_getVinIDAndEncIDX(pstHandle, s32StreamID, &s32VinDevID, &s32EncIDX);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get VinDevID or EncIDX failed");
        return s32Ret;
    }

    int s32VinIDX = -1;
    for (i = 0; i < CHIP_MAX_VI_NUM; i++)
    {
        if (s32VinDevID == pstHandle->stVinInfo[i].s32VinDevID)
        {
            s32VinIDX = i;
            break;
        }
    }
    if (s32VinIDX < 0)
    {
        TIPC_PRINTF_ERR("check vin index failed");
        return -1;
    }

    if (s32EncIDX < TVT_SDK_VIDEO_ENC_JPEG_STREAM)
    {
        TIPC_PRINTF_ERR("check stream index [%d] [%d] [%d] failed", s32EncIDX, s32VinIDX, s32StreamID);
    }

    // 找对应的Buffer
    int s32JpegIDX = s32EncIDX - TVT_SDK_VIDEO_ENC_JPEG_STREAM;
    TVT_SDK_VIDEO_BUFFER_S* pstVideoBuffer = &pstHandle->stJpegBuff[s32VinIDX][s32JpegIDX];
    pthread_mutex_t* ps32Mutex = &pstVideoBuffer->s32BufMutex;
    char* pszMutexName = pstVideoBuffer->szBufMutexName;
    unsigned int* pu32MutexLockCnt = &pstVideoBuffer->u32LockCnt;

    // 加锁，等release接口解锁
    TIPC_MUTEX_LOCK(ps32Mutex, pszMutexName, pu32MutexLockCnt, TVT_SDK_VIDEO_DEBUG);
    pstFrameInfo->ucStreamIndex = s32StreamID;
    pstFrameInfo->relativeTime = pstVideoBuffer->u64TimeStampUS;
    pstFrameInfo->pData = (unsigned char*)pstVideoBuffer->pszBuf;
    pstFrameInfo->length = pstVideoBuffer->s32EncBufSize;
    pstFrameInfo->ucFrameType = IPC_FRAME_TYPE_PIC;

    return s32Ret;
}


static TVT_SDK_VIDEO_CTRL_IMPL_S g_stFunList[] =
{
    {TVT_SDK_VIDEO_CREATE_ENCODE_STREAM,  tvt_sdk_video_createEncStream},
    {TVT_SDK_VIDEO_DESTROY_ENCODE_STREAM, tvt_sdk_video_destroyEncStream},
    {TVT_SDK_VIDEO_GET_ENCODE_STREAM_CFG, tvt_sdk_video_getEncInfo},
    {TVT_SDK_VIDEO_REQUEST_I_FRAME,       tvt_sdk_video_reqIDRStream},
    {TVT_SDK_VIDEO_START_STREAM,          tvt_sdk_video_startStream},
    {TVT_SDK_VIDEO_SET_TIME_OSD_CFG,      tvt_sdk_video_setTimeOSDCfg},
    {TVT_SDK_VIDEO_UPDATE_TIME_OSD,       tvt_sdk_video_updateTimeOSD},
    {TVT_SDK_VIDEO_SET_COMM_OSD,          tvt_sdk_video_setCommOSD},
    {TVT_SDK_VIDEO_UPDATE_COMM_OSD,       tvt_sdk_video_updateCommOSD},
    {TVT_SDK_VIDEO_SET_WATERMARK,         tvt_sdk_video_setWaterMark},
    {TVT_SDK_VIDEO_SET_SLICE,             tvt_sdk_video_setEncodeSlice},
    {TVT_SDK_VIDEO_SET_MASK,              tvt_sdk_video_setMask},
    {TVT_SDK_VIDEO_SET_ROI,               tvt_sdk_video_setROI},

};

/**
 * @brief 视频业务控制接口
 * @param [in] s32Cmd:命令字
 * @param [in] pParam:参数指针
 * @param [in] s32Length:参数长度
 * @return 0：成功；非0：失败
 */
int tvt_sdk_video_ctrl(int s32Cmd, void* pParam, int s32Length)
{
    int s32Ret = 0;
    int i = 0;

    if (NULL == pParam || 0 == s32Length)
    {
        TIPC_PRINTF_ERR("err input param");
        return -1;
    }

    TVT_SDK_VIDEO_INFO_S* pstHandle = tvt_sdk_video_getHandle();
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

    TIPC_MUTEX_LOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_VIDEO_DEBUG);

    int s32SuppFuncSize = sizeof(g_stFunList) / sizeof(TVT_SDK_VIDEO_CTRL_IMPL_S);
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

    TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_VIDEO_DEBUG);
    return s32Ret;
}

/**
 * @brief 初始化视频业务
 * @param [in] pstVideoCfg:视频输入参数指针
 * @param [in] s32VinNum:视频输入个数
 * @return 0：成功；非0：失败
 */
int tvt_sdk_video_init(TVT_SDK_VIDEO_CFG_S* pstVideoCfg, int s32VinNum)
{
    int s32Ret = -1;
    int i = 0, j = 0;
    int s32VinDevID[CHIP_MAX_VI_NUM] = { -1};
    if (NULL == pstVideoCfg || 0 == s32VinNum)
    {
        TIPC_PRINTF_ERR("video service input param err[%d]", s32VinNum);
        return s32Ret;
    }

    if (s32VinNum > CHIP_MAX_VI_NUM)
    {
        TIPC_PRINTF_ERR("vi num[%d] is larger than max support num[%d]", s32VinNum, CHIP_MAX_VI_NUM);
        return s32Ret;
    }

    if (g_pstSdkVideoInfo != NULL)
    {
        if (g_pstSdkVideoInfo->u8Init != 0)
        {
            // 已初始化
            TIPC_PRINTF_ERR("video service is running");
            return 0;
        }
    }
    else
    {
        // 创建结构体
        g_pstSdkVideoInfo = (TVT_SDK_VIDEO_INFO_S* )malloc(sizeof(TVT_SDK_VIDEO_INFO_S));
        if (NULL == g_pstSdkVideoInfo)
        {
            TIPC_PRINTF_ERR("video service malloc failed");
            return s32Ret;
        }
        memset(g_pstSdkVideoInfo, 0, sizeof(TVT_SDK_VIDEO_INFO_S));
        memset(g_pstSdkVideoInfo->stVinInfo, -1, (sizeof(TVT_SDK_VIN_INFO_S) * CHIP_MAX_VI_NUM));
        // 初始化水印数据
        g_pstSdkVideoInfo->stWaterMark.s32SDKDataLen = MAX_SDK_WATERMARK_LEN;
        char szBuf[8] = {0x0, 0x0, 0x0, 0x1, 0x66, 0xe5, 0x01, 0x00};
        memcpy(g_pstSdkVideoInfo->stWaterMark.szData, szBuf, MAX_SDK_WATERMARK_LEN);
        pthread_mutex_init(&(g_pstSdkVideoInfo->stWaterMark.s32BufMutex), NULL);
        sprintf(g_pstSdkVideoInfo->stWaterMark.szBufMutexName, "watermark_buf_mtx");
    }

    // 分配编码流的缓存
    for (i = 0; i < s32VinNum; i++)
    {
        TVT_SDK_VIDEO_CFG_S* pstSdkVideoCfg = &pstVideoCfg[i];
        if (NULL == g_pstSdkVideoInfo->stStreamBuff[i].pszBuf)
        {
            g_pstSdkVideoInfo->stStreamBuff[i].s32BufSize = ENCODE_STREAM_BUFF_SIZE;
            g_pstSdkVideoInfo->stStreamBuff[i].pszBuf = (char*)malloc(ENCODE_STREAM_BUFF_SIZE);
            sprintf(g_pstSdkVideoInfo->stStreamBuff[i].szBufMutexName, "stream_buf_mtx_%d", i);
            pthread_mutex_init(&(g_pstSdkVideoInfo->stStreamBuff[i].s32BufMutex), NULL);
            if (NULL == g_pstSdkVideoInfo->stStreamBuff[i].pszBuf)
            {
                TIPC_PRINTF_INFO("malloc stream buffer failed");
                return -1;
            }
        }
        for (j = 0; j < CHIP_VENC_JPEG_NUM; j++)
        {
            if (NULL == g_pstSdkVideoInfo->stJpegBuff[i][j].pszBuf)
            {
                int s32Width = pstSdkVideoCfg->stSensorCfg.u32MaxWidth;
                int s32Height = pstSdkVideoCfg->stSensorCfg.u32MaxHeight;
                g_pstSdkVideoInfo->stJpegBuff[i][j].s32BufSize = (s32Width * s32Height * 3) >> 2; // 暂时最大按照BGR size 的 1/4 分配, 内存不够用再优化
                g_pstSdkVideoInfo->stJpegBuff[i][j].pszBuf = (char*)malloc(g_pstSdkVideoInfo->stJpegBuff[i][j].s32BufSize);
                sprintf(g_pstSdkVideoInfo->stJpegBuff[i][j].szBufMutexName, "jpeg_buf_mtx_%d", i);
                pthread_mutex_init(&(g_pstSdkVideoInfo->stJpegBuff[i][j].s32BufMutex), NULL);
                if (NULL == g_pstSdkVideoInfo->stJpegBuff[i][j].pszBuf)
                {
                    TIPC_PRINTF_INFO("malloc jpeg buffer failed");
                    return -1;
                }
            }
        }
    }

    // 初始化视频模块
    int s32MallocSize = (sizeof(CHIP_VIDEO_CFG_S)) * s32VinNum;
    CHIP_VIDEO_CFG_S* pstChipVideoCfg = (CHIP_VIDEO_CFG_S* )malloc(s32MallocSize);
    for (i = 0; i < s32VinNum; i++)
    {
        CHIP_VIDEO_CFG_S* pstChipVideoCfgTmp = &pstChipVideoCfg[i];
        TVT_SDK_VIDEO_CFG_S* pstVideoCfgTmp = &pstVideoCfg[i];
        pstChipVideoCfgTmp->enFormat = pstVideoCfgTmp->enFormat;
        pstChipVideoCfgTmp->enWorkMode = pstVideoCfgTmp->enWorkMode;
        memcpy(&pstChipVideoCfgTmp->stSensorCfg, &pstVideoCfgTmp->stSensorCfg, sizeof(TVT_SDK_PROD_SENSOR_S));
        s32VinDevID[i] = (int)pstVideoCfgTmp->stSensorCfg.u32VinDev;
    }
    s32Ret = chip_video_init(pstChipVideoCfg, s32VinNum);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("video vin init failed");
        return s32Ret;
    }
    free(pstChipVideoCfg);

    // 初始化OSD子业务
    s32Ret = tvt_sdk_osd_init(s32VinNum, s32VinDevID);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("video osd service init failed[%d]", s32Ret);
        return s32Ret;
    }

    // 初始化Mask子业务
    s32Ret = tvt_sdk_mask_init(s32VinNum, s32VinDevID);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("video mask service init failed[%d]", s32Ret);
        return s32Ret;
    }

    // 初始化Roi子业务
    s32Ret = tvt_sdk_roi_init(s32VinNum, s32VinDevID);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("video roi service init failed[%d]", s32Ret);
        return s32Ret;
    }

    // fece_detection part init
    s32Ret = tvt_sdk_facedt_init(s32VinNum, s32VinDevID);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("video facedt service init failed[%d]", s32Ret);
        return s32Ret;
    }

    // 保存动态参数
    g_pstSdkVideoInfo->s32VinCnt = s32VinNum;
    for (i = 0; i < s32VinNum; i++)
    {
        TVT_SDK_VIDEO_CFG_S* pstVideoCfgTmp = &pstVideoCfg[i];
        g_pstSdkVideoInfo->stVinInfo[i].s32VinDevID = (int)pstVideoCfgTmp->stSensorCfg.u32VinDev;
        g_pstSdkVideoInfo->stVinInfo[i].enWorkMode = pstVideoCfgTmp->enWorkMode;
        g_pstSdkVideoInfo->stVinInfo[i].enFormat = pstVideoCfgTmp->enFormat;
        g_pstSdkVideoInfo->stVinInfo[i].s32SrcWidth = pstVideoCfgTmp->stSensorCfg.u32MaxWidth;
        g_pstSdkVideoInfo->stVinInfo[i].s32SrcHeight = pstVideoCfgTmp->stSensorCfg.u32MaxHeight;
    }

    // 初始化模块锁
    pthread_mutex_init(&g_pstSdkVideoInfo->s32Mutex, NULL);
    sprintf(g_pstSdkVideoInfo->szMutexName, "tvt_sdk_video_lock");
    g_pstSdkVideoInfo->u8Init = 1;

    return s32Ret;
}

/**
 * @brief 去初始化视频业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_video_exit()
{
    int s32Ret = 0;
    if (NULL == g_pstSdkVideoInfo)
    {
        // 未初始化
        TIPC_PRINTF_ERR("video service is not running");
        return -1;
    }

    if (g_pstSdkVideoInfo->u8Init != 1)
    {
        // 未初始化
        TIPC_PRINTF_ERR("video service is not running");
        free((void*)g_pstSdkVideoInfo);
        g_pstSdkVideoInfo = NULL;
        return -1;
    }

    TIPC_MUTEX_LOCK(&g_pstSdkVideoInfo->s32Mutex, g_pstSdkVideoInfo->szMutexName, &g_pstSdkVideoInfo->u32MutexLockCnt, TVT_SDK_VIDEO_DEBUG);

    int i = 0, j = 0;
    // 加锁
    for (i = 0; i < CHIP_MAX_VI_NUM; i++)
    {
        if (g_pstSdkVideoInfo->stStreamBuff[i].pszBuf != NULL)
        {
            TIPC_MUTEX_LOCK(&g_pstSdkVideoInfo->stStreamBuff[i].s32BufMutex,
                            g_pstSdkVideoInfo->stStreamBuff[i].szBufMutexName,
                            &g_pstSdkVideoInfo->stStreamBuff[i].u32LockCnt,
                            TVT_SDK_VIDEO_DEBUG);
        }
        for (j = 0; j < CHIP_VENC_JPEG_NUM; j++)
        {
            if (g_pstSdkVideoInfo->stJpegBuff[i][j].pszBuf != NULL)
            {
                TIPC_MUTEX_LOCK(&g_pstSdkVideoInfo->stJpegBuff[i][j].s32BufMutex,
                                g_pstSdkVideoInfo->stJpegBuff[i][j].szBufMutexName,
                                &g_pstSdkVideoInfo->stJpegBuff[i][j].u32LockCnt,
                                TVT_SDK_VIDEO_DEBUG);
            }
        }
    }

    // 关闭OSD业务
    s32Ret = tvt_sdk_osd_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("video osd service exit failed[%d]", s32Ret);
    }

    // 关闭Mask业务
    s32Ret = tvt_sdk_mask_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("video osd service exit failed[%d]", s32Ret);
    }

    // 关闭ROI业务
    s32Ret = tvt_sdk_roi_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("video roi service exit failed[%d]", s32Ret);
    }

    // fece_detection part exit
    s32Ret = tvt_sdk_facedt_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("video facedt service exit failed[%d]", s32Ret);
    }

    // 关闭模块
    s32Ret = chip_video_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("chip_video_exit failed[%d]", s32Ret);
    }

    // 释放锁和资源
    for (i = 0; i < CHIP_MAX_VI_NUM; i++)
    {
        if (g_pstSdkVideoInfo->stStreamBuff[i].pszBuf != NULL)
        {
            free(g_pstSdkVideoInfo->stStreamBuff[i].pszBuf);
            g_pstSdkVideoInfo->stStreamBuff[i].pszBuf = NULL;
            TIPC_MUTEX_UNLOCK(&g_pstSdkVideoInfo->stStreamBuff[i].s32BufMutex,
                              g_pstSdkVideoInfo->stStreamBuff[i].szBufMutexName,
                              &g_pstSdkVideoInfo->stStreamBuff[i].u32LockCnt,
                              TVT_SDK_VIDEO_DEBUG);
            pthread_mutex_destroy(&g_pstSdkVideoInfo->stStreamBuff[i].s32BufMutex);
        }
        for (j = 0; j < CHIP_VENC_JPEG_NUM; j++)
        {
            if (g_pstSdkVideoInfo->stJpegBuff[i][j].pszBuf != NULL)
            {
                free(g_pstSdkVideoInfo->stJpegBuff[i][j].pszBuf);
                g_pstSdkVideoInfo->stJpegBuff[i][j].pszBuf = NULL;
                TIPC_MUTEX_UNLOCK(&g_pstSdkVideoInfo->stJpegBuff[i][j].s32BufMutex,
                                  g_pstSdkVideoInfo->stJpegBuff[i][j].szBufMutexName,
                                  &g_pstSdkVideoInfo->stJpegBuff[i][j].u32LockCnt,
                                  TVT_SDK_VIDEO_DEBUG);
                pthread_mutex_destroy(&g_pstSdkVideoInfo->stJpegBuff[i][j].s32BufMutex);
            }
        }
    }

    TIPC_MUTEX_UNLOCK(&g_pstSdkVideoInfo->s32Mutex, g_pstSdkVideoInfo->szMutexName, &g_pstSdkVideoInfo->u32MutexLockCnt, TVT_SDK_VIDEO_DEBUG);
    pthread_mutex_destroy(&g_pstSdkVideoInfo->s32Mutex);
    pthread_mutex_destroy(&g_pstSdkVideoInfo->stWaterMark.s32BufMutex);

    free((void*)g_pstSdkVideoInfo);
    g_pstSdkVideoInfo = NULL;

    return 0;
}