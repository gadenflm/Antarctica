#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "tvt_sdk_param.h"
#include "tvt_sdk_print.h"

int s32VinNum = 1;

// оƬ�ͺ�
// static TVT_SDK_PROD_PLATFORM_S stProdPlatInfo =
// {
//     .enChipType = TVT_SDK_PROD_PLAT_CV28S55M,
// };


// Sensor������Ϣ
static TVT_SDK_PROD_SENSOR_S stProdVinInfo[] =
{
    {
        .enSensorType = TVT_SDK_PROD_SENSOR_IMX464,
        .enSensorHDRMode = TVT_SDK_PROD_SENSOR_HDR_2X,
        .u32VinDev    = 0,
        .u32MaxWidth  = 2592,
        .u32MaxHeight = 1520,
    },
};

// ��ͷ��Ϣ
// static TVT_SDK_PROD_LENS_S stProdLensInfo[] =
// {
//     {
//         .enLensType = TVT_SDK_PROD_FIXED_LENS,
//     },
// };

// ��Ȧ��Ϣ
// static TVT_SDK_PROD_IRIS_S stProdIrisInfo[] =
// {
//     {
//         .enIrisType = TVT_SDK_PROD_FIXED_IRIS,
//     },
// };

// ��Ƶ��������
static TVT_SDK_PROD_AUDIO_S stProdAudioInInfo[] =
{
    {
        .enAudioMode     = TVT_SDK_PROD_AUDIO_MODE_LEFT,
        .s32MaxVolume    = 100,
        .s32MinVolume    = 0,
        .s32SuppBitWidth = TVT_SDK_PROD_AUDIO_BIT_WIDTH_8,
        .s32SuppEncType  = TVT_SDK_PROD_AUDIO_FORMAT_G711A | TVT_SDK_PROD_AUDIO_FORMAT_G711U,
        .s32SuppBitRate  = TVT_SDK_PROD_AUDIO_RATE_8000,
        .s32LineMux      = 2,
    },
};

// ��Ƶ�������
static TVT_SDK_PROD_AUDIO_S stProdAudioOutInfo[] =
{
    {
        .enAudioMode     = TVT_SDK_PROD_AUDIO_MODE_LEFT,
        .s32MaxVolume    = 100,
        .s32MinVolume    = 0,
        .s32SuppBitWidth = TVT_SDK_PROD_AUDIO_BIT_WIDTH_8,
        .s32SuppEncType  = TVT_SDK_PROD_AUDIO_FORMAT_G711A | TVT_SDK_PROD_AUDIO_FORMAT_G711U,
        .s32SuppBitRate  = TVT_SDK_PROD_AUDIO_RATE_8000,
        .s32LineMux      = 1,
    },
};

// ��Ƶ��������
// N��
static SDK_SUPPORT_FRAME_RATE SupportFrameRateNtsc[] =
{
    {   0, DVRVIDEO_SIZE_4M, 30, "2592x1520", {{/*H264*/2048, 3072, 4096, 6144, 8192, 0, \
                /*H265*/1536, 2048, 3072, 5120, 7168, 0, /*MJPEG*/2048, 3072, 4096, 6144, 8192, 0, /*resv*/0, 0
            }
        }
    },

    {   0, DVRVIDEO_SIZE_2K, 30, "2560x1440", {{/*H264*/2048, 3072, 4096, 6144, 8192, 0, \
                /*H265*/1536, 2048, 3072, 5120, 7168, 0, /*MJPEG*/2048, 3072, 4096, 6144, 8192, 0, /*resv*/0, 0
            }
        }
    },

    {   0, DVRVIDEO_SIZE_1080P, 30, "1920x1080", /*H264*/{{1024, 2048, 3072, 4096, 6144, 0, \
                /*H265*/768, 1024, 1536, 3072, 4096, 0, /*MJPEG*/2048, 3072, 4096, 6144, 8192, 0, /*resv*/0, 0
            }
        }
    },

    {   1, DVRVIDEO_SIZE_D1, 30, "704x480", /*H264*/{{256, 512, 768, 1024, 2048, 0, \
                /*H265*/128, 256, 512, 768, 1024, 0, /*MJPEG*/512, 768, 1024, 2048, 4096, 0, /*resv*/0, 0
            }
        }
    },

    {   1, DVRVIDEO_SIZE_VGA, 30, "640x480", /*H264*/{{256, 512, 768, 1024, 2048, 0, \
                /*H265*/128, 256, 512, 768, 1024, 0, /*MJPEG*/512, 768, 1024, 2048, 4096, 0, /*resv*/0, 0
            }
        }
    },

    {   1, DVRVIDEO_SIZE_NVR_INNER, 30, "480x240", /*H264*/{{128, 256, 512, 768, 1024, 0, \
                /*H265*/64, 128, 256, 512, 768, 0,  /*MJPEG*/256, 512, 768, 1024, 2048, 0,  /*resv*/0, 0
            }
        }
    },

    {   1, DVRVIDEO_SIZE_CIF, 30, "352x240", /*H264*/{{128, 256, 512, 768, 1024, 0, \
                /*H265*/64, 128, 256, 512, 768, 0,  /*MJPEG*/256, 512, 768, 1024, 2048, 0,  /*resv*/0, 0
            }
        }
    },

    {   2, DVRVIDEO_SIZE_720P, 30, "1280x720", /*H264*/{{512, 1024, 1536, 2048, 4096, 0, \
                /*H265*/256, 512, 1024, 1536, 2048, 0, /*MJPEG*/1024, 1536, 2048, 4096, 6144, 0, /*resv*/0, 0
            }
        }
    },

    {   2, DVRVIDEO_SIZE_D1, 30, "704x480", /*H264*/{{256, 512, 768, 1024, 2048, 0, \
                /*H265*/128, 256, 512, 768, 1024, 0,  /*MJPEG*/512, 768, 1024, 2048, 4096, 0,  /*resv*/0, 0
            }
        }
    },

    {   2, DVRVIDEO_SIZE_NVR_INNER, 30, "480x240", /*H264*/{{128, 256, 512, 768, 1024, 0, \
                /*H265*/64, 128, 256, 512, 768, 0,  /*MJPEG*/256, 512, 768, 1024, 2048, 0,  /*resv*/0, 0
            }
        }
    },

    {   2, DVRVIDEO_SIZE_CIF, 30, "352x240", /*H264*/{{128, 256, 512, 768, 1024, 0, \
                /*H265*/64, 128, 256, 512, 768, 0, /*MJPEG*/256, 512, 768, 1024, 2048, 0,  /*resv*/0, 0
            }
        }
    },
};

// P��
static SDK_SUPPORT_FRAME_RATE SupportFrameRatePal[] =
{
    {   0, DVRVIDEO_SIZE_4M, 25, "2592x1520", {{/*H264*/2048, 3072, 4096, 6144, 8192, 0, \
                /*H265*/1536, 2048, 3072, 5120, 7168, 0, /*MJPEG*/2048, 3072, 4096, 6144, 8192, 0, /*resv*/0, 0
            }
        }
    },

    {   0, DVRVIDEO_SIZE_2K, 25, "2560x1440", {{/*H264*/2048, 3072, 4096, 6144, 8192, 0, \
                /*H265*/1536, 2048, 3072, 5120, 7168, 0, /*MJPEG*/2048, 3072, 4096, 6144, 8192, 0, /*resv*/0, 0
            }
        }
    },

    {   0, DVRVIDEO_SIZE_1080P, 25, "1920x1080", /*H264*/{{1024, 2048, 3072, 4096, 6144, 0, \
                /*H265*/768, 1024, 1536, 3072, 4096, 0, /*MJPEG*/2048, 3072, 4096, 6144, 8192, 0, /*resv*/0, 0
            }
        }
    },

    {   1, DVRVIDEO_SIZE_D1, 25, "704x576", /*H264*/{{256, 512, 768, 1024, 2048, 0, \
                /*H265*/128, 256, 512, 768, 1024, 0, /*MJPEG*/512, 768, 1024, 2048, 4096, 0, /*resv*/0, 0
            }
        }
    },

    {   1, DVRVIDEO_SIZE_VGA, 25, "640x480", /*H264*/{{256, 512, 768, 1024, 2048, 0, \
                /*H265*/128, 256, 512, 768, 1024, 0, /*MJPEG*/512, 768, 1024, 2048, 4096, 0, /*resv*/0, 0
            }
        }
    },

    {   1, DVRVIDEO_SIZE_NVR_INNER, 25, "480x240", /*H264*/{{128, 256, 512, 768, 1024, 0, \
                /*H265*/64, 128, 256, 512, 768, 0,  /*MJPEG*/256, 512, 768, 1024, 2048, 0,  /*resv*/0, 0
            }
        }
    },

    {   1, DVRVIDEO_SIZE_CIF, 25, "352x288", /*H264*/{{128, 256, 512, 768, 1024, 0, \
                /*H265*/64, 128, 256, 512, 768, 0,  /*MJPEG*/256, 512, 768, 1024, 2048, 0,  /*resv*/0, 0
            }
        }
    },

    {   2, DVRVIDEO_SIZE_720P, 25, "1280x720", /*H264*/{{512, 1024, 1536, 2048, 4096, 0, \
                /*H265*/256, 512, 1024, 1536, 2048, 0, /*MJPEG*/1024, 1536, 2048, 4096, 6144, 0, /*resv*/0, 0
            }
        }
    },

    {   2, DVRVIDEO_SIZE_D1, 25, "704x576", /*H264*/{{256, 512, 768, 1024, 2048, 0, \
                /*H265*/128, 256, 512, 768, 1024, 0,  /*MJPEG*/512, 768, 1024, 2048, 4096, 0,  /*resv*/0, 0
            }
        }
    },

    {   2, DVRVIDEO_SIZE_NVR_INNER, 25, "480x240", /*H264*/{{128, 256, 512, 768, 1024, 0, \
                /*H265*/64, 128, 256, 512, 768, 0,  /*MJPEG*/256, 512, 768, 1024, 2048, 0,  /*resv*/0, 0
            }
        }
    },

    {   2, DVRVIDEO_SIZE_CIF, 25, "352x288", /*H264*/{{128, 256, 512, 768, 1024, 0, \
                /*H265*/64, 128, 256, 512, 768, 0, /*MJPEG*/256, 512, 768, 1024, 2048, 0,  /*resv*/0, 0
            }
        }
    },
};

// N�Ƹ�֡��
static SDK_SUPPORT_FRAME_RATE SupportFrameRateNtscHfps[] =
{
    {   0, DVRVIDEO_SIZE_1080P, 60, "1920x1080", /*H264*/{{/*H264*/2048, 3072, 5120, 7168, 9216, 0, \
                /*H265*/1536, 2048, 3072, 5120, 7168, 0, /*MJPEG*/3072, 5120, 6656, 9216, 12288, 0, /*resv*/0, 0
            }
        }
    },

    {   0, DVRVIDEO_SIZE_720P, 60, "1280x720", /*H264*/{{1024, 2048, 3072, 4096, 6144, 0, \
                /*H265*/768, 1024, 1536, 3072, 4096, 0, /*MJPEG*/2048, 3072, 4096, 6144, 8192, 0, /*resv*/0, 0
            }
        }
    },

    {   1, DVRVIDEO_SIZE_720P, 30, "1280x720", /*H264*/{{512, 1024, 1536, 2048, 4096, 0, \
                /*H265*/256, 512, 1024, 1536, 2048, 0, /*MJPEG*/1024, 1536, 2048, 4096, 6144, 0, /*resv*/0, 0
            }
        }
    },

    {   1, DVRVIDEO_SIZE_D1, 30, "704x480", /*H264*/{{256, 512, 768, 1024, 2048, 0, \
                /*H265*/128, 256, 512, 768, 1024, 0, /*MJPEG*/512, 768, 1024, 2048, 4096, 0, /*resv*/0, 0
            }
        }
    },

    {   1, DVRVIDEO_SIZE_NVR_INNER, 30, "480x240", /*H264*/{{128, 256, 512, 768, 1024, 0, \
                /*H265*/64, 128, 256, 512, 768, 0,  /*MJPEG*/256, 512, 768, 1024, 2048, 0,  /*resv*/0, 0
            }
        }
    },

    {   1, DVRVIDEO_SIZE_CIF, 30, "352x240", /*H264*/{{128, 256, 512, 768, 1024, 0, \
                /*H265*/64, 128, 256, 512, 768, 0,  /*MJPEG*/256, 512, 768, 1024, 2048, 0,  /*resv*/0, 0
            }
        }
    },

    {   2, DVRVIDEO_SIZE_D1, 30, "704x480", /*H264*/{{256, 512, 768, 1024, 2048, 0, \
                /*H265*/128, 256, 512, 768, 1024, 0,  /*MJPEG*/512, 768, 1024, 2048, 4096, 0,  /*resv*/0, 0
            }
        }
    },

    {   2, DVRVIDEO_SIZE_NVR_INNER, 30, "480x240", /*H264*/{{128, 256, 512, 768, 1024, 0, \
                /*H265*/64, 128, 256, 512, 768, 0,  /*MJPEG*/256, 512, 768, 1024, 2048, 0,  /*resv*/0, 0
            }
        }
    },

    {   2, DVRVIDEO_SIZE_CIF, 30, "352x240", /*H264*/{{128, 256, 512, 768, 1024, 0, \
                /*H265*/64, 128, 256, 512, 768, 0, /*MJPEG*/256, 512, 768, 1024, 2048, 0,  /*resv*/0, 0
            }
        }
    },
};

// P�Ƹ�֡��
static SDK_SUPPORT_FRAME_RATE SupportFrameRatePalHfps[] =
{
    {   0, DVRVIDEO_SIZE_1080P, 50, "1920x1080", /*H264*/{{/*H264*/2048, 3072, 5120, 7168, 9216, 0, \
                /*H265*/1536, 2048, 3072, 5120, 7168, 0, /*MJPEG*/3072, 5120, 6656, 9216, 12288, 0, /*resv*/0, 0
            }
        }
    },

    {   0, DVRVIDEO_SIZE_720P, 50, "1280x720", /*H264*/{{1024, 2048, 3072, 4096, 6144, 0, \
                /*H265*/768, 1024, 1536, 3072, 4096, 0, /*MJPEG*/2048, 3072, 4096, 6144, 8192, 0, /*resv*/0, 0
            }
        }
    },

    {   1, DVRVIDEO_SIZE_720P, 25, "1280x720", /*H264*/{{512, 1024, 1536, 2048, 4096, 0, \
                /*H265*/256, 512, 1024, 1536, 2048, 0, /*MJPEG*/1024, 1536, 2048, 4096, 6144, 0, /*resv*/0, 0
            }
        }
    },

    {   1, DVRVIDEO_SIZE_D1, 25, "704x576", /*H264*/{{256, 512, 768, 1024, 2048, 0, \
                /*H265*/128, 256, 512, 768, 1024, 0, /*MJPEG*/512, 768, 1024, 2048, 4096, 0, /*resv*/0, 0
            }
        }
    },

    {   1, DVRVIDEO_SIZE_NVR_INNER, 25, "480x240", /*H264*/{{128, 256, 512, 768, 1024, 0, \
                /*H265*/64, 128, 256, 512, 768, 0,  /*MJPEG*/256, 512, 768, 1024, 2048, 0,  /*resv*/0, 0
            }
        }
    },

    {   1, DVRVIDEO_SIZE_CIF, 25, "352x288", /*H264*/{{128, 256, 512, 768, 1024, 0, \
                /*H265*/64, 128, 256, 512, 768, 0,  /*MJPEG*/256, 512, 768, 1024, 2048, 0,  /*resv*/0, 0
            }
        }
    },

    {   2, DVRVIDEO_SIZE_D1, 25, "704x576", /*H264*/{{256, 512, 768, 1024, 2048, 0, \
                /*H265*/128, 256, 512, 768, 1024, 0,  /*MJPEG*/512, 768, 1024, 2048, 4096, 0,  /*resv*/0, 0
            }
        }
    },

    {   2, DVRVIDEO_SIZE_NVR_INNER, 25, "480x240", /*H264*/{{128, 256, 512, 768, 1024, 0, \
                /*H265*/64, 128, 256, 512, 768, 0,  /*MJPEG*/256, 512, 768, 1024, 2048, 0,  /*resv*/0, 0
            }
        }
    },

    {   2, DVRVIDEO_SIZE_CIF, 25, "352x288", /*H264*/{{128, 256, 512, 768, 1024, 0, \
                /*H265*/64, 128, 256, 512, 768, 0, /*MJPEG*/256, 512, 768, 1024, 2048, 0,  /*resv*/0, 0
            }
        }
    },
};

// ���������
static TVT_SDK_PROD_IR_S stProdIR[] =
{
    {
        .s32IsGpio = 0,
        .s32IrType = 0,
        .stPwmCfg =
        {
            .s32PwmNum = 1,
            .s32Period = 50000,
            .s32MaxDuty = 50000,
        },
    },
    {
        .s32IsGpio = 0,
        .s32IrType = 1,
        .stPwmCfg =
        {
            .s32PwmNum = 2,
            .s32Period = 50000,
            .s32MaxDuty = 50000,
        },
    },
};

// RS485����
static TVT_SDK_PROD_RS485_S stProdRS485[] =
{
    {
        .s32UartID = 1, // /dev/ttyS1
        .s32Speed = 6, // B110, B300, B600, B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200
    },
};


// �澯����
static TVT_SDK_PROD_ALARM_S stProdAlarmIn[] =
{
    {
        .s32GpioGrp = 0,
        .s32GpioNum = 87, // GPIO87
    }
};


// �澯���
static TVT_SDK_PROD_ALARM_S stProdAlarmOut[] =
{
    {
        .s32GpioGrp = 0,
        .s32GpioNum = 88, // GPIO88
    }
};

// ZOOM ����
static TVT_SDK_PROD_ZOOM_BTN_S stProdZoomBtn[] =
{
    // ZOOM BTN+
    {
        .s32GpioGrp = 0,
        .s32GpioNum = 81, // GPIO81
        .enTrigMode = TVT_SDK_PROD_BTN_FALLING_EDGE,
    },
    // ZOOM BTN-
    {
        .s32GpioGrp = 0,
        .s32GpioNum = 82, // GPIO82
        .enTrigMode = TVT_SDK_PROD_BTN_FALLING_EDGE,
    }
};

// RTSET ����
static TVT_SDK_PROD_RESET_BTN_S stProdRstBtn[] =
{
    {
        .s32GpioGrp = 0,
        .s32GpioNum = 1, // GPIO1
    }
};

// CDS����
static TVT_SDK_PROD_CDS_S stProdCDS[] =
{
    {
        .s32AdcDevID = 0,
    }
};

// RTSET ����
static TVT_SDK_PROD_IRCUT_S stProdIrcut[] =
{
    {
        .s32GpioGrp = 0,
        .s32GpioNum = 43, // GPIO43
    }
};

// SD������
static char szTFCardName[] = "mmcblk1";


/**
 * @brief ��ȡ��Ƶ������Ϣ
 */
int tvt_param_getVinInfo(void* pParam, int s32ParamSize)
{
    TVT_SDK_PROD_CTRL_INFO* pstParam = (TVT_SDK_PROD_CTRL_INFO*)pParam;
    pstParam->pParam = &stProdVinInfo;
    pstParam->s32ParamSize = sizeof(stProdVinInfo);
    return 0;
}

/**
 * @brief ��ȡ��Ƶ����������
 */
int tvt_param_getEncSupp(void* pParam, int s32ParamSize)
{
    TVT_SDK_PROD_CTRL_INFO* pstParam = (TVT_SDK_PROD_CTRL_INFO*)pParam;
    int* ps32Format = (int*)pstParam->pUserData;
    int s32Format = (NULL == ps32Format) ? SDK_VIDEO_INPUT_PAL : ps32Format[0];
    int s32HFPSFlag = (NULL == ps32Format) ? 0 : ps32Format[1];
    SDK_SUPPORT_FRAME_RATE* pstEncSupp = NULL;
    int s32EncSuppSize = 0;
    if (0 == pstParam->stDevInfo.s32ModelID)
    {
        // ����
        if (SDK_VIDEO_INPUT_PAL == s32Format)
        {
            if (1 == s32HFPSFlag)
            {
                // ��֡��
                pstEncSupp = SupportFrameRatePalHfps;
                s32EncSuppSize = sizeof(SupportFrameRatePalHfps);
            }
            else
            {
                pstEncSupp = SupportFrameRatePal;
                s32EncSuppSize = sizeof(SupportFrameRatePal);
            }
        }
        else
        {
            if (1 == s32HFPSFlag)
            {
                // ��֡��
                pstEncSupp = SupportFrameRateNtscHfps;
                s32EncSuppSize = sizeof(SupportFrameRateNtscHfps);
            }
            else
            {
                pstEncSupp = SupportFrameRateNtsc;
                s32EncSuppSize = sizeof(SupportFrameRateNtsc);
            }
        }
    }

    pstParam->pParam = pstEncSupp;
    pstParam->s32ParamSize = s32EncSuppSize;
    return 0;
}


/**
 * @brief ��ȡRS485����
 */
int tvt_param_getRS485Cfg(void* pParam, int s32ParamSize)
{
    TVT_SDK_PROD_CTRL_INFO* pstParam = (TVT_SDK_PROD_CTRL_INFO*)pParam;
    if (0 == pstParam->stDevInfo.s32ModelID)
    {
        // ����
        pstParam->pParam = stProdRS485;
        pstParam->s32ParamSize = sizeof(stProdRS485);
    }
    return 0;
}

/**
 * @brief ��ȡCDS����
 */
int tvt_param_getCDSCfg(void* pParam, int s32ParamSize)
{
    TVT_SDK_PROD_CTRL_INFO* pstParam = (TVT_SDK_PROD_CTRL_INFO*)pParam;
    if (0 == pstParam->stDevInfo.s32ModelID)
    {
        // ����
        pstParam->pParam = stProdCDS;
        pstParam->s32ParamSize = sizeof(stProdCDS);
    }
    return 0;
}

/**
 * @brief ��ȡAlarmIn����
 */
int tvt_param_getAlarmInCfg(void* pParam, int s32ParamSize)
{
    TVT_SDK_PROD_CTRL_INFO* pstParam = (TVT_SDK_PROD_CTRL_INFO*)pParam;
    if (0 == pstParam->stDevInfo.s32ModelID)
    {
        // ����
        pstParam->pParam = stProdAlarmIn;
        pstParam->s32ParamSize = sizeof(stProdAlarmIn);
    }
    return 0;
}


/**
 * @brief ��ȡAlarmOut����
 */
int tvt_param_getAlarmOutCfg(void* pParam, int s32ParamSize)
{
    TVT_SDK_PROD_CTRL_INFO* pstParam = (TVT_SDK_PROD_CTRL_INFO*)pParam;
    if (0 == pstParam->stDevInfo.s32ModelID)
    {
        // ����
        pstParam->pParam = stProdAlarmOut;
        pstParam->s32ParamSize = sizeof(stProdAlarmOut);
    }
    return 0;
}

/**
 * @brief ��ȡ��λ��������
 */
int tvt_param_getRstBtnCfg(void* pParam, int s32ParamSize)
{
    TVT_SDK_PROD_CTRL_INFO* pstParam = (TVT_SDK_PROD_CTRL_INFO*)pParam;
    if (0 == pstParam->stDevInfo.s32ModelID)
    {
        // ����
        pstParam->pParam = stProdRstBtn;
        pstParam->s32ParamSize = sizeof(stProdRstBtn);
    }
    return 0;
}

/**
 * @brief ��ȡIRCUT����
 */
int tvt_param_getIrcutCfg(void* pParam, int s32ParamSize)
{
    TVT_SDK_PROD_CTRL_INFO* pstParam = (TVT_SDK_PROD_CTRL_INFO*)pParam;
    if (0 == pstParam->stDevInfo.s32ModelID)
    {
        // ����
        pstParam->pParam = stProdIrcut;
        pstParam->s32ParamSize = sizeof(stProdIrcut);
    }
    return 0;
}


/**
 * @brief ��ȡZoom��������
 */
int tvt_param_getZoomBtnCfg(void* pParam, int s32ParamSize)
{
    TVT_SDK_PROD_CTRL_INFO* pstParam = (TVT_SDK_PROD_CTRL_INFO*)pParam;
    if (0 == pstParam->stDevInfo.s32ModelID)
    {
        // ����
        pstParam->pParam = stProdZoomBtn;
        pstParam->s32ParamSize = sizeof(stProdZoomBtn);
    }
    return 0;
}

/**
 * @brief ��ȡ���������
 */
int tvt_param_getIRCfg(void* pParam, int s32ParamSize)
{
    TVT_SDK_PROD_CTRL_INFO* pstParam = (TVT_SDK_PROD_CTRL_INFO*)pParam;
    if (0 == pstParam->stDevInfo.s32ModelID)
    {
        // ����
        pstParam->pParam = stProdIR;
        pstParam->s32ParamSize = sizeof(stProdIR);
    }
    return 0;
}

/**
 * @brief ��ȡSD������
 */
int tvt_param_getSDCardCfg(void* pParam, int s32ParamSize)
{
    TVT_SDK_PROD_CTRL_INFO* pstParam = (TVT_SDK_PROD_CTRL_INFO*)pParam;
    if (0 == pstParam->stDevInfo.s32ModelID)
    {
        // ����
        pstParam->pParam = (void*)szTFCardName;
        pstParam->s32ParamSize = strlen(szTFCardName);
    }
    return 0;
}

/**
 * @brief ��ȡ��Ƶ������Ϣ
 */
int tvt_param_getAudioInSupp(void* pParam, int s32ParamSize)
{
    TVT_SDK_PROD_CTRL_INFO* pstParam = (TVT_SDK_PROD_CTRL_INFO*)pParam;
    if (0 == pstParam->stDevInfo.s32ModelID)
    {
        // ����
        // wkl debug:
        // TIPC_PRINTF_CHK("!!!!!!!!!!!!!!!!!! Get the Audio In Cfg");

        pstParam->pParam = (void*)stProdAudioInInfo;
        pstParam->s32ParamSize = sizeof(stProdAudioInInfo);
    }
    return 0;
}

/**
 * @brief ��ȡ��Ƶ�����Ϣ
 */
int tvt_param_getAudioOutSupp(void* pParam, int s32ParamSize)
{
    TVT_SDK_PROD_CTRL_INFO* pstParam = (TVT_SDK_PROD_CTRL_INFO*)pParam;
    if (0 == pstParam->stDevInfo.s32ModelID)
    {
        // ����
        pstParam->pParam = (void*)stProdAudioOutInfo;
        pstParam->s32ParamSize = sizeof(stProdAudioOutInfo);
    }
    return 0;
}

