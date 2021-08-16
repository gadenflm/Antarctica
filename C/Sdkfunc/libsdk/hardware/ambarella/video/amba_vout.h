#ifndef _AMBA_VOUT_H_
#define _AMBA_VOUT_H_

#include "tvt_sdk_chip.h"

#include "iav_ioctl.h"
#include "iav_ucode_ioctl.h"
#include "vp_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AMBA_VOUT_ID 0

/**
 * @brief ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vout_init(int s32IavFD, CHIP_VIDEO_FORMAT_E enFormat);

/**
 * @brief ȥ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vout_exit();

#ifdef __cplusplus
}
#endif

#endif