
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "amba_vout.h"

#ifndef AM_IOCTL
#define AM_IOCTL(_filp, _cmd, _arg)	\
        do {                        \
            if (ioctl(_filp, _cmd, _arg) < 0) { \
                perror(#_cmd);      \
                TIPC_PRINTF_ERR("%s err [%d]", #_cmd, errno); \
                return -1;      \
            }                       \
        } while (0)
#endif

typedef struct amba_vout_cfg_s
{
    int              s32Fd;          ///< �豸�ļ�������
    int              s32Dev;         ///< �豸��(��ӦSensor���ӵ��豸)
    int              s32Chn;         ///< ͨ����
    unsigned char    u8FrameRate;    ///< ֡��
    unsigned char    u8Init;         ///< ��ʼ����־
    unsigned char    u8DevEnable;    ///< �豸������־
    unsigned char    u8ChnEnable;    ///< ͨ����������־
}AMBA_VOUT_CFG_S;

static AMBA_VOUT_CFG_S g_stVoutCfg = {0};

/**
 * @brief ����Voutģ��󶨵�Buffer
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vout_cfgBuffer(int s32IavFD, CHIP_VIDEO_FORMAT_E enFormat)
{
    if(s32IavFD <= 0)
    {
        TIPC_PRINTF_ERR("/dev/iav check err! [%d]", s32IavFD);
        return -1;
    }

    struct iav_video_proc stIavVproc = {0};
    struct iav_dptz* pstIavDptz = &stIavVproc.arg.dptz;
    struct iav_apply_flag stApplyFlag[IAV_VIDEO_PROC_NUM] = {0};
    stIavVproc.cid = IAV_VIDEO_PROC_DPTZ;
    pstIavDptz->channel_id = 0;
    pstIavDptz->pass_id = 1;
    pstIavDptz->buf_id = IAV_SRCBUF_5;
    
    AM_IOCTL(s32IavFD, IAV_IOC_GET_VIDEO_PROC, &stIavVproc);

    // ���·ֱ���
    pstIavDptz->buf_cfg.output.width = VO_CVBS_MAX_WIDTH;
    pstIavDptz->buf_cfg.output.height = (CHIP_VIDEO_FORMAT_NTSC == enFormat)?480:576;
    AM_IOCTL(s32IavFD, IAV_IOC_CFG_VIDEO_PROC, &stIavVproc);

    stApplyFlag[IAV_VIDEO_PROC_DPTZ].param = (1 << 0);
    stApplyFlag[IAV_VIDEO_PROC_DPTZ].apply = 1;
    AM_IOCTL(s32IavFD, IAV_IOC_APPLY_VIDEO_PROC, stApplyFlag);

    return 0;
}

/**
 * @brief ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vout_init(int s32IavFD, CHIP_VIDEO_FORMAT_E enFormat)
{
    int s32Ret = 0;
    if(s32IavFD < 0)
    {
        TIPC_PRINTF_ERR("iav fd err[%d]", s32IavFD);
        return -1;
    }
    if(0 == g_stVoutCfg.u8Init)
    {
        // ����Ҫ����Buffer����
        s32Ret = amba_vout_cfgBuffer(s32IavFD, enFormat);
        if(s32Ret != 0)
        {
            return -1;
        }
        struct voutdev_format stVoutFormat = {0};
        int s32Mode = (CHIP_VIDEO_FORMAT_NTSC == enFormat)?AMBA_VIDEO_MODE_720_480:AMBA_VIDEO_MODE_720_576;
        int s32Fps = (CHIP_VIDEO_FORMAT_NTSC == enFormat)?59940:50000;

        stVoutFormat.id = AMBA_VOUT_ID;
        stVoutFormat.type = VOUT_TYPE_CVBS;
        stVoutFormat.mode = s32Mode;
        stVoutFormat.interlaced = 1;
        stVoutFormat.fps = s32Fps;

        AM_IOCTL(s32IavFD, IAV_IOC_VOUT_SET_MODE, &stVoutFormat);
        
        g_stVoutCfg.s32Fd = s32IavFD;
        g_stVoutCfg.s32Dev = AMBA_VOUT_ID;
        g_stVoutCfg.s32Chn = 0;

        g_stVoutCfg.u8Init = 1;
    }

    return 0;
}


/**
 * @brief ȥ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_vout_exit()
{
    return 0;
}