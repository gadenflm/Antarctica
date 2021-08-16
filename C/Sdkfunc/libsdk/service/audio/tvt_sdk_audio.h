#ifndef _TVT_SDK_AUDIO_H_
#define _TVT_SDK_ADUIO_H_

#include "tvt_sdk_product_param.h"
#include "sdk_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ��������ö��
 */
typedef enum tvt_sdk_audio_cmd_e
{
    TVT_SDK_AUDIO_CFG_CAPTURE,      ///< PARAM: TVT_SDK_AUDIO_CFG_S
    TVT_SDK_AUDIO_CFG_PLAYBACK,     ///< PARAM: TVT_SDK_AUDIO_CFG_S
    TVT_SDK_AUDIO_SET_CAP_VOLUME,   ///< PARAM: int [0,100]
    TVT_SDK_AUDIO_SET_PLAY_VOLUME,  ///< PARAM: int [0,100]
    TVT_SDK_AUDIO_CMD_BUTT,
} TVT_SDK_AUDIO_CMD_E;


typedef struct tvt_sdk_audio_cfg_s
{
    SDK_AUDIO_ENCODE_TYPE   enCodeType;     ///< ��������
    int                     s32Rate;        ///< ������ 8000/16000...
    int                     s32BitWidth;    ///< λ�� 8/16/24
    int                     s32FrameSize;   ///< ��֡������
} TVT_SDK_AUDIO_CFG_S;


/**
 * @brief ��ʼ����Ƶҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_audio_init(TVT_SDK_PROD_DEV_INFO* pstParamDevInfo, TvtSdkProdCb pParamCb);

/**
 * @brief ȥ��ʼ����Ƶҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_audio_exit();

/**
 * @brief ��Ƶҵ����ƽӿ�
 * @param [in] s32Cmd:������
 * @param [in] pParam:����ָ��
 * @param [in] s32Length:��������
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_audio_ctrl(int s32Cmd, void* pParam, int s32Length);

/**
 * @brief ��ȡ��Ƶ������
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_audio_getCaptureStream(IPCAMERA_AV_FRAME_INFO* pstFrameInfo);

/**
 * @brief ������Ƶ������
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_audio_sendPlayStream(IPCAMERA_AV_FRAME_INFO* pstFrameInfo);

/**
 * @brief �ͷ���Ƶ������
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_audio_releaseStream(IPCAMERA_AV_FRAME_INFO* pstFrameInfo);

#ifdef __cplusplus
}
#endif

#endif