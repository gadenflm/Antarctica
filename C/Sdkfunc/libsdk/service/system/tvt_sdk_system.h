#ifndef _TVT_SDK_SYSTEM_H_
#define _TVT_SDK_SYSTEM_H_

#include "tvt_sdk_product_param.h"
#include "tvt_sdk_chip.h"
#include "sdk_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ��ʼ��ϵͳ���ҵ��
 * @return 0���ɹ�����0��ʧ��
 */
/// TODO: ����δ���
int tvt_sdk_system_init(); 

/**
 * @brief ȥ��ʼ��ϵͳ���ҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_system_exit();

/**
 * @brief ϵͳҵ����ƽӿ�
 * @param [in] s32Cmd:������
 * @param [in] pParam:����ָ��
 * @param [in] s32Length:��������
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_system_ctrl(int s32Cmd, void* pParam, int s32Length);


#ifdef __cplusplus
}
#endif

#endif