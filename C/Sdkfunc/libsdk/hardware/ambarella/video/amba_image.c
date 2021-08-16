
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "amba_image.h"
#include "img_flow_api.h"

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


typedef struct amba_image_cfg_s
{
    int                 s32Fd;                                                     ///< �豸�ļ�������
    int                 s32VinNum;                                                 ///< Sensor����
    unsigned char       u8Init;                                                    ///< 
    pthread_t           s32Thread;                                                 ///< �̺߳�
}AMBA_IMAGE_CFG_S;

AMBA_IMAGE_CFG_S g_stImageCfg = {0};

/****************************************************
 *	ir led control
 ****************************************************/
static int ir_led_ctrl(int chan_id, u8 op, void *p_data)
{
	if(op ==IR_LED_CONTROL_DO_NOTHING){//init
		//system("/usr/local/bin/amba_debug -g	44 -d0");
	}else if(op ==IR_LED_CONTROL_TURN_ON){//on
		//system("/usr/local/bin/amba_debug -g	44 -d1");
	}else if(op ==IR_LED_CONTROL_TURN_OFF){//off
		//system("/usr/local/bin/amba_debug -g	44 -d2");
	}
	return 0;
}

/**
 * @brief 3A�߳�
 * @return 
 */
void* amba_aaa_service(void* pData)
{
    int s32Ret = 0;
    if(NULL == pData)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return NULL;
    }
    int s32Fd = *(int*)pData;
    if(s32Fd <= 0)
    {
        TIPC_PRINTF_ERR("err iav fd failed[%d]", s32Fd);
        return NULL;
    }
    // �˺�����������
    s32Ret = create_img_flow(s32Fd);
    if(s32Ret < 0)
    {
        TIPC_PRINTF_ERR("create_img_flow failed[%d]", s32Ret);
        return NULL;
    }
    return NULL;
}

/**
 * @brief ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_image_init(int s32IavFD)
{
    // int s32Ret = 0;

    if(s32IavFD <= 0)
    {
        TIPC_PRINTF_ERR("/dev/iav is not opened! [%d]", s32IavFD);
        return -1;
    }

    //////Here is the sample code to register ir_led_dev control////
    // int this_is_reg_func_sample_flag =0;
    // int led_chan_id=0;
    img_flow_extra_func_t extra_func;
    memset(&extra_func,0,sizeof(img_flow_extra_func_t));
    extra_func.do_ir_led_ctrl=ir_led_ctrl;
    register_img_flow_api(&extra_func, NULL);
    g_stImageCfg.s32Fd = s32IavFD;
    if(pthread_create(&g_stImageCfg.s32Thread, NULL, (void*)amba_aaa_service, &g_stImageCfg.s32Fd) != 0)
    {
        TIPC_PRINTF_ERR("pthread_create 3A service failed");
        return -1;
    }

    g_stImageCfg.s32Fd = s32IavFD;
    g_stImageCfg.u8Init = 1;

    return 0;
}


/**
 * @brief ȥ��ʼ��ģ��
 * @return 0���ɹ�����0��ʧ��
 */
int amba_image_exit()
{
    // int s32Ret = 0;
    if(g_stImageCfg.s32Thread != 0)
    {
        destroy_img_flow();
        pthread_join(g_stImageCfg.s32Thread, NULL);
        g_stImageCfg.s32Thread = 0;
    }
    g_stImageCfg.s32Fd = 0;
    g_stImageCfg.u8Init = 0;
    return 0;
}