#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/prctl.h>

#include "sdk_func.h"
#include "sdk_define.h"
#include "sdk_private.h"

#include "tvt_sdk_param.h"
#include "tvt_sdk_print.h"
#include "tvt_sdk_video.h"
#include "tvt_sdk_device.h"
#include "tvt_sdk_btn.h"

/**
 * @brief Zoom按键回调接口
 */
int SDKLensZoomBtnCallBack(void* pParam)
{
    if (NULL == pParam)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    SDK_ZOOM_CB_PARAM_S* pstBtnParam = (SDK_ZOOM_CB_PARAM_S*)pParam;
    SDK_LENS_RUN stZoomParam = {0};
    stZoomParam.type = SDK_AF_ZOOM;
    stZoomParam.str.status = pstBtnParam->enStatus;
    stZoomParam.str.mode = pstBtnParam->enMode;
    stZoomParam.str.direction = pstBtnParam->enDirection;
    SDKSetLensPara(SDK_LENS_AF_CTRL, &stZoomParam, sizeof(SDK_LENS_RUN));
    return 0;
}


/**
 * @brief 获取镜头参数
 */
int SDKGetLensPara(SDK_LENS_TYPE_PARA eLensType, void *pPara, unsigned int len)
{
    // 此接口不支持
    return 0;
}


/**
 * @brief 设置镜头参数
 */
int SDKSetLensPara(SDK_LENS_TYPE_PARA eLensType, void *pPara, unsigned int len)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
    int s32Ret = 0;
    if ((NULL == pPara) && (len > 0))
    {
        TIPC_PRINTF_CHK("SDKSetLensPara, cmd %d, data is NULL, iLenght=%d\n", (int)eLensType, len);
        return -1;
    }
    switch (eLensType)
    {
    case SDK_LENS_AF_CTRL:
    {
        if ((NULL != pPara) && (sizeof(SDK_LENS_RUN) == len))
        {
            /// TODO:
        }
        break;
    }
    default:
    {
        TIPC_PRINTF_ERR("invalid command define %d", eLensType);
        break;
    }
    }
    return s32Ret;
}
