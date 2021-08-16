#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include "sdk_func.h"
#include "sdk_define.h"
#include "sdk_private.h"

#include "tvt_sdk_param.h"
#include "tvt_sdk_print.h"
#include "tvt_sdk_video.h"
#include "tvt_sdk_device.h"
#include "tvt_sdk_btn.h"
#include "tvt_sdk_image.h"

typedef enum sdk_img_ctrl_direction_e
{
    SDK_IMG_CTRL_GET,
    SDK_IMG_CTRL_SET,
    SDK_IMG_CTRL_DIRECTION_BUTT,
} SDK_IMG_CTRL_DIRECTION_E;

typedef int (*SDKImageImplFunc)(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize);
typedef struct sdk_image_ctrl_impl_s
{
    int                         s32Cmd;          ///< ������
    SDKImageImplFunc            pImplFunc;       ///< ʵ�ֽӿ�
} SDK_IMAGE_CTRL_IMPL_S;

/**
 * @brief ���ȿ���
 */
static int SDKImgBrightCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief ɫ�ȿ���
 */
static int SDKImgHueCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief ���Ͷȿ���
 */
static int SDKImgSaturationCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief �Աȶȿ���
 */
static int SDKImgContrastCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief ����
 */
static int SDKImgMirrorCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
    int s32Ret = 0;
    int s32Dir = (SDK_IMG_CTRL_GET == enCtrlDir) ? TVT_SDK_IMG_CTRL_GET : TVT_SDK_IMG_CTRL_SET;
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        int s32Enable = *(int*)pParam;
        TVT_SDK_IMAGE_MIRROR_CFG_S stMirrorCfg = {0};
        stMirrorCfg.s32Enable = s32Enable;
        s32Ret = tvt_sdk_image_ctrl(TVT_SDK_IMAGE_SET_MIRROR, s32Dir, &stMirrorCfg, sizeof(stMirrorCfg));
        if (0 == s32Ret)
        {
            /// TODO: mask��roiҲ��Ҫת��
        }
    }
    return s32Ret;
}

/**
 * @brief ��ת
 */
static int SDKImgFlipCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
    int s32Ret = 0;
    int s32Dir = (SDK_IMG_CTRL_GET == enCtrlDir) ? TVT_SDK_IMG_CTRL_GET : TVT_SDK_IMG_CTRL_SET;
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        int s32Enable = *(int*)pParam;
        TVT_SDK_IMAGE_FLIP_CFG_S stFlipCfg = {0};
        stFlipCfg.s32Enable = s32Enable;
        s32Ret = tvt_sdk_image_ctrl(TVT_SDK_IMAGE_SET_FLIP, s32Dir, &stFlipCfg, sizeof(stFlipCfg));
        if (0 == s32Ret)
        {
            /// TODO: mask��roiҲ��Ҫת��
        }
    }
    return s32Ret;
}

/**
 * @brief ��ƽ�����
 */
static int SDKImgWhiteBalanceCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief ���ֿ�̬����
 */
static int SDKImgDynamicCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief �������
 */
static int SDKImgDenoiseCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief �񻯿���
 */
static int SDKImgSharpenCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief iris type
 */
static int SDKImgIrisTypeCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief ��ת����
 */
static int SDKImgRotateCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief �ع����Ϳ���
 */
static int SDKImgExposeTypeCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief �ع�ʱ�����
 */
static int SDKImgExposeTimeCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief ����У������
 */
static int SDKImgLDCCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief ͸�����
 */
static int SDKImgAntiFogCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief �������
 */
static int SDKImgDefectPixelCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief ����̬��Χ����
 */
static int SDKImgPhyDynaRangeCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief ��������
 */
static int SDKImgAntiFlickerCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief SmartIR����
 */
static int SDKImgSmartIRSuppCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief SmartIR Open����
 */
static int SDKImgSmartIROpenCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief WorkMode
 */
static int SDKImgWorkModeCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief AF Revise����
 */
static int SDKImgAFReViseCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief BLC����
 */
static int SDKImgBLCCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief ��ͷshading����
 */
static int SDKImgLensShadingCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief 3dNR����
 */
static int SDKImg3DNRCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief AF��ͷReset����
 */
static int SDKImgLensRstCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief ���淶Χ����
 */
static int SDKImgGainRangeCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    else if (SDK_IMG_CTRL_GET == enCtrlDir)
    {
        SDK_GAIN_RANGE* pstGainRange = (SDK_GAIN_RANGE*)pParam;
        pstGainRange->size = sizeof(SDK_GAIN_RANGE);
        pstGainRange->min = 0;
        pstGainRange->max = 100;
    }

    return 0;
}

/**
 * @brief �عⷶΧ����
 */
static int SDKImgExpoRangeCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

/**
 * @brief AFУ׼����
 */
static int SDKImgGetCalibCtrl(SDK_IMG_CTRL_DIRECTION_E enCtrlDir, void* pParam, int s32ParamSize)
{
    if (SDK_IMG_CTRL_SET == enCtrlDir)
    {
        /// TODO:������
    }
    return 0;
}

static SDK_IMAGE_CTRL_IMPL_S g_stImgSetFunList[] =
{
    {SDK_IMAGE_BRIGHT,                                  SDKImgBrightCtrl},
    {SDK_IMAGE_HUE,                                     SDKImgHueCtrl},
    {SDK_IMAGE_SATURATION,                              SDKImgSaturationCtrl},
    {SDK_IMAGE_CONTRAST,                                SDKImgContrastCtrl},
    {SDK_IMAGE_MIRROR,                                  SDKImgMirrorCtrl},
    {SDK_IMAGE_FLIP,                                    SDKImgFlipCtrl},
    {SDK_IMAGE_WHITE_BALANCE,                           SDKImgWhiteBalanceCtrl},
    {SDK_IMAGE_DYNAMIC_RANGE,                           SDKImgDynamicCtrl},
    {SDK_IMAGE_DENOISE,                                 SDKImgDenoiseCtrl},
    {SDK_IMAGE_SHARPEN,                                 SDKImgSharpenCtrl},
    {SDK_IMAGE_IRISTYPE,                                SDKImgIrisTypeCtrl},
    {SDK_IMAGE_ROTATE,                                  SDKImgRotateCtrl},
    {SDK_IMAGE_EXPOSE_TYPE,                             SDKImgExposeTypeCtrl},
    {SDK_IMAGE_EXPOSE_TIME,                             SDKImgExposeTimeCtrl},
    {SDK_IMAGE_LDC,                                     SDKImgLDCCtrl},
    {SDK_IMAGE_ANTI_FOG,                                SDKImgAntiFogCtrl},
    {SDK_IMAGE_DEFECT_PIXEL,                            SDKImgDefectPixelCtrl},
    {SDK_IMAGE_PHYSICAL_DYNAMIC_RANGE,                  SDKImgPhyDynaRangeCtrl},
    {SDK_IMAGE_ANTIFLICKER,                             SDKImgAntiFlickerCtrl},
    {SDK_IMAGE_SMARTIR_SUPPORT,                         SDKImgSmartIRSuppCtrl},
    {SDK_IMAGE_SMARTIR_OPEN,                            SDKImgSmartIROpenCtrl},
    {SDK_IMAGE_ISP_WORK_MODE,                           SDKImgWorkModeCtrl},
    {SDK_IMAGE_AF_REVISE,                               SDKImgAFReViseCtrl},
    {SDK_IMAGE_BLC,                                     SDKImgBLCCtrl},
    {SDK_IMAGE_LENSSHADING,                             SDKImgLensShadingCtrl},
    {SDK_IMAGE_3DNR,                                    SDKImg3DNRCtrl},
    {SDK_IMAGE_LENS_RESET,                              SDKImgLensRstCtrl},
    {SDK_IMAGE_GAIN_RANGE,                              SDKImgGainRangeCtrl},
    {SDK_IMAGE_EXPOSE_RANGE,                            SDKImgExpoRangeCtrl},
    {SDK_IMAGE_GET_CALIB_VALUE,                         SDKImgGetCalibCtrl},
};

static SDK_IMAGE_CTRL_IMPL_S g_stImgGetFunList[] =
{
    {SDK_IMAGE_GET_SHUTTER_SPEED,                       NULL},
    {SDK_IMAGE_GET_IRIS_NUMBER,                         NULL},
    {SDK_IMAGE_GET_GAIN_RANGE,                          SDKImgGainRangeCtrl},
};

/**
* @brief ����ͼ�����
* @return 0���ɹ�����0��ʧ��
*/
int SDKSetImagePara(SDK_IMAGE_TYEP_PARA eType, void* pData, int iLenght)
{
    TIPC_PRINTF_CHK("no impl ------------------------------> todo");
    return 0;
    int s32Ret = 0;
    int i = 0;
    if (NULL == pData)
    {
        TIPC_PRINTF_INFO("SDKSetImagePara, cmd %d, data is NULL\n", (int)eType);
        return -1;
    }

    int s32SuppFuncSize = sizeof(g_stImgSetFunList) / sizeof(SDK_IMAGE_CTRL_IMPL_S);
    for (i = 0; i < s32SuppFuncSize; i++)
    {
        if (((int)eType == (int)g_stImgSetFunList[i].s32Cmd) && (g_stImgSetFunList[i].pImplFunc != NULL))
        {
            s32Ret = g_stImgSetFunList[i].pImplFunc(SDK_IMG_CTRL_SET, pData, iLenght);
            break;
        }
    }

    if (i >= s32SuppFuncSize)
    {
        TIPC_PRINTF_ERR("unknow cmd[%d]", (int)eType);
    }

    return s32Ret;
}


/**
* @brief ��ȡͼ�����
* @return 0���ɹ�����0��ʧ��
*/
int SDKGetImagePara(SDK_IMAGE_TYEP_PARA eType, void *pData, int iLength)
{
    int s32Ret = 0;
    int i = 0;
    if (NULL == pData)
    {
        TIPC_PRINTF_INFO("SDKSetImagePara, cmd %d, data is NULL\n", (int)eType);
        return -1;
    }

    int s32SuppFuncSize = sizeof(g_stImgGetFunList) / sizeof(SDK_IMAGE_CTRL_IMPL_S);
    for (i = 0; i < s32SuppFuncSize; i++)
    {
        if (((int)eType == (int)g_stImgGetFunList[i].s32Cmd) && (g_stImgGetFunList[i].pImplFunc != NULL))
        {
            s32Ret = g_stImgGetFunList[i].pImplFunc(SDK_IMG_CTRL_GET, pData, iLength);
            break;
        }
    }

    if (i >= s32SuppFuncSize)
    {
        TIPC_PRINTF_ERR("unknow cmd[%d]", (int)eType);
    }

    return s32Ret;
}