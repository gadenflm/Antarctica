#ifndef _AMBA_QPROI_H_
#define _AMBA_QPROI_H_

#include "tvt_sdk_chip.h"

#include "iav_ioctl.h"
#include <basetypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ����ROI
 * @return 0���ɹ�����0��ʧ��
 */
int amba_roi_setPos(CHIP_ROI_INFO_S* pstROIInfo);

/**
 * @brief ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_roi_init(int s32IavFD, int s32VinNum);

/**
 * @brief ȥ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_roi_exit();



#ifdef __cplusplus
}
#endif

#endif