#ifndef _TVT_SDK_VIDEO_H_
#define _TVT_SDK_VIDEO_H_

#include "tvt_sdk_product_param.h"
#include "sdk_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 控制命令枚举
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
    SDK_VIDEO_FORMAT            enFormat;      ///< 视频制式
    SDK_BLC_MODE_E              enWorkMode;    ///< Sensor工作模式
    TVT_SDK_PROD_SENSOR_S    stSensorCfg;   ///< Sensor信息
    int                         s32Resv[4];
} TVT_SDK_VIDEO_CFG_S;

typedef struct tvt_sdk_video_enc_cfg_s
{
    int                         s32StreamID;   ///< 通道ID
    SDK_VIDEO_ENCODE_TYPE       enEncType;     ///< 编码类型
    unsigned int                u32Width;      ///< 宽
    unsigned int                u32Height;     ///< 高
    unsigned int                u32FrameRate;  ///< 帧率
    SDK_VENC_MODE               enBitRateMode; ///< 码率模式
    unsigned int                u32BitRate;    ///< 码率
    unsigned int                u32GOP;        ///< GOP
    unsigned int                u32Profile;    ///< Profile
    unsigned char               u8RequestIDR;  ///< 请求I帧标识
} TVT_SDK_VIDEO_ENC_CFG_S;

typedef struct tvt_sdk_video_watermark_cfg_s
{
    unsigned char       u8Enable;       ///< 1：设置；0：清除
    char*               pszUserData;    ///< 水印内容
    int                 s32DataLens;    ///< 水印长度
} TVT_SDK_VIDEO_WATERMARK_CFG_S;
////////////////////////////////////////////////////////
////////////////////// OSD ////////////////////////////////
/**
 * @brief OSD序号
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
 * @brief OSD类型
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
    TVT_SDK_OSD_TIME_GREGORIAN = 0,     ///< 公历
    TVT_SDK_OSD_TIME_PERSIAN ,          ///< 波斯历
    TVT_SDK_OSD_TIME_CALENDAR_BUTT,
} TVT_SDK_OSD_TIME_CALENDAR_E;

typedef enum tvt_sdk_osd_time_format_e
{
    TVT_SDK_OSD_TIME_24HOUR_FORMAT = 0,     ///< 24小时制
    TVT_SDK_OSD_TIME_12HOUR_FORMAT,         ///< 12小时制
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
    TVT_SDK_OSD_DATE_CROSSBAR = 0,            ///< 横杆
    TVT_SDK_OSD_DATE_CROSSBAR_TO_CHAR,        ///< 横杆转中文：02-07-2021 -> 02年07日2021年
    TVT_SDK_OSD_DATE_CROSSBAR_TO_SLASH,       ///< 横杆转斜杆：02-07-2021 -> 02/07/2021
    TVT_SDK_OSD_DATE_FORMAT_EX_BUTT,
} TVT_SDK_OSD_DATE_FORMAT_EX_E;

typedef enum tvt_sdk_osd_size_e
{
    TVT_SDK_OSD_SIZE_AUTO = 0,      ///< 自适应
    TVT_SDK_OSD_SIZE_16X16,         ///< 16x16
    TVT_SDK_OSD_SIZE_32X32,         ///< 32x32
    TVT_SDK_OSD_SIZE_64X64,         ///< 64x64
    TVT_SDK_OSD_SIZE_BUTT,
} TVT_SDK_OSD_SIZE_E;


typedef enum tvt_sdk_osd_align_mode_e
{
    TVT_SDK_OSD_ALIGN_CUSTOM = 0, ///< 自定义排列
    TVT_SDK_OSD_ALIGN_LEFT,       ///< 左对齐
    TVT_SDK_OSD_ALIGN_RIGHT,      ///< 右对齐
    TVT_SDK_OSD_ALIGN_GB,         ///< 国标对齐
    TVT_SDK_OSD_ALIGN_BUTT,
} TVT_SDK_OSD_ALIGN_MODE_E;

/**
 * @brief OSD时间格式
 */
typedef struct tvt_sdk_osd_time_fmt_s
{
    unsigned int                        u32Enable;          ///< 显示
    unsigned int                        u32EnableWeek;      ///< 使能显示星期
    TVT_SDK_OSD_TIME_CALENDAR_E         enCalendar;         ///< 历法(阳历、波斯历)
    TVT_SDK_OSD_TIME_FORMAT_E           enTimeFormat;       ///< 时间格式
    TVT_SDK_OSD_DATE_FORMAT_E           enDateFormat;       ///< 日期格式
    TVT_SDK_OSD_DATE_FORMAT_EX_E        enDateFormatEx;     ///< 日期扩展格式
} TVT_SDK_OSD_TIME_FMT_S;


/**
 * @brief OSD Comm 格式
 */
typedef struct tvt_sdk_osd_comm_info_s
{
    unsigned int            u32Enable;     ///< 显示
    TVT_SDK_OSD_TYPE_E      enOSDType;     ///< OSD类型
    unsigned char*          pszData;       ///< 点阵数据/位图数据
} TVT_SDK_OSD_COMM_INFO_S;

/**
 * @brief OSD时间相关配置
 */
typedef struct tvt_sdk_osd_time_cfg_s
{
    unsigned int            s32VinIDX;     ///< 视频输入序号
    TVT_SDK_OSD_TIME_FMT_S  stTimeFmt;     ///< 时间格式
    SDK_POSITION            stPosition;    ///< 位置信息
} TVT_SDK_OSD_TIME_CFG_S;

/**
 * @brief Comm OSD相关配置
 */
typedef struct tvt_sdk_osd_comm_cfg_s
{
    unsigned int            s32VinIDX;     ///< 视频输入序号
    unsigned int            u32OSDIDX;     ///< 第几个OSD
    TVT_SDK_OSD_COMM_INFO_S stOSDInfo;     ///< 内容
    SDK_POSITION            stPosition;    ///< 位置信息
} TVT_SDK_OSD_COMM_CFG_S;


////////////////////////////////////////////////////////
////////////////////// MASK ////////////////////////////////
/**
 * @brief MASK相关配置
 */
typedef struct tvt_sdk_video_mask_cfg_s
{
    unsigned int            s32VinIDX;     ///< 视频输入序号
    unsigned int            u32Enable;     ///< 显示
    SDK_POSITION            stPosition;    ///< 位置信息
} TVT_SDK_VIDEO_MASK_CFG_S;

////////////////////////////////////////////////////////

////////////////////// ROI ////////////////////////////////
/**
 * @brief ROI 信息
 */
typedef struct tvt_sdk_video_roi_info_s
{
    unsigned int        u32Enable;      ///< 使能
    unsigned int        u32ROIIDX;      ///< 第几个ROI
    int                 s32Level;       ///< 级别
    SDK_POSITION        stPosition;     ///< 位置信息
} TVT_SDK_VIDEO_ROI_INFO_S;


/**
 * @brief ROI配置
 */
typedef struct tvt_sdk_video_roi_cfg_s
{
    unsigned int                s32VinIDX;     ///< 视频输入序号
    int                         s32ROINum;     ///< 配置的个数
    TVT_SDK_VIDEO_ROI_INFO_S*   pstROIInfo;
} TVT_SDK_VIDEO_ROI_CFG_S;

////////////////////////////////////////////////////////

/**
 * @brief 初始化视频业务
 * @param [in] pstVideoCfg:视频输入参数指针
 * @param [in] s32VinNum:视频输入个数
 * @return 0：成功；非0：失败
 */
int tvt_sdk_video_init(TVT_SDK_VIDEO_CFG_S* pstVideoCfg, int s32VinNum);

/**
 * @brief 去初始化视频业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_video_exit();

/**
 * @brief 视频业务控制接口
 * @param [in] s32Cmd:命令字
 * @param [in] pParam:参数指针
 * @param [in] s32Length:参数长度
 * @return 0：成功；非0：失败
 */
int tvt_sdk_video_ctrl(int s32Cmd, void* pParam, int s32Length);

/**
 * @brief 获取编码流
 * @return 0：成功；非0：失败
 */
int tvt_sdk_video_getEncodeStream(int s32StreamID, IPCAMERA_AV_FRAME_INFO* pstFrameInfo, int* ps32SLice, int* ps32SliceBuf, int s32SliceBufLen);

/**
 * @brief 释放编码流
 * @return 0：成功；非0：失败
 */
int tvt_sdk_video_releaseEncodeStream(IPCAMERA_AV_FRAME_INFO* pstFrameInfo);


/**
 * @brief 获取Jpeg缓存数据
 * @return 0：成功；非0：失败
 */
int tvt_sdk_video_getJpegBuffer(int s32StreamID, IPCAMERA_AV_FRAME_INFO* pstFrameInfo);



/// TODO:获取YUV原始数据

#ifdef __cplusplus
}
#endif

#endif