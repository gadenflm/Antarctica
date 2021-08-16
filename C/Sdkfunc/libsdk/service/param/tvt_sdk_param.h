#ifndef _TVT_SDK_PARAM_H_
#define _TVT_SDK_PARAM_H_

#include "tvt_sdk_product_param.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ��ʼ������ҵ��
 */
int tvt_sdk_param_init();

/**
 * @brief ȥ��ʼ������ҵ��
 */
int tvt_sdk_param_uninit();

/**
 * @brief ����ҵ�����
 * @param s32Cmd --> TVT_SDK_PARAM_CMD_E
 * @param pParam --> TVT_SDK_PROD_CTRL_INFO
 * @param s32Length --> sizeof(TVT_SDK_PROD_CTRL_INFO)
 */
// ��Ҫ��ȡ����ģ��������ã�ת������ƷC�ļ��У����������ֵ�ɲ�ƷC�ļ��еĽӿ�ʵ�֣���˲�ƷC�ļ��ڿ��Ը������ʵ�ַ��ز�ͬ����
int tvt_sdk_param_ctrl(int s32Cmd, void* pParam, int s32Length);

#ifdef __cplusplus
}
#endif

#endif