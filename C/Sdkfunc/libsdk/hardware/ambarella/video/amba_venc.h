#ifndef _AMBA_VENC_H_
#define _AMBA_VENC_H_

#include "tvt_sdk_chip.h"

#include "iav_ioctl.h"
#include "iav_ucode_ioctl.h"
#include "vp_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct amba_venc_chn_h26x_cfg_s
{
    int             s32Gop;             // I֡���
    int             s32BitRateCtrl;     // IAV_BRC_CBR/IAV_BRC_VBR...
    int             s32BitRate;         // ����
    int             s32Profile;         // ����
    int             s32Resv[4];
} AMBA_VENC_CHN_H26X_CFG_S;

typedef struct amba_venc_chn_mjpeg_cfg_s
{
    int             s32Quality;         // ����
    int             s32Resv[7];
} AMBA_VENC_CHN_MJPEG_CFG_S;

typedef struct amba_venc_chn_cfg_s
{
    int             s32VinDevID;        // ����Vin�豸��
    int             s32StreamIDX;       // ��������
    int             s32Width;           // �����
    int             s32Height;          // �����
    int             s32Fps;             // ����֡��
    enum iav_stream_type    enStreamType;      // ��������
    union
    {
        AMBA_VENC_CHN_H26X_CFG_S    stH26xCfg;  // H26x����
        AMBA_VENC_CHN_MJPEG_CFG_S   stMjpegCfg; // Mjpeg����
    };

    int             s32Resv[3];
} AMBA_VENC_CHN_CFG_S;


/**
 * @brief ��������ͨ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_venc_createChn(const AMBA_VENC_CHN_CFG_S* pstVencChnCfg);

/**
 * @brief ��ȡ����ͨ������
 * @return 0���ɹ�����0��ʧ��
 */
int amba_venc_getChnCfg(AMBA_VENC_CHN_CFG_S* pstVencChnCfg);

/**
 * @brief ���ٱ���ͨ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_venc_destroyChn(int s32VinDevID, int s32StreamIDX);

/**
 * @brief ��ȡ����ͨ������
 * @return 0���ɹ�����0��ʧ��
 */
int amba_venc_getChnStream(int s32VinDevID, int s32StreamIDX, struct iav_framedesc* pstFrameDesc, char** pszStreamBuffer);

/**
 * @brief �ͷű���ͨ������
 * @return 0���ɹ�����0��ʧ��
 */
int amba_venc_releaseChnStream(struct iav_framedesc* pstFrameDesc);

/**
 * @brief ����IDR֡
 * @return 0���ɹ�����0��ʧ��
 */
int amba_venc_requestIDRStream(int s32VinDevID, int s32StreamIDX);

/**
 * @brief ʹ�ܱ���ͨ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_venc_startStream(int s32VinDevID, int s32StreamIDX);

/**
 * @brief ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_venc_init(int s32IavFD, int s32SensorCnt);

/**
 * @brief ȥ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_venc_exit();

/**
 * @breif get Bsb Memory Address
 */
char * amba_venc_getBsbMem (void);

#ifdef __cplusplus
}
#endif

#endif