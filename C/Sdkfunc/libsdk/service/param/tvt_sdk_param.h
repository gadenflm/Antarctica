#ifndef _TVT_SDK_PARAM_H_
#define _TVT_SDK_PARAM_H_

#include "tvt_sdk_product_param.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化参数业务
 */
int tvt_sdk_param_init();

/**
 * @brief 去初始化参数业务
 */
int tvt_sdk_param_uninit();

/**
 * @brief 参数业务控制
 * @param s32Cmd --> TVT_SDK_PARAM_CMD_E
 * @param pParam --> TVT_SDK_PROD_CTRL_INFO
 * @param s32Length --> sizeof(TVT_SDK_PROD_CTRL_INFO)
 */
// 主要获取各个模块参数配置，转发到产品C文件中，具体参数赋值由产品C文件中的接口实现，因此产品C文件内可以根据外壳实现返回不同配置
int tvt_sdk_param_ctrl(int s32Cmd, void* pParam, int s32Length);

#ifdef __cplusplus
}
#endif

#endif