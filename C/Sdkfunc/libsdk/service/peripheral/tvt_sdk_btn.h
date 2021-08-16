#ifndef _TVT_SDK_BTN_H_
#define _TVT_SDK_BTN_H_

#include "tvt_sdk_product_param.h"
#include "sdk_define.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TVT_SDK_BTN_MAX             (8)
#define TVT_SDK_BTN_EVENT_MAX       (4)

typedef int (*TvtSdkBtnCb)(void *);

typedef enum tvt_sdk_btn_event_e
{
    TVT_SDK_BTN_EVENT_NONE = 0,
    TVT_SDK_BTN_EVENT_DOWN,
    TVT_SDK_BTN_EVENT_UP,
    TVT_SDK_BTN_EVENT_BUTT,
}TVT_SDK_BTN_EVENT_E;

typedef struct tvt_sdk_btn_event_s
{
    TVT_SDK_BTN_EVENT_E enEvent;
    void*               pCbParam;
    TvtSdkBtnCb         pfBtnCb;
}TVT_SDK_BTN_EVENT_S;

typedef struct tvt_sdk_btn_s
{
    int s32EventNum;
    int s32GpioGrp;
    int s32GpioNum;
    TVT_SDK_BTN_EVENT_S stBtnEvent[TVT_SDK_BTN_EVENT_MAX];
    char szInfo[16];
} TVT_SDK_BTN_CFG_S;

/**
 * @brief 注册按键事件
 */
int tvt_sdk_btn_addBtnEvent(TVT_SDK_BTN_CFG_S* pstBtnCfg);

/**
 * @brief 初始化按键监控业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_btn_init();

/**
 * @brief 去初始化按键监控业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_btn_exit();


#ifdef __cplusplus
}
#endif

#endif