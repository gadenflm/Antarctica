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
    int             s32Gop;             // I帧间隔
    int             s32BitRateCtrl;     // IAV_BRC_CBR/IAV_BRC_VBR...
    int             s32BitRate;         // 码率
    int             s32Profile;         // 级别
    int             s32Resv[4];
} AMBA_VENC_CHN_H26X_CFG_S;

typedef struct amba_venc_chn_mjpeg_cfg_s
{
    int             s32Quality;         // 质量
    int             s32Resv[7];
} AMBA_VENC_CHN_MJPEG_CFG_S;

typedef struct amba_venc_chn_cfg_s
{
    int             s32VinDevID;        // 所在Vin设备号
    int             s32StreamIDX;       // 编码索引
    int             s32Width;           // 编码宽
    int             s32Height;          // 编码高
    int             s32Fps;             // 编码帧率
    enum iav_stream_type    enStreamType;      // 编码类型
    union
    {
        AMBA_VENC_CHN_H26X_CFG_S    stH26xCfg;  // H26x配置
        AMBA_VENC_CHN_MJPEG_CFG_S   stMjpegCfg; // Mjpeg配置
    };

    int             s32Resv[3];
} AMBA_VENC_CHN_CFG_S;


/**
 * @brief 创建编码通道
 * @return 0：成功；非0：失败
 */
int amba_venc_createChn(const AMBA_VENC_CHN_CFG_S* pstVencChnCfg);

/**
 * @brief 获取编码通道配置
 * @return 0：成功；非0：失败
 */
int amba_venc_getChnCfg(AMBA_VENC_CHN_CFG_S* pstVencChnCfg);

/**
 * @brief 销毁编码通道
 * @return 0：成功；非0：失败
 */
int amba_venc_destroyChn(int s32VinDevID, int s32StreamIDX);

/**
 * @brief 获取编码通道数据
 * @return 0：成功；非0：失败
 */
int amba_venc_getChnStream(int s32VinDevID, int s32StreamIDX, struct iav_framedesc* pstFrameDesc, char** pszStreamBuffer);

/**
 * @brief 释放编码通道数据
 * @return 0：成功；非0：失败
 */
int amba_venc_releaseChnStream(struct iav_framedesc* pstFrameDesc);

/**
 * @brief 请求IDR帧
 * @return 0：成功；非0：失败
 */
int amba_venc_requestIDRStream(int s32VinDevID, int s32StreamIDX);

/**
 * @brief 使能编码通道
 * @return 0：成功；非0：失败
 */
int amba_venc_startStream(int s32VinDevID, int s32StreamIDX);

/**
 * @brief 初始化模块
 * @return 0：成功；非0：失败
 */
int amba_venc_init(int s32IavFD, int s32SensorCnt);

/**
 * @brief 去初始化模块
 * @return 0：成功；非0：失败
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