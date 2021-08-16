#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "tvt_sdk_param.h"
#include "tvt_sdk_print.h"

#ifndef CHK_FUNC
#define CHK_FUNC(_fun)  \
        do {                        \
            if(NULL == _fun)        \
            {                       \
                TIPC_PRINTF_ERR("NULL POINTER"); \
                return -1;          \
            }                       \
        } while (0)
#endif


typedef int (*ParamImplFunc)(void* pParam, int s32ParamSize);
typedef struct tvt_sdk_param_impl_s
{
    TVT_SDK_PROD_CMD_E          enCmd;          ///< 命令字
    ParamImplFunc               pImplFunc;      ///< 实现接口
} TVT_SDK_PARAM_IMPL_S;

extern int tvt_param_getFeature(void* pParam, int s32ParamSize) __attribute__((weak));
extern int tvt_param_getVinInfo(void* pParam, int s32ParamSize) __attribute__((weak));
extern int tvt_param_getEncSupp(void* pParam, int s32ParamSize) __attribute__((weak));
extern int tvt_param_getAudioInSupp(void* pParam, int s32ParamSize) __attribute__((weak));
extern int tvt_param_getAudioOutSupp(void* pParam, int s32ParamSize) __attribute__((weak));
extern int tvt_param_getUartCfg(void* pParam, int s32ParamSize) __attribute__((weak));
extern int tvt_param_getRS485Cfg(void* pParam, int s32ParamSize) __attribute__((weak));
extern int tvt_param_getCDSCfg(void* pParam, int s32ParamSize) __attribute__((weak));
extern int tvt_param_getSDCardCfg(void* pParam, int s32ParamSize) __attribute__((weak));
extern int tvt_param_getAlarmInCfg(void* pParam, int s32ParamSize) __attribute__((weak));
extern int tvt_param_getAlarmOutCfg(void* pParam, int s32ParamSize) __attribute__((weak));
extern int tvt_param_getAlarmLightCfg(void* pParam, int s32ParamSize) __attribute__((weak));
extern int tvt_param_getRstBtnCfg(void* pParam, int s32ParamSize) __attribute__((weak));
extern int tvt_param_getZoomBtnCfg(void* pParam, int s32ParamSize) __attribute__((weak));
extern int tvt_param_getABFBtnCfg(void* pParam, int s32ParamSize) __attribute__((weak));
extern int tvt_param_getIrcutCfg(void* pParam, int s32ParamSize) __attribute__((weak));
extern int tvt_param_getWiegandCfg(void* pParam, int s32ParamSize) __attribute__((weak));
extern int tvt_param_getIRCfg(void* pParam, int s32ParamSize) __attribute__((weak));
extern int tvt_param_getWhiteLightCfg(void* pParam, int s32ParamSize) __attribute__((weak));



int tvt_param_getFeature(void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int tvt_param_getVinInfo(void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int tvt_param_getEncSupp(void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

// int tvt_param_getAudioInSupp(void* pParam, int s32ParamSize)
// {
//     TIPC_PRINTF_CHK("no impl ------------------------------> todo");
//     return 0;
// }

// int tvt_param_getAudioOutSupp(void* pParam, int s32ParamSize)
// {
//     TIPC_PRINTF_CHK("no impl ------------------------------> todo");
//     return 0;
// }

int tvt_param_getUartCfg(void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int tvt_param_getRS485Cfg(void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int tvt_param_getCDSCfg(void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int tvt_param_getSDCardCfg(void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int tvt_param_getAlarmInCfg(void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int tvt_param_getAlarmOutCfg(void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int tvt_param_getAlarmLightCfg(void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int tvt_param_getRstBtnCfg(void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int tvt_param_getZoomBtnCfg(void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int tvt_param_getABFBtnCfg(void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int tvt_param_getIrcutCfg(void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int tvt_param_getWiegandCfg(void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int tvt_param_getIRCfg(void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}

int tvt_param_getWhiteLightCfg(void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
}



/**
 * @brief 初始化参数业务
 */
int tvt_sdk_param_init()
{
    return 0;
}

/**
 * @brief 去初始化参数业务
 */
int tvt_sdk_param_uninit()
{
    return 0;
}


TVT_SDK_PARAM_IMPL_S g_stParamImpl[] =
{
    {TVT_SDK_PARAM_GET_FEATURE,             tvt_param_getFeature},
    {TVT_SDK_PARAM_GET_SENSOR_CFG,          tvt_param_getVinInfo},
    {TVT_SDK_PARAM_GET_STREAM_CFG,          tvt_param_getEncSupp},
    {TVT_SDK_PARAM_GET_AUDIO_IN_CFG,        tvt_param_getAudioInSupp},
    {TVT_SDK_PARAM_GET_AUDIO_OUT_CFG,       tvt_param_getAudioOutSupp},
    {TVT_SDK_PARAM_GET_UART_CFG,            tvt_param_getUartCfg},
    {TVT_SDK_PARAM_GET_RS485_CFG,           tvt_param_getRS485Cfg},
    {TVT_SDK_PARAM_GET_CDS_CFG,             tvt_param_getCDSCfg},
    {TVT_SDK_PARAM_GET_SDCARD_CFG,          tvt_param_getSDCardCfg},
    {TVT_SDK_PARAM_GET_ALARMIN_CFG,         tvt_param_getAlarmInCfg},
    {TVT_SDK_PARAM_GET_ALARMOUT_CFG,        tvt_param_getAlarmOutCfg},
    {TVT_SDK_PARAM_GET_ALARMLIGHT_CFG,      tvt_param_getAlarmLightCfg},
    {TVT_SDK_PARAM_GET_RESET_BTN_CFG,       tvt_param_getRstBtnCfg},
    {TVT_SDK_PARAM_GET_ZOOM_BTN_CFG,        tvt_param_getZoomBtnCfg},
    {TVT_SDK_PARAM_GET_ABF_BTN_CFG,         tvt_param_getABFBtnCfg},
    {TVT_SDK_PARAM_GET_IRCUT_CFG,           tvt_param_getIrcutCfg},
    {TVT_SDK_PARAM_GET_WIEGAND_CFG,         tvt_param_getWiegandCfg},
    {TVT_SDK_PARAM_GET_IR_CFG,              tvt_param_getIRCfg},
    {TVT_SDK_PARAM_GET_WHITELIGHT_CFG,      tvt_param_getWhiteLightCfg},
};


/**
 * @brief 参数业务控制
 */
// 主要获取各个模块参数配置，转发到产品C文件中，具体参数赋值由产品C文件中的接口实现，因此产品C文件内可以根据外壳实现返回不同配置
int tvt_sdk_param_ctrl(int s32Cmd, void* pParam, int s32Length)
{
    int s32Ret = 0;
    int i = 0;
    if (NULL == pParam)
    {
        TIPC_PRINTF_ERR("NULL pointer!! %d", s32Cmd);
        return -1;
    }

    int s32SuppFuncSize = sizeof(g_stParamImpl) / sizeof(TVT_SDK_PARAM_IMPL_S);
    for (i = 0; i < s32SuppFuncSize; i++)
    {
        if ((s32Cmd == (int)g_stParamImpl[i].enCmd))
        {
            CHK_FUNC(g_stParamImpl[i].pImplFunc);
            s32Ret = g_stParamImpl[i].pImplFunc(pParam, s32Length);
            break;
        }
    }

    if (i >= s32SuppFuncSize)
    {
        TIPC_PRINTF_ERR("param unsupport cmd[%d]!!", s32Cmd);
        return -1;
    }

    return s32Ret;
}
