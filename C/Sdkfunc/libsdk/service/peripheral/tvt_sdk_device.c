
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "tvt_sdk_device.h"
#include "tvt_sdk_print.h"
#include "tvt_sdk_chip.h"

#define TVT_SDK_DEVICE_DEBUG 0

/**
 * @brief 业务结构体
 */
typedef struct tvt_sdk_device_info_s
{
    char                                 u8Init;                            ///< 初始化标志位
    char                                 u8Exit;                            ///< 去初始化标志位，线程退出标志
    pthread_mutex_t                      s32Mutex;                          ///< 业务锁
    char                                 szMutexName[32];                   ///< 业务锁名称
    unsigned int                         u32MutexLockCnt;                   ///< 业务锁锁定次数
    pthread_t                            s32ThreadID;                       ///< 业务线程ID
    int                                  s32ThreadRunFlag;                  ///< 业务线程运行标识
    TVT_SDK_PROD_DEV_INFO                stParamDevInfo;                    ///< 
    TvtSdkProdCb                         pSdkParamCbFunc;                   ///< 参数回调函数
    int                                  s32ReserveArr[4];
}TVT_SDK_DEVICE_INFO_S;

static TVT_SDK_DEVICE_INFO_S* g_pstSdkDeviceInfo = NULL; 

typedef int (*DevImplFunc)(TVT_SDK_DEVICE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize);
typedef struct tvt_sdk_device_ctrl_impl_s
{
    TVT_SDK_DEVICE_CMD_E        enCmd;          ///< 命令字
    DevImplFunc                 pImplFunc;      ///< 实现接口
}TVT_SDK_DEVICE_CTRL_IMPL_S;

/**
 * @brief 获取模块结构体指针
 */
static TVT_SDK_DEVICE_INFO_S* tvt_sdk_dev_getHandle(void)
{
    return g_pstSdkDeviceInfo;
}

/**
 * @brief RTC控制
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_dev_rtcCtrl(TVT_SDK_DEVICE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    TVT_SDK_DEVICE_RTC_S* pstRtcInfo = (TVT_SDK_DEVICE_RTC_S*)pParam;
    CHIP_DEV_RTC_CFG_S stRtcInfo = {0};
    if(TVT_SDK_DEV_CTRL_GET == s32Dir)
    {
        s32Ret = chip_dev_getRtcTime(&stRtcInfo);
        if(s32Ret != 0)
        {
            TIPC_PRINTF_ERR("chip_dev_getRtcTime failed\n");
            return s32Ret;
        }
        pstRtcInfo->u32Date = stRtcInfo.u32Date;
        pstRtcInfo->u32Hour = stRtcInfo.u32Hour;
        pstRtcInfo->u32Minute = stRtcInfo.u32Minute;
        pstRtcInfo->u32Month = stRtcInfo.u32Month;
        pstRtcInfo->u32Second = stRtcInfo.u32Second;
        pstRtcInfo->u32Weekday = stRtcInfo.u32Weekday;
        pstRtcInfo->u32Year = stRtcInfo.u32Year;
    }
    else
    {
        stRtcInfo.u32Date = pstRtcInfo->u32Date;
        stRtcInfo.u32Hour = pstRtcInfo->u32Hour;
        stRtcInfo.u32Minute = pstRtcInfo->u32Minute;
        stRtcInfo.u32Month = pstRtcInfo->u32Month;
        stRtcInfo.u32Second = pstRtcInfo->u32Second;
        stRtcInfo.u32Weekday = pstRtcInfo->u32Weekday;
        stRtcInfo.u32Year = pstRtcInfo->u32Year;
        s32Ret = chip_dev_setRtcTime(&stRtcInfo);
        if(s32Ret != 0)
        {
            TIPC_PRINTF_ERR("chip_dev_setRtcTime failed\n");
            return s32Ret;
        }
    }
    return s32Ret;
}

/**
 * @brief 复位管脚控制
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_dev_resetCtrl(TVT_SDK_DEVICE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int* ps32Value = (int*)pParam;
    if(NULL == pstHandle->pSdkParamCbFunc)
    {
        TIPC_PRINTF_ERR("param callback should not be NULL\n");
        return -1;
    }
    if(s32Dir != TVT_SDK_DEV_CTRL_GET)
    {
        TIPC_PRINTF_ERR("unsupport ctrl direction[%s]\n", (TVT_SDK_DEV_CTRL_SET == s32Dir)?"set":"get");
        return -1;
    }
    // 获取设备参数
    TVT_SDK_PROD_CTRL_INFO stParamCtrl = {0};
    memcpy(&stParamCtrl.stDevInfo, &pstHandle->stParamDevInfo, sizeof(TVT_SDK_PROD_DEV_INFO));
    s32Ret = pstHandle->pSdkParamCbFunc(TVT_SDK_PARAM_GET_RESET_BTN_CFG, &stParamCtrl, sizeof(stParamCtrl));
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get param cfg failed[%d]!!", s32Ret);
        return s32Ret;
    }
    TVT_SDK_PROD_RESET_BTN_S* pstResetBtnCfg = (TVT_SDK_PROD_RESET_BTN_S*)stParamCtrl.pParam;
    if(NULL == pstResetBtnCfg)
    {
        TIPC_PRINTF_ERR("button config is NULL!!");
        return -1;
    }
    int s32GpioVal = 0;
    s32Ret = chip_dev_getGpioVal(pstResetBtnCfg->s32GpioGrp, pstResetBtnCfg->s32GpioNum, &s32GpioVal);
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("chip_dev_getGpioVal failed\n");
        return s32Ret;
    }
    *ps32Value = s32GpioVal;
    return s32Ret;
}

/**
 * @brief ABF管脚控制
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_dev_ABFCtrl(TVT_SDK_DEVICE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int* ps32Value = (int*)pParam;
    if(NULL == pstHandle->pSdkParamCbFunc)
    {
        TIPC_PRINTF_ERR("param callback should not be NULL\n");
        return -1;
    }
    if(s32Dir != TVT_SDK_DEV_CTRL_GET)
    {
        TIPC_PRINTF_ERR("unsupport ctrl direction[%s]\n", (TVT_SDK_DEV_CTRL_SET == s32Dir)?"set":"get");
        return -1;
    }
    // 获取设备参数
    TVT_SDK_PROD_CTRL_INFO stParamCtrl = {0};
    memcpy(&stParamCtrl.stDevInfo, &pstHandle->stParamDevInfo, sizeof(TVT_SDK_PROD_DEV_INFO));
    s32Ret = pstHandle->pSdkParamCbFunc(TVT_SDK_PARAM_GET_ABF_BTN_CFG, &stParamCtrl, sizeof(stParamCtrl));
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get param cfg failed[%d]!!", s32Ret);
        return s32Ret;
    }
    TVT_SDK_PROD_ABF_BTN_S* pstABFBtnCfg = (TVT_SDK_PROD_ABF_BTN_S*)stParamCtrl.pParam;
    if(NULL == pstABFBtnCfg)
    {
        TIPC_PRINTF_ERR("button config is NULL!!");
        return -1;
    }
    int s32GpioVal = 0;
    s32Ret = chip_dev_getGpioVal(pstABFBtnCfg->s32GpioGrp, pstABFBtnCfg->s32GpioNum, &s32GpioVal);
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("chip_dev_getGpioVal failed\n");
        return s32Ret;
    }
    *ps32Value = s32GpioVal;
    return s32Ret;
}

/**
 * @brief ircut控制
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_dev_ircutCtrl(TVT_SDK_DEVICE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    if(NULL == pstHandle->pSdkParamCbFunc)
    {
        TIPC_PRINTF_ERR("param callback should not be NULL\n");
        return -1;
    }
    if(s32Dir != TVT_SDK_DEV_CTRL_SET)
    {
        TIPC_PRINTF_ERR("unsupport ctrl direction[%s]\n", (TVT_SDK_DEV_CTRL_SET == s32Dir)?"set":"get");
        return -1;
    }
    // 获取设备参数
    TVT_SDK_PROD_CTRL_INFO stParamCtrl = {0};
    memcpy(&stParamCtrl.stDevInfo, &pstHandle->stParamDevInfo, sizeof(TVT_SDK_PROD_DEV_INFO));
    s32Ret = pstHandle->pSdkParamCbFunc(TVT_SDK_PARAM_GET_IRCUT_CFG, &stParamCtrl, sizeof(stParamCtrl));
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get param cfg failed[%d]!!", s32Ret);
        return s32Ret;
    }
    TVT_SDK_PROD_IRCUT_S* pstIrcutCfg = (TVT_SDK_PROD_IRCUT_S*)stParamCtrl.pParam;
    if(NULL == pstIrcutCfg)
    {
        TIPC_PRINTF_ERR("ircut config is NULL!!");
        return -1;
    }
    int s32GpioVal = *(int*)pParam;
    s32Ret = chip_dev_setGpioVal(pstIrcutCfg->s32GpioGrp, pstIrcutCfg->s32GpioNum, s32GpioVal);
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("chip_dev_getGpioVal failed\n");
        return s32Ret;
    }
    return s32Ret;
}

/**
 * @brief 软光敏值控制
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_dev_CDSCtrl(TVT_SDK_DEVICE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    
    return s32Ret;
}

/**
 * @brief 串口数据的获取与写入
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_dev_uartData(TVT_SDK_DEVICE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    TVT_SDK_DEVICE_UART_DATA_S* pstUartData = (TVT_SDK_DEVICE_UART_DATA_S*)pParam;
    if(NULL == pstHandle->pSdkParamCbFunc)
    {
        TIPC_PRINTF_ERR("param callback should not be NULL\n");
        return -1;
    }

    // 获取设备参数
    TVT_SDK_PROD_CTRL_INFO stParamCtrl = {0};
    memcpy(&stParamCtrl.stDevInfo, &pstHandle->stParamDevInfo, sizeof(TVT_SDK_PROD_DEV_INFO));
    s32Ret = pstHandle->pSdkParamCbFunc(TVT_SDK_PARAM_GET_UART_CFG, &stParamCtrl, sizeof(stParamCtrl));
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get param cfg failed[%d]!!", s32Ret);
        return s32Ret;
    }
    TVT_SDK_PROD_UART_S* pstUartCfg = (TVT_SDK_PROD_UART_S*)stParamCtrl.pParam;
    if(NULL == pstUartCfg)
    {
        TIPC_PRINTF_ERR("uart config is NULL!!");
        return -1;
    }

    if(TVT_SDK_DEV_CTRL_GET == s32Dir)
    {
        s32Ret = chip_dev_getUartData(pstUartCfg->s32UartID, pstUartData->pszData, pstUartData->s32DataLens);
        if(s32Ret != 0)
        {
            TIPC_PRINTF_ERR("chip_dev_getUartData failed\n");
            return s32Ret;
        }
    }
    else
    {
        s32Ret = chip_dev_setUartData(pstUartCfg->s32UartID, pstUartData->pszData, pstUartData->s32DataLens);
        if(s32Ret != 0)
        {
            TIPC_PRINTF_ERR("chip_dev_getUartData failed\n");
            return s32Ret;
        }
    }
    
    return s32Ret;
}


/**
 * @brief 串口控制
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_dev_uartCfg(TVT_SDK_DEVICE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int s32UartSpeed = *(int*)pParam;
    if(NULL == pstHandle->pSdkParamCbFunc)
    {
        TIPC_PRINTF_ERR("param callback should not be NULL\n");
        return -1;
    }
    if(s32Dir != TVT_SDK_DEV_CTRL_SET)
    {
        TIPC_PRINTF_ERR("unsupport ctrl direction[%s]\n", (TVT_SDK_DEV_CTRL_SET == s32Dir)?"set":"get");
        return -1;
    }
    // 获取设备参数
    TVT_SDK_PROD_CTRL_INFO stParamCtrl = {0};
    memcpy(&stParamCtrl.stDevInfo, &pstHandle->stParamDevInfo, sizeof(TVT_SDK_PROD_DEV_INFO));
    s32Ret = pstHandle->pSdkParamCbFunc(TVT_SDK_PARAM_GET_UART_CFG, &stParamCtrl, sizeof(stParamCtrl));
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get param cfg failed[%d]!!", s32Ret);
        return s32Ret;
    }
    TVT_SDK_PROD_UART_S* pstUartCfg = (TVT_SDK_PROD_UART_S*)stParamCtrl.pParam;
    if(NULL == pstUartCfg)
    {
        TIPC_PRINTF_ERR("uart config is NULL!!");
        return -1;
    }
    s32Ret = chip_dev_setUartSpeed(pstUartCfg->s32UartID, s32UartSpeed);
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("chip_dev_getUartData failed\n");
        return s32Ret;
    }
    return s32Ret;
}

/**
 * @brief 安全码控制
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_dev_securityInfo(TVT_SDK_DEVICE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    
    return s32Ret;
}

/**
 * @brief 告警输入控制
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_dev_alarmInCtrl(TVT_SDK_DEVICE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int i= 0;
    int* ps32Value = (int*)pParam;
    if(NULL == pstHandle->pSdkParamCbFunc)
    {
        TIPC_PRINTF_ERR("param callback should not be NULL\n");
        return -1;
    }
    if(s32Dir != TVT_SDK_DEV_CTRL_GET)
    {
        TIPC_PRINTF_ERR("unsupport ctrl direction[%s]\n", (TVT_SDK_DEV_CTRL_SET == s32Dir)?"set":"get");
        return -1;
    }
    // 获取设备参数
    TVT_SDK_PROD_CTRL_INFO stParamCtrl = {0};
    memcpy(&stParamCtrl.stDevInfo, &pstHandle->stParamDevInfo, sizeof(TVT_SDK_PROD_DEV_INFO));
    s32Ret = pstHandle->pSdkParamCbFunc(TVT_SDK_PARAM_GET_ALARMIN_CFG, &stParamCtrl, sizeof(stParamCtrl));
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get param cfg failed[%d]!!", s32Ret);
        return s32Ret;
    }
    TVT_SDK_PROD_ALARM_S* pstAlarmInCfg = (TVT_SDK_PROD_ALARM_S*)stParamCtrl.pParam;
    if(NULL == pstAlarmInCfg)
    {
        TIPC_PRINTF_ERR("alarm config is NULL!!");
        return -1;
    }
    // 计算告警输入个数
    int s32AlarmInNum = stParamCtrl.s32ParamSize / sizeof(TVT_SDK_PROD_ALARM_S);
    int s32AlarmVal = 0;
    for(i = 0; i < s32AlarmInNum; i++)
    {
        int s32GpioVal = 0;
        TVT_SDK_PROD_ALARM_S* pstAlarmInCfgTmp = &pstAlarmInCfg[i];
        s32Ret = chip_dev_getGpioVal(pstAlarmInCfgTmp->s32GpioGrp, pstAlarmInCfgTmp->s32GpioNum, &s32GpioVal);
        if(s32Ret != 0)
        {
            TIPC_PRINTF_ERR("chip_dev_getGpioVal failed\n");
            continue;
        }
        s32AlarmVal |= (s32GpioVal << i);
    }
    
    *ps32Value = s32AlarmVal;
    return s32Ret;
}


/**
 * @brief 告警输出控制
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_dev_alarmOutCtrl(TVT_SDK_DEVICE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    TVT_SDK_DEVICE_ALARM_OUT_S* pstAlarmOut = (TVT_SDK_DEVICE_ALARM_OUT_S*)pParam;
    if(NULL == pstHandle->pSdkParamCbFunc)
    {
        TIPC_PRINTF_ERR("param callback should not be NULL\n");
        return -1;
    }
    if(s32Dir != TVT_SDK_DEV_CTRL_SET)
    {
        TIPC_PRINTF_ERR("unsupport ctrl direction[%s]\n", (TVT_SDK_DEV_CTRL_SET == s32Dir)?"set":"get");
        return -1;
    }
    // 获取设备参数
    TVT_SDK_PROD_CTRL_INFO stParamCtrl = {0};
    memcpy(&stParamCtrl.stDevInfo, &pstHandle->stParamDevInfo, sizeof(TVT_SDK_PROD_DEV_INFO));
    s32Ret = pstHandle->pSdkParamCbFunc(TVT_SDK_PARAM_GET_ALARMOUT_CFG, &stParamCtrl, sizeof(stParamCtrl));
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get param cfg failed[%d]!!", s32Ret);
        return s32Ret;
    }
    TVT_SDK_PROD_ALARM_S* pstAlarmOutCfg = (TVT_SDK_PROD_ALARM_S*)stParamCtrl.pParam;
    if(NULL == pstAlarmOutCfg)
    {
        TIPC_PRINTF_ERR("alarm config is NULL!!");
        return -1;
    }
    // 计算告警输出个数
    int s32AlarmOutNum = stParamCtrl.s32ParamSize / sizeof(TVT_SDK_PROD_ALARM_S);
    if(pstAlarmOut->s32Chn >= s32AlarmOutNum)
    {
        TIPC_PRINTF_ERR("alarm Chn[%x] unexist [%d]\n", pstAlarmOut->s32Chn, s32AlarmOutNum);
        return -1;
    }
    TVT_SDK_PROD_ALARM_S* pstAlarmTmp = &pstAlarmOutCfg[pstAlarmOut->s32Chn];
    s32Ret = chip_dev_setGpioVal(pstAlarmTmp->s32GpioGrp, pstAlarmTmp->s32GpioNum, pstAlarmOut->s32OutType);
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("chip_dev_setGpioVal failed\n");
        return s32Ret;
    }
    return s32Ret;
}


/**
 * @brief 告警灯控制
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_dev_alarmLightCtrl(TVT_SDK_DEVICE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int i = 0;
    int s32Value = *(int*)pParam;
    if(NULL == pstHandle->pSdkParamCbFunc)
    {
        TIPC_PRINTF_ERR("param callback should not be NULL\n");
        return -1;
    }
    if(s32Dir != TVT_SDK_DEV_CTRL_SET)
    {
        TIPC_PRINTF_ERR("unsupport ctrl direction[%s]\n", (TVT_SDK_DEV_CTRL_SET == s32Dir)?"set":"get");
        return -1;
    }
    // 获取设备参数
    TVT_SDK_PROD_CTRL_INFO stParamCtrl = {0};
    memcpy(&stParamCtrl.stDevInfo, &pstHandle->stParamDevInfo, sizeof(TVT_SDK_PROD_DEV_INFO));
    s32Ret = pstHandle->pSdkParamCbFunc(TVT_SDK_PARAM_GET_ALARMLIGHT_CFG, &stParamCtrl, sizeof(stParamCtrl));
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get param cfg failed[%d]!!", s32Ret);
        return s32Ret;
    }
    TVT_SDK_PROD_WHITELIGHT_S* pstLightCfg = (TVT_SDK_PROD_WHITELIGHT_S*)stParamCtrl.pParam;
    if(NULL == pstLightCfg)
    {
        TIPC_PRINTF_ERR("alarm config is NULL!!");
        return -1;
    }
    // 计算告警灯个数
    int s32AlarmOutNum = stParamCtrl.s32ParamSize / sizeof(TVT_SDK_PROD_WHITELIGHT_S);
    for(i = 0; i < s32AlarmOutNum; i++)
    {
        TVT_SDK_PROD_WHITELIGHT_S* pstLightCfgTmp = &pstLightCfg[i];
        // GPIO 控制
        s32Ret = chip_dev_setGpioVal(pstLightCfgTmp->stGpioCfg.s32GpioGrp, pstLightCfgTmp->stGpioCfg.s32GpioNum, s32Value);
        if(s32Ret != 0)
        {
            TIPC_PRINTF_ERR("chip_dev_setGpioVal failed\n");
            continue;
        }
        if(0 == pstLightCfgTmp->s32IsGpio)
        {
            /// TODO: 反相控制,分组控制
            // PWM控制
            int s32PwmNum = pstLightCfgTmp->stPwmCfg.s32PwmNum;
            int s32Period = pstLightCfgTmp->stPwmCfg.s32Period;
            int s32Duty = 0;
            if(1 == s32Value)
            {
                s32Duty = pstLightCfgTmp->stPwmCfg.s32MaxDuty;
            }
            s32Ret = chip_dev_setPwmInfo(s32PwmNum, s32Period, s32Duty);
            if(s32Ret != 0)
            {
                TIPC_PRINTF_ERR("chip_dev_setPwmInfo failed\n");
                continue;
            }
        }
    }
    
    return s32Ret;
}

/**
 * @brief sd卡状态控制
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_dev_sdcardStatus(TVT_SDK_DEVICE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int* ps32Value = (int*)pParam;
    if(NULL == pstHandle->pSdkParamCbFunc)
    {
        TIPC_PRINTF_ERR("param callback should not be NULL\n");
        return -1;
    }
    if(s32Dir != TVT_SDK_DEV_CTRL_GET)
    {
        TIPC_PRINTF_ERR("unsupport ctrl direction[%s]\n", (TVT_SDK_DEV_CTRL_SET == s32Dir)?"set":"get");
        return -1;
    }
    // 获取设备参数
    TVT_SDK_PROD_CTRL_INFO stParamCtrl = {0};
    memcpy(&stParamCtrl.stDevInfo, &pstHandle->stParamDevInfo, sizeof(TVT_SDK_PROD_DEV_INFO));
    s32Ret = pstHandle->pSdkParamCbFunc(TVT_SDK_PARAM_GET_SDCARD_CFG, &stParamCtrl, sizeof(stParamCtrl));
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get param cfg failed[%d]!!", s32Ret);
        return s32Ret;
    }
    char* pszTFCardName = (char*)stParamCtrl.pParam;
    if(NULL == pszTFCardName)
    {
        TIPC_PRINTF_ERR("tfcard name is NULL!!");
        return -1;
    }
    // 检查是否存在
    int s32Status = 0;
    s32Ret = chip_dev_getTFCardStatus(pszTFCardName, &s32Status);
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("chip_dev_getTFCardStatus failed\n");
        return s32Ret;
    }
    *ps32Value = s32Status;

    return s32Ret;
}

/**
 * @brief sd卡分区信息
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_dev_sdcardPart(TVT_SDK_DEVICE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    /// TODO: 待完善
    return s32Ret;
}

/**
 * @brief 获取设置韦根信息
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_dev_wiegandData(TVT_SDK_DEVICE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    TVT_SDK_DEVICE_WIEGAND_DATA_S* pstWigandData = (TVT_SDK_DEVICE_WIEGAND_DATA_S*)pParam;
    CHIP_DEV_WIEGAND_DATA_S stChipWiegandData = {0};
    if(TVT_SDK_DEV_CTRL_GET == s32Dir)
    {
        s32Ret = chip_dev_getWiegandData(&stChipWiegandData);
        if(s32Ret != 0)
        {
            TIPC_PRINTF_ERR("chip_dev_getWiegandData failed\n");
            return s32Ret;
        }
        pstWigandData->s32BitNum = stChipWiegandData.s32BitNum;
        pstWigandData->u32Hid = stChipWiegandData.u32Hid;
        pstWigandData->u32Pid = stChipWiegandData.u32Pid;
        pstWigandData->u64CardID = stChipWiegandData.u64CardID;
        pstWigandData->u64OriData = stChipWiegandData.u64OriData;
        pstWigandData->u64TimeStampMS = stChipWiegandData.u64TimeStampMS;
    }
    else
    {
        stChipWiegandData.s32BitNum = pstWigandData->s32BitNum;
        stChipWiegandData.u32Hid = pstWigandData->u32Hid;
        stChipWiegandData.u32Pid = pstWigandData->u32Pid;
        stChipWiegandData.u64CardID = pstWigandData->u64CardID;
        stChipWiegandData.u64OriData = pstWigandData->u64OriData;
        stChipWiegandData.u64TimeStampMS = pstWigandData->u64TimeStampMS;
        s32Ret = chip_dev_setWiegandData(&stChipWiegandData);
        if(s32Ret != 0)
        {
            TIPC_PRINTF_ERR("chip_dev_setWiegandData failed\n");
            return s32Ret;
        }
    }
    return s32Ret;
}


/**
 * @brief 设置韦根模式
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_dev_wiegandModeCtrl(TVT_SDK_DEVICE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int s32Mode = *(int*)pParam;
    if(s32Dir != TVT_SDK_DEV_CTRL_SET)
    {
        TIPC_PRINTF_ERR("unsupport ctrl direction[%s]\n", (TVT_SDK_DEV_CTRL_SET == s32Dir)?"set":"get");
        return -1;
    }
    s32Ret = chip_dev_setWiegandMode(s32Mode);
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("chip_dev_setWiegandMode failed\n");
        return s32Ret;
    }
    return s32Ret;
}


/**
 * @brief 设置韦根
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_dev_wiegandCtrl(TVT_SDK_DEVICE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    TVT_SDK_DEVICE_WIEGAND_CFG_S* stWiegandCfg = (TVT_SDK_DEVICE_WIEGAND_CFG_S*)pParam;
    if(s32Dir != TVT_SDK_DEV_CTRL_SET)
    {
        TIPC_PRINTF_ERR("unsupport ctrl direction[%s]\n", (TVT_SDK_DEV_CTRL_SET == s32Dir)?"set":"get");
        return -1;
    }
    int s32Mode = stWiegandCfg->s32Mode;
    s32Ret = chip_dev_setWiegandMode(s32Mode);
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("chip_dev_setWiegandMode failed\n");
        return s32Ret;
    }
    int s32BitNum = stWiegandCfg->s32BitNum;
    s32Ret = chip_dev_setWiegandBitNum(s32BitNum);
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("chip_dev_setWiegandBitNum failed\n");
        return s32Ret;
    }
    return s32Ret;
}


static TVT_SDK_DEVICE_CTRL_IMPL_S g_stFunList[] =
{
    {TVT_SDK_DEVICE_RTC,                tvt_sdk_dev_rtcCtrl},
    {TVT_SDK_DEVICE_IRCUT,              tvt_sdk_dev_ircutCtrl},
    {TVT_SDK_DEVICE_RESET,              tvt_sdk_dev_resetCtrl},
    {TVT_SDK_DEVICE_ABF,                tvt_sdk_dev_ABFCtrl},
    {TVT_SDK_DEVICE_CDS,                tvt_sdk_dev_CDSCtrl},
    {TVT_SDK_DEVICE_UART_DATA,          tvt_sdk_dev_uartData},
    {TVT_SDK_DEVICE_UART_CFG,           tvt_sdk_dev_uartCfg},
    {TVT_SDK_DEVICE_SECURITY,           tvt_sdk_dev_securityInfo},
    {TVT_SDK_DEVICE_ALARM_IN,           tvt_sdk_dev_alarmInCtrl},
    {TVT_SDK_DEVICE_ALARM_OUT,          tvt_sdk_dev_alarmOutCtrl},
    {TVT_SDK_DEVICE_ALARM_LIGHT,        tvt_sdk_dev_alarmLightCtrl},
    {TVT_SDK_DEVICE_SDCARD_STATUS,      tvt_sdk_dev_sdcardStatus},
    {TVT_SDK_DEVICE_SDCARD_PART,        tvt_sdk_dev_sdcardPart},
    {TVT_SDK_DEVICE_WIEGAND_MODE,       tvt_sdk_dev_wiegandModeCtrl},
    {TVT_SDK_DEVICE_WIEGAND_CTRL,       tvt_sdk_dev_wiegandCtrl},
    {TVT_SDK_DEVICE_WIEGAND_DATA,       tvt_sdk_dev_wiegandData},
    {TVT_SDK_DEVICE_DOOR_KEY,           NULL},
    {TVT_SDK_DEVICE_TAMPER,             NULL},
};

/**
 * @brief 外设业务控制接口
 * @param [in] s32Cmd:命令字
 * @param [in] s32Dir:TVT_SDK_DEVICE_CTRL_DIR_E
 * @param [in] pParam:参数指针
 * @param [in] s32Length:参数长度
 * @return 0：成功；非0：失败
 */
int tvt_sdk_device_ctrl(int s32Cmd, int s32Dir, void* pParam, int s32Length)
{
    int s32Ret = 0;
    int i = 0;

    if(NULL == pParam || 0 == s32Length)
    {
        TIPC_PRINTF_ERR("err input param");
        return -1;
    }

    TVT_SDK_DEVICE_INFO_S* pstHandle = tvt_sdk_dev_getHandle();
    if(NULL == pstHandle)
    {
        TIPC_PRINTF_ERR("NULL Pointer %d", s32Cmd);
        return -1;
    }

    if(pstHandle->u8Init != 1)
    {
        TIPC_PRINTF_ERR("current module is not running");
        return -1;
    }
    
    TIPC_MUTEX_LOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_DEVICE_DEBUG);

    int s32SuppFuncSize = sizeof(g_stFunList) / sizeof(TVT_SDK_DEVICE_CTRL_IMPL_S);
    for(i = 0; i < s32SuppFuncSize; i++)
    {
        if((s32Cmd == (int)g_stFunList[i].enCmd) && (g_stFunList[i].pImplFunc != NULL))
        {
            s32Ret = g_stFunList[i].pImplFunc(pstHandle, s32Dir, pParam, s32Length);
            break;
        }
    }

    if(i >= s32SuppFuncSize)
    {
        TIPC_PRINTF_ERR("unknow cmd[%d]", s32Cmd);
    }

    TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_DEVICE_DEBUG);
    return s32Ret;
}


/**
 * @brief 初始化外设业务
 * @param [in] pstVideoCfg:视频输入参数指针
 * @param [in] s32VinNum:视频输入个数
 * @return 0：成功；非0：失败
 */
int tvt_sdk_device_init(TVT_SDK_PROD_DEV_INFO* pstParamDevInfo, TvtSdkProdCb pParamCb)
{
    int s32Ret = -1;
    if(NULL == pParamCb || NULL == pstParamDevInfo)
    {
        TIPC_PRINTF_ERR("device service input param err");
        return s32Ret;
    }

    if(g_pstSdkDeviceInfo != NULL)
    {
        if(g_pstSdkDeviceInfo->u8Init != 0)
        {
            // 已初始化
            TIPC_PRINTF_ERR("video service is running");
            return 0;
        }
    }
    else
    {
        // 创建结构体
        g_pstSdkDeviceInfo = (TVT_SDK_DEVICE_INFO_S* )malloc(sizeof(TVT_SDK_DEVICE_INFO_S));
        if(NULL == g_pstSdkDeviceInfo)
        {
            TIPC_PRINTF_ERR("video service malloc failed");
            return s32Ret;
        }
        memset(g_pstSdkDeviceInfo, 0, sizeof(TVT_SDK_DEVICE_INFO_S));
    }

    // 保存参数
    g_pstSdkDeviceInfo->pSdkParamCbFunc = pParamCb;
    memcpy(&g_pstSdkDeviceInfo->stParamDevInfo, pstParamDevInfo, sizeof(TVT_SDK_PROD_DEV_INFO));

    // 初始化模块锁
    pthread_mutex_init(&g_pstSdkDeviceInfo->s32Mutex, NULL);
    sprintf(g_pstSdkDeviceInfo->szMutexName, "tvt_sdk_device_lock");
    g_pstSdkDeviceInfo->u8Init = 1;

	TIPC_PRINTF_CHK("g_pstSdkDeviceInfo ==== %p", g_pstSdkDeviceInfo);

    return s32Ret;
}

/**
 * @brief 去初始化外设业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_device_exit()
{
    if(NULL == g_pstSdkDeviceInfo)
    {
        // 未初始化
        TIPC_PRINTF_ERR("device service is not running");
        return -1;
    }

    if (g_pstSdkDeviceInfo->u8Init != 1)
    {
        // 未初始化
        TIPC_PRINTF_ERR("device service is not running");
        free((void*)g_pstSdkDeviceInfo);
        g_pstSdkDeviceInfo = NULL;
        return -1;
    }

    pthread_mutex_destroy(&g_pstSdkDeviceInfo->s32Mutex);

    free((void*)g_pstSdkDeviceInfo);
    g_pstSdkDeviceInfo = NULL;

    return 0;
}