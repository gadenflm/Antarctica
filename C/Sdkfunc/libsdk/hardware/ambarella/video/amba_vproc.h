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
    int             s32VinDevID;        // 绑定的Vin设备号
    int             s32MainWidth;       // Vin宽
    int             s32MainHeight;      // Vin高
    enum amba_video_hdr_mode    enHdrMode;         // hdr模式
    int             s32Resv[4];
}AMBA_VPROC_RES_CFG_S;

typedef struct amba_vproc_chn_cfg_s
{
    int             s32VinDevID;        // 所在Vin设备号
    int             s32StreamIDX;       // 编码索引
    int             s32Width;           // 编码宽
    int             s32Height;          // 编码高
    int             s32Fps;             // 编码帧率
    int             s32Resv[3];
}AMBA_VPROC_CHN_CFG_S;


/**
 * @brief 获取vproc源属性
 * @param [in] pstVideoCfg：vproc属性
 * @return 0：成功；非0：失败
 */
int amba_vproc_getSourceCfg(CHIP_VIDEO_CFG_S* pstVideoCfg);

/**
 * @brief 视频处理模块通道设置
 * @return 0：成功；非0：失败
 */
int amba_vproc_chnCfg(const AMBA_VPROC_CHN_CFG_S* pstVprocChnCfg);

/**
 * @brief 视频处理模块初始化(设置iav system resource)
 * @return 0：成功；非0：失败
 */
int amba_vproc_start(const AMBA_VPROC_RES_CFG_S* pstVprocCfg, int s32VinNum);

/**
 * @brief 初始化模块
 * @return 0：成功；非0：失败
 */
int amba_vproc_init(int s32IavFD);

/**
 * @brief 去初始化模块
 * @return 0：成功；非0：失败
 */
int amba_vproc_exit();



#ifdef __cplusplus
}
#endif

#endif