#ifndef _TVT_SDK_SYSTEM_H_
#define _TVT_SDK_SYSTEM_H_

#include "tvt_sdk_product_param.h"
#include "tvt_sdk_chip.h"
#include "sdk_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化系统相关业务
 * @return 0：成功；非0：失败
 */
/// TODO: 该如何传参
int tvt_sdk_system_init(); 

/**
 * @brief 去初始化系统相关业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_system_exit();

/**
 * @brief 系统业务控制接口
 * @param [in] s32Cmd:命令字
 * @param [in] pParam:参数指针
 * @param [in] s32Length:参数长度
 * @return 0：成功；非0：失败
 */
int tvt_sdk_system_ctrl(int s32Cmd, void* pParam, int s32Length);


#ifdef __cplusplus
}
#endif

#endif