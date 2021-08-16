#ifndef _TVT_SDK_VIDEO_MASK_H_
#define _TVT_SDK_VIDEO_MASK_H_

#include "tvt_sdk_video.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ����MASKλ���Լ�����
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_mask_setPos(int s32VinIDX, int s32Enable, const SDK_POSITION* pstPos);

/**
 * @brief ��ʾ����MASK
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_mask_startDispALL();

/**
 * @brief ֹͣ��ʾ����MASK
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_mask_stopDispALL();

/**
 * @brief ��ʼ����ƵMASKҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_mask_init(int s32VinNum, const int* s32VinDevID);

/**
 * @brief ȥ��ʼ����ƵMASKҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_mask_exit();


#ifdef __cplusplus
}
#endif

#endif