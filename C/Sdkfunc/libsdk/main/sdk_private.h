#ifndef _SDK_PRIVATE_H_
#define _SDK_PRIVATE_H_

#include "tvt_sdk_product_param.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tvt_sdk_cfg_s
{
    unsigned char                       u8Init;
    int                                 s32SDKVersion;      ///< �汾��
    unsigned long long                  u64DevFeature;      ///< �豸������
    TVT_SDK_PROD_DEV_INFO               stDevModelInfo;     ///< ����
    SDK_DAYNIGHT_STATE                  enDayNightState;    ///< ��ҹ״̬
    int                                 s32WorkMode;        ///< ����/HDRģʽ
    int                                 s32VinCnt;          ///< Sensor����
    pthread_t                           s32SdkThread;
    int                                 s32SdkThreadRun;
}TVT_SDK_CFG_S;


typedef struct sdk_zoomcb_param_s
{
    SDK_LENS_DIRECTION enDirection;
    SDK_LENS_STATUS    enStatus;
    SDK_LENS_MODE      enMode;
}SDK_ZOOM_CB_PARAM_S;

/**
 * @brief ��ʼ����Ƶҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int SDKInitVideo(SDK_VIDEO_FORMAT enInputFormat);



#ifdef __cplusplus
}
#endif

#endif