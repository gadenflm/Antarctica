#ifndef _AMBA_OSD_H_
#define _AMBA_OSD_H_

#include "tvt_sdk_chip.h"

#include "iav_ioctl.h"
#include <basetypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ����OSD
 * @return 0���ɹ�����0��ʧ��
 */
int amba_osd_setOverLay(CHIP_OSD_INFO_S* pstOSDInfo);

/**
 * @brief ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_osd_init(int s32IavFD, int s32VinNum);

/**
 * @brief ȥ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_osd_exit();



#ifdef __cplusplus
}
#endif

#endif