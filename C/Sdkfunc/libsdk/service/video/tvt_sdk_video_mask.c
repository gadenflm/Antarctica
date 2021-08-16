#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "tvt_sdk_video_mask.h"
#include "tvt_sdk_print.h"
#include "tvt_sdk_chip.h"

#define TVT_SDK_MASK_MAX_COUNT 4

#define APP_MASK_REF_PIC_W  640    // 应用层下发的位置所在画面宽
#define APP_MASK_REF_PIC_H  480    // 应用层下发的位置所在画面高

/*!
 * @macros ROUND_UP(x,d)
 * @brief round up func.
 */
#define ROUND_UP(x, n)  ( ((x)+(n)-1u) & ~((n)-1u) )

#ifdef CHIP_AMBA
#define MASK_ALIGN_W 32
#define MASK_ALIGN_H 4
#else
#define MASK_ALIGN_W 16
#define MASK_ALIGN_H 2
#endif

/**
 * @brief 业务状态
 */
typedef enum tvt_sdk_mask_status_e
{
    TVT_SDK_MASK_RUN = 0,
    TVT_SDK_MASK_PAUSE,
    TVT_SDK_MASK_STATUS_BUTT,
}TVT_SDK_MASK_STATUS_E;


/**
 * @brief MASK内容信息
 */
typedef struct tvt_sdk_mask_data_info_s
{
    unsigned int                u32Enable;          ///< 使能
    SDK_POSITION                stSrcPos;           ///< 相对位置
    SDK_POSITION                stDstPos;           ///< 最终显示位置
} TVT_SDK_MASK_DATA_INFO_S;


/**
 * @brief MASK业务全局配置
 */
typedef struct tvt_sdk_mask_cfg_s
{
    int                         s32VinDev;          ///< 所在视频输入设备号
    unsigned int                u32Colour;          ///< 颜色
    TVT_SDK_MASK_DATA_INFO_S    stMaskInfo[TVT_SDK_MASK_MAX_COUNT];       ///< 每个MASK的具体信息
} TVT_SDK_MASK_CFG_S;

/**
 * @brief 业务结构体
 */
typedef struct tvt_sdk_mask_info_s
{
    char                        u8Init;                                 ///< 初始化标志位
    char                        u8Exit;                                 ///< 去初始化标志位，线程退出标志
    TVT_SDK_MASK_STATUS_E       enStatus;                               ///< 业务运行状态
    pthread_mutex_t             s32Mutex;                               ///< 业务锁
    char                        szMutexName[32];                        ///< 业务锁名称
    unsigned int                u32MutexLockCnt;                        ///< 业务锁锁定次数
    TVT_SDK_MASK_CFG_S          stMaskCfg[CHIP_MAX_VI_NUM];             ///< MASK配置
}TVT_SDK_MASK_INFO_S;

static TVT_SDK_MASK_INFO_S* g_pstSdkMaskInfo = NULL;

/**
 * @brief 获取模块结构体指针
 */
static TVT_SDK_MASK_INFO_S* tvt_sdk_mask_getHandle(void)
{
    return g_pstSdkMaskInfo;
}


/**
 * @brief 设置MASK位置以及内容
 * @return 0：成功；非0：失败
 */
int tvt_sdk_mask_setPos(int s32VinIDX, int s32Enable, const SDK_POSITION* pstPos)
{
    int s32Ret = 0;
    TVT_SDK_MASK_INFO_S* pstHandle = tvt_sdk_mask_getHandle();
    if(NULL == pstHandle || NULL == pstPos)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    if(s32VinIDX >= CHIP_MAX_VI_NUM)
    {
        TIPC_PRINTF_ERR("err Vin Index[%d]", s32VinIDX);
        return -1;
    }
    int s32VinDevID = pstHandle->stMaskCfg[s32VinIDX].s32VinDev;
    if(s32VinDevID < 0)
    {
        TIPC_PRINTF_ERR("err Vin DevID[%d] with Vin Index[%d] ", s32VinDevID, s32VinIDX);
        return -1;
    }

    // 获取Vin宽高
    CHIP_VIDEO_CFG_S stVinCfg = {0};
    stVinCfg.stSensorCfg.u32VinDev = (int)s32VinDevID;
    s32Ret = chip_video_getVinCfg(&stVinCfg);
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("err chip_video_getVinCfg");
        return -1;
    }
    int s32Width = stVinCfg.stSensorCfg.u32MaxWidth;
    int s32Height = stVinCfg.stSensorCfg.u32MaxHeight;
    if(s32Width <= 0 || s32Height <= 0)
    {
        TIPC_PRINTF_ERR("err Vin width[%d] height[%d] ", s32Width, s32Height);
        return -1;
    }

    // 分辨率转换
    SDK_POSITION stDstPos = {0};
    stDstPos.x = pstPos->x * s32Width / APP_MASK_REF_PIC_W;
    stDstPos.y = pstPos->y * s32Height / APP_MASK_REF_PIC_H;
    stDstPos.width = pstPos->width * s32Width / APP_MASK_REF_PIC_W;
    stDstPos.height = pstPos->height * s32Height / APP_MASK_REF_PIC_H;

    // 应用
    CHIP_MASK_INFO_S stMaskInfo = {0};
    stMaskInfo.s32Enable = s32Enable;
    stMaskInfo.s32X = stDstPos.x;
    stMaskInfo.s32Y = stDstPos.y;
    stMaskInfo.s32W = stDstPos.width;
    stMaskInfo.s32H = stDstPos.height;
    stMaskInfo.s32VinDevID = s32VinDevID;
    s32Ret = chip_video_setMask(&stMaskInfo);
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("err chip_video_setMask ");
        return -1;
    }

    return 0;
}


/**
 * @brief 初始化视频MASK业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_mask_init(int s32VinNum, const int* s32VinDevID)
{
    int s32Ret = -1;
    int i = 0;
    if(g_pstSdkMaskInfo != NULL)
    {
        if(g_pstSdkMaskInfo->u8Init != 0)
        {
            // 已初始化
            TIPC_PRINTF_ERR("video mask service is running");
            return 0;
        }
    }
    else
    {
        // 创建结构体
        g_pstSdkMaskInfo = (TVT_SDK_MASK_INFO_S* )malloc(sizeof(TVT_SDK_MASK_INFO_S));
        if(NULL == g_pstSdkMaskInfo)
        {
            TIPC_PRINTF_ERR("video mask service malloc failed");
            return s32Ret;
        }
        // 初始化参数
        memset(g_pstSdkMaskInfo, 0, sizeof(TVT_SDK_MASK_INFO_S));
        for(i = 0; i < CHIP_MAX_VI_NUM; i++)
        {
            g_pstSdkMaskInfo->stMaskCfg[i].s32VinDev = -1;
        }
    }

    // 初始化模块锁
    pthread_mutex_init(&g_pstSdkMaskInfo->s32Mutex, NULL);
    sprintf(g_pstSdkMaskInfo->szMutexName, "tvt_sdk_mask_lock");
    g_pstSdkMaskInfo->u8Init = 1;
    g_pstSdkMaskInfo->enStatus = TVT_SDK_MASK_RUN;
    for(i = 0; i < s32VinNum; i++)
    {
        g_pstSdkMaskInfo->stMaskCfg[i].s32VinDev = s32VinDevID[i];
    }
    return 0;
}


/**
 * @brief 去初始化视频MASK业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_mask_exit()
{
    if(NULL == g_pstSdkMaskInfo)
    {
        // 未初始化
        TIPC_PRINTF_ERR("video mask service is not running");
        return -1;
    }

    if (g_pstSdkMaskInfo->u8Init != 1)
    {
        // 未初始化
        TIPC_PRINTF_ERR("video mask service is not running");
        free((void*)g_pstSdkMaskInfo);
        g_pstSdkMaskInfo = NULL;
        return -1;
    }
    else
    {
        pthread_mutex_destroy(&g_pstSdkMaskInfo->s32Mutex);
        g_pstSdkMaskInfo->u8Init = 0;
        free((void*)g_pstSdkMaskInfo);
        g_pstSdkMaskInfo = NULL;
    }
    return 0;
}