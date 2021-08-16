#ifndef _TVT_SDK_CHIP_H_
#define _TVT_SDK_CHIP_H_

#include "tvt_sdk_product_param.h"
#include "tvt_sdk_print.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CHIP_MAX_VI_NUM     2                                                ///< Sensor֧��������
#define CHIP_MAX_VPROC_NUM_PER_DEV  5                                        ///< Vproc��Sensor���ͨ����
#define CHIP_MAX_VPROC_NUM  (CHIP_MAX_VI_NUM * CHIP_MAX_VPROC_NUM_PER_DEV)   ///< Vproc���ͨ����
#define CHIP_VENC_H26X_NUM 3                                                 ///< Venc �������ͨ����
#define CHIP_VENC_JPEG_NUM 2                                                 ///< Venc ץ�����ͨ����
#define CHIP_MAX_VENC_NUM_PER_DEV  (CHIP_VENC_H26X_NUM + CHIP_VENC_JPEG_NUM) ///< Venc��Sensor���ͨ����
#define CHIP_MAX_VENC_NUM  (CHIP_MAX_VI_NUM * CHIP_MAX_VENC_NUM_PER_DEV)     ///< Venc���ͨ����
#define CHIP_MAX_VO_NUM_PER_DEV 1
#define CHIP_MAX_VO_NUM (CHIP_MAX_VI_NUM * CHIP_MAX_VO_NUM_PER_DEV)

#define CHIP_MAX_OSD_NUM_PER_VENC 6
#define CHIP_MAX_OSD_NUM_PER_DEV (CHIP_MAX_OSD_NUM_PER_VENC * CHIP_MAX_VENC_NUM_PER_DEV)
#define CHIP_MAX_OSD_NUM (CHIP_MAX_OSD_NUM_PER_DEV * CHIP_MAX_VI_NUM)

#define VIN_MAX_FPS_PAL 25
#define VIN_MAX_FPS_NTSC 30

#define SECOND_STREAM_MAX_WIDTH 1920
#define SECOND_STREAM_MAX_HEIGHT 1080
#define THIRD_STREAM_MAX_WIDTH 1280
#define THIRD_STREAM_MAX_HEIGHT 720

#define VO_CVBS_MAX_WIDTH 720
#define VO_CVBS_MAX_HEIGHT 576

typedef enum chip_video_format_e
{
    CHIP_VIDEO_FORMAT_PAL,
    CHIP_VIDEO_FORMAT_NTSC,
    CHIP_VIDEO_FORMAT_BUTT,
} CHIP_VIDEO_FORMAT_E;

typedef enum chip_video_enc_index_e
{
    CHIP_VIDEO_ENC_MAIN_STREAM          = 0,
    CHIP_VIDEO_ENC_SECOND_STREAM        = 1,
    CHIP_VIDEO_ENC_THIRD_STREAM         = 2,

    CHIP_VIDEO_ENC_JPEG_STREAM          = 3,
    CHIP_VIDEO_ENC_JPEG_ALARM_STREAM    = 4,
    CHIP_VIDEO_ENC_STREAM_BUTT,
} CHIP_VIDEO_ENC_INDEX_E; // ע�⣺should be same of TVT_SDK_VIDEO_ENC_INDEX_E

typedef struct chip_video_cfg_s
{
    SDK_VIDEO_FORMAT            enFormat;      ///< ��Ƶ��ʽ
    SDK_BLC_MODE_E              enWorkMode;    ///< Sensor����ģʽ
    TVT_SDK_PROD_SENSOR_S       stSensorCfg;   ///< Sensor��Ϣ
    int                         s32Resv[4];
} CHIP_VIDEO_CFG_S;

typedef struct chip_video_enc_cfg_s
{
    int                         s32VinDevID;   ///< ����Sensor�豸��
    int                         s32StreamIDX;  ///< ͨ������(ָCHIP_VIDEO_ENC_INDEX_E)
    SDK_VIDEO_ENCODE_TYPE       enEncType;     ///< ��������
    unsigned int                u32Width;      ///< ��
    unsigned int                u32Height;     ///< ��
    unsigned int                u32FrameRate;  ///< ֡��
    SDK_VENC_MODE               enBitRateMode; ///< ����ģʽ
    unsigned int                u32BitRate;    ///< ����
    unsigned int                u32GOP;        ///< GOP
    unsigned int                u32Profile;    ///< Profile
    int                         s32Resv[2];    ///
} CHIP_VIDEO_ENC_CFG_S;


typedef struct chip_video_enc_stream_s
{
    int                         s32VinDevID;   ///< ����Sensor�豸��
    int                         s32StreamIDX;  ///< ͨ������(ָCHIP_VIDEO_ENC_INDEX_E)
    IPCAMERA_AV_FRAME_INFO*     pstFrameInfo;  ///<
    int*                        ps32Slice;
    int*                        ps32SliceBuf;
    int                         s32SliceBufLen;
    unsigned char               u8RequestIDR;  ///< ����I֡��ʶ
    int                         s32Resv[2];    ///
} CHIP_VIDEO_ENC_STREAM_S;


typedef enum chip_osd_buff_type_e
{
    CHIP_OSD_BUFF_CLUT8,
    CHIP_OSD_BUFF_ARGB1555,
    CHIP_OSD_BUFF_TYPE_BUTT,
} CHIP_OSD_BUFF_TYPE_E;

// typedef enum chip_osd_buff_type_e
// {
//     CHIP_OSD_TYPE_CHAR,
//     CHIP_OSD_TYPE_IMG,
//     CHIP_OSD_TYPE_BUTT,
// }CHIP_OSD_TYPE_E;

typedef struct chip_osd_info_s
{
    int                     s32VinDevID;        ///< ����Sensor�豸��
    int                     s32StreamIDX;       ///< ͨ������(ָCHIP_VIDEO_ENC_INDEX_E)
    int                     s32OSDIDX;          ///< OSD������
    int                     s32Enable;          ///< ʹ��
    int                     s32Color;           ///< ��ɫ, �ݶ�RGB888
    int                     s32Alpha;           ///< ͸����
    int                     s32X;               ///< λ��
    int                     s32Y;
    int                     s32W;
    int                     s32H;
    // CHIP_OSD_TYPE_E         enOSDType;          ///< OSD����
    CHIP_OSD_BUFF_TYPE_E    enBuffType;         ///< λͼ����
    unsigned char*          pu8BuffAddr;        ///< λͼ����
} CHIP_OSD_INFO_S;

typedef struct chip_mask_info_s
{
    int                     s32VinDevID;        ///< ����Sensor�豸��
    int                     s32StreamIDX;       ///< ͨ������(ָCHIP_VIDEO_ENC_INDEX_E)
    int                     s32MaskIDX;         ///< ������
    int                     s32Enable;          ///< ʹ��
    int                     s32X;               ///< λ��
    int                     s32Y;
    int                     s32W;
    int                     s32H;
} CHIP_MASK_INFO_S;

typedef struct chip_roi_info_s
{
    int                     s32VinDevID;        ///< ����Sensor�豸��
    int                     s32StreamIDX;       ///< ͨ������(ָCHIP_VIDEO_ENC_INDEX_E)
    int                     s32ROIIDX;          ///< ������
    int                     s32Enable;          ///< ʹ��
    int                     s32QP;              ///< ROI QPֵ
    int                     s32X;               ///< λ��
    int                     s32Y;
    int                     s32W;
    int                     s32H;
} CHIP_ROI_INFO_S;

typedef struct chip_yuv_info_s
{
    unsigned int            index;
    unsigned int            width;
    unsigned int            height;
    unsigned int            pitch;
    unsigned long long      y_phy_addr;
    unsigned long long      uv_phy_addr;
    unsigned long long      y_addr;
    unsigned long long      uv_addr;
    unsigned long long      pts;
} CHIP_YUV_INFO_S;

/**
 * start the yuv service, alloc the memory for canvas pool.
 * @return  0 Success, others Failed.
 */
int chip_video_YuvInit(void);
/**
 * stop the yuv service, release all memory for canvas pool.
 * @return  0 Success, others Failed.
 */
int chip_video_YuvExit(void);

/**
 * @breif   release a yuv capture.
 * @return  0 Success, others Failed.
 */
int chip_video_ReleaseYuvFrame (const unsigned int index);

/**
 * @breif   get a yuv capture.
 * @return  0 Success, others Failed.
 */
int chip_video_YuvGetCapture(const unsigned int index, CHIP_YUV_INFO_S * pstChipYuvInfo);

/**
 * @breif   Give the Yuv index and the encode area information. Chip will encode yuv data to jpeg image.
 * @return  0 Success, others Failed.
 */
int chip_video_YuvEncJpeg(const unsigned int index, SDK_POSITION* pos, SDK_POSITION* dest_enc_pos,
                          SDK_JPEG_INFO* jpeg_info, unsigned char AllJpegFlag);

/**
 * @brief оƬ��Ƶģ�鴴������ͨ��
 * @return 0���ɹ�����0��ʧ��
 */
int chip_video_createChn(const CHIP_VIDEO_ENC_CFG_S* pstVideoEncCfg);

/**
 * @brief оƬ��Ƶģ���ȡ����ͨ������
 * @return 0���ɹ�����0��ʧ��
 */
int chip_video_getChnCfg(CHIP_VIDEO_ENC_CFG_S* pstVideoEncCfg);

/**
 * @brief оƬ��Ƶģ�����ٱ���ͨ��
 * @return 0���ɹ�����0��ʧ��
 */
int chip_video_destroyChn(int s32VinDevID, int s32StreamIDX);

/**
 * @brief оƬ��Ƶģ���ȡ����ͨ������
 * @return 0���ɹ�����0��ʧ��
 */
int chip_video_getChnStream(CHIP_VIDEO_ENC_STREAM_S* pstEncStream);

/**
 * @brief оƬ��Ƶģ������IDR��������
 * @return 0���ɹ�����0��ʧ��
 */
int chip_video_requestIDRStream(int s32VinDevID, int s32StreamIDX);

/**
 * @brief оƬ��Ƶģ��ʹ�ܱ���ͨ��
 * @return 0���ɹ�����0��ʧ��
 */
int chip_video_startStream(int s32VinDevID, int s32StreamIDX);

/**
 * @brief оƬ��Ƶģ������Slice��Ƭ
 * @return 0���ɹ�����0��ʧ��
 */
int chip_video_setEncodeSlice(int s32Enable);

/**
 * @brief оƬ��Ƶģ������OSD
 * @return 0���ɹ�����0��ʧ��
 */
int chip_video_setOSD(CHIP_OSD_INFO_S* pstOSDInfo);

/**
 * @brief оƬ��Ƶģ������Mask
 * @return 0���ɹ�����0��ʧ��
 */
int chip_video_setMask(CHIP_MASK_INFO_S* pstMaskInfo);

/**
 * @brief оƬ��Ƶģ������ROI
 * @return 0���ɹ�����0��ʧ��
 */
int chip_video_setROI(CHIP_ROI_INFO_S* pstROIInfo);

/**
 * @brief оƬ��Ƶģ�����þ���
 * @return 0���ɹ�����0��ʧ��
 */
int chip_video_setMirror(int s32VinDevID, int s32Enable);

/**
 * @brief оƬ��Ƶģ�����÷�ת
 * @return 0���ɹ�����0��ʧ��
 */
int chip_video_setFlip(int s32VinDevID, int s32Enable);

/**
 * @brief оƬ��Ƶģ���ȡ��Ƶ��������
 * @return 0���ɹ�����0��ʧ��
 */
int chip_video_getVinCfg(CHIP_VIDEO_CFG_S* pstVinCfg);

/**
 * @brief оƬ��Ƶģ���ʼ���ӿ�
 * @return 0���ɹ�����0��ʧ��
 */
int chip_video_init(const CHIP_VIDEO_CFG_S* pstVideoCfg, int s32VinNum);

/**
 * @brief оƬ��Ƶģ���ʼ���ӿ�
 * @return 0���ɹ�����0��ʧ��
 */
int chip_video_exit();

///////// ��Ƶ��� ////////////////////
typedef enum chip_audio_stream_type_e
{
    CHIP_AUDIO_STREAM_CAPTURE,
    CHIP_AUDIO_STREAM_PLAYBACK,
    CHIP_AUDIO_STREAM_TYPE_BUTT,
} CHIP_AUDIO_STREAM_TYPE_E;

typedef enum chip_audio_code_type_e
{
    CHIP_AUDIO_CODE_PCM,
    CHIP_AUDIO_CODE_G711A,
    CHIP_AUDIO_CODE_G711U,
    CHIP_AUDIO_CODE_G726,
    CHIP_AUDIO_CODE_ADPCM,
    CHIP_AUDIO_CODE_AAC,
    CHIP_AUDIO_CODE_TYPE_BUTT,
} CHIP_AUDIO_CODE_TYPE_E;

typedef struct chip_audio_param_s
{
    CHIP_AUDIO_STREAM_TYPE_E        enStreamType;       ///< �����������
    int                             s32ChannelNum;      ///< ͨ����,[1,2]
    int                             s32ChnID;           ///< ͨ����(��ͨ��ģʽ��)
    int                             s32SampleRate;      ///< ������ 8000/16000...
    int                             s32BitWidth;        ///< λ�� 8/16/32... bit
    CHIP_AUDIO_CODE_TYPE_E          en32CodeType;       ///< ��ʽ PCM/G711a/u/G726...
    int                             s32FrameSize;       ///< ��֡��С
} CHIP_AUDIO_PARAM_S;

/**
 * @brief оƬ��Ƶģ��������ýӿڣ��ο� test_audio.cpp
 * @return 0���ɹ�����0��ʧ��
 */
int chip_audio_setParam(const CHIP_AUDIO_PARAM_S* pstAudioParam);

/**
 * @brief оƬ��Ƶģ���ȡ��Ƶ���ݣ���Ƶ���룩
 * @return 0���ɹ�����0��ʧ��
 */
int chip_audio_getCapBuffer(char* pszBuffer, int* ps32BuffSize);

/**
 * @brief оƬ��Ƶģ�鲥����Ƶ���ݣ���Ƶ�����
 * @return 0���ɹ�����0��ʧ��
 */
int chip_audio_sendPlayBuffer(char* pszBuffer, int s32BuffSize);

/**
 * @brief оƬ��Ƶģ��������Ƶ��������
 * @return 0���ɹ�����0��ʧ��
 */
int chip_audio_setVolume(CHIP_AUDIO_STREAM_TYPE_E enStreamType, int s32Volume);

/**
 * @brief оƬ��Ƶģ���ʼ���ӿ�
 * @return 0���ɹ�����0��ʧ��
 */
int chip_audio_init();

/**
 * @brief оƬ��Ƶģ��ȥ��ʼ���ӿ�
 * @return 0���ɹ�����0��ʧ��
 */
int chip_audio_exit();

///////// ������� ////////////////////
typedef struct chip_dev_rtc_cfg_s
{
    unsigned int  u32Year;
    unsigned int  u32Month;
    unsigned int  u32Date;
    unsigned int  u32Hour;
    unsigned int  u32Minute;
    unsigned int  u32Second;
    unsigned int  u32Weekday;
} CHIP_DEV_RTC_CFG_S;


typedef struct chip_dev_wiegand_data_s
{
    unsigned long long           u64TimeStampMS;   // ���յ����ݵ�ʱ��(���뼶)
    unsigned int                 u32Pid;
    unsigned int                 u32Hid;
    unsigned long long           u64CardID;
    int                          s32BitNum;
    unsigned long long           u64OriData;
    char                         szResv[12];
} CHIP_DEV_WIEGAND_DATA_S;


/**
 * @brief оƬ����ģ���ȡRTCʱ��
 * @return 0���ɹ�����0��ʧ��
 */
int chip_dev_getRtcTime(CHIP_DEV_RTC_CFG_S* pstRtcInfo);

/**
 * @brief оƬ����ģ������RTCʱ��
 * @return 0���ɹ�����0��ʧ��
 */
int chip_dev_setRtcTime(CHIP_DEV_RTC_CFG_S* pstRtcInfo);

/// TODO: оƬ����ܽŸ������ýӿ�

/**
 * @brief оƬ����GPIO�����������
 * @return 0���ɹ�����0��ʧ��
 */
int chip_dev_setGpioDir(int s32GpioGrp, int s32GpioNum, int s32Dir, int s32DefVal);

/**
 * @brief оƬ�����ȡGPIOֵ
 * @return 0���ɹ�����0��ʧ��
 */
int chip_dev_getGpioVal(int s32GpioGrp, int s32GpioNum, int* ps32Val);

/**
 * @brief оƬ��������GPIOֵ
 * @return 0���ɹ�����0��ʧ��
 */
int chip_dev_setGpioVal(int s32GpioGrp, int s32GpioNum, int s32Val);

/**
 * @brief оƬ��������PWM����ռ�ձ�
 * @return 0���ɹ�����0��ʧ��
 */
int chip_dev_setPwmInfo(int s32PwmNum, int s32PwmPeriod, int s32PwmDuty);


/**
 * @brief оƬ�����ȡ(��ȡ)Uart����
 * @return 0���ɹ�����0��ʧ��
 */
int chip_dev_getUartData(int s32UartNum, char* pszData, int s32DataLens);

/**
 * @brief оƬ��������(д��)Uart����
 * @return 0���ɹ�����0��ʧ��
 */
int chip_dev_setUartData(int s32UartNum, const char* pszData, int s32DataLens);

/**
 * @brief оƬ�������� Uart ����
 * @return 0���ɹ�����0��ʧ��
 */
int chip_dev_setUartSpeed(int s32UartNum, int s32Speed);

/**
 * @brief оƬ�����ʼ��wiegand
 * @return 0���ɹ�����0��ʧ��
 */
int chip_dev_initWiegand();

/**
 * @brief оƬ��������wiegand����/���ģʽ
 * @return 0���ɹ�����0��ʧ��
 */
int chip_dev_setWiegandMode(int s32Mode);

/**
 * @brief оƬ��������wiegandλ��
 * @return 0���ɹ�����0��ʧ��
 */
int chip_dev_setWiegandBitNum(int s32BitNum);

/**
 * @brief оƬ�����ȡwiegand����
 * @return 0���ɹ�����0��ʧ��
 */
int chip_dev_getWiegandData(CHIP_DEV_WIEGAND_DATA_S* pstWiegand);

/**
 * @brief оƬ��������wiegand����
 * @return 0���ɹ�����0��ʧ��
 */
int chip_dev_setWiegandData(const CHIP_DEV_WIEGAND_DATA_S* pstWiegand);

/**
 * @brief оƬ����ȥ��ʼ��wiegand
 * @return 0���ɹ�����0��ʧ��
 */
int chip_dev_exitWiegand();


/**
 * @brief оƬ������SD���Ƿ����
 * @return 0���ɹ�����0��ʧ��
 */
int chip_dev_getTFCardStatus(const char* pszName, int* ps32Status);


int amba_audio_wkldebug(void);

#ifdef __cplusplus
}
#endif

#endif