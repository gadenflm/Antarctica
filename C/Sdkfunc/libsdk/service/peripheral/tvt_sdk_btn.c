#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "tvt_sdk_btn.h"
#include "tvt_sdk_print.h"
#include "tvt_sdk_chip.h"

#define TVT_SDK_BTN_DEBUG 0

#define TVT_SDK_BTN_MAX_HOLD_CNT 3
#define TVT_SDK_BTN_CHK_INTERVAL_MS 100

typedef enum tvt_sdk_btn_status_e
{
    TVT_SDK_BTN_NONE,
    TVT_SDK_BTN_FALLING,
    TVT_SDK_BTN_RISING,
    TVT_SDK_BTN_STATUS_BUTT,
} TVT_SDK_BTN_STATUS_E;

typedef struct tvt_sdk_data_s
{
    int s32Exist;
    int s32HoldCnt;
    int s32GpioLastVal;
    TVT_SDK_BTN_STATUS_E enGpioStatus;
    TVT_SDK_BTN_CFG_S stBtnCfg;
} TVT_SDK_BTN_DATA_S;

/**
 * @brief 业务结构体
 */
typedef struct tvt_sdk_btn_info_s
{
    char                                 u8Init;                            ///< 初始化标志位
    char                                 u8Exit;                            ///< 去初始化标志位，线程退出标志
    pthread_mutex_t                      s32Mutex;                          ///< 业务锁
    char                                 szMutexName[32];                   ///< 业务锁名称
    unsigned int                         u32MutexLockCnt;                   ///< 业务锁锁定次数
    pthread_t                            s32ThreadID;                       ///< 业务线程ID
    int                                  s32ThreadRunFlag;                  ///< 业务线程运行标识
    TVT_SDK_BTN_DATA_S                   stBtnData[TVT_SDK_BTN_MAX];        
    int                                  s32ReserveArr[4];
}TVT_SDK_BTN_INFO_S;

static TVT_SDK_BTN_INFO_S* g_pstSdkBtnInfo = NULL; 

/**
 * @brief 获取模块结构体指针
 */
static TVT_SDK_BTN_INFO_S* tvt_sdk_btn_getHandle(void)
{
    return g_pstSdkBtnInfo;
}

void* tvt_sdk_btn_event_thread(void* pParam);

/**
 * @brief 注册按键事件
 */
int tvt_sdk_btn_addBtnEvent(TVT_SDK_BTN_CFG_S* pstBtnCfg)
{
    int i = 0;
    TVT_SDK_BTN_INFO_S* pstHandle = tvt_sdk_btn_getHandle();
    if(NULL == pstHandle || NULL == pstBtnCfg)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    // 检查启动线程
    if(0 == pstHandle->s32ThreadRunFlag)
    {
        pstHandle->s32ThreadRunFlag = 1;
        pthread_create(&pstHandle->s32ThreadID, NULL, tvt_sdk_btn_event_thread, NULL);
    }
    for(i = 0; i < TVT_SDK_BTN_MAX; i++)
    {
        int s32Exist = pstHandle->stBtnData[i].s32Exist;
        int s32GpioGrp = pstHandle->stBtnData[i].stBtnCfg.s32GpioGrp;
        int s32GpioNum = pstHandle->stBtnData[i].stBtnCfg.s32GpioNum;
        if(s32Exist != 0)
        {
            if(s32GpioGrp == pstBtnCfg->s32GpioGrp && s32GpioNum == pstBtnCfg->s32GpioNum)
            {
                TIPC_MUTEX_LOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_BTN_DEBUG);
                pstHandle->stBtnData[i].stBtnCfg.s32EventNum = pstBtnCfg->s32EventNum;
                memcpy(pstHandle->stBtnData[i].stBtnCfg.stBtnEvent, pstBtnCfg->stBtnEvent, TVT_SDK_BTN_EVENT_MAX * sizeof(TVT_SDK_BTN_EVENT_S));
                TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_BTN_DEBUG);
                return 0;
            }
            continue;
        }
        else
        {
            TIPC_MUTEX_LOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_BTN_DEBUG);
            pstHandle->stBtnData[i].stBtnCfg.s32EventNum = pstBtnCfg->s32EventNum;
            pstHandle->stBtnData[i].stBtnCfg.s32GpioGrp = pstBtnCfg->s32GpioGrp;
            pstHandle->stBtnData[i].stBtnCfg.s32GpioNum = pstBtnCfg->s32GpioNum;
            memcpy(pstHandle->stBtnData[i].stBtnCfg.stBtnEvent, pstBtnCfg->stBtnEvent, TVT_SDK_BTN_EVENT_MAX * sizeof(TVT_SDK_BTN_EVENT_S));
            pstHandle->stBtnData[i].s32Exist = 1;
            TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_BTN_DEBUG);
            return 0;
        }
    }

    if(i >= TVT_SDK_BTN_MAX)
    {
        TIPC_PRINTF_ERR("btn event full");
        return -1;
    }

    return 0;
}

/**
 * @brief 检查GPIO Val
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_btn_chkVal(TVT_SDK_BTN_DATA_S* pstBtnData)
{
    if (NULL == pstBtnData)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }

    int i = 0;
    int s32Ret = 0;
    for (i = 0; i < TVT_SDK_BTN_MAX; i++)
    {
        TVT_SDK_BTN_DATA_S* pstBtnDataTmp = &pstBtnData[i];

        if(pstBtnDataTmp->s32Exist != 1)
        {
            continue;
        }

        // 获取GPIO值
        int s32GpioGrp = pstBtnDataTmp->stBtnCfg.s32GpioGrp;
        int s32GpioNum = pstBtnDataTmp->stBtnCfg.s32GpioNum;
        int s32GpioVal = 0;
        s32Ret = chip_dev_getGpioVal(s32GpioGrp, s32GpioNum, &s32GpioVal);
        if(s32Ret != 0)
        {
            TIPC_PRINTF_ERR("Get Gpio[%d][%d] Val failed");
            continue;
        }

        // 检查状态
        if(s32GpioVal == pstBtnDataTmp->s32GpioLastVal)
        {
            pstBtnDataTmp->enGpioStatus = TVT_SDK_BTN_NONE;
            pstBtnDataTmp->s32HoldCnt = 0;
        }
        else if(s32GpioVal != pstBtnDataTmp->s32GpioLastVal)
        {
            pstBtnDataTmp->s32HoldCnt++;
            if(pstBtnDataTmp->s32HoldCnt > TVT_SDK_BTN_MAX_HOLD_CNT)
            {
                if(s32GpioVal == 0)
                {
                    pstBtnDataTmp->enGpioStatus = TVT_SDK_BTN_FALLING;
                }
                else
                {
                    pstBtnDataTmp->enGpioStatus = TVT_SDK_BTN_RISING;
                }
                pstBtnDataTmp->s32HoldCnt = 0;
                pstBtnDataTmp->s32GpioLastVal = s32GpioVal;
            }
        }
    }

    return 0;
}

/**
 * @brief 应用状态
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_btn_applyStatus(TVT_SDK_BTN_DATA_S* pstBtnData)
{
    if (NULL == pstBtnData)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }

    int i = 0, j = 0;
    for (i = 0; i < TVT_SDK_BTN_MAX; i++)
    {
        TVT_SDK_BTN_DATA_S* pstBtnDataTmp = &pstBtnData[i];
        if(pstBtnDataTmp->s32Exist != 1)
        {
            continue;
        }
        // 检查应用状态
        if(pstBtnDataTmp->enGpioStatus != TVT_SDK_BTN_FALLING || pstBtnDataTmp->enGpioStatus != TVT_SDK_BTN_RISING)
        {
            continue;
        }
        TVT_SDK_BTN_EVENT_E enBtnEvent = (TVT_SDK_BTN_FALLING == pstBtnDataTmp->enGpioStatus)?TVT_SDK_BTN_EVENT_DOWN:TVT_SDK_BTN_EVENT_UP;
        for(j = 0; j < TVT_SDK_BTN_EVENT_MAX; j++)
        {
            if(enBtnEvent == pstBtnDataTmp->stBtnCfg.stBtnEvent[j].enEvent && pstBtnDataTmp->stBtnCfg.stBtnEvent[j].pfBtnCb != NULL)
            {
                pstBtnDataTmp->stBtnCfg.stBtnEvent[j].pfBtnCb(pstBtnDataTmp->stBtnCfg.stBtnEvent[j].pCbParam);
            }
        }
    }
    return 0;
}


/**
 * @brief 按键监控线程
 * @return 0：成功；非0：失败
 */
void* tvt_sdk_btn_event_thread(void* pParam)
{
    TVT_SDK_BTN_INFO_S* pstHandle = tvt_sdk_btn_getHandle();
    if(NULL == pstHandle)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return NULL;
    }
    while(1 == pstHandle->s32ThreadRunFlag)
    {
        TIPC_MUTEX_LOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_BTN_DEBUG);
        tvt_sdk_btn_chkVal(pstHandle->stBtnData);
        tvt_sdk_btn_applyStatus(pstHandle->stBtnData);
        TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_BTN_DEBUG);
        usleep(TVT_SDK_BTN_CHK_INTERVAL_MS * 1000);
    }
    return NULL;
}


/**
 * @brief 初始化按键监控业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_btn_init()
{
    int s32Ret = -1;

    if(g_pstSdkBtnInfo != NULL)
    {
        if(g_pstSdkBtnInfo->u8Init != 0)
        {
            // 已初始化
            TIPC_PRINTF_ERR("video service is running");
            return 0;
        }
    }
    else
    {
        // 创建结构体
        g_pstSdkBtnInfo = (TVT_SDK_BTN_INFO_S* )malloc(sizeof(TVT_SDK_BTN_INFO_S));
        if(NULL == g_pstSdkBtnInfo)
        {
            TIPC_PRINTF_ERR("video service malloc failed");
            return s32Ret;
        }
        memset(g_pstSdkBtnInfo, 0, sizeof(TVT_SDK_BTN_INFO_S));
    }

    // 初始化模块锁
    pthread_mutex_init(&g_pstSdkBtnInfo->s32Mutex, NULL);
    sprintf(g_pstSdkBtnInfo->szMutexName, "tvt_sdk_btn_lock");
    g_pstSdkBtnInfo->u8Init = 1;

    return s32Ret;
}

/**
 * @brief 去初始化按键监控业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_btn_exit()
{
    if(NULL == g_pstSdkBtnInfo)
    {
        // 未初始化
        TIPC_PRINTF_ERR("device service is not running");
        return -1;
    }

    if (g_pstSdkBtnInfo->u8Init != 1)
    {
        // 未初始化
        TIPC_PRINTF_ERR("device service is not running");
        free((void*)g_pstSdkBtnInfo);
        g_pstSdkBtnInfo = NULL;
        return -1;
    }
    if(1 == g_pstSdkBtnInfo->s32ThreadRunFlag)
    {
        g_pstSdkBtnInfo->s32ThreadRunFlag = 0;
        pthread_join(g_pstSdkBtnInfo->s32ThreadID, NULL);
    }
    pthread_mutex_destroy(&g_pstSdkBtnInfo->s32Mutex);

    free((void*)g_pstSdkBtnInfo);
    g_pstSdkBtnInfo = NULL;

    return 0;
}