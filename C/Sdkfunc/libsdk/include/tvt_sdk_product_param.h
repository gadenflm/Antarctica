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
    TVT_SDK_PROD_SUPP_AF       = 0x00000001, ///< AF支持
    TVT_SDK_PROD_SUPP_ZoomBtn  = 0x00000002, ///< 
    TVT_SDK_PROD_SUPP_ABF      = 0x00000004,
    TVT_SDK_PROD_SUPP_VO       = 0x00000008,
    TVT_SDK_PROD_SUPP_RS485    = 0x00000010,
    TVT_SDK_PROD_SUPP_ALARM    = 0x00000020,
    TVT_SDK_PROD_SUPP_WIEGAND  = 0x00000040,
    TVT_SDK_PROD_SUPP_SWCDS    = 0x00000001, ///< 软光敏
    
    TVT_SDK_PROD_FEATURE_BUTT,
}TVT_SDK_PROD_FEATURE_E;

/**
 * @brief 设备机型结构体
 */
typedef struct tvt_sdk_prod_dev_info_s
{
    int     s32ModelID;     // 外壳型号
    int     s32LensID;      // 镜头型号
    int     s32CstmrID;     // 客户ID
    int     s32Resv[5];
}TVT_SDK_PROD_DEV_INFO;

/**
 * @brief 控制结构体
 */
typedef struct tvt_sdk_prod_ctrl_info_s
{
    TVT_SDK_PROD_DEV_INFO       stDevInfo;      ///< 输入数据
    void*                       pUserData;      ///< 输入数据

    void*                       pParam;         ///< 输出数据
    int                         s32ParamSize;   ///< 输出数据
    int s32Resv[2];
}TVT_SDK_PROD_CTRL_INFO;

/// 参数回调函数
typedef int (*TvtSdkProdCb)(int s32Cmd, void* pParam, int s32Length);

///// 平台相关
/**
 * @brief 芯片型号枚举
 */
typedef enum tvt_sdk_prod_chip_e
{
    TVT_SDK_PROD_PLAT_CV28S55M = 0,
    TVT_SDK_PROD_PLAT_BUTT,
}TVT_SDK_PROD_CHIP_E;

/**
 * @brief 产品平台硬件信息结构体
 */
typedef struct tvt_sdk_prod_platform_s
{
    TVT_SDK_PROD_CHIP_E     enChipType;     ///< 产品使用芯片型号
    int s32Resv[3];
}TVT_SDK_PROD_PLATFORM_S;


///// Sensor 相关
/**
 * @brief Sensor型号枚举
 */
typedef enum tvt_sdk_prod_sensor_e
{
    TVT_SDK_PROD_SENSOR_IMX464 = 0,
    TVT_SDK_PROD_SENSOR_IMX334,
    TVT_SDK_PROD_SENSOR_BUTT,
}TVT_SDK_PROD_SENSOR_E;

/**
 * @brief Sensor HDR模式枚举
 */
typedef enum tvt_sdk_prod_sensor_hdr_mode_e
{
    TVT_SDK_PROD_SENSOR_HDR_NONE = 0,
    TVT_SDK_PROD_SENSOR_HDR_2X,
    TVT_SDK_PROD_SENSOR_HDR_3X,
    TVT_SDK_PROD_SENSOR_HDR_BUTT,
}TVT_SDK_PROD_SENSOR_HDR_MODE_E;

/**
 * @brief 产品Sensor信息结构体
 */
typedef struct tvt_sdk_prod_sensor_s
{
    TVT_SDK_PROD_SENSOR_E           enSensorType;       ///< 产品使用 Sensor 类型
    TVT_SDK_PROD_SENSOR_HDR_MODE_E  enSensorHDRMode;    ///< 产品HDR模式类型
    unsigned int                    u32VinDev;          ///< 所在视频输入设备号
    unsigned int                    u32MaxWidth;        ///< 支持最大分辨率宽
    unsigned int                    u32MaxHeight;       ///< 支持最大分辨率高
    int s32Resv[4];
}TVT_SDK_PROD_SENSOR_S;


///// 镜头相关
/**
 * @brief 镜头枚举
 */
typedef enum tvt_sdk_prod_lens_e
{
    TVT_SDK_PROD_FIXED_LENS     = 0,
    TVT_SDK_PROD_LENS_IR02812FB = 1,
    TVT_SDK_PROD_LENS_BUTT,
}TVT_SDK_PROD_LENS_E;

/**
 * @brief 产品镜头信息结构体
 */
typedef struct tvt_sdk_prod_lens_s
{
    TVT_SDK_PROD_LENS_E   enLensType;  ///< 产品使用镜头类型
    int s32Resv[3];
}TVT_SDK_PROD_LENS_S;


///// 光圈相关
/**
 * @brief 光圈类型枚举
 */
typedef enum tvt_sdk_prod_iris_e
{
    TVT_SDK_PROD_FIXED_IRIS = 0,
    TVT_SDK_PROD_DC_IRIS = 1,
    TVT_SDK_PROD_P_IRIS = 2,
    TVT_SDK_PROD_IRIS_BUTT,
}TVT_SDK_PROD_IRIS_E;

/**
 * @brief 产品光圈信息结构体
 */
typedef struct tvt_sdk_prod_iris_s
{
    TVT_SDK_PROD_IRIS_E   enIrisType;  ///< 产品使用光圈类型
    int s32Resv[3];
}TVT_SDK_PROD_IRIS_S;


///// 音频相关
/**
 * @brief 产品音频采样率枚举
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
 * @brief 产品音频位宽枚举
 */
typedef enum tvt_sdk_prod_audio_bit_width_e
{
    TVT_SDK_PROD_AUDIO_BIT_WIDTH_8  = 0x1,
    TVT_SDK_PROD_AUDIO_BIT_WIDTH_16 = 0x2,
    TVT_SDK_PROD_AUDIO_BIT_WIDTH_32 = 0x4,
    TVT_SDK_PROD_AUDIO_BIT_WIDTH_BUTT,
}TVT_SDK_PROD_AUDIO_BIT_WIDTH_E;


/**
 * @brief 产品音频编码格式枚举
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
 * @brief 产品音频声道枚举
 */
typedef enum tvt_sdk_prod_audio_mode_e
{
    // TVT_SDK_PROD_AUDIO_MODE_MONO   = 0, ///< 单声道
    TVT_SDK_PROD_AUDIO_MODE_LEFT,       ///< 左声道
    TVT_SDK_PROD_AUDIO_MODE_RIGHT,      ///< 右声道
    TVT_SDK_PROD_AUDIO_MODE_STEREO,     ///< 立体声
    TVT_SDK_PROD_AUDIO_MODE_BUTT,
} TVT_SDK_PROD_AUDIO_MODE_E;

/**
 * @brief 产品音频输入配置结构体
 */
typedef struct tvt_sdk_prod_audio_s
{
    TVT_SDK_PROD_AUDIO_MODE_E      enAudioMode;           ///< 所属声道
    int                            s32SuppBitWidth;       ///< 支持的位宽
    int                            s32SuppEncType;        ///< 支持的编码格式
    int                            s32SuppBitRate;        ///< 支持的采样率
    int                            s32MinVolume;          ///< 最小音量
    int                            s32MaxVolume;          ///< 最大音量
    int                            s32LineMux;
    int s32Resv;
}TVT_SDK_PROD_AUDIO_S;

///// 视频编码相关
/// TODO: 获取各路分辨率编码等格式使用：SDK_SUPPORT_FRAME_RATE


///// 外设相关
/**
 * @brief 产品按键事件触发枚举
 */
typedef enum tvt_sdk_prod_btn_trig_mode_e
{
    TVT_SDK_PROD_BTN_FALLING_EDGE,  ///< 下降沿触发
    TVT_SDK_PROD_BTN_RISING_EDGE,   ///< 上升沿触发
    TVT_SDK_PROD_BTN_TRIG_BUTT,
} TVT_SDK_PROD_BTN_TRIG_MODE_E;

/**
 * @brief 产品串口配置结构体
 */
typedef struct tvt_sdk_prod_uart_s
{
    int     s32UartID;      ///< 所属串口ID
    int     s32Speed;       ///< 波特率
    int s32Resv[6];
}TVT_SDK_PROD_UART_S;

/**
 * @brief 产品GPIO配置结构体
 */
typedef struct tvt_sdk_prod_gpio_s
{
    int     s32GpioGrp;      ///< 所属GPIO组号
    int     s32GpioNum;      ///< 所属GPIO num
    int s32Resv[6];
}TVT_SDK_PROD_GPIO_S;

/**
 * @brief 产品按键事件结构体
 */
typedef struct tvt_sdk_prod_btn_event_s
{
    int                             s32GpioGrp;      ///< 所属GPIO组号
    int                             s32GpioNum;      ///< 所属GPIO num
    TVT_SDK_PROD_BTN_TRIG_MODE_E    enTrigMode;      ///< 触发模式
    int s32Resv[5];
}TVT_SDK_PROD_BTN_EVENT_S;

/**
 * @brief 产品PWM配置结构体
 */
typedef struct tvt_sdk_prod_pwm_s
{
    int     s32PwmNum;     ///< 所属Pwm设备号
    int     s32Period;     ///< 周期
    int     s32MaxDuty;    ///< 最大占空比
    int s32Resv[5];
}TVT_SDK_PROD_PWM_S;

/**
 * @brief 产品ADC配置结构体
 */
typedef struct tvt_sdk_prod_adc_s
{
    int     s32AdcDevID;    ///< 所属ADC设备号
    int     s32Resv[3];
}TVT_SDK_PROD_ADC_S;


/**
 * @brief 产品RS485配置结构体
 */
typedef TVT_SDK_PROD_UART_S TVT_SDK_PROD_RS485_S;

/**
 * @brief 产品报警配置结构体
 */
typedef TVT_SDK_PROD_GPIO_S TVT_SDK_PROD_ALARM_S;

/**
 * @brief 产品复位按键配置结构体
 */
typedef TVT_SDK_PROD_GPIO_S TVT_SDK_PROD_RESET_BTN_S;

/**
 * @brief 产品ZOOM按键配置结构体
 */
typedef TVT_SDK_PROD_BTN_EVENT_S TVT_SDK_PROD_ZOOM_BTN_S;

/**
 * @brief 产品ABF按键配置结构体
 */
typedef TVT_SDK_PROD_GPIO_S TVT_SDK_PROD_ABF_BTN_S;

/**
 * @brief 产品IRCUT配置结构体
 */
typedef TVT_SDK_PROD_GPIO_S TVT_SDK_PROD_IRCUT_S;

/**
 * @brief 产品CDS配置结构体
 */
typedef TVT_SDK_PROD_ADC_S TVT_SDK_PROD_CDS_S;


/**
 * @brief 产品韦根位宽枚举
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
 * @brief 产品韦根配置结构体
 */
typedef struct tvt_sdk_prod_wiegand_s
{
    int     s32SuppBitNum;
    int     s32Resv[7];
}TVT_SDK_PROD_WIEGAND_S;


/**
 * @brief 产品红外灯配置结构体
 */
typedef struct tvt_sdk_prod_ir_s
{
    int                     s32IsGpio;  ///< 1:GPIO(仅配置GPIO) 0:PWM(同时配置PWM和GPIO)
    int                     s32IrType;  ///< 红外灯类型，0：近光灯；1：远光灯
    TVT_SDK_PROD_PWM_S      stPwmCfg;   ///< 对应Pwm配置
    TVT_SDK_PROD_GPIO_S     stGpioCfg;  ///< 对应Gpio配置
}TVT_SDK_PROD_IR_S;

/**
 * @brief 产品红外灯控制配置结构体
 */
typedef struct tvt_sdk_product_ir_ctrl_s
{
    int     s32EnableGrpCtrl;   ///< 是否使能分组控制，0：不使能；1：使能
    int     s32EnablePowerCtrl; ///< 是否使能功耗控制，0：不使能；1：使能
    int     s32InvertCtrl;      ///< 是否为反相控制，0：同相；1：反相
    int s32Resv[5];
}TVT_SDK_PRODUCT_IR_CTRL_S;

/**
 * @brief 产品白光灯配置结构体
 */
typedef TVT_SDK_PROD_IR_S TVT_SDK_PROD_WHITELIGHT_S;

/**
 * @brief 产品白光灯控制配置结构体
 */
typedef TVT_SDK_PRODUCT_IR_CTRL_S TVT_SDK_PROD_WHITELIGHT_CTRL_S;

///// 升级相关
/**
 * @brief FLASH枚举
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
    char            szPartName[64];    ///< 分区名称
    unsigned int    u32PartSize;       ///< 分区大小
}TVT_SDK_PROD_UPDATE_S;

#ifdef __cplusplus
}
#endif

#endif