#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/mman.h>
#include "amba_qproi.h"

#ifndef AM_IOCTL
#define AM_IOCTL(_filp, _cmd, _arg)	\
        do {                        \
            if (ioctl(_filp, _cmd, _arg) < 0) { \
                perror(#_cmd);      \
                TIPC_PRINTF_ERR("%s err [%d]", #_cmd, errno); \
                return -1;      \
            }                       \
        } while (0)
#endif

#define CHIP_MAX_ROI_NUM_PER_VENC 8

typedef struct amba_roi_cfg_s
{
    int              s32Fd;                               ///< 设备文件描述符
    unsigned char    u8Init;                              ///< 初始化标志
    int              s32VinNum;                           ///< Sensor个数
    int              s32ROISize;                          ///< Amba ROI 内存区大小
    unsigned char*   pu8ROIAddr;                          ///< Amba ROI 内存区首地址
    int              s32QpMatrixOffset[CHIP_MAX_VI_NUM][CHIP_VENC_H26X_NUM];  ///< 每个码流ROI的内存区偏移量
    int              s32QpMatrixSize[CHIP_MAX_VI_NUM][CHIP_VENC_H26X_NUM];    ///< 每个码流ROI的内存区大小
    int              s32SingleMatrixSize[CHIP_MAX_VI_NUM][CHIP_VENC_H26X_NUM];
    int              s32StreamMatrixNum[CHIP_MAX_VI_NUM][CHIP_VENC_H26X_NUM];
}AMBA_ROI_CFG_S;


AMBA_ROI_CFG_S g_stROICfg = {0};

extern int chip_video_getVencStreamID(int s32VinDevID, int s32StreamIDX, int* ps32ResChnID, int* ps32StreamID);

/**
 * @brief ROI 内存分配
 * @return 0：成功；非0：失败
 */
static int amba_roi_mapQpMatrix(void)
{
    int i = 0, j = 0;
    struct iav_querymem stQueryMem;
    struct iav_mem_part_info* pstPartInfo;
    struct iav_mem_layout_resource stMemLayout;
    struct iav_mem_layout* pstBufLayout = NULL;
    unsigned char* pu8MapAddr = NULL;
    int s32MemLength = 0;
    int s32Fd = g_stROICfg.s32Fd;
    int s32VinNum = g_stROICfg.s32VinNum;
    if(s32Fd <= 0)
    {
        TIPC_PRINTF_ERR("fd of /dev/iav err[%d]", s32Fd);
        return -1;
    }
    // 获取Overlay内存区大小
    memset(&stQueryMem, 0, sizeof(stQueryMem));
    stQueryMem.mid = IAV_MEM_PARTITION;
    pstPartInfo = &stQueryMem.arg.partition;
    pstPartInfo->pid = IAV_PART_QPMATRIX;
    AM_IOCTL(s32Fd, IAV_IOC_QUERY_MEMBLOCK, &stQueryMem);

    if (pstPartInfo->mem.length == 0) 
    {
        TIPC_PRINTF_ERR("IAV_PART_OVERLAY is not allocated.");
        return -1;
    }

    s32MemLength = pstPartInfo->mem.length;
    pu8MapAddr = mmap(NULL, s32MemLength, PROT_WRITE, MAP_SHARED, s32Fd, pstPartInfo->mem.addr);
    if ((MAP_FAILED == pu8MapAddr) || (NULL == pu8MapAddr))
    {
        TIPC_PRINTF_ERR("mmap failed!!");
        return -1;
    }
    g_stROICfg.s32ROISize = s32MemLength;
    g_stROICfg.pu8ROIAddr = pu8MapAddr;

    if(ioctl(s32Fd, IAV_IOC_QUERY_MEM_LAYOUT, &stMemLayout) < 0) 
    {
        perror("IAV_IOC_QUERY_MEM_LAYOUT");
        TIPC_PRINTF_ERR("IAV_IOC_QUERY_MEM_LAYOUT err");
        munmap(g_stROICfg.pu8ROIAddr, g_stROICfg.s32ROISize);
        return -1;
    }

    // 根据码流分配各块ROI大小
    for(i = 0; i < s32VinNum; i++)
    {
        for(j = 0; j < CHIP_VENC_H26X_NUM; j++)
        {
            // 获得对应的StreamID
            int s32StreamID = (i * CHIP_VENC_H26X_NUM) + j;
            pstBufLayout = &stMemLayout.qpm_layout.buf_layout[s32StreamID];
            g_stROICfg.s32QpMatrixOffset[i][j] = pstBufLayout->offset;
            g_stROICfg.s32QpMatrixSize[i][j] = pstBufLayout->size;
            g_stROICfg.s32SingleMatrixSize[i][j] = pstBufLayout->unit_size;
            g_stROICfg.s32StreamMatrixNum[i][j] = pstBufLayout->unit_num;
        }
    }
    return 0;
}

/**
 * @brief 设置H265 ROI
 * @return 0：成功；非0：失败
 */
static int amba_roi_setH265Pos(const int s32VinIDX, const int s32StreamID, const CHIP_ROI_INFO_S* pstROIInfo)
{
    int s32Fd = g_stROICfg.s32Fd;
    struct iav_stream_cfg stStreamCfg = {0};
    struct iav_qpmatrix* pstQpMatrix = NULL;
    struct iav_h265_qproi_data* pstRoiAddr = NULL;
    u32 u32BufWidth = 0, u32BufPitch, u32BufHeight;
    u32 u32StartX = 0, u32StartY = 0, u32EndX = 0, u32EndY = 0;
    unsigned long u32DataOffset = 0;
    int i, j;
    u8* pu8Addr = NULL;
    u8* pu8CurAddr = NULL;

    int s32RectX = pstROIInfo->s32X;
    int s32RectY = pstROIInfo->s32Y;
    int s32RectW = pstROIInfo->s32W;
    int s32RectH = pstROIInfo->s32H;
    int s32StreamIDX = pstROIInfo->s32StreamIDX;

    // 获取码流分辨率
    stStreamCfg.id = s32StreamID;
    stStreamCfg.cid = IAV_STMCFG_FORMAT;
    AM_IOCTL(s32Fd, IAV_IOC_GET_STREAM_CONFIG, &stStreamCfg);
    struct iav_stream_format* pstIavStreamFormat = &stStreamCfg.arg.format;
    int s32EncWidth = pstIavStreamFormat->enc_win.width;
    int s32EncHeight = pstIavStreamFormat->enc_win.height;

    // 获取QP ROI参数
    memset(&stStreamCfg, 0, sizeof(stStreamCfg));
    pstQpMatrix = &stStreamCfg.arg.h265_roi;
    stStreamCfg.id = s32StreamID;
    stStreamCfg.cid = IAV_H265_CFG_QP_ROI;
    stStreamCfg.arg.h265_roi.id = s32StreamID;
    stStreamCfg.arg.h265_roi.qpm_no_update = 1;
    AM_IOCTL(s32Fd, IAV_IOC_GET_STREAM_CONFIG, &stStreamCfg);
    pstQpMatrix->id = s32StreamID;
    pstQpMatrix->enable = 1;
    pstQpMatrix->qpm_no_update = 0;
    pstQpMatrix->qpm_no_check = 0;
    pstQpMatrix->init_qpm = 1; /// TODO: 待验证
    
    // QP matrix is CTB level. One CTB is 32x32 pixels.
    u32BufWidth = ROUND_UP(s32EncWidth, 32) / 32;
    u32BufHeight = ROUND_UP(s32EncHeight, 32) / 32;
    u32BufPitch = ROUND_UP(u32BufWidth * sizeof (struct iav_h265_qproi_data), LPDDR4_ALIGN);

    u32StartX = ROUND_DOWN(s32RectX, 32) / 32;
    u32StartY = ROUND_DOWN(s32RectY, 32) / 32;
    u32EndX = ROUND_UP(s32RectW, 32) / 32 + u32StartX;
    u32EndY = ROUND_UP(s32RectH, 32) / 32 + u32StartY;

    u32DataOffset = g_stROICfg.s32QpMatrixOffset[s32VinIDX][s32StreamIDX];
    pu8Addr = g_stROICfg.pu8ROIAddr + u32DataOffset;
    pu8CurAddr = pu8Addr + u32StartY * u32BufPitch;

    int s32Qp = (0 == pstROIInfo->s32Enable)?0:pstROIInfo->s32QP;
    for (i = u32StartY; i < u32EndY && i < u32BufHeight; i++) 
    {
        pstRoiAddr = (struct iav_h265_qproi_data *)pu8CurAddr;
        for (j = u32StartX; j < u32EndX && j < u32BufWidth; j++) 
        {
            pstRoiAddr[j].qp_adjust_32.cu32 = s32Qp;
            pstRoiAddr[j].qp_adjust_16.cu16_0 = s32Qp;
            pstRoiAddr[j].qp_adjust_16.cu16_1 = s32Qp;
            pstRoiAddr[j].qp_adjust_16.cu16_2 = s32Qp;
            pstRoiAddr[j].qp_adjust_16.cu16_3 = s32Qp;
        }
        pu8CurAddr += u32BufPitch;
    }

    stStreamCfg.id = s32StreamID;
    stStreamCfg.cid = IAV_H265_CFG_QP_ROI;
    pstQpMatrix->data_size = u32BufPitch * u32BufHeight;
    pstQpMatrix->data_offset = u32DataOffset;
    AM_IOCTL(s32Fd, IAV_IOC_SET_STREAM_CONFIG, &stStreamCfg);

    return 0;
}


/**
 * @brief 设置H264 ROI
 * @return 0：成功；非0：失败
 */
static int amba_roi_setH264Pos(const int s32VinIDX, const int s32StreamID, const CHIP_ROI_INFO_S* pstROIInfo)
{
    int s32Fd = g_stROICfg.s32Fd;
    struct iav_stream_cfg stStreamCfg = {0};
    struct iav_qpmatrix* pstQpMatrix = NULL;
    struct iav_h264_qproi_data* pstRoiAddr = NULL;
    u32 u32BufWidth = 0, u32BufPitch, u32BufHeight;
    u32 u32StartX = 0, u32StartY = 0, u32EndX = 0, u32EndY = 0;
    unsigned long u32DataOffset = 0;
    int i, j;
    u8* pu8Addr = NULL;
    u8* pu8CurAddr = NULL;

    int s32RectX = pstROIInfo->s32X;
    int s32RectY = pstROIInfo->s32Y;
    int s32RectW = pstROIInfo->s32W;
    int s32RectH = pstROIInfo->s32H;
    int s32StreamIDX = pstROIInfo->s32StreamIDX;

    // 获取码流分辨率
    stStreamCfg.id = s32StreamID;
    stStreamCfg.cid = IAV_STMCFG_FORMAT;
    AM_IOCTL(s32Fd, IAV_IOC_GET_STREAM_CONFIG, &stStreamCfg);
    struct iav_stream_format* pstIavStreamFormat = &stStreamCfg.arg.format;
    int s32EncWidth = pstIavStreamFormat->enc_win.width;
    int s32EncHeight = pstIavStreamFormat->enc_win.height;

    // 获取QP ROI参数
    memset(&stStreamCfg, 0, sizeof(stStreamCfg));
    pstQpMatrix = &stStreamCfg.arg.h264_roi;
    stStreamCfg.id = s32StreamID;
    stStreamCfg.cid = IAV_H264_CFG_QP_ROI;
    stStreamCfg.arg.h264_roi.id = s32StreamID;
    stStreamCfg.arg.h264_roi.qpm_no_update = 1;
    AM_IOCTL(s32Fd, IAV_IOC_GET_STREAM_CONFIG, &stStreamCfg);
    pstQpMatrix->id = s32StreamID;
    pstQpMatrix->enable = 1;
    pstQpMatrix->qpm_no_update = 0;
    pstQpMatrix->qpm_no_check = 0;
    pstQpMatrix->init_qpm = 1; /// TODO: 待验证
    
    // QP matrix is MB level. One MB is 16x16 pixels.
    u32BufWidth = ROUND_UP(s32EncWidth, 16) / 16;
    u32BufHeight = ROUND_UP(s32EncHeight, 16) / 16;
    u32BufPitch = ROUND_UP(u32BufWidth * sizeof (struct iav_h264_qproi_data), LPDDR4_ALIGN);

    u32StartX = ROUND_DOWN(s32RectX, 16) / 16;
    u32StartY = ROUND_DOWN(s32RectY, 16) / 16;
    u32EndX = ROUND_UP(s32RectW, 16) / 16 + u32StartX;
    u32EndY = ROUND_UP(s32RectH, 16) / 16 + u32StartY;

    u32DataOffset = g_stROICfg.s32QpMatrixOffset[s32VinIDX][s32StreamIDX];
    pu8Addr = g_stROICfg.pu8ROIAddr + u32DataOffset;
    pu8CurAddr = pu8Addr + u32StartY * u32BufPitch;

    int s32Qp = (0 == pstROIInfo->s32Enable)?0:pstROIInfo->s32QP;
    for (i = u32StartY; i < u32EndY && i < u32BufHeight; i++) 
    {
        pstRoiAddr = (struct iav_h264_qproi_data *)pu8CurAddr;
        for (j = u32StartX; j < u32EndX && j < u32BufWidth; j++) 
        {
            pstRoiAddr[j].qp_adjust = s32Qp;
        }
        pu8CurAddr += u32BufPitch;
    }
    // printf("set roi...%d %d %d %d %d\n", u32StartX, u32StartY, u32EndX, u32EndY, s32Qp);
    stStreamCfg.id = s32StreamID;
    stStreamCfg.cid = IAV_H264_CFG_QP_ROI;
    pstQpMatrix->data_size = u32BufPitch * u32BufHeight;
    pstQpMatrix->data_offset = u32DataOffset;
    AM_IOCTL(s32Fd, IAV_IOC_SET_STREAM_CONFIG, &stStreamCfg);

    return 0;
}


/**
 * @brief 设置ROI
 * @return 0：成功；非0：失败
 */
int amba_roi_setPos(CHIP_ROI_INFO_S* pstROIInfo)
{
    int s32Ret = 0;
    int s32Fd = g_stROICfg.s32Fd;
    int s32VinDevID = pstROIInfo->s32VinDevID;
    int s32StreamIDX = pstROIInfo->s32StreamIDX;
    if(s32Fd <= 0)
    {
        TIPC_PRINTF_ERR("fd of /dev/iav err[%d]", s32Fd);
        return -1;
    }
    if(s32StreamIDX >= CHIP_VENC_H26X_NUM)
    {
        TIPC_PRINTF_ERR("err input VinDevID[%d] StreamIDX[%d]", s32VinDevID, s32StreamIDX);
        return -1;
    }
    if(NULL == pstROIInfo)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }

    // 寻找Sensor dev id对应的编码通道
    int s32StreamID = -1;
    int s32VinIDX = -1;
    s32Ret = chip_video_getVencStreamID(s32VinDevID, s32StreamIDX, &s32VinIDX, &s32StreamID);
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get stream id failed!!");
        return -1;
    }

    if(s32VinIDX >= CHIP_MAX_VI_NUM)
    {
        TIPC_PRINTF_ERR("err Vin index[%d]!!", s32VinIDX);
        return -1;
    }

    // 检查编码属性
    struct iav_queryinfo stQueryInfo = {0};
    stQueryInfo.qid = IAV_INFO_STREAM;
    stQueryInfo.arg.stream.id = s32StreamID;
    AM_IOCTL(s32Fd, IAV_IOC_QUERY_INFO, &stQueryInfo);
    if (stQueryInfo.arg.stream.state != IAV_STREAM_STATE_ENCODING) 
    {
        TIPC_PRINTF_ERR("Stream %d should be in ENCODE state.", s32StreamID);
        return -1;
    }

    struct iav_stream_cfg stStreamCfg = {0};
    stStreamCfg.id = s32StreamID;
    stStreamCfg.cid = IAV_STMCFG_FORMAT;
    AM_IOCTL(s32Fd, IAV_IOC_GET_STREAM_CONFIG, &stStreamCfg);
    struct iav_stream_format* pstStreamFormat = &stStreamCfg.arg.format;
    if(pstStreamFormat->type != IAV_STREAM_TYPE_H264 &&
        pstStreamFormat->type != IAV_STREAM_TYPE_H265)
    {
        TIPC_PRINTF_ERR("Stream %d encode format shall be H.26x.", s32StreamID);
        return -1;
    }

    // 应用
    if(IAV_STREAM_TYPE_H264 == pstStreamFormat->type)
    {
        s32Ret = amba_roi_setH264Pos(s32VinIDX, s32StreamID, pstROIInfo);
    }
    else
    {
        s32Ret = amba_roi_setH265Pos(s32VinIDX, s32StreamID, pstROIInfo);
    }
    return s32Ret;
}



/**
 * @brief 初始化模块
 * @return 0：成功；非0：失败
 */
int amba_roi_init(int s32IavFD, int s32VinNum)
{
    int s32Ret = 0;
    if(s32IavFD < 0)
    {
        TIPC_PRINTF_ERR("iav fd err[%d]", s32IavFD);
        return -1;
    }

    g_stROICfg.s32Fd = s32IavFD;
    g_stROICfg.u8Init = 1;
    g_stROICfg.s32VinNum = s32VinNum;
    g_stROICfg.pu8ROIAddr = NULL;

    s32Ret = amba_roi_mapQpMatrix();

    return s32Ret;
}


/**
 * @brief 去初始化模块
 * @return 0：成功；非0：失败
 */
int amba_roi_exit()
{
    int s32Ret = 0;
    if(g_stROICfg.pu8ROIAddr != NULL)
    {
        s32Ret = munmap(g_stROICfg.pu8ROIAddr, g_stROICfg.s32ROISize);
        if(s32Ret != 0)
        {
            TIPC_PRINTF_ERR("mumap overlay memory failed");
        }
        g_stROICfg.pu8ROIAddr = NULL;
        g_stROICfg.s32ROISize = 0;
    }
    memset(&g_stROICfg, 0, sizeof(g_stROICfg));

    return s32Ret;
}