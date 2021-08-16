#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "tvt_sdk_chip.h"

#include "amba_vin.h"
#include "amba_vproc.h"
#include "amba_venc.h"
#include "amba_osd.h"
#include "amba_qproi.h"
#include "amba_mask.h"
#include "amba_vout.h"
#include "amba_yuv.h"

#include "iav_ioctl.h"
#include "iav_ucode_ioctl.h"
#include "vp_config.h"

#define RUN_WITHOUT_IMAGELIB 1
#if RUN_WITHOUT_IMAGELIB
#include "amba_image.h"
#endif

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

#define JPEG_ENCODE_QUALITY 90
#define ENCODE_STREAM_BUFF_SIZE (128 * 1024 * 10 * 2) ///< 10Mbps * 2

typedef struct amba_video_cfg_s
{
    int              s32Fd;                                                     ///< 设备文件描述符
    int              s32VinNum;                                                 ///< Sensor个数
    unsigned char    u8Init;                                                    ///<
    // pthread_mutex_t  s32Mutex;                                                  ///< 互斥锁
} AMBA_VIDEO_CFG_S;

AMBA_VIDEO_CFG_S g_stVideoCfg = { -1};


int chip_video_YuvInit(void) {
    if (!g_stVideoCfg.u8Init) {
        TIPC_PRINTF_ERR ("Please Init Video First");
        return -1;
    }
    TIPC_PRINTF_CHK("amba_yuv_init");
    return amba_yuv_init(g_stVideoCfg.s32Fd, g_stVideoCfg.s32VinNum);
}

int chip_video_YuvExit(void) {
    int s32Ret;
    // Exit The YUV Part For FaceDt
    TIPC_PRINTF_CHK("amba_yuv_exit");

    s32Ret = amba_yuv_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_yuv_exit failed[%d]", s32Ret);
    }
    return s32Ret;

}

int chip_video_YuvGetCapture(const unsigned int index, CHIP_YUV_INFO_S * pstChipYuvInfo)
{
    int ret = 0;
    CHIP_YUV_INFO_S stChipYuvInfo_tmp = {0};

    AMBA_YUV_FRAMEINFO_S* yuv_frame_info = (AMBA_YUV_FRAMEINFO_S*) malloc (sizeof(AMBA_YUV_FRAMEINFO_S));
    memset (yuv_frame_info, 0, sizeof (AMBA_YUV_FRAMEINFO_S));

    ret = amba_yuv_GetFrame(index, yuv_frame_info);
    if (ret == 0) {
        stChipYuvInfo_tmp.index = index;
        stChipYuvInfo_tmp.width = yuv_frame_info->width;
        stChipYuvInfo_tmp.height = yuv_frame_info->height;
        stChipYuvInfo_tmp.pitch = yuv_frame_info->pitch;
        stChipYuvInfo_tmp.pts = yuv_frame_info->pts;
        stChipYuvInfo_tmp.y_phy_addr = yuv_frame_info->y_phy_addr;
        stChipYuvInfo_tmp.uv_phy_addr = yuv_frame_info->uv_phy_addr;

        stChipYuvInfo_tmp.y_addr = (unsigned long long)yuv_frame_info->y_vir_addr;
        stChipYuvInfo_tmp.uv_addr = (unsigned long long)yuv_frame_info->uv_vir_addr;

        memcpy (pstChipYuvInfo, &stChipYuvInfo_tmp, sizeof(CHIP_YUV_INFO_S));
    } else {
        free (yuv_frame_info);
        TIPC_PRINTF_ERR("Error, Get Yuv Capture error");
    }

    free (yuv_frame_info);
    return ret;
}

int chip_video_ReleaseYuvFrame(const unsigned int index) {
    int s32Ret;
    s32Ret = amba_yuv_ReleaseFrame(index);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_yuv_ReleaseFrame failed[%d]", s32Ret);
    }
    return s32Ret;
}

int chip_video_YuvEncJpeg(const unsigned int index, SDK_POSITION* pos, SDK_POSITION* dest_enc_pos,
                          SDK_JPEG_INFO* jpeg_info, unsigned char AllJpegFlag)
{
    int ret = 0;
    SDK_JPEG_INFO sdk_jpeg_info_temp = {0};

    AMBA_YUV_JPEGINFO_S* amba_yuv_jpeg = (AMBA_YUV_JPEGINFO_S*) malloc (sizeof(AMBA_YUV_JPEGINFO_S));
    memset (amba_yuv_jpeg, 0, sizeof(AMBA_YUV_JPEGINFO_S));

    if (AllJpegFlag) {
        // get the whole jpeg image :
        ret = amba_yuv_EncToJpeg(index, 0, 0, 1920, 1080, 1920, 1080, amba_yuv_jpeg);
        if (ret) {
            free (amba_yuv_jpeg);
            TIPC_PRINTF_ERR("amba_yuv_EncToJpeg Error!");
            return -1;
        }
        sdk_jpeg_info_temp.iJpegWidth = amba_yuv_jpeg->JpegWidth;
        sdk_jpeg_info_temp.iJpegHeight = amba_yuv_jpeg->JpegHeight;
        sdk_jpeg_info_temp.iJpegLen = amba_yuv_jpeg->JpegSize;
        sdk_jpeg_info_temp.pJpegData = amba_yuv_jpeg->JpegAddr;
    } else
    {
        // TIPC_PRINTF_CHK("AllJpegFlag == 0");
        if ((NULL == pos) || (NULL == dest_enc_pos))
        {
            free (amba_yuv_jpeg);
            TIPC_PRINTF_ERR("NULL PTR!!!");
            return -1;
        }
        ret = amba_yuv_EncToJpeg(index, pos->x, pos->y, pos->width, pos->height, \
                                 dest_enc_pos->width, dest_enc_pos->height, amba_yuv_jpeg);
        if (ret) {
            free (amba_yuv_jpeg);
            TIPC_PRINTF_ERR("amba_yuv_EncToJpeg Error!");
            return -1;
        }
        sdk_jpeg_info_temp.iJpegWidth = amba_yuv_jpeg->JpegWidth;
        sdk_jpeg_info_temp.iJpegHeight = amba_yuv_jpeg->JpegHeight;
        sdk_jpeg_info_temp.iJpegLen = amba_yuv_jpeg->JpegSize;
        sdk_jpeg_info_temp.pJpegData = amba_yuv_jpeg->JpegAddr;
    }
    memcpy (jpeg_info, &sdk_jpeg_info_temp, sizeof(SDK_JPEG_INFO));
    free (amba_yuv_jpeg);
    return 0;
}

/**
 * @brief 根据设备VinID号以及编码索引号获取Vin通道号以及码流编号
 * @return 0：成功；非0：失败
 */
int chip_video_getVencStreamID(int s32VinDevID, int s32StreamIDX, int* ps32ResChnID, int* ps32StreamID)
{
    int i = 0;
    int s32ResChnID = -1;
    if (s32StreamIDX < 0)
    {
        TIPC_PRINTF_ERR("wrong stream index[%d]!!", s32StreamIDX);
        return -1;
    }
    struct iav_system_resource stIavSysRes = {0};
    stIavSysRes.encode_mode = DSP_CURRENT_MODE;
    AM_IOCTL(g_stVideoCfg.s32Fd, IAV_IOC_GET_SYSTEM_RESOURCE, &stIavSysRes);
    for (i = 0; i < (g_stVideoCfg.s32VinNum + 1); i++)
    {
        if (stIavSysRes.chan_cfg[i].vsrc_id == s32VinDevID)
        {
            s32ResChnID = i;
            break;
        }
    }
    if (s32ResChnID < 0)
    {
        TIPC_PRINTF_ERR("check resource channel id of stream id[%d] failed!!", s32VinDevID);
        return -1;
    }
    if (ps32ResChnID != NULL)
    {
        *ps32ResChnID = s32ResChnID;
    }
    if (ps32StreamID != NULL)
    {
        if (s32StreamIDX < CHIP_VENC_H26X_NUM)
        {
            // 编码流

            *ps32StreamID = (s32ResChnID * CHIP_VENC_H26X_NUM) + s32StreamIDX;
        }
        else
        {
            // 抓拍流

            *ps32StreamID = (s32ResChnID * CHIP_MAX_VENC_NUM_PER_DEV) + s32StreamIDX;
        }
    }
    return 0;
}

/**
 * @brief 芯片视频模块视频输入初始化接口
 * @return 0：成功；非0：失败
 */
static int chip_video_vin_init(const CHIP_VIDEO_CFG_S* pstVideoCfg, int s32VinNum)
{
    int s32Ret = 0;
    int i = 0;
    if ((NULL == pstVideoCfg) || (0 == s32VinNum))
    {
        TIPC_PRINTF_ERR("video cfg param err[%d]", s32VinNum);
        return -1;
    }

    s32Ret = amba_vin_init(g_stVideoCfg.s32Fd);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_vin_init failed[%d]", s32Ret);
        return s32Ret;
    }

    for (i = 0; i < s32VinNum; i++)
    {
        s32Ret = amba_vin_dev_start(&pstVideoCfg[i]);
        if (s32Ret != 0)
        {
            TIPC_PRINTF_ERR("amba_vin_dev_start dev[%d] failed[%d]", i, s32Ret);
        }
    }

    return s32Ret;
}

/**
 * @brief 芯片视频模块视频处理初始化接口
 * @return 0：成功；非0：失败
 */
static int chip_video_vproc_init(const CHIP_VIDEO_CFG_S* pstVideoCfg, int s32VinNum)
{
    int s32Ret = 0;
    int i = 0;
    if ((NULL == pstVideoCfg) || (0 == s32VinNum))
    {
        TIPC_PRINTF_ERR("video cfg param err[%d]", s32VinNum);
        return -1;
    }
    s32Ret = amba_vproc_init(g_stVideoCfg.s32Fd);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_vproc_init failed[%d]", s32Ret);
        return s32Ret;
    }
    AMBA_VPROC_RES_CFG_S* pstVprocCfg = (AMBA_VPROC_RES_CFG_S*)malloc(sizeof(AMBA_VPROC_RES_CFG_S) * s32VinNum);
    if (NULL == pstVprocCfg)
    {
        TIPC_PRINTF_ERR("data malloc failed");
        return -1;
    }
    for (i = 0; i < s32VinNum; i++)
    {
        AMBA_VPROC_RES_CFG_S* pstVprocCfgTmp = &pstVprocCfg[i];
        const CHIP_VIDEO_CFG_S* pstChipVideoCfg = &pstVideoCfg[i];
        pstVprocCfgTmp->s32VinDevID = pstChipVideoCfg->stSensorCfg.u32VinDev;
        pstVprocCfgTmp->s32MainWidth = pstChipVideoCfg->stSensorCfg.u32MaxWidth;
        pstVprocCfgTmp->s32MainHeight = pstChipVideoCfg->stSensorCfg.u32MaxHeight;
        switch (pstChipVideoCfg->enWorkMode)
        {
        case ISP_BLC_MODE_OFF:
            pstVprocCfgTmp->enHdrMode = AMBA_VIDEO_LINEAR_MODE;
            break;
        case ISP_BLC_MODE_WDR:
            if (pstChipVideoCfg->stSensorCfg.enSensorHDRMode == TVT_SDK_PROD_SENSOR_HDR_2X)
            {
                pstVprocCfgTmp->enHdrMode = AMBA_VIDEO_2X_HDR_MODE;
            }
            else if (pstChipVideoCfg->stSensorCfg.enSensorHDRMode == TVT_SDK_PROD_SENSOR_HDR_3X)
            {
                pstVprocCfgTmp->enHdrMode = AMBA_VIDEO_3X_HDR_MODE;
            }
            break;
        default:
            pstVprocCfgTmp->enHdrMode = AMBA_VIDEO_LINEAR_MODE;
            break;

        }
    }
    s32Ret = amba_vproc_start(pstVprocCfg, s32VinNum);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_vin_dev_start dev[%d] failed[%d]", i, s32Ret);
    }

    return s32Ret;
}


/**
 * @brief 芯片视频模块视频编码初始化接口
 * @return 0：成功；非0：失败
 */
static int chip_video_venc_init(int s32VinNum)
{
    int s32Ret = 0;
    s32Ret = amba_venc_init(g_stVideoCfg.s32Fd, s32VinNum);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_venc_init failed[%d]", s32Ret);
        return s32Ret;
    }
    return s32Ret;
}

/**
 * @brief 芯片视频模块创建编码通道
 * @return 0：成功；非0：失败
 */
int chip_video_createChn(const CHIP_VIDEO_ENC_CFG_S* pstVideoEncCfg)
{
    int s32Ret = 0;
    if ((NULL == pstVideoEncCfg))
    {
        TIPC_PRINTF_ERR("video cfg param err");
        return -1;
    }
    // 先关闭通道
    s32Ret = amba_venc_destroyChn(pstVideoEncCfg->s32VinDevID, pstVideoEncCfg->s32StreamIDX);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("destroy enc channel failed[%d]", s32Ret);
    }
    // 重新分配Vproc资源
    AMBA_VPROC_CHN_CFG_S stVprocChnCfg = {0};
    stVprocChnCfg.s32VinDevID = pstVideoEncCfg->s32VinDevID;
    stVprocChnCfg.s32StreamIDX = pstVideoEncCfg->s32StreamIDX;
    stVprocChnCfg.s32Width = pstVideoEncCfg->u32Width;
    stVprocChnCfg.s32Height = pstVideoEncCfg->u32Height;
    stVprocChnCfg.s32Fps = pstVideoEncCfg->u32FrameRate;
    s32Ret = amba_vproc_chnCfg(&stVprocChnCfg);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_vproc_chnCfg failed[%d]", s32Ret);
        return s32Ret;
    }

    // 再创建编码通道
    AMBA_VENC_CHN_CFG_S stVencChnCfg = {0};
    stVencChnCfg.s32VinDevID = pstVideoEncCfg->s32VinDevID;
    stVencChnCfg.s32StreamIDX = pstVideoEncCfg->s32StreamIDX;
    stVencChnCfg.s32Width = pstVideoEncCfg->u32Width;
    stVencChnCfg.s32Height = pstVideoEncCfg->u32Height;
    stVencChnCfg.s32Fps = (int)pstVideoEncCfg->u32FrameRate;
    switch (pstVideoEncCfg->enEncType)
    {
    /// TODO:
    case SDK_VENC_TYPE_H264:
    case SDK_VENC_TYPE_H264_PLUS:
    case SDK_VENC_TYPE_H264_SMART:
        stVencChnCfg.enStreamType = IAV_STREAM_TYPE_H264;
        break;
    case SDK_VENC_TYPE_H265:
    case SDK_VENC_TYPE_H265_PLUS:
    case SDK_VENC_TYPE_H265_SMART:
        stVencChnCfg.enStreamType = IAV_STREAM_TYPE_H265;
        break;
    case SDK_VENC_TYPE_JPEG:
    case SDK_VENC_TYPE_MJPEG:
        stVencChnCfg.enStreamType = IAV_STREAM_TYPE_MJPEG;
        break;
    default:
        TIPC_PRINTF_ERR("err encode format[%d]", pstVideoEncCfg->enEncType);
        return -1;
        break;
    }
    switch (stVencChnCfg.enStreamType)
    {
    case IAV_STREAM_TYPE_H264:
    case IAV_STREAM_TYPE_H265:
        stVencChnCfg.stH26xCfg.s32BitRate = pstVideoEncCfg->u32BitRate;
        stVencChnCfg.stH26xCfg.s32BitRateCtrl = (pstVideoEncCfg->enBitRateMode == SDK_VENC_MODE_CBR) ? IAV_BRC_CBR : IAV_BRC_VBR;
        stVencChnCfg.stH26xCfg.s32Gop = pstVideoEncCfg->u32GOP;
        stVencChnCfg.stH26xCfg.s32Profile = pstVideoEncCfg->u32Profile;
        break;
    case IAV_STREAM_TYPE_MJPEG:
        stVencChnCfg.stMjpegCfg.s32Quality = JPEG_ENCODE_QUALITY;
        break;
    default:
        TIPC_PRINTF_ERR("err encode format[%d]", pstVideoEncCfg->enEncType);
        return -1;
        break;
    }
    s32Ret = amba_venc_createChn(&stVencChnCfg);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_venc_createChn failed[%d]", s32Ret);
    }

    return s32Ret;
}

/**
 * @brief 芯片视频模块获取编码通道配置
 * @return 0：成功；非0：失败
 */
int chip_video_getChnCfg(CHIP_VIDEO_ENC_CFG_S* pstVideoEncCfg)
{
    int s32Ret = 0;
    if ((NULL == pstVideoEncCfg))
    {
        TIPC_PRINTF_ERR("video cfg param err");
        return -1;
    }

    AMBA_VENC_CHN_CFG_S stVencChnCfg = {0};
    stVencChnCfg.s32VinDevID = pstVideoEncCfg->s32VinDevID;
    stVencChnCfg.s32StreamIDX = pstVideoEncCfg->s32StreamIDX;
    s32Ret = amba_venc_getChnCfg(&stVencChnCfg);
    if (s32Ret != 0)
    {
        return -1;
    }
    pstVideoEncCfg->u32Width = (unsigned int)stVencChnCfg.s32Width;
    pstVideoEncCfg->u32Height = (unsigned int)stVencChnCfg.s32Width;
    /// TODO: 暂时只填充分辨率
    return 0;
}

/**
 * @brief 芯片视频模块销毁编码通道
 * @return 0：成功；非0：失败
 */
int chip_video_destroyChn(int s32VinDevID, int s32StreamIDX)
{
    int s32Ret = 0;
    // 直接关闭编码通道
    s32Ret = amba_venc_destroyChn(s32VinDevID, s32StreamIDX);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("destroy enc channel failed[%d]", s32Ret);
    }
    return s32Ret;
}


/**
 * @brief 芯片视频模块获取编码通道数据
 * @return 0：成功；非0：失败
 */
int chip_video_getChnStream(CHIP_VIDEO_ENC_STREAM_S* pstEncStream)
{
    int s32Ret = 0;
    int i = 0;
    char* pszVencBuf = NULL;
    int s32VencBufSize = 0;
    if (NULL == pstEncStream)
    {
        TIPC_PRINTF_ERR("NULL pointer!!");
        return -1;
    }
    if (NULL == pstEncStream->pstFrameInfo)
    {
        TIPC_PRINTF_ERR("NULL pointer!!");
        return -1;
    }

    // 注意：此处需要保证编码帧结构体已分配缓存，因为此层代码不带缓存处理等逻辑
    if (NULL == pstEncStream->pstFrameInfo->pData)
    {
        TIPC_PRINTF_ERR("frame info buff is NULL!!");
        return -1;
    }
    pszVencBuf = (char*)pstEncStream->pstFrameInfo->pData;
    s32VencBufSize = pstEncStream->pstFrameInfo->length;

    // 获取编码帧
    struct iav_framedesc stFrameDesc = {0};
    char* pszStreamBuffer = NULL;
    s32Ret = amba_venc_getChnStream(pstEncStream->s32VinDevID, pstEncStream->s32StreamIDX, &stFrameDesc, &pszStreamBuffer);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get venc Chn stream failed[%d]!!", s32Ret);
        return -1;
    }
    // 赋值

    // 编码类型
    switch (stFrameDesc.stream_type)
    {
    case IAV_STREAM_TYPE_H264:
        pstEncStream->pstFrameInfo->ucEncodeType = IPC_VIDEO_ENCODE_TYPE_H264;
        pstEncStream->pstFrameInfo->ucFrameType = IPC_FRAME_TYPE_VIDEO;
        break;
    case IAV_STREAM_TYPE_H265:
        pstEncStream->pstFrameInfo->ucEncodeType = IPC_VIDEO_ENCODE_TYPE_H265;
        pstEncStream->pstFrameInfo->ucFrameType = IPC_FRAME_TYPE_VIDEO;
        break;
    case IAV_STREAM_TYPE_MJPEG:
        pstEncStream->pstFrameInfo->ucEncodeType = IPC_VIDEO_ENCODE_TYPE_MJPEG;
        pstEncStream->pstFrameInfo->ucFrameType = IPC_FRAME_TYPE_PIC;
        break;
    default:
        pstEncStream->pstFrameInfo->ucEncodeType = IPC_VIDEO_ENCODE_TYPE_H264;
        pstEncStream->pstFrameInfo->ucFrameType = IPC_FRAME_TYPE_VIDEO;
        TIPC_PRINTF_CHK("warning unknown stream type[%d]!!", stFrameDesc.stream_type);
        break;
    }

    // TIPC_PRINTF_CHK("Current Stream Frame Type:%d" , stFrameDesc.pic_type);

    // 帧类型
    if ((IAV_PIC_TYPE_I_FRAME == stFrameDesc.pic_type) || (IAV_PIC_TYPE_IDR_FRAME == stFrameDesc.pic_type))
    {
        pstEncStream->pstFrameInfo->ucKeyFrame = 1;
    }
    else
    {
        pstEncStream->pstFrameInfo->ucKeyFrame = 0;
    }

    if ((1 == pstEncStream->u8RequestIDR) && (0 == pstEncStream->pstFrameInfo->ucKeyFrame))
    {
        // 请求I帧但是获取到的不是I帧
        TIPC_PRINTF_CHK("Request IDR Again!!");
        s32Ret = -1;
        goto STREAM_RELEASE;
    }

    // 帧序号
    pstEncStream->pstFrameInfo->frameIndex = (unsigned int)stFrameDesc.frame_num;
    // 宽高
    pstEncStream->pstFrameInfo->width = stFrameDesc.reso.width;
    pstEncStream->pstFrameInfo->height = stFrameDesc.reso.height;
    // 时间戳
    pstEncStream->pstFrameInfo->relativeTime = stFrameDesc.arm_pts; // TODO:注意时间戳
    // Buffer
    if (pszStreamBuffer != NULL && pszVencBuf != NULL)
    {
        if (stFrameDesc.size < s32VencBufSize)
        {
            pstEncStream->pstFrameInfo->length = stFrameDesc.size;
            memcpy(pszVencBuf, pszStreamBuffer, stFrameDesc.size);
        }
        else
        {
            TIPC_PRINTF_ERR("stream buffer[%d] is larger than malloc buffer[%d]!!", stFrameDesc.size, pszVencBuf);
            pstEncStream->pstFrameInfo->length = 0;
            // 放弃此帧，请求下一帧I帧
            amba_venc_releaseChnStream(&stFrameDesc);
            chip_video_requestIDRStream(pstEncStream->s32VinDevID, pstEncStream->s32StreamIDX);
            return -1;
        }
    }
    else
    {
        TIPC_PRINTF_ERR("Stream Buffer of %s is NULL", (pszStreamBuffer == NULL) ? "Venc" : "OS");
        pstEncStream->pstFrameInfo->length = 0;
    }
    // TIPC_PRINTF_CHK("slice_num : %d" , stFrameDesc.slice_num);
    //
    if (pstEncStream->ps32SliceBuf != NULL)
    {
        memset(pstEncStream->ps32SliceBuf, 0, (sizeof(int)*pstEncStream->s32SliceBufLen));
        /// TODO: stFrameDesc.slice_num目前都是为1，如果是多个slice_num的话，需要我们遍历
        /// amba:Rock说:对于4K或者比较大的分辨率，Slice一般设置为3，这样解码端收到1/3个I帧就开始解码，节省时间
        for (i = 0; i < stFrameDesc.slice_num; i++)
        {
            /// TODO: 多个需要遍历
            pstEncStream->ps32SliceBuf[i] = stFrameDesc.size;
        }
    }


STREAM_RELEASE:
    // 释放编码帧
    amba_venc_releaseChnStream(&stFrameDesc);

    return s32Ret;
}


/**
 * @brief 芯片视频模块请求IDR编码数据
 * @return 0：成功；非0：失败
 */
int chip_video_requestIDRStream(int s32VinDevID, int s32StreamIDX)
{
    int s32Ret = 0;
    s32Ret = amba_venc_requestIDRStream(s32VinDevID, s32StreamIDX);
    return s32Ret;
}

/**
 * @brief 芯片视频模块使能编码通道
 * @return 0：成功；非0：失败
 */
int chip_video_startStream(int s32VinDevID, int s32StreamIDX)
{
    int s32Ret = 0;
    s32Ret = amba_venc_startStream(s32VinDevID, s32StreamIDX);
    return s32Ret;
}

/**
 * @brief 芯片视频模块设置Slice分片
 * @return 0：成功；非0：失败
 */
int chip_video_setEncodeSlice(int s32Enable)
{
    // not support yet
    return 0;
}

/**
 * @brief 芯片视频模块设置OSD
 * @return 0：成功；非0：失败
 */
int chip_video_setOSD(CHIP_OSD_INFO_S* pstOSDInfo)
{
    int s32Ret = 0;
    s32Ret = amba_osd_setOverLay(pstOSDInfo);
    return s32Ret;
}

/**
 * @brief 芯片视频模块设置Mask
 * @return 0：成功；非0：失败
 */
int chip_video_setMask(CHIP_MASK_INFO_S* pstMaskInfo)
{
    int s32Ret = 0;
    s32Ret = amba_mask_setPos(pstMaskInfo);
    return s32Ret;
}

/**
 * @brief 芯片视频模块设置ROI
 * @return 0：成功；非0：失败
 */
int chip_video_setROI(CHIP_ROI_INFO_S* pstROIInfo)
{
    int s32Ret = 0;
    s32Ret = amba_roi_setPos(pstROIInfo);
    return s32Ret;
}

/**
 * @brief 芯片视频模块设置镜像
 * @return 0：成功；非0：失败
 */
int chip_video_setMirror(int s32VinDevID, int s32Enable)
{
    int s32Ret = 0;
    s32Ret = amba_vin_dev_setMirror(s32VinDevID, s32Enable);
    return s32Ret;
}

/**
 * @brief 芯片视频模块设置翻转
 * @return 0：成功；非0：失败
 */
int chip_video_setFlip(int s32VinDevID, int s32Enable)
{
    int s32Ret = 0;
    s32Ret = amba_vin_dev_setFlip(s32VinDevID, s32Enable);
    return s32Ret;
}

/**
 * @brief 芯片视频模块获取视频输入配置
 * @return 0：成功；非0：失败
 */
int chip_video_getVinCfg(CHIP_VIDEO_CFG_S* pstVinCfg)
{
    int s32Ret = 0;
    s32Ret = amba_vproc_getSourceCfg(pstVinCfg);
    return s32Ret;
}

/**
 * @brief 芯片视频模块初始化接口
 * @return 0：成功；非0：失败
 */
int chip_video_init(const CHIP_VIDEO_CFG_S* pstVideoCfg, int s32VinNum)
{
    int s32Ret = 0;
    if ((NULL == pstVideoCfg) || (0 == s32VinNum))
    {
        TIPC_PRINTF_ERR("video cfg param err[%d]", s32VinNum);
        return -1;
    }


    //WKL update: For Function param test
    g_stVideoCfg.u8Init = 0;

    // 打开iav模块
    if (g_stVideoCfg.s32Fd < 0)
    {
        g_stVideoCfg.s32Fd = open("/dev/iav", O_RDWR, 0);
        if (g_stVideoCfg.s32Fd < 0)
        {
            perror("/dev/iav");
            TIPC_PRINTF_ERR("/dev/iav open failed[%d]", g_stVideoCfg.s32Fd);
            return -1;
        }
    }

#if RUN_WITHOUT_IMAGELIB
    // 初始化图像
    s32Ret = amba_image_init(g_stVideoCfg.s32Fd);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_image_init failed");
        return -1;
    }
#endif

    // 配置Sensor
    s32Ret = chip_video_vin_init(pstVideoCfg, s32VinNum);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("chip_video_vi_init failed[%d]", s32Ret);
        return s32Ret;
    }

    // 设置IAV为IDLE状态
    TIPC_PRINTF_INFO("set iav status IAV_IOC_ENTER_IDLE");
    struct iav_idle_params stIdleParams = {0};
    stIdleParams.poweroff_vin = 1;
    AM_IOCTL(g_stVideoCfg.s32Fd, IAV_IOC_ENTER_IDLE, &stIdleParams);

    // 设置pyramid，算法可能会用到
    struct iav_pyramid_cfg stPyramidCfg;
    stPyramidCfg.chan_id = 0;
    AM_IOCTL(g_stVideoCfg.s32Fd, IAV_IOC_GET_PYRAMID_CFG, &stPyramidCfg);
    /// TODO:先关闭
    stPyramidCfg.enable = 0;
    AM_IOCTL(g_stVideoCfg.s32Fd, IAV_IOC_SET_PYRAMID_CFG, &stPyramidCfg);


    // 配置视频处理资源
    s32Ret = chip_video_vproc_init(pstVideoCfg, s32VinNum);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("chip_video_vproc_init failed[%d]", s32Ret);
        return s32Ret;
    }

    // 初始化编码模块
    s32Ret = chip_video_venc_init(s32VinNum);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("chip_video_venc_init failed[%d]", s32Ret);
        return s32Ret;
    }

    // 设置IAV为PREVIEW状态
    TIPC_PRINTF_INFO("set iav status IAV_IOC_ENABLE_PREVIEW");
    AM_IOCTL(g_stVideoCfg.s32Fd, IAV_IOC_ENABLE_PREVIEW, 0);

    // 初始化VO模块
    CHIP_VIDEO_FORMAT_E enFormat = (SDK_VIDEO_INPUT_PAL == pstVideoCfg->enFormat) ? CHIP_VIDEO_FORMAT_PAL : CHIP_VIDEO_FORMAT_NTSC;
    s32Ret = amba_vout_init(g_stVideoCfg.s32Fd, enFormat);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_vout_init failed[%d]", s32Ret);
        return s32Ret;
    }

    // 初始化OSD模块
    s32Ret = amba_osd_init(g_stVideoCfg.s32Fd, s32VinNum);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_osd_init failed[%d]", s32Ret);
        return s32Ret;
    }

    // 初始化MASK模块
    s32Ret = amba_mask_init(g_stVideoCfg.s32Fd, s32VinNum);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_mask_init failed[%d]", s32Ret);
        return s32Ret;
    }

    // 初始化ROI模块
    s32Ret = amba_roi_init(g_stVideoCfg.s32Fd, s32VinNum);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_mask_init failed[%d]", s32Ret);
        return s32Ret;
    }

    // // Init The YUV Part For FaceDt
    // s32Ret = amba_yuv_init(g_stVideoCfg.s32Fd, s32VinNum);
    // if (s32Ret != 0)
    // {
    //     TIPC_PRINTF_ERR("amba_yuv_init failed[%d]", s32Ret);
    //     return s32Ret;
    // }


    g_stVideoCfg.s32VinNum = s32VinNum;

    //WKL update: For Function param test
    g_stVideoCfg.u8Init = 1;

    return 0;
}


/**
 * @brief 芯片视频模块初始化接口
 * @return 0：成功；非0：失败
 */
int chip_video_exit()
{
    int s32Ret = 0;

    if (g_stVideoCfg.s32Fd <= 0)
    {
        TIPC_PRINTF_ERR("/dev/iav is not opened");
        g_stVideoCfg.s32Fd = -1;
        return s32Ret;
    }


    // 释放ROI模块
    TIPC_PRINTF_CHK("amba_roi_exit");
    s32Ret = amba_roi_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_roi_exit failed[%d]", s32Ret);
    }

    // 释放MASK模块
    TIPC_PRINTF_CHK("amba_mask_exit");
    s32Ret = amba_mask_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_mask_exit failed[%d]", s32Ret);
    }

    // 释放OSD模块
    TIPC_PRINTF_CHK("amba_osd_exit");
    s32Ret = amba_osd_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_osd_exit failed[%d]", s32Ret);
    }

    // 释放编码模块
    TIPC_PRINTF_CHK("amba_venc_exit");
    s32Ret = amba_venc_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_venc_exit failed[%d]", s32Ret);
    }

    // 释放VO模块
    s32Ret = amba_vout_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_vout_exit failed[%d]", s32Ret);
    }

    // 释放Vproc模块
    TIPC_PRINTF_CHK("amba_vproc_exit");
    s32Ret = amba_vproc_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_vproc_exit failed[%d]", s32Ret);
    }
    // 释放Vin模块
    TIPC_PRINTF_CHK("amba_vin_exit");
    s32Ret = amba_vin_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_vin_exit failed[%d]", s32Ret);
    }

#if RUN_WITHOUT_IMAGELIB
    // 释放Image模块
    TIPC_PRINTF_CHK("amba_image_exit");
    s32Ret = amba_image_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("amba_image_exit failed[%d]", s32Ret);
    }
#endif

    // 设置IAV为IDLE状态
    TIPC_PRINTF_CHK("set idle");
    struct iav_idle_params stIdleParams = {0};
    stIdleParams.poweroff_vin = 1;
    ioctl(g_stVideoCfg.s32Fd, IAV_IOC_ENTER_IDLE, &stIdleParams);

    // Reset iav
    TIPC_PRINTF_CHK("reset iav");
    // ioctl(g_stVideoCfg.s32Fd, IAV_IOC_RESET);

    TIPC_PRINTF_CHK("close iav");
    close(g_stVideoCfg.s32Fd);
    g_stVideoCfg.s32Fd = -1;
    printf("bye bye\n");

    return 0;
}