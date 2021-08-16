#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "tvt_sdk_video_roi.h"
#include "tvt_sdk_print.h"
#include "tvt_sdk_chip.h"

#define TVT_SDK_ROI_MAX_COUNT 8

#define TVT_SDK_ROI_DEBUG 0

#define APP_ROI_REF_PIC_W  10000    // Ӧ�ò��·���λ�����ڻ����
#define APP_ROI_REF_PIC_H  10000    // Ӧ�ò��·���λ�����ڻ����

/*!
 * @macros ROUND_UP(x,d)
 * @brief round up func.
 */
#define ROUND_UP(x, n)  ( ((x)+(n)-1u) & ~((n)-1u) )

/**
 * @brief ҵ��״̬
 */
typedef enum tvt_sdk_roi_status_e
{
    TVT_SDK_ROI_RUN = 0,
    TVT_SDK_ROI_PAUSE,
    TVT_SDK_ROI_STATUS_BUTT,
}TVT_SDK_ROI_STATUS_E;


/**
 * @brief ROI������Ϣ
 */
typedef struct tvt_sdk_roi_data_info_s
{
    unsigned int                u32Enable;          ///< ʹ��
    int                         s32Qp;              ///< ����
    SDK_POSITION                stSrcPos;           ///< ���λ��
    SDK_POSITION                stDstPos;           ///< ������ʾλ��
} TVT_SDK_ROI_DATA_INFO_S;


/**
 * @brief ROIҵ��ȫ������
 */
typedef struct tvt_sdk_roi_cfg_s
{
    int                         s32VinDev;          ///< ������Ƶ�����豸��
    TVT_SDK_ROI_DATA_INFO_S     stROIInfo[CHIP_VENC_H26X_NUM][TVT_SDK_ROI_MAX_COUNT];       ///< ÿ��ROI�ľ�����Ϣ
} TVT_SDK_ROI_CFG_S;

/**
 * @brief ҵ��ṹ��
 */
typedef struct tvt_sdk_roi_info_s
{
    char                        u8Init;                                 ///< ��ʼ����־λ
    char                        u8Exit;                                 ///< ȥ��ʼ����־λ���߳��˳���־
    TVT_SDK_ROI_STATUS_E        enStatus;                               ///< ҵ������״̬
    pthread_mutex_t             s32Mutex;                               ///< ҵ����
    char                        szMutexName[32];                        ///< ҵ��������
    unsigned int                u32MutexLockCnt;                        ///< ҵ������������
    TVT_SDK_ROI_CFG_S           stROICfg[CHIP_MAX_VI_NUM];              ///< ROI����
}TVT_SDK_ROI_INFO_S;

static TVT_SDK_ROI_INFO_S* g_pstSdkROIInfo = NULL;

/**
 * @brief ��ȡģ��ṹ��ָ��
 */
static TVT_SDK_ROI_INFO_S* tvt_sdk_roi_getHandle(void)
{
    return g_pstSdkROIInfo;
}


/**
 * @brief ����ROIλ���Լ�����
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_roi_setPos(int s32VinIDX, int s32ROINum, const TVT_SDK_VIDEO_ROI_INFO_S* pstROIInfo)
{
    int s32Ret = 0;
    int i = 0;
    int s32Qp = 0;
    TVT_SDK_ROI_INFO_S* pstHandle = tvt_sdk_roi_getHandle();
    if(NULL == pstHandle || NULL == pstROIInfo)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    if(s32VinIDX >= CHIP_MAX_VI_NUM)
    {
        TIPC_PRINTF_ERR("err Vin Index[%d]", s32VinIDX);
        return -1;
    }
    if(s32ROINum <= 0 || s32ROINum > TVT_SDK_ROI_MAX_COUNT)
    {
        TIPC_PRINTF_ERR("err ROI Num[%d]", s32ROINum);
        return -1;
    }
    int s32VinDevID = pstHandle->stROICfg[s32VinIDX].s32VinDev;
    if(s32VinDevID < 0)
    {
        TIPC_PRINTF_ERR("err Vin DevID[%d] with Vin Index[%d] ", s32VinDevID, s32VinIDX);
        return -1;
    }

    unsigned int u32ROITotalAreaSize = 0;
    unsigned int u32ROIQpSetType = 2;
    for(i = 0; i < s32ROINum; i++)
    {
        if(pstROIInfo[i].u32Enable != 1)
        {
            continue;
        }
        u32ROITotalAreaSize += pstROIInfo[i].stPosition.width * pstROIInfo[i].stPosition.height;
    }

    // ���� Level
    u32ROITotalAreaSize = (u32ROITotalAreaSize / (10000*100));
    if(u32ROITotalAreaSize > 75)
    {
        u32ROIQpSetType = 10;
    }
    else if(u32ROITotalAreaSize > 50)
    {
        u32ROIQpSetType = 5;
    }
    else if(u32ROITotalAreaSize > 25)
    {
        u32ROIQpSetType = 3;
    }
    else if(u32ROITotalAreaSize > 15)
    {
        u32ROIQpSetType = 2;
    }
    else
    {
        u32ROIQpSetType = 2;
    }

    // ����
    TIPC_MUTEX_LOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_ROI_DEBUG);

    for(i = 0; i < s32ROINum; i++)
    {
        int s32RectX = pstROIInfo[i].stPosition.x;
        int s32RectY = pstROIInfo[i].stPosition.y;
        int s32RectW = pstROIInfo[i].stPosition.width;
        int s32RectH = pstROIInfo[i].stPosition.height;
        int s32ROILevel = pstROIInfo[i].s32Level;
        int s32ROIEnable = (int)pstROIInfo[i].u32Enable;
        if(((s32RectX + s32RectW) > APP_ROI_REF_PIC_W) ||
            ((s32RectY + s32RectH) > APP_ROI_REF_PIC_H) ||
            (s32ROILevel > 10))
        {
            TIPC_PRINTF_ERR("ROI[%d] invalid param[%d %d %d %d %d] ", s32RectX, s32RectY, s32RectW, s32RectH, s32ROILevel);
            continue;
        }
        if((0 == (s32RectX + s32RectW)) && (0 == (s32RectY + s32RectH)))
        {
            s32ROIEnable = 0;
        }
        int s32StreamIDX = 0;
        for(s32StreamIDX = 0; s32StreamIDX < CHIP_VENC_H26X_NUM; s32StreamIDX++)
        {
            CHIP_ROI_INFO_S stChipROIInfo = {0};
            // ��ȡ�������Ŀ��
            CHIP_VIDEO_ENC_CFG_S stEncCfg = {0};
            stEncCfg.s32VinDevID = s32VinDevID;
            stEncCfg.s32StreamIDX = s32StreamIDX;
            chip_video_getChnCfg(&stEncCfg);
            if((int)stEncCfg.u32Width <= 0 || (int)stEncCfg.u32Height <= 0)
            {
                // ��Ϊͨ��δ����
                continue;
            }
            // �������λ����ԭ�ȵ�ROI
            int s32DstX = pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][i].stDstPos.x;
            int s32DstY = pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][i].stDstPos.y;
            int s32DstW = pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][i].stDstPos.width;
            int s32DstH = pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][i].stDstPos.height;
            if(s32DstW > 0 && s32DstH > 0)
            {
                stChipROIInfo.s32Enable = 0;
                stChipROIInfo.s32VinDevID = s32VinDevID;
                stChipROIInfo.s32StreamIDX = s32StreamIDX;
                stChipROIInfo.s32ROIIDX = i;
                stChipROIInfo.s32X = s32DstX;
                stChipROIInfo.s32Y = s32DstY;
                stChipROIInfo.s32W = s32DstW;
                stChipROIInfo.s32H = s32DstH;
                s32Ret = chip_video_setROI(&stChipROIInfo);
                if(s32Ret != 0)
                {
                    TIPC_PRINTF_ERR("Clear ROI[%d] failed", i);
                }
            }
            
            if(1 == s32ROIEnable)
            {
                s32Qp = 0 - ((s32ROILevel * 2) / u32ROIQpSetType);
                // ת��λ��
                s32DstX = (s32RectX * stEncCfg.u32Width) / APP_ROI_REF_PIC_W;
                s32DstY = (s32RectX * stEncCfg.u32Height) / APP_ROI_REF_PIC_H;
                s32DstW = (s32RectW * stEncCfg.u32Width) / APP_ROI_REF_PIC_W;
                s32DstH = (s32RectH * stEncCfg.u32Height) / APP_ROI_REF_PIC_H;
                if((s32DstX + s32DstW) > stEncCfg.u32Width)
                {
                    s32DstW = stEncCfg.u32Width - s32DstX - 1;
                }
                if((s32DstY + s32DstH) > stEncCfg.u32Height)
                {
                    s32DstH = stEncCfg.u32Height - s32DstY - 1;
                }
                stChipROIInfo.s32Enable = 1;
                stChipROIInfo.s32VinDevID = s32VinDevID;
                stChipROIInfo.s32StreamIDX = s32StreamIDX;
                stChipROIInfo.s32ROIIDX = i;
                stChipROIInfo.s32QP = s32Qp;
                stChipROIInfo.s32X = s32DstX;
                stChipROIInfo.s32Y = s32DstY;
                stChipROIInfo.s32W = s32DstW;
                stChipROIInfo.s32H = s32DstH;
                s32Ret = chip_video_setROI(&stChipROIInfo);
                if(s32Ret != 0)
                {
                    TIPC_PRINTF_ERR("Set ROI[%d] failed", i);
                }
            }
            
            // ����λ����Ϣ
            if(1 == s32ROIEnable)
            {
                pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][i].s32Qp = s32Qp;
                pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][i].u32Enable = (unsigned int)s32ROIEnable;
                pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][i].stDstPos.x = s32DstX;
                pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][i].stDstPos.y = s32DstY;
                pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][i].stDstPos.width = s32DstW;
                pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][i].stDstPos.height = s32DstH;

                pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][i].stSrcPos.x = s32RectX;
                pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][i].stSrcPos.y = s32RectY;
                pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][i].stSrcPos.width = s32RectW;
                pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][i].stSrcPos.height = s32RectH;
            }
            else
            {
                pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][i].s32Qp = 0;
                pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][i].u32Enable = 0;
                memset(&pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][i].stDstPos, 0, sizeof(SDK_POSITION));
                memset(&pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][i].stSrcPos, 0, sizeof(SDK_POSITION));
            }
        }
    }

    // ����
    TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_ROI_DEBUG);

    return 0;
}

/**
 * @brief ֹͣ��ʾ����ROI
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_roi_stopDispALL()
{
    int s32Ret = 0;
    int s32StreamIDX = 0;
    int s32VinIDX = 0;
    int s32ROIIDX = 0;
    TVT_SDK_ROI_INFO_S* pstHandle = tvt_sdk_roi_getHandle();
    if(NULL == pstHandle)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }

    // ����
    TIPC_MUTEX_LOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_ROI_DEBUG);

    for(s32VinIDX = 0; s32VinIDX < CHIP_MAX_VI_NUM; s32VinIDX++)
    {
        int s32VinDevID = pstHandle->stROICfg[s32VinIDX].s32VinDev;
        if(s32VinDevID < 0)
        {
            continue;
        }
        for(s32ROIIDX = 0; s32ROIIDX < TVT_SDK_ROI_MAX_COUNT; s32ROIIDX++)
        {
            for(s32StreamIDX = 0; s32StreamIDX < CHIP_VENC_H26X_NUM; s32StreamIDX++)
            {
                int s32DstX = pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][s32ROIIDX].stDstPos.x;
                int s32DstY = pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][s32ROIIDX].stDstPos.y;
                int s32DstW = pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][s32ROIIDX].stDstPos.width;
                int s32DstH = pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][s32ROIIDX].stDstPos.height;
                if(s32DstW <= 0 || s32DstH <= 0)
                {
                    continue;
                }
                CHIP_ROI_INFO_S stChipROIInfo = {0};
                stChipROIInfo.s32Enable = 0;
                stChipROIInfo.s32VinDevID = s32VinDevID;
                stChipROIInfo.s32StreamIDX = s32StreamIDX;
                stChipROIInfo.s32ROIIDX = s32ROIIDX;
                stChipROIInfo.s32X = s32DstX;
                stChipROIInfo.s32Y = s32DstY;
                stChipROIInfo.s32W = s32DstW;
                stChipROIInfo.s32H = s32DstH;
                s32Ret = chip_video_setROI(&stChipROIInfo);
                if(s32Ret != 0)
                {
                    TIPC_PRINTF_ERR("Clear ROI[%d] failed", s32ROIIDX);
                }
            }
        }
    }

    // ����
    TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_ROI_DEBUG);
    return 0;
}


/**
 * @brief ��ʾ����ROI
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_roi_startDispALL()
{
    int s32Ret = 0;
    int s32StreamIDX = 0;
    int s32VinIDX = 0;
    int s32ROIIDX = 0;
    TVT_SDK_ROI_INFO_S* pstHandle = tvt_sdk_roi_getHandle();
    if(NULL == pstHandle)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    // ����
    TIPC_MUTEX_LOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_ROI_DEBUG);
    for(s32VinIDX = 0; s32VinIDX < CHIP_MAX_VI_NUM; s32VinIDX++)
    {
        int s32VinDevID = pstHandle->stROICfg[s32VinIDX].s32VinDev;
        if(s32VinDevID < 0)
        {
            continue;
        }
        for(s32ROIIDX = 0; s32ROIIDX < TVT_SDK_ROI_MAX_COUNT; s32ROIIDX++)
        {
            for(s32StreamIDX = 0; s32StreamIDX < CHIP_VENC_H26X_NUM; s32StreamIDX++)
            {
                int s32Enable = (int)pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][s32ROIIDX].u32Enable;
                if(0 == s32Enable)
                {
                    continue;
                }

                // ��ȡ�ϴα����λ��
                int s32RectX = pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][s32ROIIDX].stSrcPos.x;
                int s32RectY = pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][s32ROIIDX].stSrcPos.y;
                int s32RectW = pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][s32ROIIDX].stSrcPos.width;
                int s32RectH = pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][s32ROIIDX].stSrcPos.height;
                if(s32RectW <= 0 || s32RectH <= 0)
                {
                    continue;
                }

                // ��ȡ��ǰ�ı���ֱ���
                CHIP_VIDEO_ENC_CFG_S stEncCfg = {0};
                stEncCfg.s32VinDevID = s32VinDevID;
                stEncCfg.s32StreamIDX = s32StreamIDX;
                chip_video_getChnCfg(&stEncCfg);
                if((int)stEncCfg.u32Width <= 0 || (int)stEncCfg.u32Height <= 0)
                {
                    // ��Ϊͨ��δ����
                    continue;
                }

                // ���¼���λ��
                int s32DstX = (s32RectX * stEncCfg.u32Width) / APP_ROI_REF_PIC_W;
                int s32DstY = (s32RectY * stEncCfg.u32Height) / APP_ROI_REF_PIC_H;
                int s32DstW = (s32RectW * stEncCfg.u32Width) / APP_ROI_REF_PIC_W;
                int s32DstH = (s32RectH * stEncCfg.u32Height) / APP_ROI_REF_PIC_H;
                if((s32DstX + s32DstW) > stEncCfg.u32Width)
                {
                    s32DstW = stEncCfg.u32Width - s32DstX - 1;
                }
                if((s32DstY + s32DstH) > stEncCfg.u32Height)
                {
                    s32DstH = stEncCfg.u32Height - s32DstY - 1;
                }
                
                // Ӧ��
                CHIP_ROI_INFO_S stChipROIInfo = {0};
                stChipROIInfo.s32Enable = s32Enable;
                stChipROIInfo.s32QP = pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][s32ROIIDX].s32Qp;
                stChipROIInfo.s32VinDevID = s32VinDevID;
                stChipROIInfo.s32StreamIDX = s32StreamIDX;
                stChipROIInfo.s32ROIIDX = s32ROIIDX;
                stChipROIInfo.s32X = s32DstX;
                stChipROIInfo.s32Y = s32DstY;
                stChipROIInfo.s32W = s32DstW;
                stChipROIInfo.s32H = s32DstH;
                s32Ret = chip_video_setROI(&stChipROIInfo);
                if(s32Ret != 0)
                {
                    TIPC_PRINTF_ERR("Clear ROI[%d] failed", s32ROIIDX);
                }

                // ���±���λ��
                pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][s32ROIIDX].stDstPos.x = s32DstX;
                pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][s32ROIIDX].stDstPos.y = s32DstY;
                pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][s32ROIIDX].stDstPos.width = s32DstW;
                pstHandle->stROICfg[s32VinIDX].stROIInfo[s32StreamIDX][s32ROIIDX].stDstPos.height = s32DstH;
            }
        }
    }

    // ����
    TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_ROI_DEBUG);
    return 0;
}

/**
 * @brief ��ʼ����ƵROIҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_roi_init(int s32VinNum, const int* s32VinDevID)
{
    int s32Ret = -1;
    int i = 0;
    if(g_pstSdkROIInfo != NULL)
    {
        if(g_pstSdkROIInfo->u8Init != 0)
        {
            // �ѳ�ʼ��
            TIPC_PRINTF_ERR("video roi service is running");
            return 0;
        }
    }
    else
    {
        // �����ṹ��
        g_pstSdkROIInfo = (TVT_SDK_ROI_INFO_S* )malloc(sizeof(TVT_SDK_ROI_INFO_S));
        if(NULL == g_pstSdkROIInfo)
        {
            TIPC_PRINTF_ERR("video roi service malloc failed");
            return s32Ret;
        }
        // ��ʼ������
        memset(g_pstSdkROIInfo, 0, sizeof(TVT_SDK_ROI_INFO_S));
        for(i = 0; i < CHIP_MAX_VI_NUM; i++)
        {
            g_pstSdkROIInfo->stROICfg[i].s32VinDev = -1;
        }
    }

    // ��ʼ��ģ����
    pthread_mutex_init(&g_pstSdkROIInfo->s32Mutex, NULL);
    sprintf(g_pstSdkROIInfo->szMutexName, "tvt_sdk_roi_lock");
    g_pstSdkROIInfo->u8Init = 1;
    g_pstSdkROIInfo->enStatus = TVT_SDK_ROI_RUN;
    for(i = 0; i < s32VinNum; i++)
    {
        g_pstSdkROIInfo->stROICfg[i].s32VinDev = s32VinDevID[i];
    }
    return 0;
}


/**
 * @brief ȥ��ʼ����ƵROIҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_roi_exit()
{
    if(NULL == g_pstSdkROIInfo)
    {
        // δ��ʼ��
        TIPC_PRINTF_ERR("video roi service is not running");
        return -1;
    }

    if (g_pstSdkROIInfo->u8Init != 1)
    {
        // δ��ʼ��
        TIPC_PRINTF_ERR("video roi service is not running");
        free((void*)g_pstSdkROIInfo);
        g_pstSdkROIInfo = NULL;
        return -1;
    }
    else
    {
        pthread_mutex_destroy(&g_pstSdkROIInfo->s32Mutex);
        g_pstSdkROIInfo->u8Init = 0;
        free((void*)g_pstSdkROIInfo);
        g_pstSdkROIInfo = NULL;
    }
    return 0;
}