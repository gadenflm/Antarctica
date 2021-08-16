
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "amba_vin.h"

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

typedef struct amba_vin_cfg_s
{
    int              s32Fd;          ///< 设备文件描述符
    int              s32Dev;         ///< 设备号(对应Sensor连接的设备)
    int              s32Chn;         ///< 通道号
    unsigned char    u8FrameRate;    ///< 帧率
    unsigned char    u8Init;         ///< 初始化标志
    unsigned char    u8DevEnable;    ///< 设备启动标志
    unsigned char    u8ChnEnable;    ///< 通道号启动标志
    // pthread_mutex_t  s32Mutex;       ///< 互斥锁
}AMBA_VIN_CFG_S;

static AMBA_VIN_CFG_S g_stViCfg[CHIP_MAX_VI_NUM] = {0};
// static int  g_s32ViNum = 0;

/**
 * @brief 打印Sensor支持的输出信息，调试打印用
 * @param [in] s32VinDevID   设备号
 * @param [in] pstVdoInfo    Sensor驱动信息
 * @param [in] s32ActiveFlag 激活标志
 * @return 0：成功；非0：失败
 */
static int amba_vin_printVdoInfo(int s32VinDevID, const struct amba_video_info* pstVdoInfo, int s32ActiveFlag)
{
    char        szFormat[32] = {0};
    char        szHdrMode[32] = {0};
    char        szFps[32] = {0};
    char        szType[32] = {0};
    char        szBits[32] = {0};
    char        szRatio[32] = {0};
    char        szSystem[32] = {0};
    u32         u32Fps = 0;
    struct vindev_fps stActiveFps;
    int     s32IavFd = g_stViCfg[s32VinDevID].s32Fd;

    switch(pstVdoInfo->format) 
    {
        case AMBA_VIDEO_FORMAT_PROGRESSIVE:
            snprintf(szFormat, 32, "%s", "P");
            break;
        case AMBA_VIDEO_FORMAT_INTERLACE:
            snprintf(szFormat, 32, "%s", "I");
            break;
        case AMBA_VIDEO_FORMAT_AUTO:
            snprintf(szFormat, 32, "%s", "Auto");
            break;
        default:
            snprintf(szFormat, 32, "format?%d", pstVdoInfo->format);
            break;
    }

    switch(pstVdoInfo->hdr_mode) 
    {
        case AMBA_VIDEO_2X_HDR_MODE:
            snprintf(szHdrMode, 32, "%s", "(HDR 2x)");
            break;
        case AMBA_VIDEO_3X_HDR_MODE:
            snprintf(szHdrMode, 32, "%s", "(HDR 3x)");
            break;
        case AMBA_VIDEO_4X_HDR_MODE:
            snprintf(szHdrMode, 32, "%s", "(HDR 4x)");
            break;
        case AMBA_VIDEO_INT_HDR_MODE:
            snprintf(szHdrMode, 32, "%s", "(WDR In)");
            break;
        case AMBA_VIDEO_LINEAR_MODE:
        default:
            snprintf(szHdrMode, 32, "%s", "(Linear)");
            break;
    }

    if (s32ActiveFlag) 
    {
        memset(&stActiveFps, 0, sizeof(stActiveFps));
        stActiveFps.vsrc_id = s32VinDevID;
        if (ioctl(s32IavFd, IAV_IOC_VIN_GET_FPS, &stActiveFps) < 0) 
        {
            perror("IAV_IOC_VIN_GET_FPS");
        }
        u32Fps = stActiveFps.fps;
    } else {
    	u32Fps = pstVdoInfo->max_fps;
    }

    switch(u32Fps) 
    {
        case AMBA_VIDEO_FPS_AUTO:
            snprintf(szFps, 32, "%s", "AUTO");
            break;
        case AMBA_VIDEO_FPS_29_97:
            snprintf(szFps, 32, "%s", "29.97");
            break;
        case AMBA_VIDEO_FPS_59_94:
            snprintf(szFps, 32, "%s", "59.94");
            break;
        default:
            {
                u32 u32Hz = DIV_CLOSEST(512000000, u32Fps);
                snprintf(szFps, 32, "%d", u32Hz);
            }
            break;
    }

    switch(pstVdoInfo->type) 
    {
        case AMBA_VIDEO_TYPE_AUTO:
            snprintf(szType, 32, "%s", "AUTO");
            break;
        case AMBA_VIDEO_TYPE_RGB_RAW:
            snprintf(szType, 32, "%s", "RGB");
            break;
        case AMBA_VIDEO_TYPE_YUV_601:
            snprintf(szType, 32, "%s", "YUV BT601");
            break;
        case AMBA_VIDEO_TYPE_YUV_656:
            snprintf(szType, 32, "%s", "YUV BT656");
            break;
        case AMBA_VIDEO_TYPE_YUV_BT1120:
            snprintf(szType, 32, "%s", "YUV BT1120");
            break;
        case AMBA_VIDEO_TYPE_RGB_601:
            snprintf(szType, 32, "%s", "RGB BT601");
            break;
        case AMBA_VIDEO_TYPE_RGB_656:
            snprintf(szType, 32, "%s", "RGB BT656");
            break;
        case AMBA_VIDEO_TYPE_RGB_BT1120:
            snprintf(szType, 32, "%s", "RGB BT1120");
            break;
        default:
            snprintf(szType, 32, "type?%d", pstVdoInfo->type);
            break;
    }

    switch(pstVdoInfo->bits) 
    {
        case AMBA_VIDEO_BITS_AUTO:
            snprintf(szBits, 32, "%s", "Bits Not Availiable");
            break;
        default:
            snprintf(szBits, 32, "%dbits", pstVdoInfo->bits);
            break;
    }

    switch(pstVdoInfo->ratio) 
    {
        case AMBA_VIDEO_RATIO_AUTO:
            snprintf(szRatio, 32, "%s", "AUTO");
            break;
        case AMBA_VIDEO_RATIO_4_3:
            snprintf(szRatio, 32, "%s", "4:3");
            break;
        case AMBA_VIDEO_RATIO_16_9:
            snprintf(szRatio, 32, "%s", "16:9");
            break;
        case AMBA_VIDEO_RATIO_1_1:
            snprintf(szRatio, 32, "%s", "1:1");
            break;
        default:
            snprintf(szRatio, 32, "ratio?%d", pstVdoInfo->ratio);
            break;
    }

    switch(pstVdoInfo->system) 
    {
        case AMBA_VIDEO_SYSTEM_AUTO:
            snprintf(szSystem, 32, "%s", "AUTO");
            break;
        case AMBA_VIDEO_SYSTEM_NTSC:
            snprintf(szSystem, 32, "%s", "NTSC");
            break;
        case AMBA_VIDEO_SYSTEM_PAL:
            snprintf(szSystem, 32, "%s", "PAL");
            break;
        case AMBA_VIDEO_SYSTEM_SECAM:
            snprintf(szSystem, 32, "%s", "SECAM");
            break;
        case AMBA_VIDEO_SYSTEM_ALL:
            snprintf(szSystem, 32, "%s", "ALL");
            break;
        default:
            snprintf(szSystem, 32, "system?%d", pstVdoInfo->system);
            break;
    }

    TIPC_PRINTF_INFO("\t%dx%d%s%s\t%s\t%s\t%s\t%s\t%s\trev[%d]",
        pstVdoInfo->width, pstVdoInfo->height,
        szFormat, szHdrMode, szFps, szType, szBits, szRatio, szSystem, pstVdoInfo->rev);
    return 0;
}

/**
 * @brief 检查Dev信息，基本也是调试打印用
 * @return 0：成功；非0：失败
 */
static int amba_vin_checkSourceInfo(int s32VinDevID)
{
    int     s32Ret = 0;
    char    szFormat[32] = {0};
    int     i, j, k;
    struct  vindev_devinfo    stVinDevInfo = {0};
    struct  vindev_video_info stVinVdoInfo = {0};
    int     s32IavFd = g_stViCfg[s32VinDevID].s32Fd;

    if(s32IavFd <= 0)
    {
        TIPC_PRINTF_ERR("/dev/iav check err! [%d]", s32IavFd);
        return -1;
    }

    // 获取指定设备号信息
    stVinDevInfo.vsrc_id = s32VinDevID;
    AM_IOCTL(s32IavFd, IAV_IOC_VIN_GET_DEVINFO, &stVinDevInfo);

    TIPC_PRINTF_INFO("\nFind Vin Source %s ", stVinDevInfo.name);

    // 获取设备源信息
    stVinVdoInfo.vsrc_status = AMBA_VSRC_STATUS_ACTIVE;
    if (stVinDevInfo.dev_type == VINDEV_TYPE_SENSOR) 
    {
        TIPC_PRINTF_INFO("it supports:");
        TIPC_PRINTF_INFO("Default mode:");
        stVinVdoInfo.vsrc_id = s32VinDevID;
        stVinVdoInfo.info.mode = AMBA_VIDEO_MODE_AUTO;
        stVinVdoInfo.info.hdr_mode = AMBA_VIDEO_LINEAR_MODE;
        stVinVdoInfo.info.bits = AMBA_VIDEO_BITS_AUTO;
        AM_IOCTL(s32IavFd, IAV_IOC_VIN_GET_VIDEOINFO, &stVinVdoInfo);
        amba_vin_printVdoInfo(s32VinDevID, &stVinVdoInfo.info, 0);
        TIPC_PRINTF_INFO("------------:");

        for (i = AMBA_VIDEO_MODE_AUTO; i < AMBA_VIDEO_MODE_NUM; i++)
        {
            if (i == AMBA_VIDEO_MODE_AUTO)
            {
                continue;
            }
            for (j = AMBA_VIDEO_MODE_FIRST; j < AMBA_VIDEO_MODE_LAST; j++) 
            {
                for (k = AMBA_VIDEO_BITS_8; k <= AMBA_VIDEO_BITS_16; k += 2) 
                {
                    stVinVdoInfo.vsrc_id = s32VinDevID;
                    stVinVdoInfo.info.mode = i;
                    stVinVdoInfo.info.hdr_mode = j;
                    stVinVdoInfo.info.bits = k;
                    s32Ret = ioctl(s32IavFd, IAV_IOC_VIN_GET_VIDEOINFO, &stVinVdoInfo);
                    if (s32Ret < 0) 
                    {
                        if (errno == EINVAL) 
                        {
                            errno = 0;
                            s32Ret = 0;
                            continue;
                        }
                        perror("IAV_IOC_VIN_GET_VIDEOINFO");
                        TIPC_PRINTF_ERR("IAV_IOC_VIN_GET_VIDEOINFO failed[%d]", s32Ret);
                        break;
                    }
                    amba_vin_printVdoInfo(s32VinDevID, &stVinVdoInfo.info, 0);
                }
            }
        }
    } 
    else 
    {
        switch(stVinDevInfo.sub_type) 
        {
            case VINDEV_SUBTYPE_CVBS:
                snprintf(szFormat, 32, "%s", "CVBS");
                break;
            case VINDEV_SUBTYPE_SVIDEO:
                snprintf(szFormat, 32, "%s", "S-Video");
                break;
            case VINDEV_SUBTYPE_YPBPR:
                snprintf(szFormat, 32, "%s", "YPbPr");
                break;
            case VINDEV_SUBTYPE_HDMI:
                snprintf(szFormat, 32, "%s", "HDMI");
                break;
            case VINDEV_SUBTYPE_VGA:
                snprintf(szFormat, 32, "%s", "VGA");
                break;
            case VINDEV_SUBTYPE_SDI:
                snprintf(szFormat, 32, "%s", "SDI");
                break;
            default:
                snprintf(szFormat, 32, "format?%d", stVinDevInfo.sub_type);
                break;
        }
        TIPC_PRINTF_INFO("Channel[%s]'s type is %s", stVinDevInfo.name, szFormat);

        stVinVdoInfo.info.mode = AMBA_VIDEO_MODE_AUTO;
        stVinVdoInfo.vsrc_id = s32VinDevID;
        AM_IOCTL(s32IavFd, IAV_IOC_VIN_GET_VIDEOINFO, &stVinVdoInfo);
        if (stVinVdoInfo.info.height == 0) 
        {
            TIPC_PRINTF_ERR("No signal yet.");
        } 
        else 
        {
            TIPC_PRINTF_INFO("The signal is:");
            amba_vin_printVdoInfo(s32VinDevID, &stVinVdoInfo.info, 0);
        }
    }
    return s32Ret;
}


/**
 * @brief 设置镜像，通过配置Sensor寄存器实现
 * @return 0：成功；非0：失败
 */
int amba_vin_dev_setMirror(int s32VinDevID, int s32Enable)
{
    int s32ApplyFlag = 0;
    int s32IavFD = g_stViCfg[0].s32Fd;
    if(s32IavFD <= 0)
    {
        TIPC_PRINTF_ERR("check /dev/iav fd failed");
        return -1;
    }
    struct vindev_mirror stMirrorMode = {0};
    stMirrorMode.vsrc_id = s32VinDevID;
    AM_IOCTL(s32IavFD, IAV_IOC_VIN_GET_MIRROR, &stMirrorMode);
    if(0 == s32Enable)
    {
        if(VINDEV_MIRROR_HORRIZONTALLY == stMirrorMode.pattern)
        {
            s32ApplyFlag = 1;
            stMirrorMode.pattern = VINDEV_MIRROR_NONE;
        }
        else if(VINDEV_MIRROR_HORRIZONTALLY_VERTICALLY == stMirrorMode.pattern)
        {
            s32ApplyFlag = 1;
            stMirrorMode.pattern = VINDEV_MIRROR_VERTICALLY;
        }
    }
    else
    {
        if(VINDEV_MIRROR_NONE == stMirrorMode.pattern)
        {
            s32ApplyFlag = 1;
            stMirrorMode.pattern = VINDEV_MIRROR_HORRIZONTALLY;
        }
        else if(VINDEV_MIRROR_VERTICALLY == stMirrorMode.pattern)
        {
            s32ApplyFlag = 1;
            stMirrorMode.pattern = VINDEV_MIRROR_HORRIZONTALLY_VERTICALLY;
        }
    }
    
    if(1 == s32ApplyFlag)
    {
        AM_IOCTL(s32IavFD, IAV_IOC_VIN_SET_MIRROR, &stMirrorMode);
    }

    return 0;
}

/**
 * @brief 设置翻转，通过配置Sensor寄存器实现
 * @return 0：成功；非0：失败
 */
int amba_vin_dev_setFlip(int s32VinDevID, int s32Enable)
{
    int s32ApplyFlag = 0;
    int s32IavFD = g_stViCfg[0].s32Fd;
    if(s32IavFD <= 0)
    {
        TIPC_PRINTF_ERR("check /dev/iav fd failed");
        return -1;
    }
    struct vindev_mirror stMirrorMode = {0};
    stMirrorMode.vsrc_id = s32VinDevID;
    AM_IOCTL(s32IavFD, IAV_IOC_VIN_GET_MIRROR, &stMirrorMode);
    if(0 == s32Enable)
    {
        if(VINDEV_MIRROR_VERTICALLY == stMirrorMode.pattern)
        {
            s32ApplyFlag = 1;
            stMirrorMode.pattern = VINDEV_MIRROR_NONE;
        }
        else if(VINDEV_MIRROR_HORRIZONTALLY_VERTICALLY == stMirrorMode.pattern)
        {
            s32ApplyFlag = 1;
            stMirrorMode.pattern = VINDEV_MIRROR_HORRIZONTALLY;
        }
    }
    else
    {
        if(VINDEV_MIRROR_NONE == stMirrorMode.pattern)
        {
            s32ApplyFlag = 1;
            stMirrorMode.pattern = VINDEV_MIRROR_VERTICALLY;
        }
        else if(VINDEV_MIRROR_HORRIZONTALLY == stMirrorMode.pattern)
        {
            s32ApplyFlag = 1;
            stMirrorMode.pattern = VINDEV_MIRROR_HORRIZONTALLY_VERTICALLY;
        }
    }
    
    if(1 == s32ApplyFlag)
    {
        AM_IOCTL(s32IavFD, IAV_IOC_VIN_SET_MIRROR, &stMirrorMode);
    }

    return 0;
}


/**
 * @brief Step1：设置dev属性，告诉Sensor当前设置的模式
 * @param [in] pstVideoCfg：Dev属性
 * @return 0：成功；非0：失败
 */
int amba_vin_dev_start(const CHIP_VIDEO_CFG_S* pstVideoCfg)
{
    int s32Ret = 0;
    int i = 0;
    if((NULL == pstVideoCfg))
    {
        TIPC_PRINTF_ERR("video cfg err");
        return -1;
    }

    if(pstVideoCfg->stSensorCfg.u32VinDev > CHIP_MAX_VI_NUM)
    {
        TIPC_PRINTF_ERR("vin input dev no [%d] is lager than max dev no[%d]", pstVideoCfg->stSensorCfg.u32VinDev, CHIP_MAX_VI_NUM);
        return -1;
    }
    int s32VinDevID = (int)pstVideoCfg->stSensorCfg.u32VinDev;
    AMBA_VIN_CFG_S* pstVinCfg = NULL;
    // 寻找对应的设备结构体
    for(i = 0; i < CHIP_MAX_VI_NUM; i++)
    {
        if(g_stViCfg[i].s32Dev == s32VinDevID)
        {
            pstVinCfg = &g_stViCfg[i];
            break;
        }
    }
    if(NULL == pstVinCfg)
    {
        // 找不到对应结构体
        TIPC_PRINTF_ERR("this vin dev pointer is NULL");
        return -1;
    }
    if(pstVinCfg->u8Init != 0)
    {
        // 该VI设备号已经初始化
        TIPC_PRINTF_ERR("this vin dev has been init, you should destroy it first");
        return -1;
    }
    
    int s32IavFD = pstVinCfg->s32Fd;
    s32Ret = amba_vin_checkSourceInfo(s32VinDevID);
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("vin check source info failed[%d]", s32Ret);
    }

    // 设置对应的模式
    struct vindev_mode stVinDevMode = {0};
    stVinDevMode.vsrc_id = s32VinDevID;
    stVinDevMode.video_mode = AMBA_VIDEO_MODE_AUTO;
    stVinDevMode.hdr_mode = (pstVideoCfg->enWorkMode == ISP_BLC_MODE_WDR)?AMBA_VIDEO_2X_HDR_MODE:AMBA_VIDEO_LINEAR_MODE; ///TODO: 根据实际需求设置为2x还是3x
    stVinDevMode.bits = AMBA_VIDEO_BITS_AUTO;
    stVinDevMode.vsrc_ctx_auto_switch = 0;
    stVinDevMode.vsrc_status = AMBA_VSRC_STATUS_ACTIVE;
    AM_IOCTL(s32IavFD, IAV_IOC_VIN_SET_MODE, &stVinDevMode);

    // 设置 Vin Dev 最大帧率
    struct vindev_fps stVsrcFps;
    stVsrcFps.vsrc_id = s32VinDevID;
    int s32Fps = (pstVideoCfg->enFormat == SDK_VIDEO_INPUT_PAL)?VIN_MAX_FPS_PAL:VIN_MAX_FPS_NTSC;
    stVsrcFps.fps = DIV_CLOSEST(512000000, s32Fps);
    if (ioctl(s32IavFD, IAV_IOC_VIN_SET_FPS, &stVsrcFps) < 0) 
    {
        TIPC_PRINTF_ERR("set vin fps failed[%d]", s32Ret);
        perror("IAV_IOC_VIN_SET_FPS");
    }

    // 更新结构体信息，设置标志位
    // pthread_mutex_init(&pstVinCfg->s32Mutex, NULL);
    pstVinCfg->u8Init = 1;
    pstVinCfg->s32Dev = s32VinDevID;

    return s32Ret;
}

/**
 * @brief 初始化模块
 * @return 0：成功；非0：失败
 */
int amba_vin_init(int s32IavFD)
{
    int i = 0;

    if(s32IavFD < 0)
    {
        TIPC_PRINTF_ERR("iav fd err[%d]", s32IavFD);
        return -1;
    }

    for (i = 0; i < CHIP_MAX_VI_NUM; i++)
    {
        memset(&g_stViCfg[i], 0,  sizeof(AMBA_VIN_CFG_S));
        g_stViCfg[i].s32Fd = s32IavFD;
        g_stViCfg[i].s32Dev = i; // 相当于amba vsrc_id
        g_stViCfg[i].s32Chn = i;
        g_stViCfg[i].u8Init = 0;
    }

    return 0;
}


/**
 * @brief 去初始化模块
 * @return 0：成功；非0：失败
 */
int amba_vin_exit()
{
    int i = 0;

    for (i = 0; i < CHIP_MAX_VI_NUM; i++)
    {
        if(1 == g_stViCfg[i].u8Init)
        {
            // pthread_mutex_destroy(&g_stViCfg[i].s32Mutex);
        }
    }

    return 0;
}