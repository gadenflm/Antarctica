#ifndef _TVT_SDK_DEVICE_H_
#define _TVT_SDK_DEVICE_H_

#include "tvt_sdk_product_param.h"
#include "sdk_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/// TODO: ö�٣������ƹ����(����)��ALARM/UART/RS485/RESET...
/**
 * @brief ��������ö��
 */
typedef enum tvt_sdk_device_cmd_e
{
    TVT_SDK_DEVICE_RTC,                 ///< param: TVT_SDK_DEVICE_RTC_S
    TVT_SDK_DEVICE_IRCUT,               ///< param: int [0, 1]
    TVT_SDK_DEVICE_RESET,               ///< param: int [0, 1]
    TVT_SDK_DEVICE_ABF,                 ///< param: int [0, 1]
    TVT_SDK_DEVICE_CDS,                 ///< param: int 
    TVT_SDK_DEVICE_UART_DATA,           ///< param: TVT_SDK_DEVICE_UART_DATA_S
    TVT_SDK_DEVICE_UART_CFG,            ///< param: int speed
    TVT_SDK_DEVICE_SECURITY,            ///< param: TVT_SDK_DEVICE_SEC_INFO_S
    TVT_SDK_DEVICE_ALARM_IN,            ///< param: int 
    TVT_SDK_DEVICE_ALARM_OUT,           ///< param: TVT_SDK_DEVICE_ALARM_OUT_S 
    TVT_SDK_DEVICE_ALARM_LIGHT,         ///< param: int  [0, 1]
    TVT_SDK_DEVICE_SDCARD_STATUS,       ///< param: TVT_SDK_DEVICE_SDCARD_STATUS_E
    TVT_SDK_DEVICE_SDCARD_PART,         ///< param: TVT_SDK_DEVICE_SDCARD_INFO_S
    TVT_SDK_DEVICE_WIEGAND_MODE,        ///< param: int  [0, 1]
    TVT_SDK_DEVICE_WIEGAND_CTRL,        ///< param: TVT_SDK_DEVICE_WIEGAND_CFG_S
    TVT_SDK_DEVICE_WIEGAND_DATA,        ///< param: TVT_SDK_DEVICE_WIEGAND_DATA_S
    TVT_SDK_DEVICE_DOOR_KEY,            ///< param: int
    TVT_SDK_DEVICE_TAMPER,              ///< param: int
    TVT_SDK_DEVICE_CMD_BUTT,
} TVT_SDK_DEVICE_CMD_E;

/**
 * @brief ���Ʒ��� set or get
 */
typedef enum tvt_sdk_device_ctrl_dir_e
{
    TVT_SDK_DEV_CTRL_GET,
    TVT_SDK_DEV_CTRL_SET,
    TVT_SDK_DEV_CTRL_DIR_BUTT,
}TVT_SDK_DEVICE_CTRL_DIR_E;

/**
 * @brief RTC
 */
typedef struct tvt_sdk_device_rtc_s
{
    unsigned int  u32Year;
    unsigned int  u32Month;
    unsigned int  u32Date;
    unsigned int  u32Hour;
    unsigned int  u32Minute;
    unsigned int  u32Second;
    unsigned int  u32Weekday;
}TVT_SDK_DEVICE_RTC_S;


/**
 * @brief UART
 */
typedef struct tvt_sdk_device_uart_data_s
{
    char*       pszData;
    int         s32DataLens;
    int         s32Rev[6];
}TVT_SDK_DEVICE_UART_DATA_S;


/**
 * @brief ��ȫ��
 */
typedef struct tvt_sdk_device_sec_info_s
{
    int             s32IsSet;
    unsigned char   u8MacAddr[6];
}TVT_SDK_DEVICE_SEC_INFO_S;


/**
 * @brief SD��״̬
 */
typedef enum tvt_sdk_device_sdcard_status_e
{
    TVT_SDK_SDCARD_NO_CARD      = 0,
    TVT_SDK_SDCARD_READ_ONLY    = 1, 
    TVT_SDK_SDCARD_READ_WRITE   = 2,
    TVT_SDK_SDCARD_STATUS_BUTT,
}TVT_SDK_DEVICE_SDCARD_STATUS_E;


/**
 * @brief SD��������Ϣ
 */
#define TVT_SDK_MAX_SD_PART_NAME_SZIE 32 //SD������������󳤶�
typedef struct tvt_sdk_device_sdcard_info_s
{
    int             s32ExistPartition;
    unsigned int    u32PartSize;
    unsigned char   szPartName[TVT_SDK_MAX_SD_PART_NAME_SZIE];
    int             s32Resv[4];
}TVT_SDK_DEVICE_SDCARD_INFO_S;


/**
 * @brief Τ����������
 */
typedef struct tvt_sdk_device_wiegand_data_s
{
    unsigned long long           u64TimeStampMS;   // ���յ����ݵ�ʱ��(���뼶)
    unsigned int                 u32Pid;
    unsigned int                 u32Hid;
    unsigned long long           u64CardID;
    int                          s32BitNum;
    unsigned long long           u64OriData;
    char                         szResv[12];
}TVT_SDK_DEVICE_WIEGAND_DATA_S;

/**
 * @brief Τ������
 */
typedef struct tvt_sdk_device_wiegand_cfg_s
{
    int         s32Mode;    ///< 0 ����; 1 ���
    int         s32BitNum;  ///< λ��
}TVT_SDK_DEVICE_WIEGAND_CFG_S;


/**
 * @brief �澯�������
 */
typedef struct tvt_sdk_device_alarm_out_s
{
    int             s32Chn;     // �ڼ�·
    int             s32OutType; // 0:close 1:open
}TVT_SDK_DEVICE_ALARM_OUT_S;

/**
 * @brief ��ʼ���������ҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_device_init(TVT_SDK_PROD_DEV_INFO* pstParamDevInfo, TvtSdkProdCb pParamCb); 

/**
 * @brief ȥ��ʼ���������ҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_device_exit();

/**
 * @brief ����ҵ����ƽӿ�
 * @param [in] s32Cmd:������
 * @param [in] s32Dir:TVT_SDK_DEVICE_CTRL_DIR_E
 * @param [in] pParam:����ָ��
 * @param [in] s32Length:��������
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_device_ctrl(int s32Cmd, int s32Dir, void* pParam, int s32Length);


#ifdef __cplusplus
}
#endif

#endif