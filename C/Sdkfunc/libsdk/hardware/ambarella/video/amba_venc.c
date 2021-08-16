
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/mman.h>
#include "amba_venc.h"

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

#define VENC_QUERY_TIMEOUT_MS 100 // 超时100ms
#define VENC_QUERY_TIMEOUT_FPS (1000 / VENC_QUERY_TIMEOUT_MS)

typedef struct amba_venc_cfg_s
{
    int              s32Fd;          ///< 设备文件描述符
    int              s32SensorCnt;   ///< sensor计数
    unsigned int     u32BsbSize;     ///< 编码缓存池大小
    char*            pszBsbMem;      ///< 编码缓存池地址
    unsigned char    u8Init;         ///< 初始化标志
} AMBA_VENC_CFG_S;

static AMBA_VENC_CFG_S g_stVencCfg = {0};

// rate control variables
static u32 g_u32RcBrTable[11] =
{
    256000,  512000,  768000,  1000000, 1500000, 2000000,
    3000000, 4000000, 6000000, 8000000, 10000000,
};

static u32 g_u32RcResoTable[11] =
{
    1920 * 1080, 1280 * 1024, 1280 * 960, 1280 * 720, 1024 * 768,  800 * 600,
    720 * 576,  720 * 480, 640 * 480,   352 * 288,   320 * 240,
};

static u32 g_u32H264RcQpForVbrLut[11][11] =
{
    {31, 29, 27, 27, 26, 23, 23, 22, 22, 17, 16},   /* 256 kbps */
    {30, 26, 25, 25, 24, 22, 21, 20, 20, 16, 15},   /* 512 kbps */
    {28, 25, 24, 24, 23, 21, 20, 19, 19, 15, 14},   /* 768 kbps */
    {27, 24, 23, 23, 22, 20, 19, 18, 18, 14, 13},   /* 1 Mbps */
    {26, 24, 22, 22, 21, 19, 18, 17, 17, 12, 11},   /* 1.5 Mbps */
    {25, 23, 22, 21, 19, 18, 17, 16, 16, 11, 10},   /* 2 Mbps */
    {24, 22, 21, 20, 19, 17, 16, 15, 15, 9,  8},    /* 3 Mbps */
    {23, 21, 20, 19, 18, 16, 15, 14, 14, 8,  7},    /* 4 Mbps */
    {22, 20, 19, 18, 17, 15, 14, 13, 12, 5,  1},    /* 6 Mbps */
    {21, 19, 18, 17, 16, 14, 13, 12, 11, 1,  1},    /* 8 Mbps */
    {21, 18, 17, 16, 15, 13, 12, 11, 10, 1,  1},    /* 10 Mbps */
};

static u32 g_u32H265RcQpForVbrLut[11][11] =
{
    {34, 32, 30, 30, 29, 26, 26, 25, 25, 20, 19},   /* 256 kbps */
    {34, 29, 28, 28, 27, 25, 24, 23, 23, 19, 18},   /* 512 kbps */
    {31, 28, 27, 27, 26, 24, 23, 22, 22, 18, 17},   /* 768 kbps */
    {30, 27, 26, 26, 25, 23, 22, 21, 21, 17, 16},   /* 1 Mbps */
    {29, 27, 25, 25, 24, 22, 20, 20, 20, 15, 14},   /* 1.5 Mbps */
    {28, 26, 25, 24, 22, 21, 20, 19, 19, 14, 13},   /* 2 Mbps */
    {27, 25, 24, 23, 22, 20, 19, 18, 18, 12,  11},  /* 3 Mbps */
    {26, 24, 23, 22, 21, 19, 18, 17, 17, 11,  10},  /* 4 Mbps */
    {25, 23, 22, 21, 20, 18, 17, 16, 15, 8,  4},    /* 6 Mbps */
    {24, 22, 21, 20, 19, 17, 16, 15, 14, 4,  4},    /* 8 Mbps */
    {24, 21, 20, 19, 18, 16, 15, 14, 13, 4,  4},    /* 10 Mbps */
};

extern int chip_video_getVencStreamID(int s32VinDevID, int s32StreamIDX, int* ps32ResChnID, int* ps32StreamID);

/**
 * @brief 计算QP
 * @return 0：成功；非0：失败
 */
static int amba_venc_calcH26xTargetQp(u32 u32Codec, u32 u32Bitrate, u32 u32Resolution)
{
    int i, j;

    for (i = 0; i < ARRAY_SIZE(g_u32RcBrTable); i++)
    {
        if (u32Bitrate <= g_u32RcBrTable[i])
        {
            break;
        }
    }

    if (ARRAY_SIZE(g_u32RcBrTable) == i)
    {
        TIPC_PRINTF_ERR("Invalid bitrate: %d", u32Bitrate);
        return -1;
    }

    for (j = 0; j < ARRAY_SIZE(g_u32RcResoTable); j++)
    {
        if (u32Resolution >= g_u32RcResoTable[j])
        {
            break;
        }
    }

    if (ARRAY_SIZE(g_u32RcResoTable) == j)
    {
        TIPC_PRINTF_ERR("Invalid resolution: %d\n", u32Resolution);
        return -1;
    }

    if (IAV_STREAM_TYPE_H264 == u32Codec)
    {
        return g_u32H264RcQpForVbrLut[i][j];
    }
    else
    {
        return g_u32H265RcQpForVbrLut[i][j];
    }
}

/**
 * @brief H26x参数配置
 * @return 0：成功；非0：失败
 */
static int amba_venc_setH26xEncodeParam(int s32StreamID, const AMBA_VENC_CHN_CFG_S* pstVencChnCfg)
{
    struct iav_h26x_cfg stH26XCfg = {0};
    struct iav_stream_cfg stStreamCfg = {0};
    struct iav_bitrate stBitrate = {0};
    struct iav_h26x_gop stGop = {0};
    enum iav_stream_type enStreamType = pstVencChnCfg->enStreamType;
    u32 u32Resolution = 0, u32QP = 0;

    int s32IavFD = g_stVencCfg.s32Fd;

    memset(&stH26XCfg, 0, sizeof(stH26XCfg));
    stH26XCfg.id = s32StreamID;

    if (IAV_STREAM_TYPE_H264 == enStreamType)
    {
        AM_IOCTL(s32IavFD, IAV_IOC_GET_H264_CONFIG, &stH26XCfg);
    }
    else
    {
        AM_IOCTL(s32IavFD, IAV_IOC_GET_H265_CONFIG, &stH26XCfg);
    }

    // 仅配置部分，其他使用默认值，若码流有问题这部分配置需要关注
    stH26XCfg.M = 1; // P帧间隔
    stH26XCfg.N = pstVencChnCfg->stH26xCfg.s32Gop; // I帧间隔
    stH26XCfg.idr_interval = 1; // 每个I帧都是IDR
    stH26XCfg.gop_structure = IAV_GOP_SIMPLE;
    // stH26XCfg.wp_mode = ;
    // 安霸不支持baseline?
    stH26XCfg.profile = (pstVencChnCfg->stH26xCfg.s32Profile == 2) ? H264_PROFILE_HIGH : H264_PROFILE_MAIN; // 0:baseline 1:main 2:high
    stH26XCfg.chroma_format = H264_CHROMA_YUV420;

    // h264 syntax settings
    /// TODO: au_type需要Check
    // stH26XCfg.au_type = ;

    if (IAV_STREAM_TYPE_H264 == enStreamType)
    {
        AM_IOCTL(s32IavFD, IAV_IOC_SET_H264_CONFIG, &stH26XCfg);
    }
    else
    {
        stH26XCfg.profile = H264_PROFILE_MAIN; // H265 仅支持main
        AM_IOCTL(s32IavFD, IAV_IOC_SET_H265_CONFIG, &stH26XCfg);
    }

    memset(&stStreamCfg, 0, sizeof(stStreamCfg));
    stStreamCfg.id = s32StreamID;
    stStreamCfg.cid = (IAV_STREAM_TYPE_H264 == enStreamType) ? IAV_H264_CFG_BITRATE : IAV_H265_CFG_BITRATE;
    AM_IOCTL(s32IavFD, IAV_IOC_GET_STREAM_CONFIG, &stStreamCfg);
    stBitrate = stStreamCfg.arg.h264_rc;
    stBitrate.average_bitrate = pstVencChnCfg->stH26xCfg.s32BitRate * 1024;
    stBitrate.qp_max_on_I = 51;
    stBitrate.qp_max_on_P = 51;
    stBitrate.qp_max_on_B = 51;
    stBitrate.qp_max_on_Q = 51;
    stBitrate.skip_flag = 0;
    u32Resolution = pstVencChnCfg->s32Width * pstVencChnCfg->s32Height;

    switch (pstVencChnCfg->stH26xCfg.s32BitRateCtrl)
    {
    /// TODO: Sample还支持IAV_C/VBR_QUALITY_KEEPING，可以参考
    // 这里CBR和VBR的设置具体是参考Sample来的，所以会出现IAV_BRC_VBR实际配的是IAV_BRC_SCBR
    case IAV_BRC_CBR:
        stBitrate.vbr_setting = IAV_BRC_SCBR;
        stBitrate.qp_min_on_I = 14;
        stBitrate.qp_min_on_P = 17;
        stBitrate.qp_min_on_B = 21;
        stBitrate.qp_min_on_Q = 15;
        break;
    case IAV_BRC_VBR:
        u32QP = amba_venc_calcH26xTargetQp(enStreamType, pstVencChnCfg->stH26xCfg.s32BitRate, u32Resolution);
        if (u32QP < 0)
        {
            return -1;
        }
        stBitrate.vbr_setting = IAV_BRC_SCBR;
        stBitrate.average_bitrate = pstVencChnCfg->stH26xCfg.s32BitRate * 1024;
        stBitrate.cbr_stable_br_adjust = 0;
        stBitrate.qp_min_on_I = u32QP;
        stBitrate.qp_min_on_P = u32QP;
        stBitrate.qp_min_on_B = u32QP;
        stBitrate.qp_min_on_Q = u32QP;
        break;
    default:
        TIPC_PRINTF_ERR("Unknown rate control mode [%d] !\n", pstVencChnCfg->stH26xCfg.s32BitRateCtrl);
        return -1;
    }

    memset(&stStreamCfg, 0, sizeof(stStreamCfg));
    stStreamCfg.id = s32StreamID;
    stStreamCfg.cid = (IAV_STREAM_TYPE_H264 == enStreamType) ? IAV_H264_CFG_BITRATE : IAV_H265_CFG_BITRATE;
    stStreamCfg.arg.h265_rc = stBitrate;
    AM_IOCTL(s32IavFD, IAV_IOC_SET_STREAM_CONFIG, &stStreamCfg);

    memset(&stStreamCfg, 0, sizeof(stStreamCfg));
    stStreamCfg.id = s32StreamID;
    stGop.id = s32StreamID;
    stGop.N = stH26XCfg.N;
    stGop.idr_interval = stH26XCfg.idr_interval;
    stStreamCfg.cid = (IAV_STREAM_TYPE_H264 == enStreamType) ? IAV_H264_CFG_GOP : IAV_H265_CFG_GOP;
    stStreamCfg.arg.h265_gop = stGop;
    AM_IOCTL(s32IavFD, IAV_IOC_SET_STREAM_CONFIG, &stStreamCfg);

    // TIPC_PRINTF_CHK("-----------------   Set H26X Encoding Stream[%d] Parameter  -----------------", s32StreamID);

    return 0;
}

/**
 * @brief mjpeg参数配置
 * @return 0：成功；非0：失败
 */
static int amba_venc_setMjpegEncodeParam(int s32StreamID, const AMBA_VENC_CHN_CFG_S* pstVencChnCfg)
{
    struct iav_mjpeg_cfg stMjpegCfg = {0};
    struct iav_queryinfo stQueryInfo = {0};
    struct iav_stream_info* pstStreamInfo = NULL;
    struct iav_stream_cfg stStreamCfg = {0};
    int s32IavFD = g_stVencCfg.s32Fd;

    memset(&stQueryInfo, 0, sizeof(stQueryInfo));
    stQueryInfo.qid = IAV_INFO_STREAM;
    pstStreamInfo = &stQueryInfo.arg.stream;
    pstStreamInfo->id = s32StreamID;
    AM_IOCTL(s32IavFD, IAV_IOC_QUERY_INFO, &stQueryInfo);

    if (pstStreamInfo->state != IAV_STREAM_STATE_ENCODING)
    {
        stMjpegCfg.id = s32StreamID;
        AM_IOCTL(s32IavFD, IAV_IOC_GET_MJPEG_CONFIG, &stMjpegCfg);
        stMjpegCfg.quality = pstVencChnCfg->stMjpegCfg.s32Quality;
        AM_IOCTL(s32IavFD, IAV_IOC_SET_MJPEG_CONFIG, &stMjpegCfg);
    }
    else
    {
        memset(&stStreamCfg, 0, sizeof(stStreamCfg));
        stStreamCfg.id = s32StreamID;
        stStreamCfg.cid = IAV_MJPEG_CFG_QUALITY;
        stStreamCfg.arg.mjpeg_quality = pstVencChnCfg->stMjpegCfg.s32Quality;
        AM_IOCTL(s32IavFD, IAV_IOC_SET_STREAM_CONFIG, &stStreamCfg);
    }

    return 0;
}

/**
 * @brief 创建编码通道
 * @return 0：成功；非0：失败
 */
int amba_venc_createChn(const AMBA_VENC_CHN_CFG_S* pstVencChnCfg)
{
    int s32Ret = 0;

    if (g_stVencCfg.s32Fd <= 0)
    {
        TIPC_PRINTF_ERR("/dev/iav check err! [%d]", g_stVencCfg.s32Fd);
        return -1;
    }
    if (NULL == pstVencChnCfg)
    {
        TIPC_PRINTF_ERR("NULL pointer!!");
        return -1;
    }

    // 寻找Sensor dev id对应的编码通道号
    int s32StreamID = -1;
    s32Ret = chip_video_getVencStreamID(pstVencChnCfg->s32VinDevID, pstVencChnCfg->s32StreamIDX, NULL, &s32StreamID);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get stream id failed!!");
        return -1;
    }

    struct iav_stream_cfg stIavStreamCfg = {0};
    struct iav_stream_format* pstIavStreamFormat = NULL;

    stIavStreamCfg.id = s32StreamID;
    stIavStreamCfg.cid = IAV_STMCFG_FORMAT;
    AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_GET_STREAM_CONFIG, &stIavStreamCfg);
    pstIavStreamFormat = &stIavStreamCfg.arg.format;
    pstIavStreamFormat->type = pstVencChnCfg->enStreamType;
    pstIavStreamFormat->enc_win.x = 0;
    pstIavStreamFormat->enc_win.y = 0;
    pstIavStreamFormat->enc_win.width = pstVencChnCfg->s32Width;
    pstIavStreamFormat->enc_win.height = pstVencChnCfg->s32Height;
    // enc_src_id 指 canvas id
    pstIavStreamFormat->enc_src_id = (s32StreamID + 1);

    AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_SET_STREAM_CONFIG, &stIavStreamCfg);

    switch (pstIavStreamFormat->type)
    {
    case IAV_STREAM_TYPE_H264:
    case IAV_STREAM_TYPE_H265:
        s32Ret = amba_venc_setH26xEncodeParam(stIavStreamCfg.id, pstVencChnCfg);
        break;
    case IAV_STREAM_TYPE_MJPEG:
        s32Ret = amba_venc_setMjpegEncodeParam(stIavStreamCfg.id, pstVencChnCfg);
        break;
    default:
        TIPC_PRINTF_ERR("check encode format[%d] err!!", pstIavStreamFormat->type);
        return -1;
        break;
    }
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba venc encode param set failed[%d]!!", s32Ret);
        return -1;
    }

    // framerate
    stIavStreamCfg.id = s32StreamID;
    stIavStreamCfg.cid = IAV_STMCFG_FPS;
    AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_GET_STREAM_CONFIG, &stIavStreamCfg);
    stIavStreamCfg.arg.fps.abs_fps_enable = 1;
    stIavStreamCfg.arg.fps.abs_fps = pstVencChnCfg->s32Fps;
    AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_SET_STREAM_CONFIG, &stIavStreamCfg);

    // start encoding
    struct iav_queryinfo stIavQueryInfo;
    struct iav_stream_info* pstStreamInfo;
    memset(&stIavQueryInfo, 0, sizeof(stIavQueryInfo));
    stIavQueryInfo.qid = IAV_INFO_STREAM;
    pstStreamInfo = &stIavQueryInfo.arg.stream;
    pstStreamInfo->id = stIavStreamCfg.id;
    AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_QUERY_INFO, &stIavQueryInfo);
    // WKL
    TIPC_PRINTF_CHK("Before start encoding, stream[%d] status[%d]\n", s32StreamID, pstStreamInfo->state);

    AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_START_ENCODE, (1 << stIavStreamCfg.id));

    // TIPC_PRINTF_CHK("-----------------   Create And Start Encode Stream[%d]   -----------------", s32StreamID);

    // amba_venc_requestIDRStream(pstVencChnCfg->s32VinDevID, pstVencChnCfg->s32StreamIDX);
    return s32Ret;
}

/**
 * @brief 获取编码通道配置
 * @return 0：成功；非0：失败
 */
int amba_venc_getChnCfg(AMBA_VENC_CHN_CFG_S* pstVencChnCfg)
{
    int s32Ret = 0;
    struct iav_queryinfo stQueryInfo = {0};
    struct iav_stream_info* pstStreamInfo = NULL;
    struct iav_stream_cfg stIavStreamCfg = {0};
    struct iav_stream_format* pstIavStreamFormat = NULL;

    if (g_stVencCfg.s32Fd <= 0)
    {
        TIPC_PRINTF_ERR("/dev/iav check err! [%d]", g_stVencCfg.s32Fd);
        return -1;
    }
    if (NULL == pstVencChnCfg)
    {
        TIPC_PRINTF_ERR("NULL pointer!!");
        return -1;
    }

    // 寻找Sensor dev id对应的编码通道号
    int s32StreamID = -1;
    s32Ret = chip_video_getVencStreamID(pstVencChnCfg->s32VinDevID, pstVencChnCfg->s32StreamIDX, NULL, &s32StreamID);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get stream id failed!!");
        return -1;
    }

    // 检查是否在编码
    stQueryInfo.qid = IAV_INFO_STREAM;
    pstStreamInfo = &stQueryInfo.arg.stream;
    pstStreamInfo->id = s32StreamID;
    AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_QUERY_INFO, &stQueryInfo);
    if (pstStreamInfo->state != IAV_STREAM_STATE_ENCODING)
    {
        // 未编码
        return -1;
    }

    //
    stIavStreamCfg.id = s32StreamID;
    stIavStreamCfg.cid = IAV_STMCFG_FORMAT;
    AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_GET_STREAM_CONFIG, &stIavStreamCfg);
    pstIavStreamFormat = &stIavStreamCfg.arg.format;
    pstVencChnCfg->s32Width = pstIavStreamFormat->enc_win.width;
    pstVencChnCfg->s32Height = pstIavStreamFormat->enc_win.height;
    pstVencChnCfg->enStreamType = pstIavStreamFormat->type;
    /// TODO: 其他的待补充

    return 0;
}

/**
 * @brief 销毁编码通道
 * @return 0：成功；非0：失败
 */
int amba_venc_destroyChn(int s32VinDevID, int s32StreamIDX)
{
    int s32Ret = 0;
    if (g_stVencCfg.s32Fd <= 0)
    {
        TIPC_PRINTF_ERR("/dev/iav check err! [%d]", g_stVencCfg.s32Fd);
        return -1;
    }

    // 寻找Sensor dev id对应的编码通道
    int s32StreamID = -1;
    s32Ret = chip_video_getVencStreamID(s32VinDevID, s32StreamIDX, NULL, &s32StreamID);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get stream id failed!!");
        return -1;
    }

    // 销毁(stop/abort)
    struct iav_queryinfo stQueryInfo = {0};
    struct iav_stream_info* pstStreamInfo = NULL;

    stQueryInfo.qid = IAV_INFO_STREAM;
    pstStreamInfo = &stQueryInfo.arg.stream;
    pstStreamInfo->id = s32StreamID;
    AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_QUERY_INFO, &stQueryInfo);
    if (pstStreamInfo->state != IAV_STREAM_STATE_ENCODING)
    {
        TIPC_PRINTF_CHK("Stream[%d] current status[%d]", s32StreamID, pstStreamInfo->state);
    }
    else
    {
        AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_STOP_ENCODE, (1 << s32StreamID));
    }

    stQueryInfo.qid = IAV_INFO_STREAM;
    pstStreamInfo = &stQueryInfo.arg.stream;
    pstStreamInfo->id = s32StreamID;
    AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_QUERY_INFO, &stQueryInfo);
    if (pstStreamInfo->state != IAV_STREAM_STATE_ENCODING &&
            pstStreamInfo->state != IAV_STREAM_STATE_STOPPING)
    {
        TIPC_PRINTF_CHK("Stream[%d] current status[%d]", s32StreamID, pstStreamInfo->state);
    }
    else
    {
        AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_ABORT_ENCODE, (1 << s32StreamID));
    }
    // TIPC_PRINTF_CHK("-----------------   Stop And Abort Encode Stream[%d]   -----------------", s32StreamID);
    return 0;
}


/**
 * @brief 获取编码通道数据
 * @return 0：成功；非0：失败
 */
int amba_venc_getChnStream(int s32VinDevID, int s32StreamIDX, struct iav_framedesc* pstFrameDesc, char** pszStreamBuffer)
{
    int s32Ret = 0;
    struct iav_queryinfo stQueryInfo = {0};
    struct iav_stream_info* pstStreamInfo = NULL;
    struct iav_querydesc stQueryDesc = {0};
    struct iav_framedesc* pstIavFrameDesc = NULL;

    if (g_stVencCfg.s32Fd <= 0)
    {
        TIPC_PRINTF_ERR("/dev/iav check err! [%d]", g_stVencCfg.s32Fd);
        return -1;
    }
    if (NULL == pstFrameDesc)
    {
        TIPC_PRINTF_ERR("NULL pointer!!");
        return -1;
    }

    // 寻找Sensor dev id对应的编码通道
    int s32StreamID = -1;
    s32Ret = chip_video_getVencStreamID(s32VinDevID, s32StreamIDX, NULL, &s32StreamID);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get stream id failed!!");
        return -1;
    }

    stQueryInfo.qid = IAV_INFO_STREAM;
    pstStreamInfo = &stQueryInfo.arg.stream;
    pstStreamInfo->id = s32StreamID;
    AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_QUERY_INFO, &stQueryInfo);
#if WKL_DEBUG
    TIPC_PRINTF_CHK("[****** WKL Debug ******]     getChnStream Stream ID = %d ", s32StreamID);
#endif
    if (pstStreamInfo->state != IAV_STREAM_STATE_ENCODING)
    {
        TIPC_PRINTF_ERR("Stream id[%d] state err[%d]", s32StreamID, pstStreamInfo->state);
        return -1;
    }

    // 获取帧率
    struct iav_stream_cfg stIavStreamCfg = {0};
    stIavStreamCfg.id = s32StreamID;
    stIavStreamCfg.cid = IAV_STMCFG_FPS;
    s32Ret = ioctl(g_stVencCfg.s32Fd, IAV_IOC_GET_STREAM_CONFIG, &stIavStreamCfg);
    if (s32Ret < 0)
    {
        perror("IAV_IOC_GET_STREAM_CONFIG");
        TIPC_PRINTF_CHK("check stream framerate failed[%d]", s32Ret);
        stIavStreamCfg.arg.fps.abs_fps = 1;
    }
    if (stIavStreamCfg.arg.fps.abs_fps == 0)
    {
        stIavStreamCfg.arg.fps.abs_fps = 1;
    }

    memset(&stQueryDesc, 0, sizeof(stQueryDesc));
    pstIavFrameDesc = &stQueryDesc.arg.frame;
    stQueryDesc.qid = IAV_DESC_FRAME;
    pstIavFrameDesc->id = s32StreamID;
    if (stIavStreamCfg.arg.fps.abs_fps > VENC_QUERY_TIMEOUT_FPS)
    {
        // 根据帧率决定超时时间
        pstIavFrameDesc->time_ms = VENC_QUERY_TIMEOUT_MS;
    }
    else
    {
        pstIavFrameDesc->time_ms = (1000 / (stIavStreamCfg.arg.fps.abs_fps)) * 2;
    }

    AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_QUERY_DESC, &stQueryDesc);

    *pszStreamBuffer = g_stVencCfg.pszBsbMem + pstIavFrameDesc->data_addr_offset;
    memcpy(pstFrameDesc, pstIavFrameDesc, sizeof(struct iav_framedesc));
    // TIPC_PRINTF_CHK("-----------------   Get a Frame from Stream[%d]   -----------------", s32StreamID);
    return s32Ret;
}

/**
 * @brief 释放编码通道数据
 * @return 0：成功；非0：失败
 */
int amba_venc_releaseChnStream(struct iav_framedesc* pstFrameDesc)
{
    if (g_stVencCfg.s32Fd <= 0)
    {
        TIPC_PRINTF_ERR("/dev/iav check err! [%d]", g_stVencCfg.s32Fd);
        return -1;
    }
    if (NULL == pstFrameDesc)
    {
        TIPC_PRINTF_ERR("NULL pointer!!");
        return -1;
    }
    AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_RELEASE_FRAMEDESC, pstFrameDesc);
    return 0;
}

/**
 * @brief 请求IDR帧
 * @return 0：成功；非0：失败
 */
int amba_venc_requestIDRStream(int s32VinDevID, int s32StreamIDX)
{
    int s32Ret = 0;
    if (g_stVencCfg.s32Fd <= 0)
    {
        TIPC_PRINTF_ERR("/dev/iav check err! [%d]", g_stVencCfg.s32Fd);
        return -1;
    }

    // 寻找Sensor dev id对应的编码通道
    int s32StreamID = -1;
    s32Ret = chip_video_getVencStreamID(s32VinDevID, s32StreamIDX, NULL, &s32StreamID);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get stream id failed!!");
        return -1;
    }

    struct iav_stream_cfg stStreamCfg = {0};

    stStreamCfg.id = s32StreamID;
    stStreamCfg.cid = IAV_STMCFG_FORMAT;
    AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_GET_STREAM_CONFIG, &stStreamCfg);

    enum iav_stream_type enStreamType = stStreamCfg.arg.format.type;
    enum iav_streamcfg_id enStreamCfgID = IAV_H264_CFG_FORCE_IDR;

    switch (enStreamType)
    {
    case IAV_STREAM_TYPE_H264:
    {
        enStreamCfgID = IAV_H264_CFG_FORCE_IDR;
    }
    break;
    case IAV_STREAM_TYPE_H265:
    {
        enStreamCfgID = IAV_H265_CFG_FORCE_IDR;
    }
    break;
    default:
    {
        TIPC_PRINTF_ERR("check stream type[%d] failed!!", enStreamType);
        return -1;
    }
    break;
    }

    memset(&stStreamCfg, 0, sizeof(stStreamCfg));
    stStreamCfg.id = s32StreamID;
    stStreamCfg.cid = enStreamCfgID;
    AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_SET_STREAM_CONFIG, &stStreamCfg);
    // TIPC_PRINTF_CHK("-----------------   Request IDR Frame for Stream[%d]   -----------------", s32StreamID);
    return 0;
}


/**
 * @brief 使能编码通道
 * @return 0：成功；非0：失败
 */
int amba_venc_startStream(int s32VinDevID, int s32StreamIDX)
{
    int s32Ret = 0;
    if (g_stVencCfg.s32Fd <= 0)
    {
        TIPC_PRINTF_ERR("/dev/iav check err! [%d]", g_stVencCfg.s32Fd);
        return -1;
    }

    // 寻找Sensor dev id对应的编码通道
    int s32StreamID = -1;
    s32Ret = chip_video_getVencStreamID(s32VinDevID, s32StreamIDX, NULL, &s32StreamID);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get stream id failed!!");
        return -1;
    }

    // start encoding
    struct iav_queryinfo stIavQueryInfo;
    struct iav_stream_info* pstStreamInfo;
    memset(&stIavQueryInfo, 0, sizeof(stIavQueryInfo));
    stIavQueryInfo.qid = IAV_INFO_STREAM;
    pstStreamInfo = &stIavQueryInfo.arg.stream;
    pstStreamInfo->id = s32StreamID;
    AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_QUERY_INFO, &stIavQueryInfo);
    TIPC_PRINTF_CHK("stream[%d] status[%d]\n", s32StreamID, pstStreamInfo->state);
    if (pstStreamInfo->state != IAV_STREAM_STATE_ENCODING)
    {
        AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_START_ENCODE, (1 << s32StreamID));
        // TIPC_PRINTF_CHK("-----------------   Start Encode Stream[%d]   -----------------", s32StreamID);
    }

    return s32Ret;
}

/**
 * @brief 初始化模块
 * @return 0：成功；非0：失败
 */
int amba_venc_init(int s32IavFD, int s32SensorCnt)
{
    if (s32IavFD <= 0)
    {
        TIPC_PRINTF_ERR("/dev/iav is not opened! [%d]", s32IavFD);
        return -1;
    }

    g_stVencCfg.s32Fd = s32IavFD;
    g_stVencCfg.u8Init = 1;
    g_stVencCfg.s32SensorCnt = s32SensorCnt;

    // map bsb
    struct iav_querymem stQueryMem;
    struct iav_mem_part_info* pstPartInfo;
    stQueryMem.mid = IAV_MEM_PARTITION;
    pstPartInfo = &stQueryMem.arg.partition;
    pstPartInfo->pid = IAV_PART_BSB;
    AM_IOCTL(s32IavFD, IAV_IOC_QUERY_MEMBLOCK, &stQueryMem);
    if (pstPartInfo->mem.length == 0)
    {
        TIPC_PRINTF_ERR("IAV_PART_BSB is not allocated.");
        return -1;
    }

    g_stVencCfg.u32BsbSize = pstPartInfo->mem.length;
    g_stVencCfg.pszBsbMem = mmap(NULL, g_stVencCfg.u32BsbSize * 2, PROT_READ, MAP_SHARED, s32IavFD, pstPartInfo->mem.addr);
    if (g_stVencCfg.pszBsbMem == MAP_FAILED)
    {
        perror("mmap (%d) failed: %s\n");
        return -1;
    }

    TIPC_PRINTF_INFO("bsb_mem = %p, size = 0x%x\n", g_stVencCfg.pszBsbMem, g_stVencCfg.u32BsbSize);

    return 0;
}


/**
 * @brief 去初始化模块
 * @return 0：成功；非0：失败
 */
int amba_venc_exit()
{
    int s32Ret = 0;
    int i = 0;

    // 释放所有编码通道
    struct iav_stream_cfg stStreamCfg = {0};
    struct iav_stream_format* pstFormat = NULL;
    for (i = 0; i < IAV_STREAM_MAX_NUM_ALL; i++)
    {
        memset(&stStreamCfg, 0, sizeof(stStreamCfg));
        stStreamCfg.id = i;
        stStreamCfg.cid = IAV_STMCFG_FORMAT;
        AM_IOCTL(g_stVencCfg.s32Fd, IAV_IOC_GET_STREAM_CONFIG, &stStreamCfg);
        pstFormat = &stStreamCfg.arg.format;
        unsigned char u8CheckExist = 0;
        switch (pstFormat->type)
        {
        case IAV_STREAM_TYPE_H265:
        case IAV_STREAM_TYPE_H264:
        case IAV_STREAM_TYPE_MJPEG:
            u8CheckExist = 1;
            break;
        default:
            u8CheckExist = 0;
            break;
        }
        if (0 == u8CheckExist)
        {
            continue;
        }
        ioctl(g_stVencCfg.s32Fd, IAV_IOC_STOP_ENCODE, (1 << i));
        ioctl(g_stVencCfg.s32Fd, IAV_IOC_ABORT_ENCODE, (1 << i));
    }

    if (g_stVencCfg.pszBsbMem != NULL)
    {
        s32Ret = munmap(g_stVencCfg.pszBsbMem, g_stVencCfg.u32BsbSize);
        if (s32Ret != 0)
        {
            TIPC_PRINTF_ERR("mumap Bsb memory failed");
        }
        g_stVencCfg.pszBsbMem = NULL;
        g_stVencCfg.u32BsbSize = 0;
    }

    return 0;
}

char * amba_venc_getBsbMem (void) {
    return g_stVencCfg.pszBsbMem;
}
