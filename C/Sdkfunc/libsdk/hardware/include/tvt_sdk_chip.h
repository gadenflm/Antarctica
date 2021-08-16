#ifndef _TVT_SDK_CHIP_H_
#define _TVT_SDK_CHIP_H_

#include "tvt_sdk_product_param.h"
#include "tvt_sdk_print.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CHIP_MAX_VI_NUM     2                                                ///< Sensor支持最大个数
#define CHIP_MAX_VPROC_NUM_PER_DEV  5                                        ///< Vproc单Sensor最大通道数
#define CHIP_MAX_VPROC_NUM  (CHIP_MAX_VI_NUM * CHIP_MAX_VPROC_NUM_PER_DEV)   ///< Vproc最大通道数
#define CHIP_VENC_H26X_NUM 3                                                 ///< Venc 编码最大通道数
#define CHIP_VENC_JPEG_NUM 2                                                 ///< Venc 抓拍最大通道数
#define CHIP_MAX_VENC_NUM_PER_DEV  (CHIP_VENC_H26X_NUM + CHIP_VENC_JPEG_NUM) ///< Venc单Sensor最大通道数
#define CHIP_MAX_VENC_NUM  (CHIP_MAX_VI_NUM * CHIP_MAX_VENC_NUM_PER_DEV)     ///< Venc最大通道数
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
} CHIP_VIDEO_ENC_INDEX_E; // 注意：should be same of TVT_SDK_VIDEO_ENC_INDEX_E

typedef struct chip_video_cfg_s
{
    SDK_VIDEO_FORMAT            enFormat;      ///< 视频制式
    SDK_BLC_MODE_E              enWorkMode;    ///< Sensor工作模式
    TVT_SDK_PROD_SENSOR_S       stSensorCfg;   ///< Sensor信息
    int                         s32Resv[4];
} CHIP_VIDEO_CFG_S;

typedef struct chip_video_enc_cfg_s
{
    int                         s32VinDevID;   ///< 所属Sensor设备号
    int                         s32StreamIDX;  ///< 通道索引(指CHIP_VIDEO_ENC_INDEX_E)
    SDK_VIDEO_ENCODE_TYPE       enEncType;     ///< 编码类型
    unsigned int                u32Width;      ///< 宽
    unsigned int                u32Height;     ///< 高
    unsigned int                u32FrameRate;  ///< 帧率
    SDK_VENC_MODE               enBitRateMode; ///< 码率模式
    unsigned int                u32BitRate;    ///< 码率
    unsigned int                u32GOP;        ///< GOP
    unsigned int                u32Profile;    ///< Profile
    int                         s32Resv[2];    ///
} CHIP_VIDEO_ENC_CFG_S;


typedef struct chip_video_enc_stream_s
{
    int                         s32VinDevID;   ///< 所属Sensor设备号
    int                         s32StreamIDX;  ///< 通道索引(指CHIP_VIDEO_ENC_INDEX_E)
    IPCAMERA_AV_FRAME_INFO*     pstFrameInfo;  ///<
    int*                        ps32Slice;
    int*                        ps32SliceBuf;
    int                         s32SliceBufLen;
    unsigned char               u8RequestIDR;  ///< 请求I帧标识
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
    int                     s32VinDevID;        ///< 所属Sensor设备号
    int                     s32StreamIDX;       ///< 通道索引(指CHIP_VIDEO_ENC_INDEX_E)
    int                     s32OSDIDX;          ///< OSD索引号
    int                     s32Enable;          ///< 使能
    int                     s32Color;           ///< 颜色, 暂定RGB888
    int                     s32Alpha;           ///< 透明度
    int                     s32X;               ///< 位置
    int                     s32Y;
    int                     s32W;
    int                     s32H;
    // CHIP_OSD_TYPE_E         enOSDType;          ///< OSD类型
    CHIP_OSD_BUFF_TYPE_E    enBuffType;         ///< 位图类型
    unsigned char*          pu8BuffAddr;        ///< 位图内容
} CHIP_OSD_INFO_S;

typedef struct chip_mask_info_s
{
    int                     s32VinDevID;        ///< 所属Sensor设备号
    int                     s32StreamIDX;       ///< 通道索引(指CHIP_VIDEO_ENC_INDEX_E)
    int                     s32MaskIDX;         ///< 索引号
    int                     s32Enable;          ///< 使能
    int                     s32X;               ///< 位置
    int                     s32Y;
    int                     s32W;
    int                     s32H;
} CHIP_MASK_INFO_S;

typedef struct chip_roi_info_s
{
    int                     s32VinDevID;        ///< 所属Sensor设备号
    int                     s32StreamIDX;       ///< 通道索引(指CHIP_VIDEO_ENC_INDEX_E)
    int                     s32ROIIDX;          ///< 索引号
    int                     s32Enable;          ///< 使能
    int                     s32QP;              ///< ROI QP值
    int                     s32X;               ///< 位置
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
 * @brief 芯片视频模块创建编码通道
 * @return 0：成功；非0：失败
 */
int chip_video_createChn(const CHIP_VIDEO_ENC_CFG_S* pstVideoEncCfg);

/**
 * @brief 芯片视频模块获取编码通道配置
 * @return 0：成功；非0：失败
 */
int chip_video_getChnCfg(CHIP_VIDEO_ENC_CFG_S* pstVideoEncCfg);

/**
 * @brief 芯片视频模块销毁编码通道
 * @return 0：成功；非0：失败
 */
int chip_video_destroyChn(int s32VinDevID, int s32StreamIDX);

/**
 * @brief 芯片视频模块获取编码通道数据
 * @return 0：成功；非0：失败
 */
int chip_video_getChnStream(CHIP_VIDEO_ENC_STREAM_S* pstEncStream);

/**
 * @brief 芯片视频模块请求IDR编码数据
 * @return 0：成功；非0：失败
 */
int chip_video_requestIDRStream(int s32VinDevID, int s32StreamIDX);

/**
 * @brief 芯片视频模块使能编码通道
 * @return 0：成功；非0：失败
 */
int chip_video_startStream(int s32VinDevID, int s32StreamIDX);

/**
 * @brief 芯片视频模块设置Slice分片
 * @return 0：成功；非0：失败
 */
int chip_video_setEncodeSlice(int s32Enable);

/**
 * @brief 芯片视频模块设置OSD
 * @return 0：成功；非0：失败
 */
int chip_video_setOSD(CHIP_OSD_INFO_S* pstOSDInfo);

/**
 * @brief 芯片视频模块设置Mask
 * @return 0：成功；非0：失败
 */
int chip_video_setMask(CHIP_MASK_INFO_S* pstMaskInfo);

/**
 * @brief 芯片视频模块设置ROI
 * @return 0：成功；非0：失败
 */
int chip_video_setROI(CHIP_ROI_INFO_S* pstROIInfo);

/**
 * @brief 芯片视频模块设置镜像
 * @return 0：成功；非0：失败
 */
int chip_video_setMirror(int s32VinDevID, int s32Enable);

/**
 * @brief 芯片视频模块设置翻转
 * @return 0：成功；非0：失败
 */
int chip_video_setFlip(int s32VinDevID, int s32Enable);

/**
 * @brief 芯片视频模块获取视频输入配置
 * @return 0：成功；非0：失败
 */
int chip_video_getVinCfg(CHIP_VIDEO_CFG_S* pstVinCfg);

/**
 * @brief 芯片视频模块初始化接口
 * @return 0：成功；非0：失败
 */
int chip_video_init(const CHIP_VIDEO_CFG_S* pstVideoCfg, int s32VinNum);

/**
 * @brief 芯片视频模块初始化接口
 * @return 0：成功；非0：失败
 */
int chip_video_exit();

///////// 音频相关 ////////////////////
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
    CHIP_AUDIO_STREAM_TYPE_E        enStreamType;       ///< 输入输出类型
    int                             s32ChannelNum;      ///< 通道数,[1,2]
    int                             s32ChnID;           ///< 通道号(单通道模式用)
    int                             s32SampleRate;      ///< 采样率 8000/16000...
    int                             s32BitWidth;        ///< 位宽 8/16/32... bit
    CHIP_AUDIO_CODE_TYPE_E          en32CodeType;       ///< 格式 PCM/G711a/u/G726...
    int                             s32FrameSize;       ///< 单帧大小
} CHIP_AUDIO_PARAM_S;

/**
 * @brief 芯片音频模块参数设置接口，参考 test_audio.cpp
 * @return 0：成功；非0：失败
 */
int chip_audio_setParam(const CHIP_AUDIO_PARAM_S* pstAudioParam);

/**
 * @brief 芯片音频模块获取音频数据（音频输入）
 * @return 0：成功；非0：失败
 */
int chip_audio_getCapBuffer(char* pszBuffer, int* ps32BuffSize);

/**
 * @brief 芯片音频模块播放音频数据（音频输出）
 * @return 0：成功；非0：失败
 */
int chip_audio_sendPlayBuffer(char* pszBuffer, int s32BuffSize);

/**
 * @brief 芯片音频模块设置音频输入音量
 * @return 0：成功；非0：失败
 */
int chip_audio_setVolume(CHIP_AUDIO_STREAM_TYPE_E enStreamType, int s32Volume);

/**
 * @brief 芯片音频模块初始化接口
 * @return 0：成功；非0：失败
 */
int chip_audio_init();

/**
 * @brief 芯片音频模块去初始化接口
 * @return 0：成功；非0：失败
 */
int chip_audio_exit();

///////// 外设相关 ////////////////////
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
    unsigned long long           u64TimeStampMS;   // 接收到数据的时间(毫秒级)
    unsigned int                 u32Pid;
    unsigned int                 u32Hid;
    unsigned long long           u64CardID;
    int                          s32BitNum;
    unsigned long long           u64OriData;
    char                         szResv[12];
} CHIP_DEV_WIEGAND_DATA_S;


/**
 * @brief 芯片外设模块获取RTC时间
 * @return 0：成功；非0：失败
 */
int chip_dev_getRtcTime(CHIP_DEV_RTC_CFG_S* pstRtcInfo);

/**
 * @brief 芯片外设模块设置RTC时间
 * @return 0：成功；非0：失败
 */
int chip_dev_setRtcTime(CHIP_DEV_RTC_CFG_S* pstRtcInfo);

/// TODO: 芯片外设管脚复用设置接口

/**
 * @brief 芯片外设GPIO输入输出配置
 * @return 0：成功；非0：失败
 */
int chip_dev_setGpioDir(int s32GpioGrp, int s32GpioNum, int s32Dir, int s32DefVal);

/**
 * @brief 芯片外设获取GPIO值
 * @return 0：成功；非0：失败
 */
int chip_dev_getGpioVal(int s32GpioGrp, int s32GpioNum, int* ps32Val);

/**
 * @brief 芯片外设设置GPIO值
 * @return 0：成功；非0：失败
 */
int chip_dev_setGpioVal(int s32GpioGrp, int s32GpioNum, int s32Val);

/**
 * @brief 芯片外设设置PWM周期占空比
 * @return 0：成功；非0：失败
 */
int chip_dev_setPwmInfo(int s32PwmNum, int s32PwmPeriod, int s32PwmDuty);


/**
 * @brief 芯片外设获取(读取)Uart数据
 * @return 0：成功；非0：失败
 */
int chip_dev_getUartData(int s32UartNum, char* pszData, int s32DataLens);

/**
 * @brief 芯片外设设置(写入)Uart数据
 * @return 0：成功；非0：失败
 */
int chip_dev_setUartData(int s32UartNum, const char* pszData, int s32DataLens);

/**
 * @brief 芯片外设设置 Uart 速率
 * @return 0：成功；非0：失败
 */
int chip_dev_setUartSpeed(int s32UartNum, int s32Speed);

/**
 * @brief 芯片外设初始化wiegand
 * @return 0：成功；非0：失败
 */
int chip_dev_initWiegand();

/**
 * @brief 芯片外设设置wiegand输入/输出模式
 * @return 0：成功；非0：失败
 */
int chip_dev_setWiegandMode(int s32Mode);

/**
 * @brief 芯片外设设置wiegand位数
 * @return 0：成功；非0：失败
 */
int chip_dev_setWiegandBitNum(int s32BitNum);

/**
 * @brief 芯片外设获取wiegand数据
 * @return 0：成功；非0：失败
 */
int chip_dev_getWiegandData(CHIP_DEV_WIEGAND_DATA_S* pstWiegand);

/**
 * @brief 芯片外设设置wiegand数据
 * @return 0：成功；非0：失败
 */
int chip_dev_setWiegandData(const CHIP_DEV_WIEGAND_DATA_S* pstWiegand);

/**
 * @brief 芯片外设去初始化wiegand
 * @return 0：成功；非0：失败
 */
int chip_dev_exitWiegand();


/**
 * @brief 芯片外设检查SD卡是否接入
 * @return 0：成功；非0：失败
 */
int chip_dev_getTFCardStatus(const char* pszName, int* ps32Status);


int amba_audio_wkldebug(void);

#ifdef __cplusplus
}
#endif

#endif