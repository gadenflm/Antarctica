#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/mman.h>
#include "amba_mask.h"

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

extern int chip_video_getVencStreamID(int s32VinDevID, int s32StreamIDX, int* ps32ResChnID, int* ps32StreamID);

typedef struct amba_mask_cfg_s
{
    int              s32Fd;                               ///< 设备文件描述符
    unsigned char    u8Init;                              ///< 初始化标志
    int              s32VinNum;                           ///< Sensor个数
    int              s32MaskSize;                      ///< Amba mask 内存区大小
    unsigned char*   pu8MaskAddr;                      ///< Amba mask 内存区首地址
    int              s32MaskBuffOffset[CONFIG_AMBARELLA_MAX_CHANNEL_NUM];  ///< 每个Vin mask内存区偏移量
    int              s32MaskBuffSize[CONFIG_AMBARELLA_MAX_CHANNEL_NUM];    ///< 每个Vin mask内存区大小
    
}AMBA_MASK_CFG_S;

AMBA_MASK_CFG_S g_stMaskCfg = {0};

/**
 * @brief MASK 内存分配
 * @return 0：成功；非0：失败
 */
static int amba_mask_mapMem(void)
{
    int i = 0;
    struct iav_querymem stQueryMem;
    struct iav_mem_part_info* pstPartInfo;
    struct iav_mem_layout_resource stMemLayout = {0};
    struct iav_mem_layout* pstBufLayout = NULL;
    unsigned char* pu8MapAddr = NULL;
    int s32MemLength = 0;
    int s32Fd = g_stMaskCfg.s32Fd;
    if(s32Fd <= 0)
    {
        TIPC_PRINTF_ERR("fd of /dev/iav err[%d]", s32Fd);
        return -1;
    }
    // 获取Mask内存区大小
    memset(&stQueryMem, 0, sizeof(stQueryMem));
    stQueryMem.mid = IAV_MEM_PARTITION;
    pstPartInfo = &stQueryMem.arg.partition;
    pstPartInfo->pid = IAV_PART_MASK;
    AM_IOCTL(s32Fd, IAV_IOC_QUERY_MEMBLOCK, &stQueryMem);

    if (pstPartInfo->mem.length == 0) 
    {
        TIPC_PRINTF_ERR("IAV_PART_MASK is not allocated.");
        return -1;
    }

    s32MemLength = pstPartInfo->mem.length;
    pu8MapAddr = mmap(NULL, s32MemLength, PROT_WRITE, MAP_SHARED, s32Fd, pstPartInfo->mem.addr);
    if ((MAP_FAILED == pu8MapAddr) || (NULL == pu8MapAddr))
    {
        TIPC_PRINTF_ERR("mmap failed!!");
        return -1;
    }
    printf("mask map addr %p\n", pu8MapAddr);
    g_stMaskCfg.s32MaskSize = s32MemLength;
    g_stMaskCfg.pu8MaskAddr = pu8MapAddr;

    AM_IOCTL(s32Fd, IAV_IOC_QUERY_MEM_LAYOUT, &stMemLayout);

    for (i = 0; i < CONFIG_AMBARELLA_MAX_CHANNEL_NUM; i++) 
    {
        pstBufLayout = &stMemLayout.mask_layout.buf_layout[i];
        g_stMaskCfg.s32MaskBuffOffset[i] = pstBufLayout->offset;
        g_stMaskCfg.s32MaskBuffSize[i] = pstBufLayout->size;
        if (g_stMaskCfg.s32MaskBuffSize[i] > 0)
        {
            printf("Channel %d: mask size = 0x%x, mask offset = 0x%x!\n", i,
                g_stMaskCfg.s32MaskBuffSize[i], g_stMaskCfg.s32MaskBuffOffset[i]);
        }
    }

    return 0;
}

/**
 * @brief 设置Mask
 * @return 0：成功；非0：失败
 */
int amba_mask_setPos(CHIP_MASK_INFO_S* pstMaskInfo)
{
    int i = 0, j = 0;
    int s32Ret = 0;
    int s32Fd = g_stMaskCfg.s32Fd;
    int s32VinDevID = pstMaskInfo->s32VinDevID;
    int s32StreamIDX = pstMaskInfo->s32StreamIDX;
    u8* pu8Row = NULL;
    u8* pu8Column = NULL;
    u8  u8Value = 0;
    if(s32Fd <= 0)
    {
        TIPC_PRINTF_ERR("fd of /dev/iav err[%d]", s32Fd);
        return -1;
    }
    if(s32StreamIDX >= CHIP_MAX_VENC_NUM_PER_DEV)
    {
        TIPC_PRINTF_ERR("err input VinDevID[%d] StreamIDX[%d]", s32VinDevID, s32StreamIDX);
        return -1;
    }
    if(NULL == pstMaskInfo)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    
    /* IAV must be in ENOCDE or PREVIEW state */
    int s32IavState = 0;
    AM_IOCTL(s32Fd, IAV_IOC_GET_IAV_STATE, &s32IavState);
    if ((s32IavState != IAV_STATE_PREVIEW) && (s32IavState != IAV_STATE_ENCODING)) 
    {
        TIPC_PRINTF_ERR("IAV must be in PREVIEW or ENCODE for Mask.");
        return -1;
    }

    // 寻找Sensor dev id对应的Channel ID
    int s32StreamID = -1;
    int s32ChanID = -1;
    s32Ret = chip_video_getVencStreamID(s32VinDevID, s32StreamIDX, &s32ChanID, &s32StreamID);
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get stream id failed!!");
        return -1;
    }

    if(s32ChanID >= CHIP_MAX_VI_NUM)
    {
        TIPC_PRINTF_ERR("err Vin index[%d]!!", s32ChanID);
        return -1;
    }

    // get mask info
    struct iav_queryinfo stQueryInfo = {0};
    stQueryInfo.qid = IAV_INFO_MASK;
    stQueryInfo.arg.mask_info.channel_id = s32ChanID;
    AM_IOCTL(s32Fd, IAV_IOC_QUERY_INFO, &stQueryInfo);
    int s32Pitch = stQueryInfo.arg.mask_info.buffer_pitch;

    if(pstMaskInfo->s32W <= 0 || pstMaskInfo->s32H <= 0)
    {
        TIPC_PRINTF_ERR("the image size %dx%d err", pstMaskInfo->s32W, pstMaskInfo->s32H);
        return -1;
    }

    pu8Row = g_stMaskCfg.pu8MaskAddr + g_stMaskCfg.s32MaskBuffOffset[s32ChanID] + s32Pitch * pstMaskInfo->s32Y;
    int s32Left = pstMaskInfo->s32X / 8;
    int s32Right = (pstMaskInfo->s32X + pstMaskInfo->s32W) / 8;
    int s32LeftStart = pstMaskInfo->s32X % 8;
    int s32RightRemain = (pstMaskInfo->s32X + pstMaskInfo->s32W) % 8;

    if(pstMaskInfo->s32Enable)
    {
        // 根据填充的颜色来决定使能
        u8Value = 0xff;
    }

    for (j = 0; j < pstMaskInfo->s32H; j++) 
    {
        if (s32Left == s32Right && (s32LeftStart + pstMaskInfo->s32W < 8)) 
        {
            /*This is for small PM width is within 8 pixel*/
            pu8Column = pu8Row + s32Left;
            if (0 == pstMaskInfo->s32Enable) 
            {
                u8Value = 0xFF;
                u8Value <<= (s32LeftStart + pstMaskInfo->s32W);
                u8Value |= (0xFF >> (8 - s32LeftStart));
                *pu8Column &= u8Value;
            } 
            else 
            {
                u8Value = 0xFF;
                u8Value <<= s32LeftStart;
                u8Value &= (0xFF >> (8 - s32LeftStart - pstMaskInfo->s32W));
                *pu8Column |= u8Value;
            }
        } 
        else 
        {
            /* This is for PM ocupies more than one byte(8 pixel) */
            pu8Column = pu8Row + s32Left + 1;
            for (i = 0; i < s32Right - s32Left - 1; i++) 
            {
                *pu8Column = u8Value;
                pu8Column++;
            }

            pu8Column = pu8Row + s32Left;
            if (0 == pstMaskInfo->s32Enable) {
                *pu8Column &= (0xFF >> (8 - s32LeftStart));
            } 
            else 
            {
                *pu8Column |= (0xFF << s32LeftStart);
            }

            pu8Column = pu8Row + s32Right;
            if (s32RightRemain) 
            {
                if (0 == pstMaskInfo->s32Enable) 
                {
                    *pu8Column &= (0xFF << s32RightRemain);
                } 
                else 
                {
                    *pu8Column |= (0xFF >> (8 - s32RightRemain));
                }
            }
        }

        /*Switch to next line*/
        pu8Row += s32Pitch;
    }

    struct iav_video_proc stVproc = {0};
    stVproc.cid = IAV_VIDEO_PROC_MASK;
    stVproc.arg.mask.channel_id = s32ChanID;
    AM_IOCTL(s32Fd, IAV_IOC_GET_VIDEO_PROC, &stVproc);
    struct iav_apply_flag stApplyFlag[IAV_VIDEO_PROC_NUM] = {0};
    stApplyFlag[IAV_VIDEO_PROC_MASK].param |= (1 << s32ChanID);
    stApplyFlag[IAV_VIDEO_PROC_MASK].apply = 1;
    stVproc.cid = IAV_VIDEO_PROC_MASK;
    stVproc.arg.mask.enable = 1;
    stVproc.arg.mask.y = 0;
    stVproc.arg.mask.u = 128;
    stVproc.arg.mask.v = 128;
    stVproc.arg.mask.channel_id = s32ChanID;
    AM_IOCTL(s32Fd, IAV_IOC_CFG_VIDEO_PROC, &stVproc);
    AM_IOCTL(s32Fd, IAV_IOC_APPLY_VIDEO_PROC, stApplyFlag);

    return 0;
}



/**
 * @brief 初始化模块
 * @return 0：成功；非0：失败
 */
int amba_mask_init(int s32IavFD, int s32VinNum)
{
    int s32Ret = 0;
    int i = 0;
    if(s32IavFD < 0)
    {
        TIPC_PRINTF_ERR("iav fd err[%d]", s32IavFD);
        return -1;
    }

    g_stMaskCfg.s32Fd = s32IavFD;
    g_stMaskCfg.u8Init = 1;
    g_stMaskCfg.s32VinNum = s32VinNum;
    g_stMaskCfg.pu8MaskAddr = NULL;

    s32Ret = amba_mask_mapMem();

    // 清空mask
    for(i = 0; i < s32VinNum; i++)
    {
        struct iav_video_proc stVproc = {0};
        stVproc.cid = IAV_VIDEO_PROC_MASK;
        stVproc.arg.mask.channel_id = i;
        ioctl(s32IavFD, IAV_IOC_GET_VIDEO_PROC, &stVproc);
        struct iav_apply_flag stApplyFlag[IAV_VIDEO_PROC_NUM] = {0};
        stApplyFlag[IAV_VIDEO_PROC_MASK].param |= (1 << i);
        stApplyFlag[IAV_VIDEO_PROC_MASK].apply = 1;
        stVproc.cid = IAV_VIDEO_PROC_MASK;
        stVproc.arg.mask.enable = 0;
        stVproc.arg.mask.y = 0;
        stVproc.arg.mask.u = 128;
        stVproc.arg.mask.v = 128;
        stVproc.arg.mask.channel_id = i;
        s32Ret = ioctl(s32IavFD, IAV_IOC_CFG_VIDEO_PROC, &stVproc);
        if(s32Ret != 0)
        {
            perror("IAV_IOC_CFG_VIDEO_PROC err\n");
        }
        s32Ret = ioctl(s32IavFD, IAV_IOC_APPLY_VIDEO_PROC, stApplyFlag);
        if(s32Ret != 0)
        {
            perror("IAV_IOC_APPLY_VIDEO_PROC err\n");
        }
    }

    return 0;
}


/**
 * @brief 去初始化模块
 * @return 0：成功；非0：失败
 */
int amba_mask_exit()
{
    int s32Ret = 0;
    if(g_stMaskCfg.pu8MaskAddr != NULL)
    {
        s32Ret = munmap(g_stMaskCfg.pu8MaskAddr, g_stMaskCfg.s32MaskSize);
        if(s32Ret != 0)
        {
            TIPC_PRINTF_ERR("mumap overlay memory failed");
        }
        g_stMaskCfg.pu8MaskAddr = NULL;
        g_stMaskCfg.s32MaskSize = 0;
    }
    memset(&g_stMaskCfg, 0, sizeof(g_stMaskCfg));

    return s32Ret;
}