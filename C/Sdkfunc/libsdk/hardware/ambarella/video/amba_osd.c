#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/mman.h>
#include "amba_osd.h"

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

#define AMBA_OSD_SMALL_CNT 72  ///< �Ե��������ֱ���OSD��С����Ϊ1��λ�����б���������OSD��������(4x) + �ڶ�(2x) + ����(1x) + Jpeg(1x) + Alarm(4x)

#define AMBA_OVERLAY_CLUT_NUM    (16)
#define AMBA_OVERLAY_CLUT_SIZE   (1024)
#define AMBA_OVERLAY_CLUT_OFFSET (0)
#define AMBA_OVERLAY_YUV_OFFSET  (AMBA_OVERLAY_CLUT_NUM * AMBA_OVERLAY_CLUT_SIZE)

typedef struct amba_osd_clut_s 
{
    u8 v;
    u8 u;
    u8 y;
    u8 alpha;
} AMBA_OSD_CLUT_S;

// static AMBA_OSD_CLUT_S clut[] = 
// {
//     /* alpha 0 is full transparent */
//     { .y = 82, .u = 90, .v = 240, .alpha = 255 },	/* red */
//     { .y = 41, .u = 240, .v = 110, .alpha = 255 },	/* blue */
//     { .y = 12, .u = 128, .v = 128, .alpha = 255 },	/* black */
//     { .y = 235, .u = 128, .v = 128, .alpha = 255 },	/* white */
//     { .y = 145, .u = 54, .v = 34, .alpha = 255 },	/* green */
//     { .y = 210, .u = 16, .v = 146, .alpha = 255 },	/* yellow */
//     { .y = 170, .u = 166, .v = 16, .alpha = 255 },	/* cyan */
//     { .y = 107, .u = 202, .v = 222, .alpha = 255 }, /* magenta */
// };

static AMBA_OSD_CLUT_S g_stDefWhiteClut[] = 
{
    { .y = 235, .u = 128, .v = 128, .alpha = 0 },   /* full transparent */
    { .y = 235, .u = 128, .v = 128, .alpha = 255 }, /* white */
    { .y = 12, .u = 128, .v = 128, .alpha = 255 },  /* black */
};

typedef struct amba_osd_cfg_s
{
    int              s32Fd;                               ///< �豸�ļ�������
    unsigned char    u8Init;                              ///< ��ʼ����־
    int              s32VinNum;                           ///< Sensor����
    int              s32OverLaySize;                      ///< Amba OverLay �ڴ�����С
    unsigned char*   pu8OverlayAddr;                      ///< Amba OverLay �ڴ����׵�ַ
    int              s32OSDBuffOffset[CHIP_MAX_VI_NUM][CHIP_MAX_VENC_NUM_PER_DEV][CHIP_MAX_OSD_NUM_PER_VENC];  ///< ÿ��OSD��ŵ��ڴ���ƫ����
    int              s32OSDBuffSize[CHIP_MAX_VI_NUM][CHIP_MAX_VENC_NUM_PER_DEV][CHIP_MAX_OSD_NUM_PER_VENC];    ///< ÿ��OSD��ŵ��ڴ�����С
    
}AMBA_OSD_CFG_S;


AMBA_OSD_CFG_S g_stOSDCfg = {0};

extern int chip_video_getVencStreamID(int s32VinDevID, int s32StreamIDX, int* ps32ResChnID, int* ps32StreamID);

/**
 * @brief rgb����תyuv
 */
static void amba_osd_rgb2yuv(unsigned char u8Red, unsigned char u8Green, unsigned char u8Blue, AMBA_OSD_CLUT_S* pstDstClut)
{
    pstDstClut->y = (u8)(0.257f * u8Red + 0.504f * u8Green + 0.098f * u8Blue + 16);
    pstDstClut->u = (u8)(0.439f * u8Blue - 0.291f * u8Green - 0.148f * u8Red + 128);
    pstDstClut->v = (u8)(0.439f * u8Red - 0.368f * u8Green - 0.071f * u8Blue + 128);
}

/**
 * @brief OSD �ڴ����
 * @return 0���ɹ�����0��ʧ��
 */
static int amba_osd_mapOverlay(void)
{
    int i = 0, j = 0, k = 0;
    struct iav_querymem stQueryMem;
    struct iav_mem_part_info* pstPartInfo;
    unsigned char* pu8MapAddr = NULL;
    int s32MemLength = 0;
    int s32Fd = g_stOSDCfg.s32Fd;
    int s32VinNum = g_stOSDCfg.s32VinNum;
    if(s32Fd <= 0)
    {
        TIPC_PRINTF_ERR("fd of /dev/iav err[%d]", s32Fd);
        return -1;
    }
    // ��ȡOverlay�ڴ�����С
    memset(&stQueryMem, 0, sizeof(stQueryMem));
    stQueryMem.mid = IAV_MEM_PARTITION;
    pstPartInfo = &stQueryMem.arg.partition;
    pstPartInfo->pid = IAV_PART_OVERLAY;
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
    g_stOSDCfg.s32OverLaySize = s32MemLength;
    g_stOSDCfg.pu8OverlayAddr = pu8MapAddr;

    // ����Vin�����Լ������������OSD��С
    int s32OSDNum = AMBA_OSD_SMALL_CNT * s32VinNum;
    // ��ͬ������OSD Buffer�������Ӧ��ͬ
    int s32BufOffset = AMBA_OVERLAY_YUV_OFFSET;
    int s32SmallOSDSize = (s32MemLength - AMBA_OVERLAY_YUV_OFFSET) / s32OSDNum;
    if(s32SmallOSDSize <= 0)
    {
        TIPC_PRINTF_ERR("split osd size failed[%d] [%d]", s32MemLength, s32OSDNum);
        return -1;
    }
    for(i = 0; i < s32VinNum; i++)
    {
        for(j = 0; j < CHIP_VIDEO_ENC_STREAM_BUTT; j++)
        {
            int s32BuffSize = 0;
            switch(j)
            {
                case CHIP_VIDEO_ENC_MAIN_STREAM:
                case CHIP_VIDEO_ENC_JPEG_ALARM_STREAM:
                {
                    s32BuffSize = s32SmallOSDSize * 4;
                }
                break;
                case CHIP_VIDEO_ENC_SECOND_STREAM:
                {
                    s32BuffSize = s32SmallOSDSize * 2;
                }
                break;
                case CHIP_VIDEO_ENC_THIRD_STREAM:
                case CHIP_VIDEO_ENC_JPEG_STREAM:
                {
                    s32BuffSize = s32SmallOSDSize * 1;
                }
                break;
                default:
                {
                    TIPC_PRINTF_ERR("err stream index [%d]", j);
                    continue;
                }
                break;
            }
            for(k = 0; k < CHIP_MAX_OSD_NUM_PER_VENC; k++)
            {
                s32BufOffset += s32BuffSize;
                g_stOSDCfg.s32OSDBuffSize[i][j][k] = s32BuffSize;
                g_stOSDCfg.s32OSDBuffOffset[i][j][k] = s32BufOffset;
            }
        }
    }
    return 0;
}

/**
 * @brief ����OSD
 * @return 0���ɹ�����0��ʧ��
 */
int amba_osd_setOverLay(CHIP_OSD_INFO_S* pstOSDInfo)
{
    int s32Ret = 0;
    int s32Fd = g_stOSDCfg.s32Fd;
    int s32VinDevID = pstOSDInfo->s32VinDevID;
    int s32StreamIDX = pstOSDInfo->s32StreamIDX;
    int s32OSDIDX = pstOSDInfo->s32OSDIDX;
    if(s32Fd <= 0)
    {
        TIPC_PRINTF_ERR("fd of /dev/iav err[%d]", s32Fd);
        return -1;
    }
    if(s32StreamIDX >= CHIP_MAX_VENC_NUM_PER_DEV || s32OSDIDX >= CHIP_MAX_OSD_NUM_PER_VENC)
    {
        TIPC_PRINTF_ERR("err input VinDevID[%d] StreamIDX[%d] OSDIDX[%d]", s32VinDevID, s32StreamIDX, s32OSDIDX);
        return -1;
    }
    if(NULL == pstOSDInfo)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    
    /* IAV must be in ENOCDE or PREVIEW state */
    int s32IavState = 0;
    AM_IOCTL(s32Fd, IAV_IOC_GET_IAV_STATE, &s32IavState);
    if ((s32IavState != IAV_STATE_PREVIEW) && (s32IavState != IAV_STATE_ENCODING)) 
    {
        TIPC_PRINTF_ERR("IAV must be in PREVIEW or ENCODE for OSD.");
        return -1;
    }

    // Ѱ��Sensor dev id��Ӧ�ı���ͨ��
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

    if(1 == pstOSDInfo->s32Enable)
    {
        if(pstOSDInfo->s32W <= 0 || pstOSDInfo->s32H <= 0)
        {
            TIPC_PRINTF_ERR("the image size %dx%d err", pstOSDInfo->s32W, pstOSDInfo->s32H);
            return -1;
        }

        if ((pstOSDInfo->s32W & 0x1F) || (pstOSDInfo->s32H & 0x3)) 
        {
            TIPC_PRINTF_ERR("the image size %dx%d, width must be multiple of 32, height must be multiple of 4.", pstOSDInfo->s32W, pstOSDInfo->s32H);
            return -1;
        }
    }

    // ���ͨ���Ƿ��ڱ���
    struct iav_queryinfo stQueryInfo = {0};
    struct iav_stream_info* pstStreamInfo = NULL;
    stQueryInfo.qid = IAV_INFO_STREAM;
    pstStreamInfo = &stQueryInfo.arg.stream;
    pstStreamInfo->id = s32StreamID;
    AM_IOCTL(s32Fd, IAV_IOC_QUERY_INFO, &stQueryInfo);

    if(pstStreamInfo->state != IAV_STREAM_STATE_ENCODING)
    {
        return -1;
    }

    struct iav_overlay_insert stOverlayInsert = {0};
    stOverlayInsert.id = s32StreamID;
    AM_IOCTL(s32Fd, IAV_IOC_GET_OVERLAY_INSERT, &stOverlayInsert);
    stOverlayInsert.area[s32OSDIDX].enable = (u16)pstOSDInfo->s32Enable;
    if(1 == stOverlayInsert.area[s32OSDIDX].enable)
    {
        if(NULL == pstOSDInfo->pu8BuffAddr)
        {
            TIPC_PRINTF_ERR("OSD Buff is NULL!!");
            return -1;
        }
        // �ж��ڴ��Ƿ��㹻
        int s32OSDBufSize = pstOSDInfo->s32W * pstOSDInfo->s32H;
        if(s32OSDBufSize > g_stOSDCfg.s32OSDBuffSize[s32VinIDX][s32StreamIDX][s32OSDIDX])
        {
            TIPC_PRINTF_ERR("Need mem size (%d) > max prealloc size (%d)", s32OSDBufSize, g_stOSDCfg.s32OSDBuffSize[s32VinIDX][s32StreamIDX][s32OSDIDX]);
            return -1;
        }
        stOverlayInsert.area[s32OSDIDX].start_x = (u16)pstOSDInfo->s32X;
        stOverlayInsert.area[s32OSDIDX].start_y = (u16)pstOSDInfo->s32Y;
        stOverlayInsert.area[s32OSDIDX].width = (u16)pstOSDInfo->s32W;
        stOverlayInsert.area[s32OSDIDX].height = (u16)pstOSDInfo->s32H;
        stOverlayInsert.area[s32OSDIDX].pitch = (u16)pstOSDInfo->s32W;
        stOverlayInsert.area[s32OSDIDX].total_size = pstOSDInfo->s32W * pstOSDInfo->s32H;
        stOverlayInsert.area[s32OSDIDX].data_addr_offset = g_stOSDCfg.s32OSDBuffOffset[s32VinIDX][s32StreamIDX][s32OSDIDX];
        
        if(CHIP_OSD_BUFF_ARGB1555 == pstOSDInfo->enBuffType)
        {
            // ��ʾ����ͼ��
            // ҵ����·�����ARGB1555�����ݣ������Ե�overlay��֧������ͼ��(CLUT8)
            // ����������Ҫ��ARGB1555ͼ��ת��CLUT8���ٽ���������������clut_addr_offset��
            /// TODO:�Ƚ��鷳��������
            return 0;
        }
        else if(CHIP_OSD_BUFF_CLUT8 == pstOSDInfo->enBuffType)
        {
            // ��ʾ�����ַ�����
            // Ŀǰ���������ַ�����OSD����ͳһ��ɫ�ģ�����ʹ��Offset 0����������ʾ����
            stOverlayInsert.area[s32OSDIDX].clut_addr_offset = 0;
            // ������ɫ���CLUT��
            if(0 == pstOSDInfo->s32Color)
            {
                // ʹ��Ĭ����ɫ����ߺڰ�, ͸���ȸ��ݴ����Ĳ�������
                g_stDefWhiteClut[1].alpha = (u8)pstOSDInfo->s32Alpha;
                g_stDefWhiteClut[2].alpha = (u8)pstOSDInfo->s32Alpha;
                memcpy(g_stOSDCfg.pu8OverlayAddr, g_stDefWhiteClut, sizeof(g_stDefWhiteClut));
            }
            else
            {
                AMBA_OSD_CLUT_S stOSDClut[3] = {0};
                // ��һ��������ȫ͸��
                memcpy(stOSDClut, g_stDefWhiteClut, sizeof(AMBA_OSD_CLUT_S));
                // �����������Ǻ�ɫ
                memcpy(&stOSDClut[2], &g_stDefWhiteClut[2], sizeof(AMBA_OSD_CLUT_S));
                // �ڶ�������������ɫȷ��
                unsigned char u8Red = (u8)(pstOSDInfo->s32Color & 0x00FF0000 >> 8);
                unsigned char u8Green = (u8)(pstOSDInfo->s32Color & 0x0000FF00 >> 4);
                unsigned char u8Blue = (u8)(pstOSDInfo->s32Color & 0x000000FF);
                amba_osd_rgb2yuv(u8Red, u8Green, u8Blue, &stOSDClut[1]);
                // ȷ��͸����
                stOSDClut[1].alpha = (u8)pstOSDInfo->s32Alpha;
                stOSDClut[2].alpha = (u8)pstOSDInfo->s32Alpha;
                memcpy(g_stOSDCfg.pu8OverlayAddr, stOSDClut, sizeof(stOSDClut));
            }
            // ֱ�ӿ������ݵ��ڴ���
            unsigned char* pu8MemBuffAddr = g_stOSDCfg.pu8OverlayAddr + g_stOSDCfg.s32OSDBuffOffset[s32VinIDX][s32StreamIDX][s32OSDIDX];
            int s32BuffSize = pstOSDInfo->s32W * pstOSDInfo->s32H;
            memcpy(pu8MemBuffAddr, pstOSDInfo->pu8BuffAddr, s32BuffSize);
        }
        else
        {
            TIPC_PRINTF_ERR("err osd buffer type[%d]", pstOSDInfo->enBuffType);
            return -1;
        }
    }
    stOverlayInsert.enable = 1;
    AM_IOCTL(s32Fd, IAV_IOC_SET_OVERLAY_INSERT, &stOverlayInsert);
    
    return s32Ret;
}



/**
 * @brief ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_osd_init(int s32IavFD, int s32VinNum)
{
    int s32Ret = 0;
    if(s32IavFD < 0)
    {
        TIPC_PRINTF_ERR("iav fd err[%d]", s32IavFD);
        return -1;
    }

    g_stOSDCfg.s32Fd = s32IavFD;
    g_stOSDCfg.u8Init = 1;
    g_stOSDCfg.s32VinNum = s32VinNum;
    g_stOSDCfg.pu8OverlayAddr = NULL;

    s32Ret = amba_osd_mapOverlay();

    return s32Ret;
}


/**
 * @brief ȥ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_osd_exit()
{
    int s32Ret = 0;
    if(g_stOSDCfg.pu8OverlayAddr != NULL)
    {
        s32Ret = munmap(g_stOSDCfg.pu8OverlayAddr, g_stOSDCfg.s32OverLaySize);
        if(s32Ret != 0)
        {
            TIPC_PRINTF_ERR("mumap overlay memory failed");
        }
        g_stOSDCfg.pu8OverlayAddr = NULL;
        g_stOSDCfg.s32OverLaySize = 0;
    }
    memset(&g_stOSDCfg, 0, sizeof(g_stOSDCfg));

    return s32Ret;
}