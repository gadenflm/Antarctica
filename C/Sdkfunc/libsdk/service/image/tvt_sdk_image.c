#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "tvt_sdk_image.h"
#include "tvt_sdk_print.h"
#include "tvt_sdk_chip.h"

#define TVT_SDK_IMAGE_DEBUG 0

/**
 * @brief ҵ��ṹ��
 */
typedef struct tvt_sdk_image_info_s
{
    char                        u8Init;                                 ///< ��ʼ����־λ
    char                        u8Exit;                                 ///< ȥ��ʼ����־λ���߳��˳���־
    pthread_mutex_t             s32Mutex;                               ///< ҵ����
    char                        szMutexName[32];                        ///< ҵ��������
    unsigned int                u32MutexLockCnt;                        ///< ҵ������������
    int                         s32VinDevID[CHIP_MAX_VI_NUM];
}TVT_SDK_IMAGE_INFO_S;

typedef int (*ImgImplFunc)(TVT_SDK_IMAGE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize);
typedef struct tvt_sdk_image_ctrl_impl_s
{
    TVT_SDK_IMAGE_CMD_E         enCmd;          ///< ������
    ImgImplFunc                 pImplFunc;      ///< ʵ�ֽӿ�
}TVT_SDK_IMAGE_CTRL_IMPL_S;

static TVT_SDK_IMAGE_INFO_S* g_pstSdkImageInfo = NULL;

/**
 * @brief ��ȡģ��ṹ��ָ��
 */
static TVT_SDK_IMAGE_INFO_S* tvt_sdk_image_getHandle(void)
{
    return g_pstSdkImageInfo;
}

/**
 * @brief ��ʼ����Ƶͼ����
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_image_setMirror(TVT_SDK_IMAGE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    TVT_SDK_IMAGE_MIRROR_CFG_S* pstMirrorCfg = (TVT_SDK_IMAGE_MIRROR_CFG_S*)pParam;
    int s32VinIDX = pstMirrorCfg->s32VinIDX;
    int s32Enable = pstMirrorCfg->s32Enable;
    if(s32VinIDX >= CHIP_MAX_VI_NUM)
    {
        TIPC_PRINTF_ERR("err Vin Index[%d]", s32VinIDX);
        return -1;
    }
    int s32VinDevID = pstHandle->s32VinDevID[s32VinIDX];
    if(s32VinDevID < 0)
    {
        TIPC_PRINTF_ERR("err Vin DevID[%d] with Vin Index[%d] ", s32VinDevID, s32VinIDX);
        return -1;
    }
    s32Ret = chip_video_setMirror(s32VinDevID, s32Enable);
    return s32Ret;
}

/**
 * @brief ��ʼ����Ƶͼ��ת
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_image_setFlip(TVT_SDK_IMAGE_INFO_S* pstHandle, int s32Dir, void* pParam, int s32ParamSize)
{
    int s32Ret = 0;
    TVT_SDK_IMAGE_FLIP_CFG_S* pstMirrorCfg = (TVT_SDK_IMAGE_FLIP_CFG_S*)pParam;
    int s32VinIDX = pstMirrorCfg->s32VinIDX;
    int s32Enable = pstMirrorCfg->s32Enable;
    if(s32VinIDX >= CHIP_MAX_VI_NUM)
    {
        TIPC_PRINTF_ERR("err Vin Index[%d]", s32VinIDX);
        return -1;
    }
    int s32VinDevID = pstHandle->s32VinDevID[s32VinIDX];
    if(s32VinDevID < 0)
    {
        TIPC_PRINTF_ERR("err Vin DevID[%d] with Vin Index[%d] ", s32VinDevID, s32VinIDX);
        return -1;
    }
    s32Ret = chip_video_setFlip(s32VinDevID, s32Enable);
    return s32Ret;
}


static TVT_SDK_IMAGE_CTRL_IMPL_S g_stFunList[] =
{
    {TVT_SDK_IMAGE_SET_MIRROR,                tvt_sdk_image_setMirror},
    {TVT_SDK_IMAGE_SET_FLIP,                  tvt_sdk_image_setFlip},
};

/**
 * @brief ����ҵ����ƽӿ�
 * @param [in] s32Cmd:������
 * @param [in] s32Dir:TVT_SDK_IMAGE_CTRL_DIR_E
 * @param [in] pParam:����ָ��
 * @param [in] s32Length:��������
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_image_ctrl(int s32Cmd, int s32Dir, void* pParam, int s32Length)
{
    int s32Ret = 0;
    int i = 0;

    if(NULL == pParam || 0 == s32Length)
    {
        TIPC_PRINTF_ERR("err input param");
        return -1;
    }

    TVT_SDK_IMAGE_INFO_S* pstHandle = tvt_sdk_image_getHandle();
    if(NULL == pstHandle)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }

    if(pstHandle->u8Init != 1)
    {
        TIPC_PRINTF_ERR("current module is not running");
        return -1;
    }
    
    TIPC_MUTEX_LOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_IMAGE_DEBUG);

    int s32SuppFuncSize = sizeof(g_stFunList) / sizeof(TVT_SDK_IMAGE_CTRL_IMPL_S);
    for(i = 0; i < s32SuppFuncSize; i++)
    {
        if((s32Cmd == (int)g_stFunList[i].enCmd) && (g_stFunList[i].pImplFunc != NULL))
        {
            s32Ret = g_stFunList[i].pImplFunc(pstHandle, s32Dir, pParam, s32Length);
            break;
        }
    }

    if(i >= s32SuppFuncSize)
    {
        TIPC_PRINTF_ERR("unknow cmd[%d]", s32Cmd);
    }

    TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_IMAGE_DEBUG);
    return s32Ret;
}

/**
 * @brief ��ʼ����Ƶͼ��ҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_image_init(int s32VinNum, const int* s32VinDevID)
{
    int s32Ret = -1;
    int i = 0;
    if(g_pstSdkImageInfo != NULL)
    {
        if(g_pstSdkImageInfo->u8Init != 0)
        {
            // �ѳ�ʼ��
            TIPC_PRINTF_ERR("video mask service is running");
            return 0;
        }
    }
    else
    {
        // �����ṹ��
        g_pstSdkImageInfo = (TVT_SDK_IMAGE_INFO_S* )malloc(sizeof(TVT_SDK_IMAGE_INFO_S));
        if(NULL == g_pstSdkImageInfo)
        {
            TIPC_PRINTF_ERR("video mask service malloc failed");
            return s32Ret;
        }
        // ��ʼ������
        memset(g_pstSdkImageInfo, 0, sizeof(TVT_SDK_IMAGE_INFO_S));
        for(i = 0; i < CHIP_MAX_VI_NUM; i++)
        {
            g_pstSdkImageInfo->s32VinDevID[i] = -1;
        }
    }

    // ��ʼ��ģ����
    pthread_mutex_init(&g_pstSdkImageInfo->s32Mutex, NULL);
    sprintf(g_pstSdkImageInfo->szMutexName, "tvt_sdk_mask_lock");
    g_pstSdkImageInfo->u8Init = 1;
    for(i = 0; i < s32VinNum; i++)
    {
        g_pstSdkImageInfo->s32VinDevID[i] = s32VinDevID[i];
    }
    return 0;
}


/**
 * @brief ȥ��ʼ����Ƶͼ��ҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_image_exit()
{
    if(NULL == g_pstSdkImageInfo)
    {
        // δ��ʼ��
        TIPC_PRINTF_ERR("video mask service is not running");
        return -1;
    }

    if (g_pstSdkImageInfo->u8Init != 1)
    {
        // δ��ʼ��
        TIPC_PRINTF_ERR("video mask service is not running");
        free((void*)g_pstSdkImageInfo);
        g_pstSdkImageInfo = NULL;
        return -1;
    }
    else
    {
        pthread_mutex_destroy(&g_pstSdkImageInfo->s32Mutex);
        g_pstSdkImageInfo->u8Init = 0;
        free((void*)g_pstSdkImageInfo);
        g_pstSdkImageInfo = NULL;
    }
    return 0;
}