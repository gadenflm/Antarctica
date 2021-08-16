#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "tvt_sdk_video_mask.h"
#include "tvt_sdk_print.h"
#include "tvt_sdk_chip.h"

#define TVT_SDK_MASK_MAX_COUNT 4

#define APP_MASK_REF_PIC_W  640    // Ӧ�ò��·���λ�����ڻ����
#define APP_MASK_REF_PIC_H  480    // Ӧ�ò��·���λ�����ڻ����

/*!
 * @macros ROUND_UP(x,d)
 * @brief round up func.
 */
#define ROUND_UP(x, n)  ( ((x)+(n)-1u) & ~((n)-1u) )

#ifdef CHIP_AMBA
#define MASK_ALIGN_W 32
#define MASK_ALIGN_H 4
#else
#define MASK_ALIGN_W 16
#define MASK_ALIGN_H 2
#endif

/**
 * @brief ҵ��״̬
 */
typedef enum tvt_sdk_mask_status_e
{
    TVT_SDK_MASK_RUN = 0,
    TVT_SDK_MASK_PAUSE,
    TVT_SDK_MASK_STATUS_BUTT,
}TVT_SDK_MASK_STATUS_E;


/**
 * @brief MASK������Ϣ
 */
typedef struct tvt_sdk_mask_data_info_s
{
    unsigned int                u32Enable;          ///< ʹ��
    SDK_POSITION                stSrcPos;           ///< ���λ��
    SDK_POSITION                stDstPos;           ///< ������ʾλ��
} TVT_SDK_MASK_DATA_INFO_S;


/**
 * @brief MASKҵ��ȫ������
 */
typedef struct tvt_sdk_mask_cfg_s
{
    int                         s32VinDev;          ///< ������Ƶ�����豸��
    unsigned int                u32Colour;          ///< ��ɫ
    TVT_SDK_MASK_DATA_INFO_S    stMaskInfo[TVT_SDK_MASK_MAX_COUNT];       ///< ÿ��MASK�ľ�����Ϣ
} TVT_SDK_MASK_CFG_S;

/**
 * @brief ҵ��ṹ��
 */
typedef struct tvt_sdk_mask_info_s
{
    char                        u8Init;                                 ///< ��ʼ����־λ
    char                        u8Exit;                                 ///< ȥ��ʼ����־λ���߳��˳���־
    TVT_SDK_MASK_STATUS_E       enStatus;                               ///< ҵ������״̬
    pthread_mutex_t             s32Mutex;                               ///< ҵ����
    char                        szMutexName[32];                        ///< ҵ��������
    unsigned int                u32MutexLockCnt;                        ///< ҵ������������
    TVT_SDK_MASK_CFG_S          stMaskCfg[CHIP_MAX_VI_NUM];             ///< MASK����
}TVT_SDK_MASK_INFO_S;

static TVT_SDK_MASK_INFO_S* g_pstSdkMaskInfo = NULL;

/**
 * @brief ��ȡģ��ṹ��ָ��
 */
static TVT_SDK_MASK_INFO_S* tvt_sdk_mask_getHandle(void)
{
    return g_pstSdkMaskInfo;
}


/**
 * @brief ����MASKλ���Լ�����
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_mask_setPos(int s32VinIDX, int s32Enable, const SDK_POSITION* pstPos)
{
    int s32Ret = 0;
    TVT_SDK_MASK_INFO_S* pstHandle = tvt_sdk_mask_getHandle();
    if(NULL == pstHandle || NULL == pstPos)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    if(s32VinIDX >= CHIP_MAX_VI_NUM)
    {
        TIPC_PRINTF_ERR("err Vin Index[%d]", s32VinIDX);
        return -1;
    }
    int s32VinDevID = pstHandle->stMaskCfg[s32VinIDX].s32VinDev;
    if(s32VinDevID < 0)
    {
        TIPC_PRINTF_ERR("err Vin DevID[%d] with Vin Index[%d] ", s32VinDevID, s32VinIDX);
        return -1;
    }

    // ��ȡVin���
    CHIP_VIDEO_CFG_S stVinCfg = {0};
    stVinCfg.stSensorCfg.u32VinDev = (int)s32VinDevID;
    s32Ret = chip_video_getVinCfg(&stVinCfg);
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("err chip_video_getVinCfg");
        return -1;
    }
    int s32Width = stVinCfg.stSensorCfg.u32MaxWidth;
    int s32Height = stVinCfg.stSensorCfg.u32MaxHeight;
    if(s32Width <= 0 || s32Height <= 0)
    {
        TIPC_PRINTF_ERR("err Vin width[%d] height[%d] ", s32Width, s32Height);
        return -1;
    }

    // �ֱ���ת��
    SDK_POSITION stDstPos = {0};
    stDstPos.x = pstPos->x * s32Width / APP_MASK_REF_PIC_W;
    stDstPos.y = pstPos->y * s32Height / APP_MASK_REF_PIC_H;
    stDstPos.width = pstPos->width * s32Width / APP_MASK_REF_PIC_W;
    stDstPos.height = pstPos->height * s32Height / APP_MASK_REF_PIC_H;

    // Ӧ��
    CHIP_MASK_INFO_S stMaskInfo = {0};
    stMaskInfo.s32Enable = s32Enable;
    stMaskInfo.s32X = stDstPos.x;
    stMaskInfo.s32Y = stDstPos.y;
    stMaskInfo.s32W = stDstPos.width;
    stMaskInfo.s32H = stDstPos.height;
    stMaskInfo.s32VinDevID = s32VinDevID;
    s32Ret = chip_video_setMask(&stMaskInfo);
    if(s32Ret != 0)
    {
        TIPC_PRINTF_ERR("err chip_video_setMask ");
        return -1;
    }

    return 0;
}


/**
 * @brief ��ʼ����ƵMASKҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_mask_init(int s32VinNum, const int* s32VinDevID)
{
    int s32Ret = -1;
    int i = 0;
    if(g_pstSdkMaskInfo != NULL)
    {
        if(g_pstSdkMaskInfo->u8Init != 0)
        {
            // �ѳ�ʼ��
            TIPC_PRINTF_ERR("video mask service is running");
            return 0;
        }
    }
    else
    {
        // �����ṹ��
        g_pstSdkMaskInfo = (TVT_SDK_MASK_INFO_S* )malloc(sizeof(TVT_SDK_MASK_INFO_S));
        if(NULL == g_pstSdkMaskInfo)
        {
            TIPC_PRINTF_ERR("video mask service malloc failed");
            return s32Ret;
        }
        // ��ʼ������
        memset(g_pstSdkMaskInfo, 0, sizeof(TVT_SDK_MASK_INFO_S));
        for(i = 0; i < CHIP_MAX_VI_NUM; i++)
        {
            g_pstSdkMaskInfo->stMaskCfg[i].s32VinDev = -1;
        }
    }

    // ��ʼ��ģ����
    pthread_mutex_init(&g_pstSdkMaskInfo->s32Mutex, NULL);
    sprintf(g_pstSdkMaskInfo->szMutexName, "tvt_sdk_mask_lock");
    g_pstSdkMaskInfo->u8Init = 1;
    g_pstSdkMaskInfo->enStatus = TVT_SDK_MASK_RUN;
    for(i = 0; i < s32VinNum; i++)
    {
        g_pstSdkMaskInfo->stMaskCfg[i].s32VinDev = s32VinDevID[i];
    }
    return 0;
}


/**
 * @brief ȥ��ʼ����ƵMASKҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_mask_exit()
{
    if(NULL == g_pstSdkMaskInfo)
    {
        // δ��ʼ��
        TIPC_PRINTF_ERR("video mask service is not running");
        return -1;
    }

    if (g_pstSdkMaskInfo->u8Init != 1)
    {
        // δ��ʼ��
        TIPC_PRINTF_ERR("video mask service is not running");
        free((void*)g_pstSdkMaskInfo);
        g_pstSdkMaskInfo = NULL;
        return -1;
    }
    else
    {
        pthread_mutex_destroy(&g_pstSdkMaskInfo->s32Mutex);
        g_pstSdkMaskInfo->u8Init = 0;
        free((void*)g_pstSdkMaskInfo);
        g_pstSdkMaskInfo = NULL;
    }
    return 0;
}