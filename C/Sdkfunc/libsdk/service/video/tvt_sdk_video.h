#ifndef _TVT_SDK_VIDEO_H_
#define _TVT_SDK_VIDEO_H_

#include "tvt_sdk_product_param.h"
#include "sdk_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ��������ö��
 */
typedef enum tvt_sdk_video_cmd_e
{
    TVT_SDK_VIDEO_CREATE_ENCODE_STREAM,         ///< param: TVT_SDK_VIDEO_ENC_CFG_S
    TVT_SDK_VIDEO_DESTROY_ENCODE_STREAM,        ///< param: int s32StreamID
    TVT_SDK_VIDEO_GET_ENCODE_STREAM_CFG,        ///< param: TVT_SDK_VIDEO_ENC_CFG_S
    TVT_SDK_VIDEO_REQUEST_I_FRAME,              ///< param: int s32StreamID
    TVT_SDK_VIDEO_START_STREAM,                 ///< param: int s32StreamID
    TVT_SDK_VIDEO_SET_TIME_OSD_CFG,             ///< param: TVT_SDK_OSD_TIME_CFG_S
    TVT_SDK_VIDEO_UPDATE_TIME_OSD,              ///< param: int s32VinIDX
    TVT_SDK_VIDEO_SET_COMM_OSD,                 ///< param: TVT_SDK_OSD_COMM_CFG_S
    TVT_SDK_VIDEO_UPDATE_COMM_OSD,              ///< param: int s32VinIDX
    TVT_SDK_VIDEO_SET_WATERMARK,                ///< param: TVT_SDK_VIDEO_WATERMARK_CFG_S
    TVT_SDK_VIDEO_SET_SLICE,
    TVT_SDK_VIDEO_SET_MASK,                     ///< param: TVT_SDK_VIDEO_MASK_CFG_S
    TVT_SDK_VIDEO_SET_ROI,                      ///< param: TVT_SDK_VIDEO_ROI_CFG_S
    /// TODO:MD...
    TVT_SDK_VIDEO_CMD_BUTT,
} TVT_SDK_VIDEO_CMD_E;


////////////// VIDEO ////////////////////////

typedef enum tvt_sdk_video_enc_index_e
{
    TVT_SDK_VIDEO_ENC_MAIN_STREAM = 0,
    TVT_SDK_VIDEO_ENC_SECOND_STREAM,
    TVT_SDK_VIDEO_ENC_THIRD_STREAM,
    TVT_SDK_VIDEO_ENC_JPEG_STREAM,
    TVT_SDK_VIDEO_ENC_JPEG_ALARM_STREAM,
    TVT_SDK_VIDEO_ENC_STREAM_BUTT
} TVT_SDK_VIDEO_ENC_INDEX_E;

typedef struct tvt_sdk_video_cfg_s
{
    SDK_VIDEO_FORMAT            enFormat;      ///< ��Ƶ��ʽ
    SDK_BLC_MODE_E              enWorkMode;    ///< Sensor����ģʽ
    TVT_SDK_PROD_SENSOR_S    stSensorCfg;   ///< Sensor��Ϣ
    int                         s32Resv[4];
} TVT_SDK_VIDEO_CFG_S;

typedef struct tvt_sdk_video_enc_cfg_s
{
    int                         s32StreamID;   ///< ͨ��ID
    SDK_VIDEO_ENCODE_TYPE       enEncType;     ///< ��������
    unsigned int                u32Width;      ///< ��
    unsigned int                u32Height;     ///< ��
    unsigned int                u32FrameRate;  ///< ֡��
    SDK_VENC_MODE               enBitRateMode; ///< ����ģʽ
    unsigned int                u32BitRate;    ///< ����
    unsigned int                u32GOP;        ///< GOP
    unsigned int                u32Profile;    ///< Profile
    unsigned char               u8RequestIDR;  ///< ����I֡��ʶ
} TVT_SDK_VIDEO_ENC_CFG_S;

typedef struct tvt_sdk_video_watermark_cfg_s
{
    unsigned char       u8Enable;       ///< 1�����ã�0�����
    char*               pszUserData;    ///< ˮӡ����
    int                 s32DataLens;    ///< ˮӡ����
} TVT_SDK_VIDEO_WATERMARK_CFG_S;
////////////////////////////////////////////////////////
////////////////////// OSD ////////////////////////////////
/**
 * @brief OSD���
 */
typedef enum tvt_sdk_osd_index_e
{
    TVT_SDK_OSD_TIME = 0,
    TVT_SDK_OSD_DEV,
    TVT_SDK_OSD_COMM1,
    TVT_SDK_OSD_COMM2,
    TVT_SDK_OSD_COMM3,
    TVT_SDK_OSD_COMM4,
    TVT_SDK_OSD_INDEX_BUTT,
} TVT_SDK_OSD_INDEX_E;

/**
 * @brief OSD����
 */
typedef enum tvt_sdk_osd_type_e
{
    TVT_SDK_OSD_TYPE_TIME,
    TVT_SDK_OSD_TYPE_CHAR,
    TVT_SDK_OSD_TYPE_IMG,
    TVT_SDK_OSD_TYPE_BUTT,
} TVT_SDK_OSD_TYPE_E;

typedef enum tvt_sdk_osd_time_calendar_e
{
    TVT_SDK_OSD_TIME_GREGORIAN = 0,     ///< ����
    TVT_SDK_OSD_TIME_PERSIAN ,          ///< ��˹��
    TVT_SDK_OSD_TIME_CALENDAR_BUTT,
} TVT_SDK_OSD_TIME_CALENDAR_E;

typedef enum tvt_sdk_osd_time_format_e
{
    TVT_SDK_OSD_TIME_24HOUR_FORMAT = 0,     ///< 24Сʱ��
    TVT_SDK_OSD_TIME_12HOUR_FORMAT,         ///< 12Сʱ��
    TVT_SDK_OSD_TIME_FORMAT_BUTT,
} TVT_SDK_OSD_TIME_FORMAT_E;


typedef enum tvt_sdk_osd_date_format_e
{
    TVT_SDK_OSD_DATE_MM_DD_YYYY = 0,       ///< MM-DD-YYYY  02-07-2021
    TVT_SDK_OSD_DATE_YYYY_MM_DD,           ///< YYYY-MM-DD  2021-02-07
    TVT_SDK_OSD_DATE_DD_MM_YYYY,           ///< DD-MM-YYYY  07-02-2021
    TVT_SDK_OSD_DATE_FORMAT_BUTT,
} TVT_SDK_OSD_DATE_FORMAT_E;


typedef enum tvt_sdk_osd_date_format_ex_e
{
    TVT_SDK_OSD_DATE_CROSSBAR = 0,            ///< ���
    TVT_SDK_OSD_DATE_CROSSBAR_TO_CHAR,        ///< ���ת���ģ�02-07-2021 -> 02��07��2021��
    TVT_SDK_OSD_DATE_CROSSBAR_TO_SLASH,       ///< ���תб�ˣ�02-07-2021 -> 02/07/2021
    TVT_SDK_OSD_DATE_FORMAT_EX_BUTT,
} TVT_SDK_OSD_DATE_FORMAT_EX_E;

typedef enum tvt_sdk_osd_size_e
{
    TVT_SDK_OSD_SIZE_AUTO = 0,      ///< ����Ӧ
    TVT_SDK_OSD_SIZE_16X16,         ///< 16x16
    TVT_SDK_OSD_SIZE_32X32,         ///< 32x32
    TVT_SDK_OSD_SIZE_64X64,         ///< 64x64
    TVT_SDK_OSD_SIZE_BUTT,
} TVT_SDK_OSD_SIZE_E;


typedef enum tvt_sdk_osd_align_mode_e
{
    TVT_SDK_OSD_ALIGN_CUSTOM = 0, ///< �Զ�������
    TVT_SDK_OSD_ALIGN_LEFT,       ///< �����
    TVT_SDK_OSD_ALIGN_RIGHT,      ///< �Ҷ���
    TVT_SDK_OSD_ALIGN_GB,         ///< �������
    TVT_SDK_OSD_ALIGN_BUTT,
} TVT_SDK_OSD_ALIGN_MODE_E;

/**
 * @brief OSDʱ���ʽ
 */
typedef struct tvt_sdk_osd_time_fmt_s
{
    unsigned int                        u32Enable;          ///< ��ʾ
    unsigned int                        u32EnableWeek;      ///< ʹ����ʾ����
    TVT_SDK_OSD_TIME_CALENDAR_E         enCalendar;         ///< ����(��������˹��)
    TVT_SDK_OSD_TIME_FORMAT_E           enTimeFormat;       ///< ʱ���ʽ
    TVT_SDK_OSD_DATE_FORMAT_E           enDateFormat;       ///< ���ڸ�ʽ
    TVT_SDK_OSD_DATE_FORMAT_EX_E        enDateFormatEx;     ///< ������չ��ʽ
} TVT_SDK_OSD_TIME_FMT_S;


/**
 * @brief OSD Comm ��ʽ
 */
typedef struct tvt_sdk_osd_comm_info_s
{
    unsigned int            u32Enable;     ///< ��ʾ
    TVT_SDK_OSD_TYPE_E      enOSDType;     ///< OSD����
    unsigned char*          pszData;       ///< ��������/λͼ����
} TVT_SDK_OSD_COMM_INFO_S;

/**
 * @brief OSDʱ���������
 */
typedef struct tvt_sdk_osd_time_cfg_s
{
    unsigned int            s32VinIDX;     ///< ��Ƶ�������
    TVT_SDK_OSD_TIME_FMT_S  stTimeFmt;     ///< ʱ���ʽ
    SDK_POSITION            stPosition;    ///< λ����Ϣ
} TVT_SDK_OSD_TIME_CFG_S;

/**
 * @brief Comm OSD�������
 */
typedef struct tvt_sdk_osd_comm_cfg_s
{
    unsigned int            s32VinIDX;     ///< ��Ƶ�������
    unsigned int            u32OSDIDX;     ///< �ڼ���OSD
    TVT_SDK_OSD_COMM_INFO_S stOSDInfo;     ///< ����
    SDK_POSITION            stPosition;    ///< λ����Ϣ
} TVT_SDK_OSD_COMM_CFG_S;


////////////////////////////////////////////////////////
////////////////////// MASK ////////////////////////////////
/**
 * @brief MASK�������
 */
typedef struct tvt_sdk_video_mask_cfg_s
{
    unsigned int            s32VinIDX;     ///< ��Ƶ�������
    unsigned int            u32Enable;     ///< ��ʾ
    SDK_POSITION            stPosition;    ///< λ����Ϣ
} TVT_SDK_VIDEO_MASK_CFG_S;

////////////////////////////////////////////////////////

////////////////////// ROI ////////////////////////////////
/**
 * @brief ROI ��Ϣ
 */
typedef struct tvt_sdk_video_roi_info_s
{
    unsigned int        u32Enable;      ///< ʹ��
    unsigned int        u32ROIIDX;      ///< �ڼ���ROI
    int                 s32Level;       ///< ����
    SDK_POSITION        stPosition;     ///< λ����Ϣ
} TVT_SDK_VIDEO_ROI_INFO_S;


/**
 * @brief ROI����
 */
typedef struct tvt_sdk_video_roi_cfg_s
{
    unsigned int                s32VinIDX;     ///< ��Ƶ�������
    int                         s32ROINum;     ///< ���õĸ���
    TVT_SDK_VIDEO_ROI_INFO_S*   pstROIInfo;
} TVT_SDK_VIDEO_ROI_CFG_S;

////////////////////////////////////////////////////////

/**
 * @brief ��ʼ����Ƶҵ��
 * @param [in] pstVideoCfg:��Ƶ�������ָ��
 * @param [in] s32VinNum:��Ƶ�������
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_video_init(TVT_SDK_VIDEO_CFG_S* pstVideoCfg, int s32VinNum);

/**
 * @brief ȥ��ʼ����Ƶҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_video_exit();

/**
 * @brief ��Ƶҵ����ƽӿ�
 * @param [in] s32Cmd:������
 * @param [in] pParam:����ָ��
 * @param [in] s32Length:��������
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_video_ctrl(int s32Cmd, void* pParam, int s32Length);

/**
 * @brief ��ȡ������
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_video_getEncodeStream(int s32StreamID, IPCAMERA_AV_FRAME_INFO* pstFrameInfo, int* ps32SLice, int* ps32SliceBuf, int s32SliceBufLen);

/**
 * @brief �ͷű�����
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_video_releaseEncodeStream(IPCAMERA_AV_FRAME_INFO* pstFrameInfo);


/**
 * @brief ��ȡJpeg��������
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_video_getJpegBuffer(int s32StreamID, IPCAMERA_AV_FRAME_INFO* pstFrameInfo);



/// TODO:��ȡYUVԭʼ����

#ifdef __cplusplus
}
#endif

#endif