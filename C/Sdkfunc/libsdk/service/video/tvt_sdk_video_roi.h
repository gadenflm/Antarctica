#ifndef _TVT_SDK_VIDEO_ROI_H_
#define _TVT_SDK_VIDEO_ROI_H_

#include "tvt_sdk_video.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief ����ROIλ���Լ�����
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_roi_setPos(int s32VinIDX, int s32ROINum, const TVT_SDK_VIDEO_ROI_INFO_S* pstROIInfo);

/**
 * @brief ��ʾ����ROI
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_roi_startDispALL();

/**
 * @brief ֹͣ��ʾ����ROI
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_roi_stopDispALL();


/**
 * @brief ��ʼ����ƵROIҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_roi_init(int s32VinNum, const int* s32VinDevID);

/**
 * @brief ȥ��ʼ����ƵROIҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_roi_exit();


#ifdef __cplusplus
}
#endif

#endif