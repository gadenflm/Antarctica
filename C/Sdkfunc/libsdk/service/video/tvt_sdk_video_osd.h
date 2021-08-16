#ifndef _TVT_SDK_VIDEO_OSD_H_
#define _TVT_SDK_VIDEO_OSD_H_

#include "tvt_sdk_video.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief ����ʱ��OSD��ʽλ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_osd_setTimeOSDCfg(int s32VinIDX, const TVT_SDK_OSD_TIME_FMT_S* pstTimeFmt, const SDK_POSITION* pstPos);

/**
 * @brief ����Comm OSDλ���Լ�����
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_osd_setCommOSD(int s32VinIDX, int s32OSDIDX, const TVT_SDK_OSD_COMM_INFO_S* pstCfg, const SDK_POSITION* pstPos);

/**
 * @brief ˢ��Comm OSD��ʾ����
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_osd_updateCommOSD(int s32VinIDX, int s32OSDIDX);

/**
 * @brief ˢ��ʱ��OSD����
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_osd_updateTimeOSD(int s32VinIDX);

/**
 * @brief ��ʾ����OSD
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_osd_startDispALLOSD();

/**
 * @brief ֹͣ��ʾ����OSD
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_osd_stopDispALLOSD();


/**
 * @brief ��ʼ����ƵOSDҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_osd_init(int s32VinNum, const int* s32VinDevID);

/**
 * @brief ȥ��ʼ����ƵOSDҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_osd_exit();


#ifdef __cplusplus
}
#endif

#endif