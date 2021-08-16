#ifndef _TVT_SDK_PRODUCT_PARAM_H_
#define _TVT_SDK_PRODUCT_PARAM_H_

#include "sdk_define.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum tvt_sdk_prod_cmd_e
{
    TVT_SDK_PARAM_GET_FEATURE = 0,
    TVT_SDK_PARAM_GET_SENSOR_CFG,
    TVT_SDK_PARAM_GET_STREAM_CFG,
    TVT_SDK_PARAM_GET_AUDIO_IN_CFG,
    TVT_SDK_PARAM_GET_AUDIO_OUT_CFG,
    TVT_SDK_PARAM_GET_UART_CFG,
    TVT_SDK_PARAM_GET_RS485_CFG,
    TVT_SDK_PARAM_GET_CDS_CFG,
    TVT_SDK_PARAM_GET_SDCARD_CFG,
    TVT_SDK_PARAM_GET_ALARMIN_CFG,
    TVT_SDK_PARAM_GET_ALARMOUT_CFG,
    TVT_SDK_PARAM_GET_ALARMLIGHT_CFG,
    TVT_SDK_PARAM_GET_RESET_BTN_CFG,
    TVT_SDK_PARAM_GET_ZOOM_BTN_CFG,
    TVT_SDK_PARAM_GET_ABF_BTN_CFG,
    TVT_SDK_PARAM_GET_IRCUT_CFG,
    TVT_SDK_PARAM_GET_WIEGAND_CFG,
    TVT_SDK_PARAM_GET_IR_CFG,
    TVT_SDK_PARAM_GET_WHITELIGHT_CFG,
    TVT_SDK_PARAM_GET_BUTT,
}TVT_SDK_PROD_CMD_E;


typedef enum tvt_sdk_prod_feature_e
{
    TVT_SDK_PROD_SUPP_AF       = 0x00000001, ///< AF֧��
    TVT_SDK_PROD_SUPP_ZoomBtn  = 0x00000002, ///< 
    TVT_SDK_PROD_SUPP_ABF      = 0x00000004,
    TVT_SDK_PROD_SUPP_VO       = 0x00000008,
    TVT_SDK_PROD_SUPP_RS485    = 0x00000010,
    TVT_SDK_PROD_SUPP_ALARM    = 0x00000020,
    TVT_SDK_PROD_SUPP_WIEGAND  = 0x00000040,
    TVT_SDK_PROD_SUPP_SWCDS    = 0x00000001, ///< �����
    
    TVT_SDK_PROD_FEATURE_BUTT,
}TVT_SDK_PROD_FEATURE_E;

/**
 * @brief �豸���ͽṹ��
 */
typedef struct tvt_sdk_prod_dev_info_s
{
    int     s32ModelID;     // ����ͺ�
    int     s32LensID;      // ��ͷ�ͺ�
    int     s32CstmrID;     // �ͻ�ID
    int     s32Resv[5];
}TVT_SDK_PROD_DEV_INFO;

/**
 * @brief ���ƽṹ��
 */
typedef struct tvt_sdk_prod_ctrl_info_s
{
    TVT_SDK_PROD_DEV_INFO       stDevInfo;      ///< ��������
    void*                       pUserData;      ///< ��������

    void*                       pParam;         ///< �������
    int                         s32ParamSize;   ///< �������
    int s32Resv[2];
}TVT_SDK_PROD_CTRL_INFO;

/// �����ص�����
typedef int (*TvtSdkProdCb)(int s32Cmd, void* pParam, int s32Length);

///// ƽ̨���
/**
 * @brief оƬ�ͺ�ö��
 */
typedef enum tvt_sdk_prod_chip_e
{
    TVT_SDK_PROD_PLAT_CV28S55M = 0,
    TVT_SDK_PROD_PLAT_BUTT,
}TVT_SDK_PROD_CHIP_E;

/**
 * @brief ��Ʒƽ̨Ӳ����Ϣ�ṹ��
 */
typedef struct tvt_sdk_prod_platform_s
{
    TVT_SDK_PROD_CHIP_E     enChipType;     ///< ��Ʒʹ��оƬ�ͺ�
    int s32Resv[3];
}TVT_SDK_PROD_PLATFORM_S;


///// Sensor ���
/**
 * @brief Sensor�ͺ�ö��
 */
typedef enum tvt_sdk_prod_sensor_e
{
    TVT_SDK_PROD_SENSOR_IMX464 = 0,
    TVT_SDK_PROD_SENSOR_IMX334,
    TVT_SDK_PROD_SENSOR_BUTT,
}TVT_SDK_PROD_SENSOR_E;

/**
 * @brief Sensor HDRģʽö��
 */
typedef enum tvt_sdk_prod_sensor_hdr_mode_e
{
    TVT_SDK_PROD_SENSOR_HDR_NONE = 0,
    TVT_SDK_PROD_SENSOR_HDR_2X,
    TVT_SDK_PROD_SENSOR_HDR_3X,
    TVT_SDK_PROD_SENSOR_HDR_BUTT,
}TVT_SDK_PROD_SENSOR_HDR_MODE_E;

/**
 * @brief ��ƷSensor��Ϣ�ṹ��
 */
typedef struct tvt_sdk_prod_sensor_s
{
    TVT_SDK_PROD_SENSOR_E           enSensorType;       ///< ��Ʒʹ�� Sensor ����
    TVT_SDK_PROD_SENSOR_HDR_MODE_E  enSensorHDRMode;    ///< ��ƷHDRģʽ����
    unsigned int                    u32VinDev;          ///< ������Ƶ�����豸��
    unsigned int                    u32MaxWidth;        ///< ֧�����ֱ��ʿ�
    unsigned int                    u32MaxHeight;       ///< ֧�����ֱ��ʸ�
    int s32Resv[4];
}TVT_SDK_PROD_SENSOR_S;


///// ��ͷ���
/**
 * @brief ��ͷö��
 */
typedef enum tvt_sdk_prod_lens_e
{
    TVT_SDK_PROD_FIXED_LENS     = 0,
    TVT_SDK_PROD_LENS_IR02812FB = 1,
    TVT_SDK_PROD_LENS_BUTT,
}TVT_SDK_PROD_LENS_E;

/**
 * @brief ��Ʒ��ͷ��Ϣ�ṹ��
 */
typedef struct tvt_sdk_prod_lens_s
{
    TVT_SDK_PROD_LENS_E   enLensType;  ///< ��Ʒʹ�þ�ͷ����
    int s32Resv[3];
}TVT_SDK_PROD_LENS_S;


///// ��Ȧ���
/**
 * @brief ��Ȧ����ö��
 */
typedef enum tvt_sdk_prod_iris_e
{
    TVT_SDK_PROD_FIXED_IRIS = 0,
    TVT_SDK_PROD_DC_IRIS = 1,
    TVT_SDK_PROD_P_IRIS = 2,
    TVT_SDK_PROD_IRIS_BUTT,
}TVT_SDK_PROD_IRIS_E;

/**
 * @brief ��Ʒ��Ȧ��Ϣ�ṹ��
 */
typedef struct tvt_sdk_prod_iris_s
{
    TVT_SDK_PROD_IRIS_E   enIrisType;  ///< ��Ʒʹ�ù�Ȧ����
    int s32Resv[3];
}TVT_SDK_PROD_IRIS_S;


///// ��Ƶ���
/**
 * @brief ��Ʒ��Ƶ������ö��
 */
typedef enum tvt_sdk_prod_audio_rate_e
{
    TVT_SDK_PROD_AUDIO_RATE_8000  = 0x1,
    TVT_SDK_PROD_AUDIO_RATE_12000 = 0x2,
    TVT_SDK_PROD_AUDIO_RATE_11025 = 0x4,
    TVT_SDK_PROD_AUDIO_RATE_16000 = 0x8,
    TVT_SDK_PROD_AUDIO_RATE_22050 = 0x10,
    TVT_SDK_PROD_AUDIO_RATE_24000 = 0x20,
    TVT_SDK_PROD_AUDIO_RATE_32000 = 0x40,
    TVT_SDK_PROD_AUDIO_RATE_44100 = 0x80,
    TVT_SDK_PROD_AUDIO_RATE_48000 = 0x100,
    TVT_SDK_PROD_AUDIO_RATE_BUTT,
}TVT_SDK_PROD_AUDIO_RATE_E;

/**
 * @brief ��Ʒ��Ƶλ��ö��
 */
typedef enum tvt_sdk_prod_audio_bit_width_e
{
    TVT_SDK_PROD_AUDIO_BIT_WIDTH_8  = 0x1,
    TVT_SDK_PROD_AUDIO_BIT_WIDTH_16 = 0x2,
    TVT_SDK_PROD_AUDIO_BIT_WIDTH_32 = 0x4,
    TVT_SDK_PROD_AUDIO_BIT_WIDTH_BUTT,
}TVT_SDK_PROD_AUDIO_BIT_WIDTH_E;


/**
 * @brief ��Ʒ��Ƶ�����ʽö��
 */
typedef enum tvt_sdk_prod_audio_format_e
{
    TVT_SDK_PROD_AUDIO_FORMAT_G726  = 0x1,
    TVT_SDK_PROD_AUDIO_FORMAT_G711A = 0x2,
    TVT_SDK_PROD_AUDIO_FORMAT_G711U = 0x4,
    TVT_SDK_PROD_AUDIO_FORMAT_ADPCM = 0x8,
    TVT_SDK_PROD_AUDIO_FORMAT_AAC   = 0x10,
    TVT_SDK_PROD_AUDIO_FORMAT_BUTT,
}TVT_SDK_PROD_AUDIO_FORMAT_E;

/**
 * @brief ��Ʒ��Ƶ����ö��
 */
typedef enum tvt_sdk_prod_audio_mode_e
{
    // TVT_SDK_PROD_AUDIO_MODE_MONO   = 0, ///< ������
    TVT_SDK_PROD_AUDIO_MODE_LEFT,       ///< ������
    TVT_SDK_PROD_AUDIO_MODE_RIGHT,      ///< ������
    TVT_SDK_PROD_AUDIO_MODE_STEREO,     ///< ������
    TVT_SDK_PROD_AUDIO_MODE_BUTT,
} TVT_SDK_PROD_AUDIO_MODE_E;

/**
 * @brief ��Ʒ��Ƶ�������ýṹ��
 */
typedef struct tvt_sdk_prod_audio_s
{
    TVT_SDK_PROD_AUDIO_MODE_E      enAudioMode;           ///< ��������
    int                            s32SuppBitWidth;       ///< ֧�ֵ�λ��
    int                            s32SuppEncType;        ///< ֧�ֵı����ʽ
    int                            s32SuppBitRate;        ///< ֧�ֵĲ�����
    int                            s32MinVolume;          ///< ��С����
    int                            s32MaxVolume;          ///< �������
    int                            s32LineMux;
    int s32Resv;
}TVT_SDK_PROD_AUDIO_S;

///// ��Ƶ�������
/// TODO: ��ȡ��·�ֱ��ʱ���ȸ�ʽʹ�ã�SDK_SUPPORT_FRAME_RATE


///// �������
/**
 * @brief ��Ʒ�����¼�����ö��
 */
typedef enum tvt_sdk_prod_btn_trig_mode_e
{
    TVT_SDK_PROD_BTN_FALLING_EDGE,  ///< �½��ش���
    TVT_SDK_PROD_BTN_RISING_EDGE,   ///< �����ش���
    TVT_SDK_PROD_BTN_TRIG_BUTT,
} TVT_SDK_PROD_BTN_TRIG_MODE_E;

/**
 * @brief ��Ʒ�������ýṹ��
 */
typedef struct tvt_sdk_prod_uart_s
{
    int     s32UartID;      ///< ��������ID
    int     s32Speed;       ///< ������
    int s32Resv[6];
}TVT_SDK_PROD_UART_S;

/**
 * @brief ��ƷGPIO���ýṹ��
 */
typedef struct tvt_sdk_prod_gpio_s
{
    int     s32GpioGrp;      ///< ����GPIO���
    int     s32GpioNum;      ///< ����GPIO num
    int s32Resv[6];
}TVT_SDK_PROD_GPIO_S;

/**
 * @brief ��Ʒ�����¼��ṹ��
 */
typedef struct tvt_sdk_prod_btn_event_s
{
    int                             s32GpioGrp;      ///< ����GPIO���
    int                             s32GpioNum;      ///< ����GPIO num
    TVT_SDK_PROD_BTN_TRIG_MODE_E    enTrigMode;      ///< ����ģʽ
    int s32Resv[5];
}TVT_SDK_PROD_BTN_EVENT_S;

/**
 * @brief ��ƷPWM���ýṹ��
 */
typedef struct tvt_sdk_prod_pwm_s
{
    int     s32PwmNum;     ///< ����Pwm�豸��
    int     s32Period;     ///< ����
    int     s32MaxDuty;    ///< ���ռ�ձ�
    int s32Resv[5];
}TVT_SDK_PROD_PWM_S;

/**
 * @brief ��ƷADC���ýṹ��
 */
typedef struct tvt_sdk_prod_adc_s
{
    int     s32AdcDevID;    ///< ����ADC�豸��
    int     s32Resv[3];
}TVT_SDK_PROD_ADC_S;


/**
 * @brief ��ƷRS485���ýṹ��
 */
typedef TVT_SDK_PROD_UART_S TVT_SDK_PROD_RS485_S;

/**
 * @brief ��Ʒ�������ýṹ��
 */
typedef TVT_SDK_PROD_GPIO_S TVT_SDK_PROD_ALARM_S;

/**
 * @brief ��Ʒ��λ�������ýṹ��
 */
typedef TVT_SDK_PROD_GPIO_S TVT_SDK_PROD_RESET_BTN_S;

/**
 * @brief ��ƷZOOM�������ýṹ��
 */
typedef TVT_SDK_PROD_BTN_EVENT_S TVT_SDK_PROD_ZOOM_BTN_S;

/**
 * @brief ��ƷABF�������ýṹ��
 */
typedef TVT_SDK_PROD_GPIO_S TVT_SDK_PROD_ABF_BTN_S;

/**
 * @brief ��ƷIRCUT���ýṹ��
 */
typedef TVT_SDK_PROD_GPIO_S TVT_SDK_PROD_IRCUT_S;

/**
 * @brief ��ƷCDS���ýṹ��
 */
typedef TVT_SDK_PROD_ADC_S TVT_SDK_PROD_CDS_S;


/**
 * @brief ��ƷΤ��λ��ö��
 */
typedef enum tvt_sdk_prod_wiegand_bit_width_e
{
    TVT_SDK_PROD_WIEGAND_BIT_NUM_26 = 0x1,
    TVT_SDK_PROD_WIEGAND_BIT_NUM_34 = 0x2,
    TVT_SDK_PROD_WIEGAND_BIT_NUM_37 = 0x4,
    TVT_SDK_PROD_WIEGAND_BIT_NUM_42 = 0x8,
    TVT_SDK_PROD_WIEGAND_BIT_NUM_46 = 0x10,
    TVT_SDK_PROD_WIEGAND_BIT_NUM_58 = 0x20,
    TVT_SDK_PROD_WIEGAND_BIT_NUM_66 = 0x40,
    TVT_SDK_PROD_WIEGAND_BIT_NUM_BUTT,
} TVT_SDK_PROD_WEIGAND_BIT_WIDTH_E;


/**
 * @brief ��ƷΤ�����ýṹ��
 */
typedef struct tvt_sdk_prod_wiegand_s
{
    int     s32SuppBitNum;
    int     s32Resv[7];
}TVT_SDK_PROD_WIEGAND_S;


/**
 * @brief ��Ʒ��������ýṹ��
 */
typedef struct tvt_sdk_prod_ir_s
{
    int                     s32IsGpio;  ///< 1:GPIO(������GPIO) 0:PWM(ͬʱ����PWM��GPIO)
    int                     s32IrType;  ///< ��������ͣ�0������ƣ�1��Զ���
    TVT_SDK_PROD_PWM_S      stPwmCfg;   ///< ��ӦPwm����
    TVT_SDK_PROD_GPIO_S     stGpioCfg;  ///< ��ӦGpio����
}TVT_SDK_PROD_IR_S;

/**
 * @brief ��Ʒ����ƿ������ýṹ��
 */
typedef struct tvt_sdk_product_ir_ctrl_s
{
    int     s32EnableGrpCtrl;   ///< �Ƿ�ʹ�ܷ�����ƣ�0����ʹ�ܣ�1��ʹ��
    int     s32EnablePowerCtrl; ///< �Ƿ�ʹ�ܹ��Ŀ��ƣ�0����ʹ�ܣ�1��ʹ��
    int     s32InvertCtrl;      ///< �Ƿ�Ϊ������ƣ�0��ͬ�ࣻ1������
    int s32Resv[5];
}TVT_SDK_PRODUCT_IR_CTRL_S;

/**
 * @brief ��Ʒ�׹�����ýṹ��
 */
typedef TVT_SDK_PROD_IR_S TVT_SDK_PROD_WHITELIGHT_S;

/**
 * @brief ��Ʒ�׹�ƿ������ýṹ��
 */
typedef TVT_SDK_PRODUCT_IR_CTRL_S TVT_SDK_PROD_WHITELIGHT_CTRL_S;

///// �������
/**
 * @brief FLASHö��
 */
typedef enum tvt_sdk_prod_flash_e
{
    TVT_SDK_PROD_FLASH_SPI_NOR = 0,
    TVT_SDK_PROD_FLASH_SPI_NAND,
    TVT_SDK_PROD_FLASH_EMMC,
    TVT_SDK_PROD_FLASH_BUTT,
}TVT_SDK_PROD_FLASH_E;

typedef struct tvt_sdk_prod_update_s
{
    char            szPartName[64];    ///< ��������
    unsigned int    u32PartSize;       ///< ������С
}TVT_SDK_PROD_UPDATE_S;

#ifdef __cplusplus
}
#endif

#endif