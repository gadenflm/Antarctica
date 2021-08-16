#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include "sdk_func.h"
#include "sdk_define.h"
#include "sdk_private.h"

#include "tvt_sdk_param.h"
#include "tvt_sdk_print.h"
#include "tvt_sdk_video.h"
#include "tvt_sdk_device.h"
#include "tvt_sdk_btn.h"

extern TVT_SDK_CFG_S g_stSdkCfg;
extern int BuildSdkVersion(void);
extern int SDKLensZoomBtnCallBack(void* pParam);

typedef enum sdk_dev_ctrl_direction_e
{
    SDK_DEV_CTRL_GET,
    SDK_DEV_CTRL_SET,
    SDK_DEV_CTRL_DIRECTION_BUTT,
} SDK_DEV_CTRL_DIRECTION_E;

typedef int (*SDKDeviceImplFunc)(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize);
typedef struct sdk_device_ctrl_impl_s
{
    int                         s32Cmd;          ///< 命令字
    SDKDeviceImplFunc           pImplFunc;       ///< 实现接口
} SDK_DEVICE_CTRL_IMPL_S;


int SDKGetSecCode(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    SDK_SEC_CODE* pstSecCode = (SDK_SEC_CODE*)pParam;
    if ( pstSecCode )
    {
        pstSecCode->cmd_run_with_sec_code = 0;
    }
    return 0;
}

int SDKSetVideo480P(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int SDKSetIRPowerEnable(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int SDKSetAudioInMode(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int SDKSetAudioVolume(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int SDKSetProductID(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int SDKSetProductSubID(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int SDKSetCustomID(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int SDKSetCustomSubID(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int SDKSetLensType(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int SDKSetDoorLockValue(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int SDKSetBitrateUnlimit(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int SDKSetHoursMode(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int SDKSetSpecialDayNightMode(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int SDKSetAlarmOut(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}


/**
 * @brief 控制RTC时间
 */
static int SDKDevRtcTimeCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    SDK_RTC_TIME* pstRtcInfo = (SDK_RTC_TIME*)pParam;
    // int s32Dir = (SDK_DEV_CTRL_GET == enCtrlDir)?TVT_SDK_DEV_CTRL_GET:TVT_SDK_DEV_CTRL_SET;
    TVT_SDK_DEVICE_RTC_S stRtcInfo = {0};
    if (SDK_DEV_CTRL_SET == enCtrlDir)
    {
        stRtcInfo.u32Date = pstRtcInfo->date;
        stRtcInfo.u32Hour = pstRtcInfo->hour;
        stRtcInfo.u32Minute = pstRtcInfo->minute;
        stRtcInfo.u32Month = pstRtcInfo->month;
        stRtcInfo.u32Second = pstRtcInfo->second;
        stRtcInfo.u32Weekday = pstRtcInfo->weekday;
        stRtcInfo.u32Year = pstRtcInfo->year;
        s32Ret = 0; // tvt_sdk_device_ctrl(TVT_SDK_DEVICE_RTC, s32Dir, &stRtcInfo, sizeof(TVT_SDK_DEVICE_RTC_S));
    }
    else
    {
        s32Ret = 0; // tvt_sdk_device_ctrl(TVT_SDK_DEVICE_RTC, s32Dir, &stRtcInfo, sizeof(TVT_SDK_DEVICE_RTC_S));
        stRtcInfo.u32Year = 2021 - 1900;
        stRtcInfo.u32Month = 6;
        stRtcInfo.u32Date = 28;
        stRtcInfo.u32Hour = 16;
        stRtcInfo.u32Minute = 39;
        stRtcInfo.u32Second = 20;
        stRtcInfo.u32Weekday = 3;

        pstRtcInfo->date = stRtcInfo.u32Date;
        pstRtcInfo->hour = stRtcInfo.u32Hour;
        pstRtcInfo->minute = stRtcInfo.u32Minute;
        pstRtcInfo->month = stRtcInfo.u32Month;
        pstRtcInfo->second = stRtcInfo.u32Second;
        pstRtcInfo->weekday = stRtcInfo.u32Weekday;
        pstRtcInfo->year = stRtcInfo.u32Year;
    }

    return s32Ret;
}

/**
 * @brief 复位键
 */
static int SDKDevDefaultCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int s32Dir = (SDK_DEV_CTRL_GET == enCtrlDir) ? TVT_SDK_DEV_CTRL_GET : TVT_SDK_DEV_CTRL_SET;
    s32Ret = tvt_sdk_device_ctrl(TVT_SDK_DEVICE_RESET, s32Dir, pParam, s32ParamSize);
    return s32Ret;
}

/**
 * @brief ABF键
 */
static int SDKDevABFCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int s32Dir = (SDK_DEV_CTRL_GET == enCtrlDir) ? TVT_SDK_DEV_CTRL_GET : TVT_SDK_DEV_CTRL_SET;
    s32Ret = tvt_sdk_device_ctrl(TVT_SDK_DEVICE_ABF, s32Dir, pParam, s32ParamSize);
    return s32Ret;
}

static SDK_ZOOM_CB_PARAM_S g_stZoomCBParam[3] =
{
    // Zoom +
    {
        .enDirection = SDK_LENS_CCW,
        .enMode = 0,
        .enStatus = SDK_LENS_STATUS_START,
    },
    // Zoom -
    {
        .enDirection = SDK_LENS_CW,
        .enMode = 0,
        .enStatus = SDK_LENS_STATUS_START,
    },
    // Stop
    {
        .enDirection = 0,
        .enMode = SDK_LENS_ADJUST,
        .enStatus = SDK_LENS_STATUS_STOP,
    },
};


/**
 * @brief zoom 按键
 */
static int SDKDevZoomBtnCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0, i = 0;
    // 获取设备参数
    TVT_SDK_PROD_CTRL_INFO stParamCtrl = {0};
    memcpy(&stParamCtrl.stDevInfo, &g_stSdkCfg.stDevModelInfo, sizeof(TVT_SDK_PROD_DEV_INFO));
    s32Ret = tvt_sdk_param_ctrl(TVT_SDK_PARAM_GET_ZOOM_BTN_CFG, &stParamCtrl, sizeof(stParamCtrl));
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get param cfg failed[%d]!!", s32Ret);
        return s32Ret;
    }
    TVT_SDK_PROD_ZOOM_BTN_S* pstZoomBtnCfg = (TVT_SDK_PROD_ZOOM_BTN_S*)stParamCtrl.pParam;
    if (NULL == pstZoomBtnCfg)
    {
        TIPC_PRINTF_ERR("button config is NULL!!");
        return -1;
    }
    int s32ZoomBtnNum = stParamCtrl.s32ParamSize / sizeof(TVT_SDK_PROD_ZOOM_BTN_S);
    for (i = 0; i < s32ZoomBtnNum; i++)
    {
        // 这里Zoom+/-的位置固定，第一个是+，第二个是-
        TVT_SDK_PROD_ZOOM_BTN_S* pstZoomBtnCfgTmp = &pstZoomBtnCfg[i];
        // 注册事件
        TVT_SDK_BTN_CFG_S stBtnCfg = {0};
        stBtnCfg.s32GpioGrp = pstZoomBtnCfgTmp->s32GpioGrp;
        stBtnCfg.s32GpioNum = pstZoomBtnCfgTmp->s32GpioNum;
        stBtnCfg.s32EventNum = 2; // 按下松手两个事件
        stBtnCfg.stBtnEvent[0].enEvent = (TVT_SDK_PROD_BTN_FALLING_EDGE == pstZoomBtnCfgTmp->enTrigMode) ? TVT_SDK_BTN_EVENT_DOWN : TVT_SDK_BTN_EVENT_UP;
        stBtnCfg.stBtnEvent[0].pfBtnCb = SDKLensZoomBtnCallBack;
        stBtnCfg.stBtnEvent[0].pCbParam = &g_stZoomCBParam[i];
        stBtnCfg.stBtnEvent[1].enEvent = (TVT_SDK_PROD_BTN_FALLING_EDGE == pstZoomBtnCfgTmp->enTrigMode) ? TVT_SDK_BTN_EVENT_UP : TVT_SDK_BTN_EVENT_DOWN;
        stBtnCfg.stBtnEvent[1].pfBtnCb = SDKLensZoomBtnCallBack;
        stBtnCfg.stBtnEvent[1].pCbParam = &g_stZoomCBParam[2];
        s32Ret = tvt_sdk_btn_addBtnEvent(&stBtnCfg);
        if (s32Ret != 0)
        {
            TIPC_PRINTF_ERR("add Button event failed!!");
        }
    }
    return s32Ret;
}

/**
 * @brief alarm in
 */
static int SDKDevAlarmInCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int s32Dir = (SDK_DEV_CTRL_GET == enCtrlDir) ? TVT_SDK_DEV_CTRL_GET : TVT_SDK_DEV_CTRL_SET;
    s32Ret = tvt_sdk_device_ctrl(TVT_SDK_DEVICE_ALARM_IN, s32Dir, pParam, s32ParamSize);
    return s32Ret;
}

/**
 * @brief Alarm Out
 */
static int SDKDevAlarmOutCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
    int s32Ret = 0;
    SDK_MULTI_ALARM_OUT * pstMultiAlarmOut = (SDK_MULTI_ALARM_OUT *)pParam;
    if (SDK_DEV_CTRL_SET == enCtrlDir)
    {
        int s32Dir = TVT_SDK_DEV_CTRL_SET;
        TVT_SDK_DEVICE_ALARM_OUT_S stAlarmOutCfg = {0};
        stAlarmOutCfg.s32Chn = pstMultiAlarmOut->iChannel;
        stAlarmOutCfg.s32OutType = (SDK_ALARM_OUT_CLOSE == pstMultiAlarmOut->iOutType) ? 0 : 1;
        s32Ret = tvt_sdk_device_ctrl(TVT_SDK_DEVICE_ALARM_OUT, s32Dir, &stAlarmOutCfg, sizeof(stAlarmOutCfg));
    }
    return s32Ret;
}

/**
 * @brief CDS
 */
static int SDKDevCDSCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int s32Dir = (SDK_DEV_CTRL_GET == enCtrlDir) ? TVT_SDK_DEV_CTRL_GET : TVT_SDK_DEV_CTRL_SET;
    s32Ret = tvt_sdk_device_ctrl(TVT_SDK_DEVICE_CDS, s32Dir, pParam, s32ParamSize);
    return s32Ret;
}

/**
 * @brief UART Data
 */
static int SDKDevUartDataCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    TVT_SDK_DEVICE_UART_DATA_S stUartData = {0};
    stUartData.pszData = (void*)pParam;
    stUartData.s32DataLens = s32ParamSize;
    int s32Dir = (SDK_DEV_CTRL_GET == enCtrlDir) ? TVT_SDK_DEV_CTRL_GET : TVT_SDK_DEV_CTRL_SET;
    s32Ret = tvt_sdk_device_ctrl(TVT_SDK_DEVICE_UART_DATA, s32Dir, &stUartData, sizeof(TVT_SDK_DEVICE_UART_DATA_S));
    return s32Ret;
}

/**
 * @brief UART Cfg
 */
static int SDKDevUartCfgCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int s32Dir = (SDK_DEV_CTRL_GET == enCtrlDir) ? TVT_SDK_DEV_CTRL_GET : TVT_SDK_DEV_CTRL_SET;
    s32Ret = tvt_sdk_device_ctrl(TVT_SDK_DEVICE_UART_CFG, s32Dir, pParam, s32ParamSize);
    return s32Ret;
}


/**
 * @brief secure code
 */
static int SDKDevSecCodeCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    /// TODO
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return s32Ret;
}

/**
 * @brief sdcard
 */
static int SDKDevSdCardCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int s32Dir = (SDK_DEV_CTRL_GET == enCtrlDir) ? TVT_SDK_DEV_CTRL_GET : TVT_SDK_DEV_CTRL_SET;
    s32Ret = tvt_sdk_device_ctrl(TVT_SDK_DEVICE_SDCARD_STATUS, s32Dir, pParam, s32ParamSize);
    return s32Ret;
}

/**
 * @brief sdcard partition
 */
static int SDKDevSdCardPartCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    /// TODO
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return s32Ret;
}

/**
 * @brief day night
 */
static int SDKDevDayNightCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    /// TODO
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return s32Ret;
}

/**
 * @brief sdk version
 */
static int SDKDevVersionCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    BuildSdkVersion();
    *(int *)pParam = g_stSdkCfg.s32SDKVersion;
    return s32Ret;
}

/**
 * @brief motion desc
 */
static int SDKDevMotionDescCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    /// TODO
    // TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return s32Ret;
}

/**
 * @brief obj move
 */
static int SDKDevOBJMoveStatusCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    /// TODO
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return s32Ret;
}

/**
 * @brief Exception
 */
static int SDKDevExceptionStatusCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    /// TODO
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return s32Ret;
}

/**
 * @brief wiegand
 */
static int SDKDevWiegandDataCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    SDK_WIEGAND_DATA* pstWiegandData = (SDK_WIEGAND_DATA*)pParam;
    int s32Dir = (SDK_DEV_CTRL_GET == enCtrlDir) ? TVT_SDK_DEV_CTRL_GET : TVT_SDK_DEV_CTRL_SET;
    TVT_SDK_DEVICE_WIEGAND_DATA_S stWiegandData = {0};
    if (SDK_DEV_CTRL_SET == enCtrlDir)
    {
        stWiegandData.s32BitNum = pstWiegandData->bitNum;
        stWiegandData.u32Hid = pstWiegandData->iHid;
        stWiegandData.u32Pid = pstWiegandData->iPid;
        stWiegandData.u64CardID = pstWiegandData->u64CardID;
        stWiegandData.u64OriData = pstWiegandData->u64OriData;
        stWiegandData.u64TimeStampMS = pstWiegandData->u64ProduceTick;
        s32Ret = tvt_sdk_device_ctrl(TVT_SDK_DEVICE_WIEGAND_DATA, s32Dir, &stWiegandData, sizeof(TVT_SDK_DEVICE_WIEGAND_DATA_S));
    }
    else
    {
        s32Ret = tvt_sdk_device_ctrl(TVT_SDK_DEVICE_WIEGAND_DATA, s32Dir, &stWiegandData, sizeof(TVT_SDK_DEVICE_WIEGAND_DATA_S));
        pstWiegandData->bitNum = stWiegandData.s32BitNum;
        pstWiegandData->iHid = stWiegandData.u32Hid;
        pstWiegandData->iPid = stWiegandData.u32Pid;
        pstWiegandData->u64CardID = stWiegandData.u64CardID;
        pstWiegandData->u64OriData = stWiegandData.u64OriData;
        pstWiegandData->u64ProduceTick = stWiegandData.u64TimeStampMS;
    }
    return s32Ret;
}

/**
 * @brief 韦根模式更新
 */
static int SDKDevWiegandModeCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int s32Dir = (SDK_DEV_CTRL_GET == enCtrlDir) ? TVT_SDK_DEV_CTRL_GET : TVT_SDK_DEV_CTRL_SET;
    s32Ret = tvt_sdk_device_ctrl(TVT_SDK_DEVICE_WIEGAND_MODE, s32Dir, pParam, s32ParamSize);
    return s32Ret;
}

/**
 * @brief 韦根配置更新
 */
static int SDKDevWiegandCfgCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    if (SDK_DEV_CTRL_SET == enCtrlDir)
    {
        SDK_WG_CFG_PARA* pstWgCfgPara = (SDK_WG_CFG_PARA *)pParam;
        int s32Dir = TVT_SDK_DEV_CTRL_SET;
        TVT_SDK_DEVICE_WIEGAND_CFG_S stWiegandCfg = {0};
        stWiegandCfg.s32Mode = pstWgCfgPara->mode;
        stWiegandCfg.s32BitNum = pstWgCfgPara->enBitNum;
        s32Ret = tvt_sdk_device_ctrl(TVT_SDK_DEVICE_WIEGAND_CTRL, s32Dir, &stWiegandCfg, sizeof(TVT_SDK_DEVICE_WIEGAND_CFG_S));
    }
    return s32Ret;
}

/**
 * @brief Door Key
 */
static int SDKDevDoorKeyCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    /// TODO
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return s32Ret;
}

/**
 * @brief Tamper
 */
static int SDKDevTamperCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    /// TODO
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return s32Ret;
}

/**
 * @brief ircut, 切换日夜
 */
static int SDKDevIRCutCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    //
    int s32Ret = 0;
    if (enCtrlDir != SDK_DEV_CTRL_SET)
    {
        TIPC_PRINTF_ERR("SDKDevIRCutCtrl unsupport %s control", (enCtrlDir == SDK_DEV_CTRL_SET) ? "set" : "get");
        return -1;
    }
    // 判断当前日夜状态
    if (g_stSdkCfg.enDayNightState != *(int*)pParam)
    {
        int s32Dir = TVT_SDK_DEV_CTRL_SET;
        int s32Val = (SDK_DAYNIGHT_DAY == *(int*)pParam) ? 0 : 1;
        g_stSdkCfg.enDayNightState = *(int*)pParam;
        if (g_stSdkCfg.u64DevFeature & TVT_SDK_PROD_SUPP_SWCDS)
        {
            /// TODO:软光敏机型需要更新接口
        }
        if (1 == s32Val)//d2n
        {
            /// TODO: 1: 暂停motion检测
            /// TODO: 2: 设置日夜模式
            // 3: 修改ircut gpio
            s32Ret = tvt_sdk_device_ctrl(TVT_SDK_DEVICE_IRCUT, s32Dir, &s32Val, sizeof(int));
        }
        else//n2d
        {
            // 1: 修改ircut gpio
            s32Ret = tvt_sdk_device_ctrl(TVT_SDK_DEVICE_IRCUT, s32Dir, &s32Val, sizeof(int));
            usleep(230000); // 休眠一下?
            /// TODO: 2: 暂停motion检测
            /// TODO: 3: 设置日夜模式
        }
    }
    return s32Ret;
}

/**
 * @brief 红外开关
 */
static int SDKDevInfraredOpenCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    /// TODO: 红外灯的控制另起业务
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return s32Ret;
}

/**
 * @brief 红外开关
 */
static int SDKDevInfraredPowerCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    /// TODO: 红外灯的控制另起业务
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return s32Ret;
}

/**
 * @brief 告警白光灯
 */
static int SDKDevAlarmLightCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    int s32Dir = (SDK_DEV_CTRL_GET == enCtrlDir) ? TVT_SDK_DEV_CTRL_GET : TVT_SDK_DEV_CTRL_SET;
    s32Ret = tvt_sdk_device_ctrl(TVT_SDK_DEVICE_ALARM_LIGHT, s32Dir, pParam, s32ParamSize);
    return s32Ret;
}

/**
 * @brief 外壳型号
 */
static int SDKDevModelCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    if (SDK_DEV_CTRL_SET == enCtrlDir)
    {
        g_stSdkCfg.stDevModelInfo.s32ModelID = *(int*)pParam;
    }
    return s32Ret;
}

/**
 * @brief 镜头型号
 */
static int SDKDevModelLensCtrl(SDK_DEV_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    if (SDK_DEV_CTRL_SET == enCtrlDir)
    {
        g_stSdkCfg.stDevModelInfo.s32LensID = *(int*)pParam;
    }
    return s32Ret;
}



static SDK_DEVICE_CTRL_IMPL_S g_stGetFunList[] =
{
    {SDK_DEVICE_GET_RTC_TIME,                           SDKDevRtcTimeCtrl},
    {SDK_DEVICE_GET_DEFAULT,                            SDKDevDefaultCtrl},
    {SDK_DEVICE_GET_ABF_VALUE,                          SDKDevABFCtrl},
    {SDK_DEVICE_GET_CDS,                                SDKDevCDSCtrl},
    {SDK_DEVICE_GET_UART_DATA,                          SDKDevUartDataCtrl},
    {SDK_DEVICE_GET_SECURITY,                           SDKDevSecCodeCtrl},
    {SDK_DEVICE_GET_ALARM_IN,                           SDKDevAlarmInCtrl},
    {SDK_DEVICE_GET_SDCARD,                             SDKDevSdCardCtrl},
    {SDK_DEVICE_GET_SDCARD_USE_PARTITIONS,              SDKDevSdCardPartCtrl},
    {SDK_DEVICE_GET_DN_STATUS,                          SDKDevDayNightCtrl},
    {SDK_DEVICE_GET_SDK_VERSION,                        SDKDevVersionCtrl},
    {SDK_DEVICE_GET_MOTION_DESCRIBE,                    SDKDevMotionDescCtrl},
    {SDK_DEVICE_GET_OBJECTREMOVE_STATUS,                SDKDevOBJMoveStatusCtrl},
    {SDK_DEVICE_GET_EXCEPTION_STATUS,                   SDKDevExceptionStatusCtrl},
    {SDK_DEVICE_GET_WIEGAND_DATA,                       SDKDevWiegandDataCtrl},
    {SDK_DEVICE_GET_DOOR_KEY_VALUE,                     SDKDevDoorKeyCtrl},
    {SDK_DEVICE_GET_TAMPER_VALUE,                       SDKDevTamperCtrl},
    {SDK_DEVICE_GET_SEC_CODE,                           SDKGetSecCode},
};

static SDK_DEVICE_CTRL_IMPL_S g_stSetFunList[] =
{
    {SDK_DEVICE_SET_RTC_TIME,                           SDKDevRtcTimeCtrl},
    {SDK_DEVICE_SET_IRCUT,                              SDKDevIRCutCtrl},
    {SDK_DEVICE_SET_SUPPORT_ZOOM_BUTTON,                SDKDevZoomBtnCtrl},
    {SDK_DEVICE_SET_INFRARED,                           SDKDevInfraredOpenCtrl},
    {SDK_DEVICE_SET_ALARM_WHITE_LIGHT,                  SDKDevAlarmLightCtrl},
    {SDK_DEVICE_SET_UART_ATTR,                          SDKDevUartCfgCtrl},
    {SDK_DEVICE_SET_UART_DATA,                          SDKDevUartDataCtrl},
    {SDK_DEVICE_SET_IR_POW_EN,                          SDKSetIRPowerEnable},
    {SDK_DEVICE_SET_MULTI_ALARM_OUT,                    SDKDevAlarmOutCtrl},
    {SDK_DEVICE_SET_AI_MODE,                            SDKSetAudioInMode}, /// TODO: SDKAudioInModeCtrl
    {SDK_DEVICE_SET_PRODUCT_ID,                         SDKSetProductID},
    {SDK_DEVICE_SET_PRODUCT_SUB_ID,                     SDKSetProductSubID},
    {SDK_DEVICE_SET_CUSTOM_ID,                          SDKSetCustomID},
    {SDK_DEVICE_SET_CUSTOM_SUB_ID,                      SDKSetCustomSubID},
    {SDK_DEVICE_SET_LENS_TYPE,                          SDKSetLensType},
    {SDK_DEVICE_SET_MODEL_ID,                           SDKDevModelCtrl},
    {SDK_DEVICE_SET_MODEL_LENS,                         SDKDevModelLensCtrl},
    {SDK_DEVICE_SET_VOLUME,                             SDKSetAudioVolume}, /// TODO: SDKAudioVolumeCtrl
    {SDK_DEVICE_SET_WIEGAND_TRANSPORT_MODE,             SDKDevWiegandModeCtrl},
    {SDK_DEVICE_SET_WIEGAND_CFG,                        SDKDevWiegandCfgCtrl},
    {SDK_DEVICE_SET_WIEGAND_DATA,                       SDKDevWiegandDataCtrl},
    {SDK_DEVICE_SET_DOOR_LOCK_VALUE,                    SDKSetDoorLockValue},
    {SDK_DEVICE_SET_DEV_INFRARED_STRENGTH,              SDKDevInfraredPowerCtrl},
    {SDK_DEVICE_SET_SUPPORT_480P,                       SDKSetVideo480P},
    {SDK_DEVICE_SET_BITRATE_UNLIMIT,                    SDKSetBitrateUnlimit},
    {SDK_DEVICE_SET_HOURS_MODE,                         SDKSetHoursMode},
    {SDK_DEVICE_SET_SPECIAL_DAY_NIGHT_MODE,             SDKSetSpecialDayNightMode},
    {SDK_DEVICE_SET_ALARM_OUT,                          SDKSetAlarmOut},
};

/**
 * @brief 获取外设资源
 */
int SDKGetDeviceState(SDK_GET_DEVICE_TYPE_PARA eDevPara, void *pData, int iBufLen)
{
    int s32Ret = 0;
    int i = 0;
    if (NULL == pData)
    {
        TIPC_PRINTF_INFO("SDKGetDeviceState, cmd %d, data is NULL\n", (int)eDevPara);
        return -1;
    }

    int s32SuppFuncSize = sizeof(g_stGetFunList) / sizeof(SDK_DEVICE_CTRL_IMPL_S);
    for (i = 0; i < s32SuppFuncSize; i++)
    {
        if (((int)eDevPara == (int)g_stGetFunList[i].s32Cmd) && (g_stGetFunList[i].pImplFunc != NULL))
        {
            s32Ret = g_stGetFunList[i].pImplFunc(SDK_DEV_CTRL_GET, pData, iBufLen);
            break;
        }
    }

    if (i >= s32SuppFuncSize)
    {
        TIPC_PRINTF_ERR("unknow cmd[%d]", (int)eDevPara);
    }

    return s32Ret;
}


/**
 * @brief 设置外设资源
 */
int SDKSetDeviceState(SDK_SET_DEVICE_TYPE_PARA eDevPara, const void* pData, int iDataLen)
{
    int s32Ret = 0;
    int i = 0;
    if (NULL == pData)
    {
        TIPC_PRINTF_INFO("SDKGetDeviceState, cmd %d, data is NULL\n", (int)eDevPara);
        return -1;
    }

    int s32SuppFuncSize = sizeof(g_stSetFunList) / sizeof(SDK_DEVICE_CTRL_IMPL_S);
    for (i = 0; i < s32SuppFuncSize; i++)
    {
        if (((int)eDevPara == (int)g_stSetFunList[i].s32Cmd) && (g_stSetFunList[i].pImplFunc != NULL))
        {
            s32Ret = g_stSetFunList[i].pImplFunc(SDK_DEV_CTRL_SET, pData, iDataLen);
            break;
        }
    }

    if (i >= s32SuppFuncSize)
    {
        TIPC_PRINTF_ERR("unknow cmd[%d]", (int)eDevPara);
    }

    return s32Ret;
}

int SDKMotionDetectEnable(void)
{
    return 0;
}

int SDKMotionDetectDisable(void)
{
    return 0;
}

int SDKGetMotionStatus(unsigned char *pStatus)
{
    *pStatus = 0;
    return 0;
}

int SDKGetMotionGrid(unsigned long *pWidthGrid, unsigned long *pHeightGrid)
{
    if (pWidthGrid)
    {
        *pWidthGrid = 22;
    }
    if (pHeightGrid)
    {
        if (0)
        {
            *pHeightGrid = 18;
        }
        else
        {
            *pHeightGrid = 15;
        }
    }
    return 0;
}

int SDKSetMotion(int iMotionSensitive, const ULONGLONG *pArea)
{
    return 0;
}


