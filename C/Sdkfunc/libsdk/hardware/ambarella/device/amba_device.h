#ifndef _AMBA_DEVICE_H_
#define _AMBA_DEVICE_H_

#include "tvt_sdk_chip.h"

#include "iav_ioctl.h"
#include <basetypes.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_device_init();

/**
 * @brief ȥ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_device_exit();



#ifdef __cplusplus
}
#endif

#endif