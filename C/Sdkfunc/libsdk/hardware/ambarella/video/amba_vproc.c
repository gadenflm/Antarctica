
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "amba_vproc.h"
#include "amba_vout.h"


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

extern int chip_video_getVencStreamID(int s32VinDevID, int s32StreamIDX, int* ps32ResChnID, int* ps32StreamID);

#define AMBA_RES_PASSNUM 2
#define AMBA_RES_H26X_PASSID 0
#define AMBA_RES_JPEG_PASSID 1

typedef struct amba_vproc_cfg_s
{
    int              s32Fd;          ///< �豸�ļ�������
    int              s32SensorCnt;   ///< sensor����
    unsigned char    u8Init;         ///< ��ʼ����־
    // pthread_mutex_t  s32Mutex;       ///< ������
} AMBA_VPROC_CFG_S;

static AMBA_VPROC_CFG_S g_stVprocCfg = {0};

/**
 * @brief ����Stream indexȷ��Buffer ID
 * @return 0���ɹ�����0��ʧ��
 */
static int amba_vproc_findBufferID(const int s32StreamIDX, int* ps32BufferID, int* ps32PassID)
{
    int s32Ret = 0;
    int s32BufferID = 0;
    int s32PassID = 0;
    if (s32StreamIDX < 0)
    {
        TIPC_PRINTF_ERR("wrong stream index[%d]!!", s32StreamIDX);
        return -1;
    }
    s32PassID = (s32StreamIDX < CHIP_VENC_H26X_NUM) ? 0 : 1;
    switch (s32StreamIDX)
    {
    case CHIP_VIDEO_ENC_MAIN_STREAM:
    case CHIP_VIDEO_ENC_JPEG_STREAM:
        s32BufferID = IAV_SRCBUF_3;
        break;
    case CHIP_VIDEO_ENC_SECOND_STREAM:
    case CHIP_VIDEO_ENC_JPEG_ALARM_STREAM:
        s32BufferID = IAV_SRCBUF_4;
        break;
    case CHIP_VIDEO_ENC_THIRD_STREAM:
        s32BufferID = IAV_SRCBUF_2;
        break;
    default:
        TIPC_PRINTF_ERR("wrong stream index[%d]!!", s32StreamIDX);
        s32Ret = -1;
        break;
    }
    if (0 == s32Ret)
    {
        if (ps32BufferID != NULL)
        {
            *ps32BufferID = s32BufferID;
        }
        if (ps32PassID != NULL)
        {
            *ps32PassID = s32PassID;
        }
    }
    return s32Ret;
}

/**
 * @brief ��Ƶ����ģ��ͨ������
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vproc_chnCfg(const AMBA_VPROC_CHN_CFG_S* pstVprocChnCfg)
{
    int s32Ret = 0;
    if (g_stVprocCfg.s32Fd <= 0)
    {
        TIPC_PRINTF_ERR("/dev/iav check err! [%d]", g_stVprocCfg.s32Fd);
        return -1;
    }
    if (NULL == pstVprocChnCfg)
    {
        TIPC_PRINTF_ERR("NULL pointer!!");
        return -1;
    }

    // Ѱ��Sensor dev id��Ӧ��resourceͨ�����Լ�����ͨ����
    int s32StreamID = -1, s32ResChnID = -1;
    s32Ret = chip_video_getVencStreamID(pstVprocChnCfg->s32VinDevID, pstVprocChnCfg->s32StreamIDX, &s32ResChnID, &s32StreamID);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get stream id failed!!");
        return -1;
    }

    // ��ȡPassID�Լ�BuffID
    int s32PassID = 0;
    int s32BuffID = 0;
    s32Ret = amba_vproc_findBufferID(pstVprocChnCfg->s32StreamIDX, &s32BuffID, &s32PassID);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("check bufferid failed!!");
        return -1;
    }

    // ���ݴ�����ͨ����������buffer
    struct iav_video_proc stIavVproc = {0};
    struct iav_dptz* pstIavDptz = &stIavVproc.arg.dptz;
    struct iav_apply_flag stApplyFlag[IAV_VIDEO_PROC_NUM] = {0};
    stIavVproc.cid = IAV_VIDEO_PROC_DPTZ;
    pstIavDptz->channel_id = s32ResChnID;
    pstIavDptz->pass_id = s32PassID;
    pstIavDptz->buf_id = s32BuffID;

    AM_IOCTL(g_stVprocCfg.s32Fd, IAV_IOC_GET_VIDEO_PROC, &stIavVproc);

    // ���·ֱ���
    pstIavDptz->buf_cfg.output.width = pstVprocChnCfg->s32Width;
    pstIavDptz->buf_cfg.output.height = pstVprocChnCfg->s32Height;
    // TIPC_PRINTF_CHK("[****** WKL ******]buffer %d: %dx%d", pstIavDptz->buf_id, pstIavDptz->buf_cfg.output.width, pstIavDptz->buf_cfg.output.height);
    AM_IOCTL(g_stVprocCfg.s32Fd, IAV_IOC_CFG_VIDEO_PROC, &stIavVproc);

    // ����֡��
    memset(&stIavVproc , 0, sizeof(stIavVproc));
    stIavVproc.cid = IAV_VIDEO_PROC_CANVAS_FPS;
    stIavVproc.arg.canvas_fps.id = (s32StreamID + 1); // canvas_id = streamID + 1
    stIavVproc.arg.canvas_fps.frame_rate = pstVprocChnCfg->s32Fps;
    AM_IOCTL(g_stVprocCfg.s32Fd, IAV_IOC_CFG_VIDEO_PROC, &stIavVproc);

    stApplyFlag[IAV_VIDEO_PROC_DPTZ].param = (1 << s32ResChnID);
    stApplyFlag[IAV_VIDEO_PROC_DPTZ].apply = 1;
    AM_IOCTL(g_stVprocCfg.s32Fd, IAV_IOC_APPLY_VIDEO_PROC, stApplyFlag);

    return s32Ret;
}

//  ��Sensor����Ϊ2, PipeLine����������
//   vin    channelID   passID   bufferID   canvasID   streamID   streamIDX
//    0         0          0         0          0           x         x
//    0         0          0         1          3           2         2       (��������)
//    0         0          0         2          1           0         0       (������)
//    0         0          0         3          2           1         1       (�ڶ�����)
//    0         0          1         2          7           6         3       (ץ������)
//    0         0          1         3          8           7         4       (�澯����)
//    1         1          0         0          ?           x         x       canvasID = vinnum * 5 + vin
//    1         1          0         1          6           5         2       (��������)
//    1         1          0         2          4           3         0       (������)
//    1         1          0         3          5           4         1       (�ڶ�����)
//    1         1          1         2          9           8         3       (ץ������)
//    1         1          1         3          10          9         4       (�澯����)
//  canvasID = streamID + 1
//  bufferID and passID �������streamIDXһһ��Ӧ
//  h26x: streamID = (vin * 3) + streamIDX;
//  jpeg: streamID = (vin * 2) + streamIDX + (vin * 3);
//                 = (vin * 5) + streamIDX;


/**
 * @brief ��Ƶ����ģ���ʼ��(����iav system resource)
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vproc_start(const AMBA_VPROC_RES_CFG_S* pstVprocCfg, int s32VinNum)
{
    int s32Ret = 0;
    int i = 0, j = 0;
    if (g_stVprocCfg.s32Fd <= 0)
    {
        TIPC_PRINTF_ERR("/dev/iav check err! [%d]", g_stVprocCfg.s32Fd);
        return -1;
    }

    if ((NULL == pstVprocCfg) || (0 == s32VinNum))
    {
        TIPC_PRINTF_ERR("NULL pointer ? vin num [%d]!!", s32VinNum);
        return -1;
    }

    g_stVprocCfg.s32SensorCnt = s32VinNum;

    // ����ϵͳ��Դ����Ҫ��IAVģ�鴦��idle�½���
    struct iav_system_resource stIavSysRes = {0};
    stIavSysRes.encode_mode = DSP_CURRENT_MODE;
    AM_IOCTL(g_stVprocCfg.s32Fd, IAV_IOC_GET_SYSTEM_RESOURCE, &stIavSysRes);
    stIavSysRes.chan_num = g_stVprocCfg.s32SensorCnt;                       // ��ͬ��Sensor����
    for (i = 0; i < s32VinNum; i++)
    {
        const AMBA_VPROC_RES_CFG_S* pstVprocCfgTmp = &pstVprocCfg[i];
        stIavSysRes.chan_cfg[i].idsp_fps = 0;                                   // Default is 0(equals to VIN FPS)
        stIavSysRes.chan_cfg[i].expo_num = (pstVprocCfgTmp->enHdrMode + 1);
        stIavSysRes.chan_cfg[i].vsrc_id = pstVprocCfgTmp->s32VinDevID;
        stIavSysRes.chan_cfg[i].sensor_ctrl = 1;
        stIavSysRes.chan_cfg[i].mctf_cmpr = 1; // 3D NR����
        stIavSysRes.chan_cfg[i].main_burst_tile = 1;
        stIavSysRes.chan_cfg[i].pass_num = AMBA_RES_PASSNUM;
        stIavSysRes.chan_cfg[i].input.width = pstVprocCfgTmp->s32MainWidth;
        stIavSysRes.chan_cfg[i].input.height = pstVprocCfgTmp->s32MainHeight;
        stIavSysRes.chan_cfg[i].extra_downscale_enable = 1; // Enable extra downscale if downscale >= 6X
        stIavSysRes.chan_cfg[i].buf_max[AMBA_RES_H26X_PASSID][IAV_SRCBUF_1].width = pstVprocCfgTmp->s32MainWidth; // main buffer, cannot change
        stIavSysRes.chan_cfg[i].buf_max[AMBA_RES_H26X_PASSID][IAV_SRCBUF_2].width = pstVprocCfgTmp->s32MainWidth; // main stream
        stIavSysRes.chan_cfg[i].buf_max[AMBA_RES_H26X_PASSID][IAV_SRCBUF_3].width = pstVprocCfgTmp->s32MainWidth; // second stream
        stIavSysRes.chan_cfg[i].buf_max[AMBA_RES_H26X_PASSID][IAV_SRCBUF_4].width = pstVprocCfgTmp->s32MainWidth; // third stream
        stIavSysRes.chan_cfg[i].buf_max[AMBA_RES_JPEG_PASSID][IAV_SRCBUF_3].width = pstVprocCfgTmp->s32MainWidth; // jpeg stream
        stIavSysRes.chan_cfg[i].buf_max[AMBA_RES_JPEG_PASSID][IAV_SRCBUF_4].width = pstVprocCfgTmp->s32MainWidth; // alarm jpeg stream
        // update:WKL
        // Yuv
        stIavSysRes.chan_cfg[i].buf_max[AMBA_RES_H26X_PASSID][IAV_SRCBUF_5].width = pstVprocCfgTmp->s32MainWidth; // YUV data
    }

    stIavSysRes.max_stream_num = CHIP_MAX_VENC_NUM_PER_DEV * s32VinNum;
    for (i = 0; i < stIavSysRes.max_stream_num; i++)
    {
        int s32Index = i / CHIP_MAX_VENC_NUM_PER_DEV;
        const AMBA_VPROC_RES_CFG_S* pstVprocCfgTmp = &pstVprocCfg[s32Index];
        // ��������ģʽ
        int s32MaxSize = (pstVprocCfgTmp->s32MainWidth > pstVprocCfgTmp->s32MainHeight) ? pstVprocCfgTmp->s32MainWidth : pstVprocCfgTmp->s32MainHeight;
        stIavSysRes.stream[i].max_size.width = s32MaxSize;
        stIavSysRes.stream[i].max_size.height = s32MaxSize;
        stIavSysRes.stream[i].max_M = 1;
    }

    // update:WKL
    // Yuv Stream, Set to efm (encode from memory) mode.
    int YuvStreamId = stIavSysRes.max_stream_num;
    stIavSysRes.max_stream_num += 1;
    stIavSysRes.stream[YuvStreamId].max_size.width = 1920;
    stIavSysRes.stream[YuvStreamId].max_size.height = 1080;
    stIavSysRes.stream[YuvStreamId].max_M = 1;
    stIavSysRes.stream[YuvStreamId].efm_enable = 1;             //enable the efm mode
    stIavSysRes.stream[YuvStreamId].efm_buf_num = 5;           //set max buffer cached number 8 will error ?? TODO WHY ?
    stIavSysRes.stream[YuvStreamId].efm_size.width = 1920;
    stIavSysRes.stream[YuvStreamId].efm_size.height = 1080;
    stIavSysRes.stream[YuvStreamId].efm_skip_waiting = 1;       //skip waiting for encoding start/stop status

    stIavSysRes.canvas_num = (CHIP_MAX_VPROC_NUM_PER_DEV + 1) * s32VinNum; // ÿ��sensor���main buffer
    /// TODO: ��Ҫע��canvas��������оƬ������Ƶ����
    for (i = 0; i < stIavSysRes.canvas_num; i++)
    {
        int s32Index = i / (CHIP_MAX_VPROC_NUM_PER_DEV + 1);
        const AMBA_VPROC_RES_CFG_S* pstVprocCfgTmp = &pstVprocCfg[s32Index];
        stIavSysRes.canvas_cfg[i].type = IAV_CANVAS_TYPE_ENCODE;
        stIavSysRes.canvas_cfg[i].max.width = pstVprocCfgTmp->s32MainWidth;
        stIavSysRes.canvas_cfg[i].max.height = pstVprocCfgTmp->s32MainHeight;
        // stIavSysRes.canvas_cfg[i].extra_dram_buf = 0;
        stIavSysRes.canvas_cfg[i].cached_items = 1;
    }

    // for Vout
    int s32VoutCanvasID = stIavSysRes.canvas_num;
    stIavSysRes.canvas_num += 1;
    stIavSysRes.canvas_cfg[s32VoutCanvasID].type = IAV_CANVAS_TYPE_PREVIEW;
    stIavSysRes.canvas_cfg[s32VoutCanvasID].vout_id = AMBA_VOUT_ID;
    stIavSysRes.canvas_cfg[s32VoutCanvasID].vout_YUV422 = 0; // Specify VOUT YUV data format. 0: YUV420; 1: YUV422
    stIavSysRes.canvas_cfg[s32VoutCanvasID].max.width = VO_CVBS_MAX_WIDTH;
    stIavSysRes.canvas_cfg[s32VoutCanvasID].max.height = VO_CVBS_MAX_HEIGHT;

    // Update : WKL
    // for Face Detection YUV data
    int s32YuvCanvasID = stIavSysRes.canvas_num;
    stIavSysRes.canvas_num += 1;
    stIavSysRes.canvas_cfg[s32YuvCanvasID].type = IAV_CANVAS_TYPE_ENCODE;
    stIavSysRes.canvas_cfg[s32YuvCanvasID].max.width = 1920; // only for 400M YUV Face dect
    stIavSysRes.canvas_cfg[s32YuvCanvasID].max.height = 1080; // only for 400M YUV Face dect
    stIavSysRes.canvas_cfg[s32YuvCanvasID].manual_feed = 1; // key to control mannually
    stIavSysRes.canvas_cfg[s32YuvCanvasID].feed_frame_num = 10; // can cached 10 frame totally, this value maximum is 12.
    stIavSysRes.canvas_cfg[s32YuvCanvasID].cached_items = 1;

#if WKL_DEBUG
    TIPC_PRINTF_CHK("[****** WKL ******]s32YuvCanvasID:%d", s32YuvCanvasID);
#endif
    AM_IOCTL(g_stVprocCfg.s32Fd, IAV_IOC_SET_SYSTEM_RESOURCE, &stIavSysRes);

    // ��ʼ����������ͨ����Buffer
    for (i = 0; i < s32VinNum; i++)
    {
        const AMBA_VPROC_RES_CFG_S* pstVprocCfgTmp = &pstVprocCfg[i];
        int s32StreamIDX = CHIP_VIDEO_ENC_MAIN_STREAM;
        // ���ȴ���pass id 0, ��������������
        for (j = IAV_SRCBUF_FIRST; j < IAV_SUB_SRCBUF_LAST; j++)
        {
            struct iav_video_proc stIavVproc = {0};
            struct iav_dptz* pstIavDptz = &stIavVproc.arg.dptz;
            stIavVproc.cid = IAV_VIDEO_PROC_DPTZ;
            pstIavDptz->channel_id = i;                     // --> stIavSysRes.chan_cfg
            pstIavDptz->pass_id = AMBA_RES_H26X_PASSID;     // --> pass_num
            pstIavDptz->buf_id = j;                         // --> stIavSysRes.chan_cfg[i].buf_max
            pstIavDptz->buf_cfg.input.width = 0;
            pstIavDptz->buf_cfg.input.height = 0;
            switch (j)
            {
            case IAV_SRCBUF_FIRST:
                // main buffer
                // ��Buffer�������ڼ䲻�����޸ķֱ��ʣ�����ⲿ�ֲ���������
                // �ο�Doxygen����
                if (0 == i)
                {
                    pstIavDptz->buf_cfg.canvas_id = 0;
                }
                else
                {
                    // vinnum * 5 + vin
                    /// CV2X Canvas����12��
                    pstIavDptz->buf_cfg.canvas_id = (s32VinNum * CHIP_MAX_VENC_NUM_PER_DEV) + i;
                }
                pstIavDptz->buf_cfg.output.width = pstVprocCfgTmp->s32MainWidth;
                pstIavDptz->buf_cfg.output.height = pstVprocCfgTmp->s32MainHeight;
                break;
            case IAV_SRCBUF_2:
                // third stream
                // ��Buffer�ķֱ��������ƣ�������ڵ�������
                s32StreamIDX = CHIP_VIDEO_ENC_THIRD_STREAM;
                pstIavDptz->buf_cfg.canvas_id = (i * CHIP_VENC_H26X_NUM) + s32StreamIDX + 1;
                pstIavDptz->buf_cfg.output.width = THIRD_STREAM_MAX_WIDTH;
                pstIavDptz->buf_cfg.output.height = THIRD_STREAM_MAX_HEIGHT;
                break;
            case IAV_SRCBUF_3:
                // main stream
                // ������
                s32StreamIDX = CHIP_VIDEO_ENC_MAIN_STREAM;
                pstIavDptz->buf_cfg.canvas_id = (i * CHIP_VENC_H26X_NUM) + s32StreamIDX + 1;
                pstIavDptz->buf_cfg.output.width = pstVprocCfgTmp->s32MainWidth;
                pstIavDptz->buf_cfg.output.height = pstVprocCfgTmp->s32MainHeight;


                break;
            case IAV_SRCBUF_4:
                // 2th stream
                // �ڶ�����
                s32StreamIDX = CHIP_VIDEO_ENC_SECOND_STREAM;
                pstIavDptz->buf_cfg.canvas_id = (i * CHIP_VENC_H26X_NUM) + s32StreamIDX + 1;
                pstIavDptz->buf_cfg.output.width = SECOND_STREAM_MAX_WIDTH;
                pstIavDptz->buf_cfg.output.height = SECOND_STREAM_MAX_HEIGHT;
                break;
            default:
                // ����������
                pstIavDptz->buf_cfg.output.width = 0;
                pstIavDptz->buf_cfg.output.height = 0;
                pstIavDptz->buf_cfg.canvas_id  = IAV_INVALID_CANVAS_ID;
                break;
            }
            AM_IOCTL(g_stVprocCfg.s32Fd, IAV_IOC_CFG_VIDEO_PROC, &stIavVproc);
        }
        // ����pass id 1, ��������ץ����
        for (j = IAV_SRCBUF_FIRST; j < IAV_SUB_SRCBUF_LAST; j++)
        {
            // ��ʼ����������ͨ����Vproc Buffer
            struct iav_video_proc stIavVproc = {0};
            struct iav_dptz* pstIavDptz = &stIavVproc.arg.dptz;
            stIavVproc.cid = IAV_VIDEO_PROC_DPTZ;
            pstIavDptz->channel_id = i;                    // --> stIavSysRes.chan_cfg
            pstIavDptz->pass_id = AMBA_RES_JPEG_PASSID;    // --> pass_num
            pstIavDptz->buf_id = j;                        // --> stIavSysRes.chan_cfg[i].buf_max
            pstIavDptz->buf_cfg.input.width = 0;
            pstIavDptz->buf_cfg.input.height = 0;
            pstIavDptz->buf_cfg.output.width = pstVprocCfgTmp->s32MainWidth;
            pstIavDptz->buf_cfg.output.height = pstVprocCfgTmp->s32MainHeight;
            switch (j)
            {
            case IAV_SRCBUF_3:
                // jpeg
                s32StreamIDX = CHIP_VIDEO_ENC_JPEG_STREAM;
                pstIavDptz->buf_cfg.canvas_id = (i * CHIP_MAX_VENC_NUM_PER_DEV) + s32StreamIDX + 1;
                break;
            case IAV_SRCBUF_4:
                // alarm jpeg
                s32StreamIDX = CHIP_VIDEO_ENC_JPEG_ALARM_STREAM;
                pstIavDptz->buf_cfg.canvas_id = (i * CHIP_MAX_VENC_NUM_PER_DEV) + s32StreamIDX + 1;
                break;
            default:
                pstIavDptz->buf_cfg.output.width = 0;
                pstIavDptz->buf_cfg.output.height = 0;
                pstIavDptz->buf_cfg.canvas_id  = IAV_INVALID_CANVAS_ID;
                break;
            }
            AM_IOCTL(g_stVprocCfg.s32Fd, IAV_IOC_CFG_VIDEO_PROC, &stIavVproc);
        }
    }

    // Vout Source Buffer ����
    {
        struct iav_video_proc stIavVproc = {0};
        struct iav_dptz* pstIavDptz = &stIavVproc.arg.dptz;
        stIavVproc.cid = IAV_VIDEO_PROC_DPTZ;
        pstIavDptz->channel_id = 0;                    // --> stIavSysRes.chan_cfg����ʱ�����Sensor0���Ͼ�VO��һ·
        pstIavDptz->pass_id = AMBA_RES_JPEG_PASSID;    // --> pass_num
        pstIavDptz->buf_id = IAV_SRCBUF_5;             // --> stIavSysRes.chan_cfg[i].buf_max
        pstIavDptz->buf_cfg.input.width = 0;
        pstIavDptz->buf_cfg.input.height = 0;
        pstIavDptz->buf_cfg.output.width = VO_CVBS_MAX_WIDTH;
        pstIavDptz->buf_cfg.output.height = VO_CVBS_MAX_HEIGHT;
        pstIavDptz->buf_cfg.canvas_id = s32VoutCanvasID;
        AM_IOCTL(g_stVprocCfg.s32Fd, IAV_IOC_CFG_VIDEO_PROC, &stIavVproc);
    }


    // Update:WKL
    // for Face Detection YUV data
    // pass 0 , IAV_SRCBUF_5
    {
        struct iav_video_proc stIavVproc = {0};
        struct iav_dptz* pstIavDptz = &stIavVproc.arg.dptz;
        stIavVproc.cid = IAV_VIDEO_PROC_DPTZ;
        pstIavDptz->channel_id = 0;                    // --> stIavSysRes.chan_cfg����ʱ�����Sensor0���Ͼ�VO��һ·
        pstIavDptz->pass_id = AMBA_RES_H26X_PASSID;    // --> pass_num = 0
        pstIavDptz->buf_id = IAV_SRCBUF_5;             // --> stIavSysRes.chan_cfg[i].buf_max
        pstIavDptz->buf_cfg.input.width = 0;
        pstIavDptz->buf_cfg.input.height = 0;
        pstIavDptz->buf_cfg.output.width = 1920;        // --> only for 400M
        pstIavDptz->buf_cfg.output.height = 1080;       // --> only for 400M
        pstIavDptz->buf_cfg.canvas_id = s32YuvCanvasID;
        AM_IOCTL(g_stVprocCfg.s32Fd, IAV_IOC_CFG_VIDEO_PROC, &stIavVproc);
    }
    return s32Ret;
}

/**
 * @brief ��ȡvprocԴ����
 * @param [in] pstVideoCfg��vproc����
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vproc_getSourceCfg(CHIP_VIDEO_CFG_S* pstVideoCfg)
{
    int s32Ret = 0;
    if (g_stVprocCfg.s32Fd <= 0)
    {
        TIPC_PRINTF_ERR("/dev/iav check err! [%d]", g_stVprocCfg.s32Fd);
        return -1;
    }
    if ((NULL == pstVideoCfg))
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    int s32VinDevID = (int)pstVideoCfg->stSensorCfg.u32VinDev;
    // Ѱ��Sensor dev id��Ӧ��resourceͨ�����Լ�����ͨ����
    int s32ResChnID = -1;
    s32Ret = chip_video_getVencStreamID(s32VinDevID, 0, &s32ResChnID, NULL);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get source id failed!!");
        return -1;
    }
    // ��ȡ�ֱ���
    struct iav_video_proc stIavVproc = {0};
    struct iav_dptz* pstIavDptz = &stIavVproc.arg.dptz;
    stIavVproc.cid = IAV_VIDEO_PROC_DPTZ;
    pstIavDptz->channel_id = s32ResChnID;
    pstIavDptz->pass_id = 0;
    pstIavDptz->buf_id = 0;
    AM_IOCTL(g_stVprocCfg.s32Fd, IAV_IOC_GET_VIDEO_PROC, &stIavVproc);

    /// TODO: ��ʱ�����طֱ�����Ϣ,������Ҫ�ٲ���
    pstVideoCfg->stSensorCfg.u32MaxWidth = pstIavDptz->buf_cfg.output.width;
    pstVideoCfg->stSensorCfg.u32MaxHeight = pstIavDptz->buf_cfg.output.height;

    return 0;
}

/**
 * @brief ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vproc_init(int s32IavFD)
{

    if (s32IavFD <= 0)
    {
        TIPC_PRINTF_ERR("/dev/iav is not opened! [%d]", s32IavFD);
        return -1;
    }

    g_stVprocCfg.s32Fd = s32IavFD;
    g_stVprocCfg.u8Init = 1;

    return 0;
}


/**
 * @brief ȥ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vproc_exit()
{
    g_stVprocCfg.s32Fd = 0;
    g_stVprocCfg.s32SensorCnt = 0;
    g_stVprocCfg.u8Init = 0;
    return 0;
}