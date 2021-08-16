#ifndef _AMBA_VIN_H_
#define _AMBA_VIN_H_

#include "tvt_sdk_chip.h"

#include "iav_ioctl.h"
#include "iav_ucode_ioctl.h"
#include "vp_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ���þ���ͨ������Sensor�Ĵ���ʵ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vin_dev_setMirror(int s32VinDevID, int s32Enable);

/**
 * @brief ���÷�ת��ͨ������Sensor�Ĵ���ʵ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vin_dev_setFlip(int s32VinDevID, int s32Enable);

/**
 * @brief Step1������dev���ԣ�����Sensor��ǰ���õ�ģʽ
 * @param [in] pstVideoCfg��Dev����
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vin_dev_start(const CHIP_VIDEO_CFG_S* pstVideoCfg);


/**
 * @brief ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vin_init(int s32IavFD);

/**
 * @brief ȥ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vin_exit();



#ifdef __cplusplus
}
#endif

#endif