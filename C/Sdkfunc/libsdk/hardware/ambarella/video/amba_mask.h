#ifndef _AMBA_MASK_H_
#define _AMBA_MASK_H_

#include "tvt_sdk_chip.h"

#include "iav_ioctl.h"
#include <basetypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ����MASK
 * @return 0���ɹ�����0��ʧ��
 */
int amba_mask_setPos(CHIP_MASK_INFO_S* pstMaskInfo);

/**
 * @brief ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_mask_init(int s32IavFD, int s32VinNum);

/**
 * @brief ȥ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_mask_exit();


#ifdef __cplusplus
}
#endif

#endif