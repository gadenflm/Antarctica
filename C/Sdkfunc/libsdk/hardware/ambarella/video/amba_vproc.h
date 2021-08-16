#ifndef _AMBA_VPROC_H_
#define _AMBA_VPROC_H_

#include "tvt_sdk_chip.h"

#include "iav_ioctl.h"
#include "iav_ucode_ioctl.h"
#include "vp_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct amba_vproc_res_cfg_s
{
    int             s32VinDevID;        // �󶨵�Vin�豸��
    int             s32MainWidth;       // Vin��
    int             s32MainHeight;      // Vin��
    enum amba_video_hdr_mode    enHdrMode;         // hdrģʽ
    int             s32Resv[4];
}AMBA_VPROC_RES_CFG_S;

typedef struct amba_vproc_chn_cfg_s
{
    int             s32VinDevID;        // ����Vin�豸��
    int             s32StreamIDX;       // ��������
    int             s32Width;           // �����
    int             s32Height;          // �����
    int             s32Fps;             // ����֡��
    int             s32Resv[3];
}AMBA_VPROC_CHN_CFG_S;


/**
 * @brief ��ȡvprocԴ����
 * @param [in] pstVideoCfg��vproc����
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vproc_getSourceCfg(CHIP_VIDEO_CFG_S* pstVideoCfg);

/**
 * @brief ��Ƶ����ģ��ͨ������
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vproc_chnCfg(const AMBA_VPROC_CHN_CFG_S* pstVprocChnCfg);

/**
 * @brief ��Ƶ����ģ���ʼ��(����iav system resource)
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vproc_start(const AMBA_VPROC_RES_CFG_S* pstVprocCfg, int s32VinNum);

/**
 * @brief ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vproc_init(int s32IavFD);

/**
 * @brief ȥ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vproc_exit();



#ifdef __cplusplus
}
#endif

#endif