#ifndef SDK_DEFINE_H
#define SDK_DEFINE_H

#if !defined(SYS_USED_64ADDRESS) && !defined(SYS_USED_32ADDRESS)
#include "ipcamera.h"
#else

#pragma pack(4)
typedef unsigned long       DWORD;
typedef unsigned long       ULONG;

typedef long                LONG;
typedef unsigned long long  ULONGLONG;
typedef int                 TVT_BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef int                 INT;
typedef unsigned int        UINT;
typedef long long           LONGLONG;
typedef signed char         SBYTE;
typedef char                tchar, tint8;/////8bit signed
typedef unsigned char       tbyte, tuint8; ///////8bit unsigned
typedef short               tint16; ////16bit  signed
typedef unsigned short      tuint16; ////16bit  unsigned
typedef int                 tint32; /////32 bit  signed
typedef unsigned int        tuint32; /////32 bit unsigned
typedef long long           tint64; //////64bit  signed
typedef unsigned long long  tuint64; ////64bit  unsigned
typedef tuint32 tport32;

//ÊÓÆµÖÆÊ½¶¨Òå
typedef enum _video_format
{
    VIDEO_FORMAT_NTSC    = 0x00,
    VIDEO_FORMAT_PAL    = 0x01
} VIDEO_FORMAT;


typedef enum _dvr_video_size
{
    DVRVIDEO_SIZE_NONE  = 0x0000,           // NONE
    DVRVIDEO_SIZE_QCIF  = 0x0001,           // QCIF
    DVRVIDEO_SIZE_CIF   = 0x0002,           // CIF
    DVRVIDEO_SIZE_HD1   = 0x0003,           // HD1
    DVRVIDEO_SIZE_D1    = 0x0004,           // D1

    DVRVIDEO_SIZE_QQVGA = 0x0005,           // QQVGA
    DVRVIDEO_SIZE_QVGA  = 0x0006,           // QVGA
    DVRVIDEO_SIZE_VGA   = 0x0007,           // VGA
    DVRVIDEO_SIZE_XVGA  = 0x0008,           // XVGA 1280*960

    DVRVIDEO_SIZE_720P  = 0x0009,           // 1280*720
    DVRVIDEO_SIZE_1080P = 0x000A,           // 1920*1080

    DVRVIDEO_SIZE_UXVGA = 0x000B,           // UXVGA 1600*1200

    DVRVIDEO_SIZE_NVR_INNER = 0x000C,       // NVR inner use 640*480
    DVRVIDEO_SIZE_14M = 0x000D,             // 1280*1024 1.4M
    DVRVIDEO_SIZE_QXGA  = 0x000E,           // 2048*1536
    DVRVIDEO_SIZE_8M  = 0x000F,             // 3840*2160
    DVRVIDEO_SIZE_3M_16_9 = 0x0010,         // 2304*1296
    DVRVIDEO_SIZE_5M = 0x0011,              // 2592*1944
    DVRVIDEO_SIZE_4M = 0x0012,              // 2592*1520
    DVRVIDEO_SIZE_2K = 0x0013,              // 2560*1440
    DVRVIDEO_SIZE_2304X1296 = 0x0014,
    DVRVIDEO_SIZE_EXT_3M_NRT = 0x0015, //dvs 1920*1536

    DVRVIDEO_SIZE_2160X2160 = 0x0020,       // 2160*2160
    DVRVIDEO_SIZE_4K = 0x0021,              // 4096*2160
    DVRVIDEO_SIZE_QHD = 0x0022, // 960*540
    DVRVIDEO_SIZE_960X960 = 0x0023, // 960*960
    DVRVIDEO_SIZE_480X480 = 0x0024, // 480*480
    DVRVIDEO_SIZE_1920X1920 = 0x0025, //1920*1920
    DVRVIDEO_SIZE_1440X1440 = 0x0027, //1440*1440
    DVRVIDEO_SIZE_3072X2048 = 0x0028, //3072*2048

    DVRVIDEO_SIZE_CIF_N = 0x00c1,
    DVRVIDEO_SIZE_CIF_P = 0x00c2,
    DVRVIDEO_SIZE_D1_N = 0x00c3,
    DVRVIDEO_SIZE_D1_P = 0x00c4,

    DVRVIDEO_SIZE_3840X1680 = 0x00f0,
    DVRVIDEO_SIZE_2880X1264 = 0x00f1,
    DVRVIDEO_SIZE_1024X452 = 0x00f2,
    DVRVIDEO_SIZE_1280X560 = 0x00f3,
    DVRVIDEO_SIZE_1920X832 = 0x00f4,
    DVRVIDEO_SIZE_704X320 = 0x00f5,

    DVRVIDEO_SIZE_END = DVRVIDEO_SIZE_D1_P,

    DVRVIDEO_SIZE_4096X1800 = 0x100,
    DVRVIDEO_SIZE_4320X1920 = 0x101,

} DVR_VIDEO_SIZE;

//ucFrameType
typedef enum _ipcamera_frame_type
{
    IPC_FRAME_TYPE_VIDEO    = 0,
    IPC_FRAME_TYPE_AUDIO    = 1,
    IPC_FRAME_TYPE_PIC  = 2,
} IPCAMERA_FRAME_TYPE;

//ucEncodeType
typedef enum _ipcamera_encode_type
{
    IPC_VIDEO_ENCODE_TYPE_H264 = 0,
    IPC_VIDEO_ENCODE_TYPE_MGP4 = 1,
    IPC_VIDEO_ENCODE_TYPE_MJPEG = 2,
    IPC_VIDEO_ENCODE_TYPE_H265 = 3,
    IPC_AUDIO_ENCODE_TYPE_RAW = 32,
    IPC_AUDIO_ENCODE_TYPE_G726 = 33,
    IPC_AUDIO_ENCODE_TYPE_G711 = 34,  // means G711A
    IPC_AUDIO_ENCODE_TYPE_ADPCM = 35,
    IPC_AUDIO_ENCODE_TYPE_G711U = 36,
    IPC_AUDIO_ENCODE_TYPE_LPCM = 37,  // LPCM
    IPC_AUDIO_ENCODE_TYPE_AAC = 38,  // AAC
    IPC_PIC_ENCODE_TYPE_JPG = 64,
    IPC_PIC_ENCODE_TYPE_BMP = 65,
    IPC_VIDEO_ENCODE_TYPE_YUV = 0xFF, // get SDK YUV data
} IPCAMERA_ENCODE_TYPE;

struct IPCAMERA_FRAME_INFO
{
    //ÉùÒô  ÊÓÆµ  Í¼Æ¬    IPCAMERA_FRAME_TYPE_LIVE_VIDEO
    tuint8        ucFrameType;
    //±àÂë·½Ê½£¬ÉùÒô£¬ÊÓÆµ£¬Í¼Æ¬µÄ¶¼²»Ò»Ñù
    tuint8        ucEncodeType;
    //Á÷ÐòºÅ£¬´Ó0¿ªÊ¼
    tuint8        ucStreamIndex;
    //0 PÖ¡  1 IÖ¡  2 ÐéÄâIÖ¡
    tuint8        ucKeyFrame;

    //Ö¡ÐòºÅ£¬Ò»Ö±µÝÔö£¬¼Óµ½Òç³öÔÙ´Ó0¿ªÊ¼µÝÔö
    tuint32        frameIndex;

    tuint32       width;
    tuint32       height;

    //IPCameraµÄ±¾µØÊ±¼ä£¨¼´IPCamera±¾µØÄ³Ê±ÇøµÄÊ±·ÖÃë£©£¬¿Í»§¶ËÓÃ´¦ÀíUTCÊ±¼äµÄº¯Êý´¦Àí£¬µÃµ½µÄÊ±·ÖÃëÖ±½ÓÏÔÊ¾£¬
    tint64            time;    /* Î¢Ãë¼¶ */

    //Ïà¶ÔÊ±¼ä£¬ÔÚ¸Ä±äÉè±¸Ê±¼äÊ±²»»á±ä»¯£¬ÊÇÒ»Ö±µÝÔöµÄÊ±¼ä
    tint64            relativeTime;    /* Î¢Ãë¼¶ */

    tuint32        length;
    tuint8      *pData;
};    //ÓÃÓÚ±¾µØÊ¹ÓÃ

#pragma pack()
#endif

typedef struct IPCAMERA_FRAME_INFO IPCAMERA_AV_FRAME_INFO;

//Æô¶¯AFÐ£×¼ºÍ¼ì²éÐ£×¼×´Ì¬ÃüÁîºê
typedef enum _sdk_lens_calibration_cmd
{
    START_CALIB = 0X2,
    CHECK_CALIB = 0X3,
} SDK_LENS_CALIBRATION_CMD;

//AZ¾µÍ·Ð£×¼·µ»ØÖµ
typedef enum _sdk_lens_calibration_result
{
    AF_LENS_CALIB_OK  = 0x0,                    //AFÐ£ÕýÍê±Ï¹æ¸ñÅÐ¶¨È«OK
    AF_LENS_CALIB_RUNNING = 0x10,           //AFÐ£ÕýÖ´ÐÐÖÐ
    AF_LENS_CALIB_NONEXEC = 0x20,       /* AFÐ£ÕýÎ´Ö´ÐÐ*/
    AF_LENS_CALIB_SENSOR_NG = 0x40,         //AFÐ£ÕýÇãÐ±¹æ¸ñÅÐ¶¨NG
    AF_LENS_CALIB_CURVE_WIDE_NG = 0x41,     //AFÐ£ÕýÇúÏß¹æ¸ñÅÐ¶¨NG(WIDE)
    AF_LENS_CALIB_CURVE_TELE_NG = 0x42,     //AFÐ£ÕýÇúÏß¹æ¸ñÅÐ¶¨NG(TELE)
    AF_LENS_CALIB_CURVE_LEFT_NG = 0x43,     //AFÐ£ÕýÇúÏß¹æ¸ñÅÐ¶¨NG(×ó±ß½ç³¬¹æ)
    AF_LENS_CALIB_CURVE_RIGHT_NG = 0x44,        //AFÐ£ÕýÇúÏß¹æ¸ñÅÐ¶¨NG(ÓÒ±ß½ç³¬¹æ)
} SDK_LENS_CALIBRATION_RESULT;


typedef enum _sdk_ret
{
    SDK_RET_SUCCESS = 0,
    SDK_RET_SYS_NOT_INIT = 1,
    SDK_RET_FAIL = 0xFFFF,
} SDK_RET;

//Æô¶¯AF³õÊ¼»¯ºÍ¼ì²éÐ£×¼×´Ì¬ÃüÁîºê
typedef enum _sdk_lens_init_cmd
{
    START_INIT = 0X2,
    CHECK_INIT = 0X3,
} SDK_LENS_INIT_CMD;

//AZ¾µÍ·³õÊ¼»¯·µ»ØÖµ
typedef enum _sdk_lens_init_result
{
    AF_LENS_INIT_RUNNING = 0x0,                    //AF³õÊ¼»¯Íê±Ï¹æ¸ñÅÐ¶¨È«OK
    AF_LENS_INIT_OK = 0x1,         //AF³õÊ¼»¯Ö´ÐÐÖÐ
} SDK_LENS_INIT_RESULT;


typedef enum _sdk_vidoe_format
{
    SDK_VIDEO_INPUT_PAL = 0,
    SDK_VIDEO_INPUT_NTSC = 1,
    SDK_VIDEO_INPUT_BUTT,
} SDK_VIDEO_FORMAT;

typedef enum _sdk_venc_mode
{
    SDK_VENC_MODE_CBR = 0,
    SDK_VENC_MODE_VBR = 1,
    SDK_VENC_MODE_BUTT,
} SDK_VENC_MODE;

typedef enum _sdk_venc_profile
{
    SDK_VENC_PROFILE_BASELINE = 0,
    SDK_VENC_PROFILE_MAIN = 1,
    SDK_VENC_PROFILE_HIGH = 2,
} SDK_VENC_PROFILE;

typedef enum _time_display_format
{
    year_mon_day = 1, // Äê/ÔÂ/ÈÕ
    mon_day_year,
    day_mon_year,
    year_mon_day_line, // Äê-ÔÂ-ÈÕ
    mon_day_year_line,
    day_mon_year_line
} time_dispaly_format;

typedef enum _sdk_audio_rate
{
    SDK_AUDIO_RATE_8000   = 8000,
    SDK_AUDIO_RATE_12000  = 12000,
    SDK_AUDIO_RATE_11025  = 11025,
    SDK_AUDIO_RATE_16000  = 16000,
    SDK_AUDIO_RATE_22050  = 22050,
    SDK_AUDIO_RATE_24000  = 24000,
    SDK_AUDIO_RATE_32000  = 32000,
    SDK_AUDIO_RATE_44100  = 44100,
    SDK_AUDIO_RATE_48000  = 48000,
    SDK_AUDIO_RATE_BUTT,
} SDK_AUDIO_RATE;

typedef enum _sdk_audio_bit_width
{
    SDK_AUDIO_BIT_WIDTH_8   = 0,
    SDK_AUDIO_BIT_WIDTH_16  = 1,
    SDK_AUDIO_BIT_WIDTH_32  = 2,
    SDK_AUDIO_BIT_WIDTH_BUTT,
} SDK_AUDIO_BIT_WIDTH;

typedef enum _sdk_audio_encode_type
{
    SDK_AENC_TYPE_BOTTOM = 0,
    SDK_AENC_TYPE_G726 = 1,
    SDK_AENC_TYPE_G711A = 2,
    SDK_AENC_TYPE_G711U = 3,
    SDK_AENC_TYPE_ADPCM = 4,
    SDK_AENC_TYPE_AAC = 5,
} SDK_AUDIO_ENCODE_TYPE;

typedef enum _SDK_AIO_SOUND_MODE_E
{
    SDK_AUDIO_SOUND_MODE_MONO   = 0, /*mono*/
    SDK_AUDIO_SOUND_MODE_STEREO = 1, /*stereo*/
    SDK_AUDIO_SOUND_MODE_BUTT
} SDK_AUDIO_SOUND_MODE_E;

typedef enum
{
    SDK_GET_AUDIO_IN_SUPPORT = 0,
    SDK_GET_AUDIO_OUT_SUPPORT,
    SDK_SET_AUDIO_IN_PARAM,
    SDK_SET_AUDIO_OUT_PARAM,
    SDK_SET_AUDIO_IN_START,
    SDK_SET_AUDIO_OUT_START,
    SDK_SET_AUDIO_IN_STOP,
    SDK_SET_AUDIO_OUT_STOP,
    SDK_SET_AUDIO_VOLUME,

    SDK_AUDIO_CTRL_BUTT
} SDK_AUDIO_CTRL_E;

typedef enum _SDK_ADEC_MODE_E
{
    SDK_ADEC_MODE_PACK = 0,
    SDK_ADEC_MODE_STREAM ,
    SDK_ADEC_MODE_BUTT
} SDK_ADEC_MODE_E;


#define MAX_AUDIO_RATE_TYPE  9
typedef struct _sdk_support_audio_param
{
    SDK_AUDIO_ENCODE_TYPE  audioEncodeType;
    SDK_AUDIO_SOUND_MODE_E audioMode;
    unsigned int maxPtNumPerFrm;
    SDK_AUDIO_BIT_WIDTH     audioBitWidthList[SDK_AUDIO_BIT_WIDTH_BUTT];
    SDK_AUDIO_RATE   audioRateList[MAX_AUDIO_RATE_TYPE];
    unsigned int reserve[4];
} SDK_SUPPORT_AUDIO_PARAM;

typedef struct _sdk_audio_param
{
    SDK_AUDIO_ENCODE_TYPE  audioEncodeType;
    SDK_AUDIO_SOUND_MODE_E audioMode;
    unsigned int ptNumPerFrm;
    SDK_AUDIO_BIT_WIDTH     audioBitWidth;
    SDK_AUDIO_RATE   audioRate;
    SDK_ADEC_MODE_E adecMode;
    unsigned int reserve[4];
} SDK_AUDIO_PARAM;

typedef enum _sdk_video_encode_type
{
    SDK_VENC_TYPE_BOTTOM = 0,
    SDK_VENC_TYPE_H264 = 1,
    SDK_VENC_TYPE_JPEG = 2,
    SDK_VENC_TYPE_H265 = 3,
    SDK_VENC_TYPE_MJPEG = 4,
    SDK_VENC_TYPE_H264_PLUS = 0x11,
    SDK_VENC_TYPE_H265_PLUS = 0x13,
    SDK_VENC_TYPE_H264_SMART = 0x101,
    SDK_VENC_TYPE_H265_SMART = 0x103,
} SDK_VIDEO_ENCODE_TYPE;

typedef enum _sdk_video_rotate
{
    SDK_VIVOD_0 = 0,
    SDK_VIVOD_90 = 1,
    SDK_VIVOD_180 = 2,
    SDK_VIVOD_270 = 3,
} SDK_VIDEO_ROTATE;

typedef enum _sdk_daynight_state
{
    SDK_DAYNIGHT_BOTTOM = 0,
    SDK_DAYNIGHT_DAY = 1,
    SDK_DAYNIGHT_NIGHT = 2,
    SDK_DAYNIGHT_AUTO = 3,  //  ZJ
    SDK_DAYNIGHT_TIMER = 4, //  ZJ
    SDK_DAYNIGHT_ANTIFOG    =   5,//optical anti-flog
} SDK_DAYNIGHT_STATE;

typedef enum _sdk_default_state
{
    SDK_DEFAULT_STATE_BOTTOM = 0,
    SDK_DEFAULT_STATE_RESET = 1,
    SDK_DEFAULT_STATE_NORMAL = 2,
} SDK_DEFAULT_STATE;

typedef enum _sdk_exception_mode
{
    SDK_EXCEPTION_SCENECHANGE = 0,
    SDK_EXCEPTION_BLUR,
    SDK_EXCEPTION_VEDIOCOLOR,
    SDK_EXCEPTION_BUTT,
} SDK_EXCEPTION_MODE;

typedef struct _sdk_3DNR
{
    unsigned char ucMode;// 0 ¹Ø±Õ 1 µÍ2ÖÐ3¸ß
} SDK_3DNR;

typedef struct _sdk_shutter_speed_list
{
    int size;

    int shutter_speed[30];
    int shutter_size;
    int default_speed;

    int set_mode; // 0 : autio 1 : ÊÖ¶¯
    int set_auto_min;
    int set_auto_max;
    int set_manual_val; // µ±Ç°ÊýÖµ

    int iManualShutterDefVal;
    int iAutoShutterDefMinVal;
    int iAutoShutterDefMaxVal;

    int resv[1];
} SDK_SHUTTER_SPEED_LIST;

typedef struct _sdk_iris_number_list
{
    int size;
    int iris_type;// 0: ¹Ì¶¨¹âÈ¦,1:DC-IRIS,2:PIRIS
    int iris_number[30];
    int iris_size;
    int default_iris_number;

    int set_mode; // Ä£Ê½ 0 : autio 1 : ÊÖ¶¯
    int set_iris_val; // µ±Ç°ÊýÖµ

    int resv[4];
} SDK_IRIS_NUMBER_LIST;

typedef struct _sdk_gain_range
{
    int size;
    int min;
    int max;

    int set_mode; // Ä£Ê½ 0 : autio 1 : ÊÖ¶¯
    int set_min; //
    int set_max;
    int set_cur_val; // µ±Ç°ÊýÖµ

    int iManualGainDefVal;  // ÊÖ¶¯ÔöÒæÄ£Ê½ÏÂÔöÒæÄ¬ÈÏÖµ
    int iAutoGainDefMaxVal; // ×Ô¶¯ÔöÒæÄ£Ê½ÏÂ×î´óÔöÒæÏÞÖÆÄ¬ÈÏÖµ

    int resv[8];
} SDK_GAIN_RANGE;

typedef enum  _sdk_ev_status_e
{
    SDK_EV_LEVEL_LOW = 0,
    SDK_EV_LEVEL_MIDDLE,
    SDK_EV_LEVEL_HIGH,
    SDK_EV_BLACK_LIGHT,
    SDK_EV_ERROR,
} SDK_EV_STATUS_E;

typedef struct _sdk_get_again_val
{
    int retval;
    float val;
    SDK_EV_STATUS_E ev_status;
} SDK_GET_AGAIN_VAL;

typedef struct _sdk_image_over_exposure
{
    int s32Enable;      // 1:¿ªÆô;0:¹Ø±Õ
    int s32Resv[7];     // ±£Áô
} SDK_IMAGE_OVER_EXPOSURE_S;


typedef enum _sdk_image_type_para
{
    SDK_IMAGE_BOTTOM = 0,
    SDK_IMAGE_BRIGHT = 1,       // int
    SDK_IMAGE_HUE = 2,          // int
    SDK_IMAGE_SATURATION = 3,  // int
    SDK_IMAGE_CONTRAST = 4,    // int
    SDK_IMAGE_MIRROR = 5,      // int 0 off ,1 on
    SDK_IMAGE_FLIP = 6,        // int 0 off ,1 on
    SDK_IMAGE_WHITE_BALANCE = 7, // SDK_WhiteBalance
    SDK_IMAGE_DYNAMIC_RANGE = 8, // SDK_dynamicRange  digital dynamic range
    SDK_IMAGE_DENOISE = 9,      // SDK_DENOISE
    SDK_IMAGE_SHARPEN = 10,      // SDK_SHARPEN
    SDK_IMAGE_IRISTYPE = 11,    // SDK_IRISTYPE
    SDK_IMAGE_ROTATE = 12,      // SDK_VIDEO_ROTATE
    SDK_IMAGE_EXPOSE_TYPE = 13, // SDK_ISP_EXPOSE_MODE
    SDK_IMAGE_EXPOSE_TIME = 14,
    SDK_IMAGE_LDC = 15,  //  SDK_LDC
    SDK_IMAGE_ANTI_FOG = 16, //  SDK_ANTI_FOG
    SDK_IMAGE_DEFECT_PIXEL = 17,
    SDK_IMAGE_NIGHT_HIGH_PARAM = 18, //  SDK_NIGHT_HIGH_PARA
    SDK_IMAGE_PHYSICAL_DYNAMIC_RANGE = 19, // int,0 off,1 on; physical dynamic range
    SDK_IMAGE_ANTIFLICKER = 20,  // anti flicker range

    /*  ÖÇÄÜºìÍâ,ÐèÒªÓ²¼þÖ§³Ö;  Èí¼þÖ§³ÖÓÉ ÅäÖÃÎÄ¼þ¿ØÖÆ,
        AP¶ÁÈ¡ÅäÖÃºó, ÔÙ¸æËßSDK; Õâ¸ö½Ó¿ÚÐèÒªÔÚSDKInitÇ°µ÷ÓÃ;
    */
    SDK_IMAGE_SMARTIR_SUPPORT = 21,
    SDK_IMAGE_SMARTIR_OPEN = 22,  //¶¯Ì¬¿ª¹ØsmartIR

    /*
        Ö§³Ö¹¤×÷Ä£Ê½:  0 ÆÕÍ¨Ä£Ê½; 1 WDRÄ£Ê½; 2 ¸ßÖ¡ÂÊÄ£Ê½;
        3 HLC Ä£Ê½; 4 BLCÄ£Ê½.
        ÆäÖÐ¸ßÖ¡ÂÊÄ£Ê½²»ÄÜ¶¯Ì¬ÉèÖÃ(·Ö±æÂÊ»áÓÐ±ä»¯),ÐèÒªÖØÆô.
        ±¾º¯ÊýÔÚµ÷ÓÃSDKInit , SDKGetVEncSupportº¯ÊýÖ®Ç°µ÷ÓÃÒ»´Î;
        ³õÊ¼»¯ºó,¿ÉÒÔ¶¯Ì¬ÇÐ»»(¸ßÖ¡ÂÊÄ£Ê½³ýÍâ).
    */
    SDK_IMAGE_ISP_WORK_MODE = 23,
    SDK_IMAGE_AF_REVISE = 24,

    SDK_IMAGE_BLC         = 25, // int=0 ON:1,OFF:0
    SDK_IMAGE_LENSSHADING = 26,
    SDK_IMAGE_3DNR        = 27,
    SDK_IMAGE_LENS_RESET = 28,
    SDK_IMAGE_GAIN_RANGE = 29,
    SDK_IMAGE_EXPOSE_RANGE = 30,
    SDK_IMAGE_HLC   =   31, //»úÐ¾ÁÙÊ±Ê¹ÓÃ

    SDK_IMAGE_V2_EFFECT = 32,
    SDK_IMAGE_V2_EXPORSUR = 33,
    SDK_IMAGE_V2_DAYNIGHT_SET = 34,
    SDK_IMAGE_V2_EXPORSURE_COMPEN = 35,
    SDK_IMAGE_V2_WHITE_BALENCE = 36,
    SDK_IMAGE_V2_DENOISE = 37,
    SDK_IMAGE_V2_FEATUR_FUNCTION = 38,
    SDK_IMAGE_V2_STATUS = 39,

    SDK_IR_DOME_MODE        =    41,  //OFF=0,ON=1

    SDK_IMAGE_FACE_TONE_MODE = 100,
    SDK_IMAGE_FACE_EXP_CFG = 101,

    SDK_IMAGE_GET_SHUTTER_SPEED = 1000,
    SDK_IMAGE_SET_SHUTTER_SPEED = 1001,
    SDK_IMAGE_GET_IRIS_NUMBER = 1002,
    SDK_IMAGE_SET_IRIS_NUMBER = 1003,
    SDK_IMAGE_GET_GAIN_RANGE = 1004,
    SDK_IMAGE_SET_GAIN_RANGE = 1005,

    SDK_IMAGE_HW_SMARTIR_CFG = 2000,

    SDK_IMAGE_WHITE_LIGHT_CFG = 3000,

    SDK_IMAGE_BACK_LIGHT_CTRL = 4000,

    SDK_IMAGE_OVER_EXPOSURE_CTRL = 5000, // ·À²¹¹â¹ýÆØ,¶ÔÓ¦½á¹¹Ìå SDK_IMAGE_OVER_EXPOSURE_S

    SDK_IMAGE_GET_CALIB_VALUE = 9001,

    SDK_IMAGE_GET_EV_STATUS = 9901,
} SDK_IMAGE_TYEP_PARA;
typedef enum _sdk_image_v2_status
{
    SDK_IMAGE_V2_STATUS_NULL = 0,
    SDK_IMAGE_V2_STATUS_INIT = 1,
    SDK_IMAGE_V2_STATUS_EXIT = 2,

    SDK_IMAGE_V2_STATUS_BUTT,
} SDK_IMAGE_V2_STATUS_E;

typedef enum _sdk_isp_expose_mode
{
    SDK_ISP_EXPOSE_MODE_AUTO = 0,
    SDK_ISP_EXPOSE_MODE_MANUAL = 1,
} SDK_ISP_EXPOSE_MODE;

typedef struct _sdk_isp_expose_time
{
    int mintime;
    int maxtime;
} SDK_ISP_EXPOSE_TIME;

typedef enum _sdk_security
{
    SDK_SECURITY_SUCCESS = 0,
    SDK_SECURITY_FAIL = 1,
} SDK_SECURITY;

typedef enum _sdk_alarm_out
{
    SDK_ALARM_OUT_CLOSE = 0,
    SDK_ALARM_OUT_OPEN = 1,
} SDK_ALARM_OUT;

typedef struct _sdk_multi_alarm_out
{
    int                 iChannel;
    SDK_ALARM_OUT       iOutType;
} SDK_MULTI_ALARM_OUT;

typedef enum _sdk_alarm_in
{
    SDK_ALARM_IN_LOW = 0,
    SDK_ALARM_IN_HIGH = 1,
} SDK_ALARM_IN;

typedef enum _sdk_sd_card
{
    SDK_SD_CARD_NO_CARD = 0,
    SDK_SD_CARD_READ_ONLY = 1,
    SDK_SD_CARD_READ_WRITE = 2,
} SDK_SD_CARD;

typedef enum _sdk_audio_input
{
    SDK_AUDIO_INPUT_MIC = 0,
    SDK_AUDIO_INPUT_LIN = 1,
} SDK_AUDIO_INPUT;

typedef struct _sdk_audio_volume
{
    int inputLineInVolume;
    int inputMicInVolume;
    int ouputVolume;
} SDK_AUDIO_VOLUME;

typedef enum _sdk_get_device_type_para
{
    SDK_DEVICE_GET_BOTTOM = 0,
    SDK_DEVICE_GET_DEFAULT = 1,
    SDK_DEVICE_GET_LIGHT = 2,
    SDK_DEVICE_GET_CDS = 3,
    SDK_DEVICE_GET_RTC_TIME = 4,    // SDK_RTC_TIME
    SDK_DEVICE_GET_UART_DATA = 5, // normal ptz
    SDK_DEVICE_GET_SECURITY = 6,
    SDK_DEVICE_GET_ALARM_IN = 7,
    SDK_DEVICE_GET_SDCARD = 8,
    SDK_DEVICE_GET_TVT_UART_DATA = 9, // tvt ptz
    SDK_DEVICE_GET_TEMPERTURE = 10, // get temperture
    SDK_DEVICE_GET_IS_AR0130_LENS = 11, //
    SDK_DEVICE_GET_DVS_STATUS = 12,
    SDK_DEVICE_GET_DN_STATUS = 13,
    SDK_DEVICE_GET_SDK_VERSION = 14,
    SDK_DEVICE_GET_MOTION_DESCRIBE = 15,
    SDK_DEVICE_GET_PIR_STATUS = 16,  //get pir status
    SDK_DEVICE_GET_WIEGAND_DATA = 17,
    SDK_DEVICE_GET_USB_DISK_INFO = 18,
    SDK_DEVICE_GET_EXCEPTION_STATUS = 19,
    SDK_DEVICE_GET_OBJECTREMOVE_STATUS = 22,
    SDK_DEVICE_GET_AI_MODE = 50,
    SDK_DEVICE_GET_SEC_CODE = 71,
    SDK_DEVICE_GET_SDCARD_USE_PARTITIONS = 88,
    SDK_DEVICE_GET_BODY_TEMPT = 90,
    SDK_DEVICE_GET_BOARD_INFO = 95,

    SDK_DEVICE_GET_ABF_VALUE = 1001,
    SDK_DEVICE_GET_DOOR_KEY_VALUE,
    SDK_DEVICE_GET_TAMPER_VALUE,
    SDK_DEVICE_GET_GATE_MAGNETISM_VALUE,

    SDK_DEVICE_GET_AVS_PIPE_JPG = 2001,  //get avs pipe jpg

    SDK_DEVICE_GET_DALI_UART_DATA = 2002, //»ñÈ¡´óÁ¦Ç°¶ËÍ¨ÐÅÊý¾Ý
    SDK_DEVICE_GET_5236E_UART_DATA = 2003,  //ÈÈ³ÉÏñ5236E
} SDK_GET_DEVICE_TYPE_PARA;

typedef enum  _sdk_wiegand_bit_mode
{
    SDK_WG_26BIT_MODE = 26,
    SDK_WG_34BIT_MODE = 34,
    SDK_WG_37BIT_MODE = 37,
    SDK_WG_42BIT_MODE = 42,
    SDK_WG_46BIT_MODE = 46,
    SDK_WG_58BIT_MODE = 58,
    SDK_WG_66BIT_MODE = 66,

    SDK_WG_MODE_BUTT,
} SDK_WG_BIT_MODE;

typedef enum _sdk_set_device_type_para
{
    SDK_DEVICE_SET_BOTTOM = 0,
    SDK_DEVICE_SET_IRCUT = 1,
    SDK_DEVICE_SET_INFRARED = 2,
    SDK_DEVICE_SET_RTC_TIME = 3,    // SDK_RTC_TIME
    SDK_DEVICE_SET_UART_ATTR = 4,
    SDK_DEVICE_SET_UART_DATA = 5,
    SDK_DEVICE_SET_ALARM_OUT = 6,
    SDK_DEVICE_SET_WDG_START = 7,  //  timeout secords
    SDK_DEVICE_SET_WDG_KEEP = 8,
    SDK_DEVICE_SET_WDG_STOP = 9,
    SDK_DEVICE_SET_AI_MODE = 10, // SDK_AUDIO_INPUT : mic in, lin in
    SDK_DEVICE_SET_TVT_UART_ATTR = 11,
    SDK_DEVICE_SET_TVT_UART_DATA = 12,
    SDK_DEVICE_SET_PRODUCT_ID = 13,
    SDK_DEVICE_SET_PRODUCT_SUB_ID = 14,
    SDK_DEVICE_SET_CUSTOM_ID = 15,
    SDK_DEVICE_SET_CUSTOM_SUB_ID = 16,
    SDK_DEVICE_SET_LENS_TYPE = 17,
    SDK_DEVICE_SET_VOLUME = 18,
    SDK_DEVICE_SET_LENS_MULTIPLE = 19,
    SDK_DEVICE_SET_DVS_STATUS = 20,
    SDK_DEVICE_SET_SPECIAL_DAY_NIGHT_MODE = 21,
    SDK_DEVICE_SET_PRODUCT_TYPE = 22,      //device type
    SDK_DEVICE_SET_SUPPORT_AUDIO_TYPE = 23, //SDK_AI_CFG_S
    SDK_DEVICE_SET_SUPPORT_ZOOM_BUTTON = 24,
    SDK_DEVICE_SET_MAX_VIDEO_SIZE_CFG = 25,
    SDK_DEVICE_SET_MODEL_ID = 26,
    SDK_DEVICE_SET_MODEL_LENS = 27,
    SDK_DEVICE_SET_MULTI_ALARM_OUT = 28,
    SDK_DEVICE_SET_WIEGAND_TRANSPORT_MODE = 29,
    SDK_DEVICE_SET_WIEGAND_DATA = 30,
    SDK_DEVICE_SET_OSD_EXT_ATTRIB = 31,
    SDK_DEVICE_SET_ALARM_LIGHT = 32,
    SDK_DEVICE_SET_SD_CARD_LIGHT = 33,
    SDK_DEVICE_SET_PHY_LIGHT = 34,
    SDK_DEVICE_SET_DOME_TYPE = 51,
    SDK_DEVICE_SET_DOME_START = 52,
    SDK_DEVICE_SET_AUDIO_HORN_OUT = 53,
    SDK_DEVICE_SET_ALARM_WHITE_LIGHT = 54,

    SDK_DEVICE_SET_WIEGAND_CFG = 60,

    SDK_DEVICE_SET_SEC_CODE = 70,  //ï¿½ï¿½ï¿½Ã°ï¿½È«ï¿½ï¿½
    SDK_DEVICE_SET_HOURS_MODE = 81,  // ï¿½ï¿½ï¿½ï¿½Ê±ï¿½ï¿½ï¿½ï¿½Ê¾Ä£Ê½: 1 - 12Ð¡Ê±ï¿½ï¿½; 0 - 24Ð¡Ê±ï¿½ï¿½
    SDK_DEVICE_SET_AI_SNAP_JPEG_SIZE_MODE = 82, //ï¿½ï¿½ï¿½ï¿½AI×¥Í¼ï¿½ï¿½Ð¡Ä£Ê½: 1 ï¿½ï¿½sensorï¿½ï¿½ï¿½ï¿½ï¿½Ð¡×¥Í¼;0 ï¿½ï¿½1080Pï¿½ï¿½Ð¡×¥Í¼

    SDK_DEVICE_SET_SUPPORT_480P = 100,
    SDK_DEVICE_SET_BITRATE_UNLIMIT = 101,

    SDK_DEVICE_SET_AVS_PIPE_STATUS = 1000,   //AVS LUT status

    SDK_DEVICE_SET_AVS_PIPE_READY = 1001,  //×¼±¸ÉÏ´«±ê¶¨ÎÄ¼þÐ­Òé
    SDK_DEVICE_SET_NO_SPLICE_MODE = 1002,  //ÇÐ»»Îª·ÇÆ´½ÓÄ£Ê½Ð­Òé

    SDK_DEVICE_SET_DOOR_LOCK_VALUE = 2001,
    SDK_DEVICE_SET_DEV_INFRARED_STRENGTH = 2002,
    SDK_DEVICE_SET_WRITE_IMAGE_LOGO = 2003,
    SDK_DEVICE_SET_VO_SHOW_IMAGE = 2004,
    SDK_DEVICE_SET_DEC_FILE_DATA = 2005,
    SDK_DEVICE_SET_IMAGE_EXCHANG_BLACK_COLOR = 2006,
    SDK_DEVICE_SET_BLACK_IMAGE_SECOND = 2007,

    SDK_DEVICE_SET_DALI_UART_ATTR = 3001,   //ÉèÖÃ´óÁ¢Ç°¶ËÍ¨ÐÅÊôÐÔ
    SDK_DEVICE_SET_DALI_UART_DATA = 3002,   //ÉèÖÃ´óÁ¦Ç°¶ËÍ¨ÐÅÊý¾Ý
    SDK_DEVICE_SET_5236E_UART_ATTR = 3003,  //ÈÈ³ÉÏñ5236E
    SDK_DEVICE_SET_5236E_UART_DATA = 3004,  //ÈÈ³ÉÏñ5236E
    SDK_DEVICE_SET_IR_POW_EN = 3005,  //ºìÍâÄ£×é¿ª¹Ø
} SDK_SET_DEVICE_TYPE_PARA;

typedef enum DOME_TPE
{
    TD_9632E2_B20IM2_AR15 = 0,
    TD_9632E2_B30IM_AR15  = 1 ,
    TD_8523IE_20IM_AR15     = 2,
    TD_8523IE_30IM_AR15     = 3,
    TD_9622E2_B18IS2_AR10  = 4,
    TD_9632M2_B20IM2_AR15 = 5,
    TD_9632M2_B30IM_AR15  = 6,
    TD_8623IM_20IM_AR15     = 7,
    TD_8623IM_30IM_AR15     = 8,
    TD_8623IM_20IM_VL30      = 9,
    TD_8623IM_30IM_VL30      = 10,
    TD_9637E2_B20IM2_AR3    = 11,
    TD_9627E2_B20IM2_AR3    = 12,
    TD_9627E2_B18IS2_AR3     = 13,
    TD_9649E2_B03IM_AR2      = 14,
    TD_8423IS_20IM_AR15       = 15,
    TD_88531IM_31M_VL50      = 16,
} SDK_DOME_TYPE_VALUE;

typedef enum _sdk_uart_rate
{
    SDK_UART_RATE_110 = 0,
    SDK_UART_RATE_300 = 1,
    SDK_UART_RATE_600 = 2,
    SDK_UART_RATE_1200 = 3,
    SDK_UART_RATE_2400 = 4,
    SDK_UART_RATE_4800 = 5,
    SDK_UART_RATE_9600 = 6,
    SDK_UART_RATE_19200 = 7,
    SDK_UART_RATE_38400 = 8,
    SDK_UART_RATE_57600 = 9,
    SDK_UART_RATE_115200 = 10,
} SDK_UART_RATE;

typedef enum _sdk_baud_rate
{
    SDK_BAUD_RATE_110 = 0,
    SDK_BAUD_RATE_300 = 1,
    SDK_BAUD_RATE_600 = 2,
    SDK_BAUD_RATE_1200 = 3,
    SDK_BAUD_RATE_2400 = 4,
    SDK_BAUD_RATE_4800 = 5,
    SDK_BAUD_RATE_9600 = 6,
    SDK_BAUD_RATE_19200 = 7,
    SDK_BAUD_RATE_38400 = 8,
    SDK_BAUD_RATE_57600 = 9,
    SDK_BAUD_RATE_115200 = 10,
} SDK_BAUD_RATE;

typedef struct _sdk_position
{
    int x;
    int y;
    int width;
    int height;
} SDK_POSITION;

typedef struct _sdk_venc_support_para
{
    int iWidth;
    int iHeight;
    int iMaxFrameRate;
    int iMaxBitRate;
} SDK_VENC_SUPPORT_PARA;


typedef struct _sdk_night_high_param
{
    unsigned int again;
    unsigned int dgain;
    unsigned int dns2d;
    unsigned int dns3d;
} SDK_NIGHT_HIGH_PARAM;

typedef struct _sdk_osd_info_config
{
    unsigned int    uShowOSDFlag;     //  1 show, 0 hide
    int             iStartX;                   //  ÆðÊ¼Î»ÖÃxµÄ×ø±ê
    int             iStartY;                   //  ÆðÊ¼Î»ÖÃyµÄ×ø±ê
    unsigned char   ucOSDBuf[64];    //  gb2312±àÂëºóµÄÄÚÈÝ
    unsigned int    uDataLen;         //  ucOSDBuf µÄ³¤¶È£¬²»ÄÜ³¬¹ý30,Ò²¾ÍÊÇ²»ÄÜ³¬¹ý15¸ö×Ö·û
    unsigned int    uShowLevel;      //  ÏÔÊ¾µÈ¼¶, 0 low, 1 middle, 2 high
    unsigned int    uFlicker;        //  ÊÇ·ñÉÁË¸, 0  not flicker, 1 flicker
} SDK_OSD_INFO_CONFIG;

// Í¸Ã÷¶È - hisi not support.
typedef enum _sdk_FOSD_TRANSPARENT_e
{
    SDK_FOSD_TRANSPARENT_PERCENT_0,
    SDK_FOSD_TRANSPARENT_PERCENT_50,
    SDK_FOSD_TRANSPARENT_PERCENT_75,
    SDK_FOSD_TRANSPARENT_PERCENT_100,
} SDK_FOSD_TRANSPARENT_e;

typedef struct _sdk_osd_mask_config
{
    int           iShow;
    SDK_POSITION  rectang;
    unsigned int  color; // 0x00RRGGBB - RGB need turing to YUV on GM soc.
    SDK_FOSD_TRANSPARENT_e transparent;
    //wyf 2017-7-25
    int RotateValue;    //Ðý×ª½Ç¶È 0,90,180,270
    int mirror; //¾µÏñ
    int flip;   //·­×ª
    unsigned int iCurStep_h;    //ÔÆÌ¨Ë®Æ½²½Êý
    unsigned int iCurStep_v;    //ÔÆÌ¨´¹Ö±²½Êý
    double fZoomVal;    //±¶ÂÊ

    // guwei
    int mask_mode;
} SDK_OSD_MASK_CONFIG;

typedef struct _sdk_roi_config
{
    int iShow;
    SDK_POSITION rectang;
    unsigned int level;
} SDK_ROI_CONFIG;

typedef struct  _sdk_image_flip_config_
{
    unsigned short u16Image;
    unsigned short u16Flip;
} SDK_IMAGE_FLIP_CONFIG;

typedef struct _sdk_white_balance
{
    unsigned char ucMode;// 0 ×Ô¶¯ 1 ÊÖ¶¯ 2 »§Íâ 3 ÊÒÄÚ
    unsigned int red;   //ºìÉ«ÔöÒæ
    unsigned int blue;  //À¶É«ÔöÒæ
    //unsigned char unused;
} SDK_WHITE_BALANCE;

typedef struct _sdk_dynamic_range
{
    unsigned char ucMode;// 0 ¹Ø±Õ 1 ¿ªÆô
    unsigned char value;
    unsigned char unused[2];
} SDK_dYNAMIC_RANGE;

typedef struct _sdk_denoise
{
    unsigned char ucMode;// 0 ¹Ø±Õ 1 ¿ªÆô
    unsigned char value;
    unsigned char unused[2];
} SDK_DENOISE;

typedef struct _sdk_iristype
{
    unsigned char ucMode;// 0 ¹Ø±Õ 1 ¿ªÆô
    unsigned char unused[3];
} SDK_IRISTYPE;

typedef struct _sdk_sharpen
{
    unsigned char ucMode;// 0 ¹Ø±Õ 1 ¿ªÆô
    unsigned char value;
    unsigned char unused[2];
} SDK_SHARPEN;

typedef struct _sdk_face_exp_cfg
{
    int iEnable;  // 0 ¹Ø±Õ 1 ¿ªÆô
    int iStrength;
    int unused[2];
} SDK_FACE_EXP_CFG;

typedef struct _sdk_ldc
{
    unsigned char ucMode;// 0 ¹Ø±Õ 1 ¿ªÆô
    unsigned char value;  //  0 -- 255
    unsigned char unused[2];
} SDK_LDC;

typedef struct _sdk_anti_fog
{
    unsigned char ucMode;// 0 ¹Ø±Õ 1 ¿ªÆô
    unsigned char value;  // 0 -- 255
    unsigned char unused[2];
} SDK_ANTI_FOG;

typedef struct _sdk_venc_type_bs_size
{
    unsigned short supportDataRate_H264[6];//H264   Ö§³ÖµÄÂëÂÊ
    unsigned short supportDataRate_H265[6];//H265   Ö§³ÖµÄÂëÂÊ
    unsigned short supportDataRate_MJPEG[6];//MJPEG Ö§³ÖµÄÂëÂÊ
    unsigned short supportDataRate_resv[2];//Ö§³ÖµÄÂë?
} SDK_VENC_TYPE_BS_SIZE;

typedef struct _sdk_support_frame_rateEx
{
    unsigned int dwStreamIndex; //µÚ¼¸Â·ÂëÁ÷
    unsigned int dwResolution;  //Ê²Ã´·Ö±æÂÊ
    unsigned int dwRate;        //Ö§³ÖÄÄÐ©Ö¡ÂÊ
    char          strResolutionName[16];//·Ö±æÂÊÃû×Ö
    union
    {
        unsigned short supportDataRate[20];//Ö§³ÖµÄÂëÂÊ
        SDK_VENC_TYPE_BS_SIZE stVencBsSize;
    } unBitRates;
} SDK_SUPPORT_FRAME_RATE;

typedef struct {
    unsigned int  year;
    unsigned int  month;
    unsigned int  date;
    unsigned int  hour;
    unsigned int  minute;
    unsigned int  second;
    unsigned int  weekday;
} SDK_RTC_TIME;

typedef struct _sdk_security_check
{
    SDK_SECURITY RetValue;
    unsigned char ucMacAddr[6];
} SDK_SECURITY_CHECK;

typedef enum _af_focus_action_type
{
    SDK_AUTO = 0,
    SDK_MANALU = 2,
    SDK_ADJUST,
} SDK_AF_FOCUS_TYPE;


typedef enum _af_action_type
{
    SDK_AF_FOCUS = 0,
    SDK_AF_ZOOM = 1,
} SDK_AF_ACTION;
typedef enum _af_action_status
{
    SDK_AF_INIT = 0,
    SDK_AF_START = 1,
    SDK_AF_STOP = 2,
    SDK_AF_FixedPos = 3, //zj
    SDK_AF_FixedStep = 4, //zj
} SDK_AF_STATUS;

typedef struct
{
    SDK_AF_FOCUS_TYPE action;
    SDK_AF_STATUS status;
    int value;
} SDK_AF_FOCUS_STR;

typedef struct
{
    SDK_AF_STATUS status;
    int value;
} SDK_AF_ZOOM_STR;

typedef enum _sdk_high_par {
    SDK_SUPPORTPAR_ROI = 0,
    SDK_SUPPORTPAR_CLRBRIGHITNESS,//ÁÁ¶È
    SDK_SUPPORTPAR_CLRCONTRAST,//¶Ô±È¶È
    SDK_SUPPORTPAR_CLRSATURATION,//±¥ºÍ¶È
    SDK_SUPPORTPAR_CLRHUE,//»Ò¶È
    SDK_SUPPORTPAR_WHITEBALANCE,//°×Æ½ºâ
    SDK_SUPPORTPAR_DENOISE,//½µÔë
    SDK_SUPPORTPAR_SHARPEN,//Èñ¶È
    SDK_SUPPORTPAR_IMAGEMIRROR,//¾µÏñ
    SDK_SUPPORTPAR_IMAGEFLIP,//·­×ª
    SDK_SUPPORTPAR_EXPOSURECTRL,//ÆØ¹â¿ØÖÆ
    SDK_SUPPORTPAR_DIS,//Êý×Ö·À¶¶
    SDK_SUPPORTPAR_DYNAMICRANGE,//¿í¶¯Ì¬
    SDK_SUPPORTPAR_IRISTYPE,//¹âÈ¦
    SDK_SUPPORTPAR_BLACKLEVEL,// ºÚµçÆ½
    SDK_SUPPORTPAR_IMAGEROTATO,//Ðý×ª
    SDK_SUPPORTPAR_VIDEOGAIN,//ÊÓÆµÔöÒæ
    SDK_SUPPORTPAR_ANTIOVEREXPOSURE,//·À¹ýÆØ
    SDK_SUPPORTPAR_LENDISTORTION,//¾µÍ·»û±ä
    SDK_SUPPORTPAR_FOGREDUCTION,//È¥Îí
    SDK_SUPPORTPAR_NIGHT_ADVANCED,//Ò¹ÊÓ¸ß¼¶²ÎÊý
    SDK_SUPPORTPAR_ANTIFLICKER,     //¿¹ÉÁ
    SDK_SUPPORTPAR_WDR,                   // ¿í¶¯Ì¬
} SDK_SUPPORT_HIGHPAR;

typedef enum _sdk_blc_mode_e
{
    ISP_BLC_MODE_OFF  = 0,
    ISP_BLC_MODE_WDR,
    ISP_BLC_MODE_HLC,
    ISP_BLC_MODE_BLC,
    ISP_BLC_MODE_NOT_INIT,

    ISP_BLC_MODE_BUTT
} SDK_BLC_MODE_E;

typedef enum _SDK_BLC_OP_TYPE_E
{
    BLC_OP_TYPE_AUTO    = 0,
    BLC_OP_TYPE_MANUAL  = 1,
    BLC_OP_TYPE_BUTT
} SDK_BLC_OP_TYPE_E;

typedef struct _sdk_work_mode_s
{
    unsigned int hfpsOpen;
    SDK_BLC_MODE_E enBlcMode;
    SDK_BLC_OP_TYPE_E enBlcOPType;
    SDK_POSITION stAreaPos;
} SDK_WORK_MODE_S;

//¿¹ÉÁÄ£Ê½
typedef enum _sdk_anti_flicker_mode_e
{
    ANTIFLICKER_MODE_OFF  = 0,
    ANTIFLICKER_MODE_50HZ,
    ANTIFLICKER_MODE_60HZ,

    ANTIFLICKER_MODE_BUTT
} SDK_ANTIFLICKER_MODE_E;


typedef struct _sdk_smartIR_mode
{
    unsigned char SMARTIR_MODE;// 0=OFF 1=manual  2=auto
    unsigned char SMARTIR_VALUE;
    unsigned char UNUSED[2];
} SDK_SMARTIR_MODE;

typedef struct _sdk_smartIR_stitch_mode
{
    unsigned char mode;                         // 0 -- close, 1 - manu, 2 -- auto
    unsigned char value[5]; // 0 ~ 100
    unsigned char sync_with_zoom;               // valid for az device
    unsigned char close_far_ir;                 // valid for fz device

} SDK_SMARTIR_STITCH_MODE;


typedef enum _sdk_product_type_id_
{
    // S1ÏµÁÐ 900 ¿ªÊ¼
    IPC_PROD_S1_100M_9X1X = 900,
    // IPC_PROD_S1_200M_9X2X = 1002  ¿´ÏÂÃæ¶¨Òå,ÒÑ¾­ÓÐ¶¨Òå£¬×¢Òâ²»ÄÜ¸ü¸Ä¡£
    // IPC_PROD_S1_300M_9X3X = 1003  ¿´ÏÂÃæ¶¨Òå,ÒÑ¾­ÓÐ¶¨Òå£¬×¢Òâ²»ÄÜ¸ü¸Ä¡£

    // S1H ÏµÁÐ 950
    IPC_RPOD_S1H_100M_9X1X = 950,
    IPC_RPOD_S1H_200M_9X2X = 951,
    IPC_RPOD_S1H_300M_9X3X = 952,

    //ÒÔÏÂ¶¨ÒåÊýÖµÓÐÎÊÌâ£¬µ«ÒÑ¾­ÐÎ³É£¬ÔÝÊ±²»ÄÜ¸ü¸Ä¡£
    IPC_PROD_E1_130M_9X1X = 1000,
    IPC_PROD_E1_200M_9X2X = 1001,
    IPC_PROD_S1_200M_9X2X = 1002,
    IPC_PROD_S1_300M_9X3X = 1003,
    IPC_PROD_E2_300M_9X3X = 1004,
    IPC_PROD_M2_300M_9X3X = 1005,
    IPC_PROD_E2_400M_9X4X = 1006,
    IPC_PROD_M2_400M_9X4X = 1007,
    IPC_PROD_M2_500M_9X5X = 1008,
    IPC_PROD_E2_800M_9X8X = 1009,
    IPC_PROD_E2_800M_9X8X2 = 1010,

    // ºóÃæµÄ¶¨Òå¶¼Ôö¼ÓÐÍºÅ¼ä¸ô50¸ö´¦Àí¡£

    // E1 ÏµÁÐ 1050 ¿ªÊ¼
    IPC_PROD_E1_100M_9X1X = 1050,
    // IPC_PROD_E1_130M_9X1X = 1000 ÉÏÃæÓÐ¶¨Òå,ÒÑ¾­ÓÐ¶¨Òå£¬×¢Òâ²»ÄÜ¸ü¸Ä¡£
    // IPC_PROD_E1_200M_9X2X = 1001 ÉÏÃæÓÐ¶¨Òå,ÒÑ¾­ÓÐ¶¨Òå£¬×¢Òâ²»ÄÜ¸ü¸Ä¡£

    // E2 ÏµÁÐ 1100 ¿ªÊ¼
    IPC_PROD_E2_100M_9X1X = 1100,
    IPC_PROD_E2_200M_9X2X = 1101,
    // IPC_PROD_E2_300M_9X3X = 1004  ÉÏÃæÓÐ¶¨Òå,ÒÑ¾­ÓÐ¶¨Òå£¬×¢Òâ²»ÄÜ¸ü¸Ä¡£
    // IPC_PROD_E2_400M_9X4X = 1006  ÉÏÃæÓÐ¶¨Òå,ÒÑ¾­ÓÐ¶¨Òå£¬×¢Òâ²»ÄÜ¸ü¸Ä¡£
    IPC_PROD_E2_500M_9X5X = 1104,
    IPC_PROD_E2_600M_9X6X = 1105,
    IPC_PROD_E2_700M_9X7X = 1106,
    // IPC_PROD_E2_800M_9X8X = 1009  ÉÏÃæÓÐ¶¨Òå,ÒÑ¾­ÓÐ¶¨Òå£¬×¢Òâ²»ÄÜ¸ü¸Ä¡£


    // M2 ÏµÁÐ 1150 ¿ªÊ¼
    IPC_PROD_M2_100M_9X1X = 1150,
    IPC_PROD_M2_200M_9X2X = 1151,
    // IPC_PROD_M2_300M_9X3X = 1005  ÉÏÃæÓÐ¶¨Òå,ÒÑ¾­ÓÐ¶¨Òå£¬×¢Òâ²»ÄÜ¸ü¸Ä¡£
    // IPC_PROD_M2_400M_9X4X = 1007  ÉÏÃæÓÐ¶¨Òå,ÒÑ¾­ÓÐ¶¨Òå£¬×¢Òâ²»ÄÜ¸ü¸Ä¡£
    // IPC_PROD_M2_500M_9X5X = 1008  ÉÏÃæÓÐ¶¨Òå,ÒÑ¾­ÓÐ¶¨Òå£¬×¢Òâ²»ÄÜ¸ü¸Ä¡£
    IPC_PROD_M2_600M_9X6X = 1155,
    IPC_PROD_M2_700M_9X7X = 1156,
    IPC_PROD_M2_800M_9X8X = 1157,

    // M2H ÏµÁÐ 1200 ¿ªÊ¼
    IPC_PROD_M2H_100M_9X1X = 1200,
    IPC_PROD_M2H_200M_9X2X = 1201,

    // S2 ÏµÁÐ
    IPC_PROD_S2_100M_9X1X = 1250,
    IPC_PROD_S2_200M_9X2X = 1251,
    IPC_PROD_S2_300M_9X3X = 1252,
    IPC_PROD_S2_400M_9X4X = 1253,
    IPC_PROD_S2_500M_9X5X = 1254,
    IPC_PROD_S2_800M_9X8X1 = 1257,
    IPC_PROD_S2_800M_9X8X2 = 1258,
    IPC_PROD_S2H_200M_9X2X = 1260,
    IPC_PROD_S2_200M_9X2X_KPJ = 1262,  //¿¨Æ¬»ú

    // DVS ÏµÁÐ
    IPC_PROD_E2_3516D_DVS = 1300,

    // A1 ÏµÁÐ
    IPC_PROD_A1_200M_9X2X = 1350,

    // M3H ÏµÁÐ 1400 ¿ªÊ¼
    IPC_PROD_M3H_100M_9X1X = 1400,
    IPC_PROD_M3H_200M_9X2X = 1401,


    IPC_PROD_PF_200M_E2128  = 1450,

    // A2 ÏµÁÐ
    IPC_PROD_A2_200M_9X2X = 1500,

    // M3 Æ´½ÓÉãÏñ»ú
    IPC_PROD_M3_200M_9X2X = 1700,

    IPC_PROD_M3_400M_9X4X = 1701,

    // A3ÏµÁÐ
    IPC_PROD_A3_600M_9X6X  = 1600,
    IPC_PROD_A3_200M_9X2X  = 1601,
    IPC_PROD_A3_500M_9X5X  = 1602,
    IPC_PROD_KK_A3_200M_9X2X = 1603,  // ¿¨¿ÚÉãÏñ»ú

    IPC_PROD_YT_A3_200M_9X2X  = 1608,

    // ÁªÓÀ·½°¸ NT
    IPC_PROD_NT_S3_100M_9X1X = 1796,
    IPC_PROD_NT_S3_200M_9X2X = 1797,
    IPC_PROD_NT_S3_300M_9X3X = 1798,
    IPC_PROD_NT_S3_400M_9X4X = 1799,
    IPC_PROD_NT_S3_500M_9X5X = 1800,
    IPC_PROD_NT_S3_600M_9X6X = 1801,
    // 1802~1807 Ô¤Áô£¬EÏµÁÐ´Ó1807¿ªÊ¼
    IPC_PROD_NT_E3_500M_9X5X = 1807,
    IPC_PROD_NT_TE1_384_9X5X = 1808,
    IPC_PROD_NT_E3H_400M_9X4X = 1809,
    IPC_PROD_NT_E3B_200M_9X2X = 1810,
    IPC_PROD_NT_TE1_160_9X5X = 1811,
    IPC_PROD_NT_A3L_200M_9X2X = 1812,
    IPC_PROD_NT_200M_E2123 = 1813,
    IPC_PROD_NT_E3B_800M_9X8X = 1814,
    IPC_PROD_NT_200M_E2127 = 1815,
    IPC_PROD_NT_A3B_400M_9X4X_PA = 1816,
    IPC_PROD_NT_S3B_800M_9X8X = 1817,

    // S3
    IPC_PROD_HISI_S3_200M_9X2X = 1820,
    IPC_PROD_HISI_S3L_200M_9X2X = 1821,
    IPC_PROD_HISI_S3H_200M_9X2X = 1822,

    // A3H
    IPC_PROD_A3H_200M_9X2X = 1900,

    // Çò»ú»úÐ¾¶¨Òå
    IPC_PROD_E2_DOM_961X       = 1999,
    IPC_PROD_E2_DOM_962X       = 2000,
    IPC_PROD_E2_DOM_963X       = 2001,
    IPC_PROD_E2_DOM_964X       = 2004,

    IPC_PROD_M2_DOM_V030_961X  = 2050,
    IPC_PROD_M2_DOM_V030_962X  = 2051,
    IPC_PROD_M2_DOM_V030_963X  = 2052,
    IPC_PROD_M2_DOM_V030_964X  = 2053,

    IPC_PROD_E2_DOM_V030_961X  = 2100,
    IPC_PROD_E2_DOM_V030_962X  = 2101,
    IPC_PROD_E2_DOM_V030_963X  = 2102,
    IPC_PROD_E2_DOM_V030_964X  = 2103,

    IPC_PROD_M2_DOM_961X       = 2150,
    IPC_PROD_M2_DOM_962X       = 2151,
    IPC_PROD_M2_DOM_963X       = 2152,
    IPC_PROD_M2_DOM_964X       = 2153,

    // E3 ÏµÁÐ
    IPC_PROD_E3_200M_9X2X  = 2300,
    IPC_PROD_E3_500M_9X5X  = 2301,
    IPC_PROD_E3_400M_9X4X  = 2302,
    IPC_PROD_E3_600M_9X6X  = 2303,
    IPC_PROD_E3_800M_9X8X  = 2310,

    // »úÐ¾Ä£¿é
    IPC_PROD_MOD_DOM_M3XXIM2  = 3000,
    IPC_PROD_MOD_DOM_M2XXIS   = 3001,
    IPC_PROD_MOD_DOM_M2XXIS2   = 3002,
    IPC_PROD_MOD_DOM_32M_M3XXIM2  = 3003,
    IPC_PROD_MOD_DOM_32M_M230IM_LC898201  = 3004,
    IPC_PROD_MOD_DOM_32M_M230IM_41908A  = 3005,
    IPC_PROD_MOD_DOM_32M_M531IM  = 3006,
    IPC_PROD_MOD_DOM_32M_M220IM_LC898201  = 3007,//zjadd for m220im
    IPC_PROD_MOD_DOM_M430IE = 3008, //add for M430IE
    IPC_PROD_MOD_DOM_M437IE = 3009, //add for M437IE
    // »úÐ¾¿Í»§¶¨ÖÆÄ£¿é
    IPC_PROD_MOD_DOM_YAAN_M3XXIM2  = 4000,

} SDK_PRODUCT_TYPE_ID;

typedef enum _sdk_custom_id_
{
    IPC_CUSTOM_ID_EN_NORMAL = 1000,
    IPC_CUSTOM_ID_CH_NORMAL,
    IPC_CUSTOM_ID_PL14,
    IPC_CUSTOM_ID_BR27,
    IPC_UCSTOM_ID_END,
} SDK_CUSTOM_ID_VALUE;

typedef enum _sdk_lens_type_id_
{
    IPC_LENS_TYPE_NULL   = 10,
    IPC_LENS_TYPE_AR0130 = 1000,
    IPC_LENS_TYPE_AR0230,
    IPC_LENS_TYPE_AR0330,
    IPC_LENS_TYPE_SC2035,
    IPC_LENS_TYPE_IMX323,
    IPC_LENS_TYPE_OV4689,
    IPC_LENS_TYPE_IMX274,
    IPC_LENS_TYPE_SC1045,
    IPC_LENS_TYPE_AR0237,
    IPC_LENS_TYPE_IMX185,
    IPC_LENS_TYPE_IMX123,
    IPC_LENS_TYPE_IMX178,
    IPC_LENS_TYPE_IMX326,
    IPC_LENS_TYPE_MN34229,
    IPC_LENS_TYPE_SC3035,
    IPC_LENS_TYPE_IMX290,
    IPC_LENS_TYPE_IMX291,
    IPC_LENS_TYPE_SC5035,
    IPC_LENS_TYPE_AR0521,
    IPC_LENS_TYPE_IMX385,
    IPC_LENS_TYPE_SC2238,
    IPC_LENS_TYPE_IMX327,
    IPC_LENS_TYPE_IMX307,
    IPC_LENS_TYPE_OS05A20,
    IPC_LENS_TYPE_OS08A20,
    IPC_LENS_TYPE_AR0522,
    IPC_LENS_TYPE_SC4238,
    IPC_LENS_TYPE_SC4210,
    IPC_LENS_TYPE_SC8238,
    IPC_LENS_TYPE_IMX415,
    IPC_LENS_TYPE_OS04A10,

    IPC_LENS_TYPE_AR033X = 3000, // = IPC_LENS_TYPE_AR0330£¬ Ö»ÊÇÖ¡ÂÊÊÇ30Ö¡
    IPC_LENS_TYPE_SC2232H,
    IPC_LENS_TYPE_SC2231,
    IPC_LENS_TYPE_SC2230,
} SDK_LENS_TYPE_ID;

typedef enum _flash_param_area_def_
{
    IPC_FLASH_PARAM_OS_MEM = 10, // ÏµÍ³ÄÚ´æ
    IPC_FLASH_PARAM_LOAD_DRIVER_FLAG, //Çý¶¯Æô¶¯½Å±¾µÄ±êÖ¾
    IPC_FLASH_PARAM_ENCRY_VER,

    IPC_FLASH_PARAM_CHECK_SUM0 = 20, //Ð£Ñé
    IPC_FLASH_PARAM_CHECK_SUM1,
    IPC_FLASH_PARAM_CHECK_SUM2,
    IPC_FLASH_PARAM_CHECK_SUM3,
    IPC_FLASH_PARAM_CHECK_SUM4,
    IPC_FLASH_PARAM_CHECK_SUM5,

    // ÈçÏÂÕâ¸öÅäÖÃÐÅÏ¢ÊÇÔÚÐ´macµØÖ·µÄÊ±ºòÐ´ÈëµÄ¡£
    IPC_FLASH_PARAM_MAC_ADDR = 32,// MACµØÖ·
    IPC_FLASH_PARAM_CUSTOM_ID, // ¿Í»§Ö÷ID
    IPC_FLASH_PARAM_CUSTOM_SUB_ID, // ¿Í»§×ÓID
    IPC_FLASH_PARAM_PRODUCT_TYPE, //Ö÷²úÆ·ID
    IPC_FLASH_PARAM_PRODUCT_SUB_TYPE, //×Ó²úÆ·ID
    IPC_FLASH_PARAM_HARDWARE_VERSION, //Ó²¼þ°æ±¾
    IPC_FLASH_PARAM_HARDWARE_SUB_VERSION, //Ó²¼þ×Ó°æ±¾ºÅ
    IPC_FLASH_PARAM_SALE_AREA, // ÏúÊÛÇøÓò
    IPC_FLASH_PARAM_MANUFACTURE_DATE, // Éú²úÈÕÆÚ
    IPC_FLASH_PARAM_MANUFACTURE_SEQ, // Éú²úÐòÁÐÂë
    IPC_FLASH_PARAM_UNIQUE_IDENTIFICATION_CODE, // Éè±¸Î¨Ò»Ê¶±ðÂë
    IPC_FALSE_PARAM_LENS_TYPE, // ¾µÍ·ÀàÐÍ
    IPC_FALSE_PARAM_AUTHCODE0, // ÃÜÂëÎÄ¼þ
    IPC_FALSE_PARAM_AUTHCODE1, // ÃÜÂëÎÄ¼þ
    IPC_FALSE_PARAM_AUTHCODE2, // ÃÜÂëÎÄ¼þ
    IPC_FALSE_PARAM_DELIVERYTIME0, // ÃÜÂëÎÄ¼þ
    IPC_FALSE_PARAM_DELIVERYTIME1, // ÃÜÂëÎÄ¼þ
    IPC_FALSE_PARAM_DELIVERYTIME2, // ÃÜÂëÎÄ¼þ
    IPC_FALSE_PARAM_AZ_DATA0, // AZµÄÊý¾ÝÎÄ¼þ
    IPC_FALSE_PARAM_AZ_DATA1, // AZµÄÊý¾ÝÎÄ¼þ
    IPC_FALSE_PARAM_AZ_DATA2, // AZµÄÊý¾ÝÎÄ¼þ
    IPC_FALSE_PARAM_AZ_DATA3, // AZµÄÊý¾ÝÎÄ¼þ
    IPC_FALSE_PARAM_AZ_DATA4, // AZµÄÊý¾ÝÎÄ¼þ
    IPC_FALSE_PARAM_AZ_DATA5, // AZµÄÊý¾ÝÎÄ¼þ
    IPC_FALSE_PARAM_AZ_DATA6, // AZµÄÊý¾ÝÎÄ¼þ
    IPC_FALSE_PARAM_AZ_DATA7, // AZµÄÊý¾ÝÎÄ¼þ
    IPC_FALSE_PARAM_DOM_CTRL_DATA0, // Çò»ú¿ØÖÆµÄÊý¾ÝÎÄ¼þ
    IPC_FALSE_PARAM_DOM_CTRL_DATA1, // Çò»ú¿ØÖÆµÄÊý¾ÝÎÄ¼þ
    IPC_FALSE_PARAM_DOM_CTRL_DATA2, // Çò»ú¿ØÖÆµÄÊý¾ÝÎÄ¼þ
    IPC_FALSE_PARAM_DOM_CTRL_DATA3, // Çò»ú¿ØÖÆµÄÊý¾ÝÎÄ¼þ
    IPC_FALSE_PARAM_DOM_CTRL_DATA4, // Çò»ú¿ØÖÆµÄÊý¾ÝÎÄ¼þ
    IPC_FALSE_PARAM_DOM_CTRL_DATA5, // Çò»ú¿ØÖÆµÄÊý¾ÝÎÄ¼þ
    IPC_FALSE_PARAM_DOM_CTRL_DATA6, // Çò»ú¿ØÖÆµÄÊý¾ÝÎÄ¼þ
    IPC_FALSE_PARAM_DOM_CTRL_DATA7, // Çò»ú¿ØÖÆµÄÊý¾ÝÎÄ¼þ
    IPC_FALSE_PARAM_DOM_CTRL_IS_CHANGED, //Çò»ú²ÎÊýÊÇ·ñÖØÐÂÐ£×¼

    // ÈçÏÂÊÇÔÚÐ´²ÎÊýÐ´ÈëµÄ
    IPC_FALSE_PARAM_DAY_NIGHT_HIGH_VAL_CDS = 96, // ÈÕÒ¹·§Öµ
    IPC_FALSE_PARAM_DAY_NIGHT_MID_VAL_CDS,       // ÈÕÒ¹·§Öµ
    IPC_FALSE_PARAM_DAY_NIGHT_LOW_VAL_CDS,       // ÈÕÒ¹·§Öµ
    IPC_FALSE_PARAM_NIGHT_DAY_HIGH_VAL_CDS,      // ÈÕÒ¹·§Öµ
    IPC_FALSE_PARAM_NIGHT_DAY_MID_VAL_CDS,       // ÈÕÒ¹·§Öµ
    IPC_FALSE_PARAM_NIGHT_DAY_LOW_VAL_CDS,       // ÈÕÒ¹·§Öµ

    IPC_FALSE_PARAM_DAY_NIGHT_HIGH_VAL,      // ÈÕÒ¹·§Öµ ·ÇºìÍâ»úÐÍ
    IPC_FALSE_PARAM_DAY_NIGHT_MID_VAL, // ÈÕÒ¹·§Öµ
    IPC_FALSE_PARAM_DAY_NIGHT_LOW_VAL, // ÈÕÒ¹·§Öµ
    IPC_FALSE_PARAM_NIGHT_DAY_HIGH_VAL, // ÈÕÒ¹·§Öµ
    IPC_FALSE_PARAM_NIGHT_DAY_MID_VAL, // ÈÕÒ¹·§Öµ
    IPC_FALSE_PARAM_NIGHT_DAY_LOW_VAL, // ÈÕÒ¹·§Öµ

} FLASH_PARAM_AREA_DEF;

typedef enum _sdk_ae_mode
{
    SDK_AE_MODE_IDLE    = 0x00,
    SDK_AE_MODE_AUTO    = 0x01,
    SDK_AE_MODE_STOP    = 0x02,
    SDK_AE_MODE_WDR     = 0x03,
    SDK_AE_MODE_MANNUAL = 0x04,
    SDK_AE_MODE_FNOCAL  = 0x05,
    SDK_AE_MODE_GAINCAL = 0x06,
    SDK_AE_MODE_AFIDLE  = 0x07,
    SDK_AE_MODE_IRISPRI = 0x08,
    SDK_AE_MODE_SHUTTER = 0x09,
    SDK_AE_MODE_BRIGHT  = 0x0A,
    SDK_AE_MODE_GAIN2   = 0x0B,
    SDK_AE_MODE_LC898201CAL = 0xE0, // LC898201 CAL MODE
} SDK_AE_MODE;

typedef struct
{
    SDK_AE_MODE  aeMode;
    unsigned char    value;
} SDK_LENS_AE_INDEX_PARA;

typedef struct
{
    SDK_AF_ACTION  afAction;
    unsigned char        value;
} SDK_LENS_SPEED_PARA;

typedef enum
{
    SDK_CAL_STASTE_NORMAL        = 0x00,
    SDK_CAL_GAIN_NO_CAL          = 0x01,
    SDK_CAL_IRIS_NO_CAL          = 0x02,
    SDK_CAL_AF_NO_CAL            = 0x04,
    SDK_CAL_DEADPIXELS_NO_CAL    = 0x08,
    SDK_CAL_LC898201_NO_CAL    = 0x10,//jameszhang 0408
} SDK_CAL_STASTE;

typedef enum
{
    SDK_LENS_INIT             = 0,  ///<: void
    SDK_LENS_POS,                   ///<: int (enum SDK_AF_ACTION)
    SDK_LENS_SPEED,                 ///<: struct SDK_LENS_SPEED_PARA (new)
    SDK_LENS_FOCUS_MIN_DIS,         ///<: int (enum SDK_LENS_MINDIS)
    SDK_LENS_ZOOM_RATIO,            ///<: void
    SDK_LENS_VIEW_ANGLE,            ///<: void
    SDK_LENS_VIEW_ANGLE_ZOOM,       ///<: int (View Angle)
    SDK_LENS_AE_INFO,               ///<: struct SDK_MAE_INDEX   (no use)
    SDK_LENS_AE_MODE,               ///<: int (enum SDK_AE_MODE)
    SDK_LENS_AE_INDEX,              ///<: struct SDK_LENS_AE_INDEX_PARA (new)
    SDK_LENS_AE_GAIN_LIM,           ///<: tuint8
    SDK_LENS_AF_CTRL,               ///<: struct SDK_LENS_RUN
    SDK_LENS_AF_MODE,               ///<: struct SDK_AF_CTRL_SET
    SDK_LENS_FUNCTEST,              ///<: int
    SDK_LENS_CAL_STASTE,            ///<: void
    SDK_LENS_SUPPORT_MAX_ZOOM,      ///<: int
    SDK_LENS_LES_NAME,              ///<: char (32Byte)
    SDK_LENS_MOUNT_MODE,            ///<: int(¾µÍ·°²×°·½Ê½£¬0:Õý×Å°²×°£¬1:µ¹×Å°²×°)
    SDK_LENS_ZOOMWIDEPOS,           ///<: int
    SDK_LENS_ZOOMTELEPOS,           ///<: int
    SDK_LENS_FOCUSFARPOS,           ///<: int
    SDK_LENS_FOCUSNEARPOS,          ///<: int
    SDK_LENS_ZOOMPPS,               //<: int
} SDK_LENS_TYPE_PARA;


typedef struct
{
    int flicker;
    int line;
    int position;
    int chCount;
} SDK_PTZ_FLICKER_PARA;

typedef enum
{
    SDK_PTZ_OSD_CTRL_START  = 0,
    SDK_PTZ_OSD_CTRL_STOP,
    SDK_PTZ_OSD_CTRL_CLEAR,
} SDK_PTZ_OSD_CTRL_TYPE;

typedef struct
{
    SDK_PTZ_OSD_CTRL_TYPE eCtrlType;
    unsigned int        value;
} SDK_PTZ_OSD_PARA;

typedef enum
{
    SDK_PTZ_RECV_VIDEO_START  = 0,
    SDK_PTZ_RECV_VIDEO_STOP,
} SDK_PTZ_RECV_VIDEO_TYPE;

typedef enum
{
    SDK_PTZ_CRTL_START  = 0,
    SDK_PTZ_CRTL_STOP,
} SDK_PTZ_CRTL_TYPE;

typedef struct
{
    int               line;
    SDK_POSITION      sdkPos;
    unsigned int      osdBufLen;
    unsigned char     *pOsdBuf;
    unsigned int       charNum; //ÐèÒª¸üÐÂµÄ×Ö·û¸öÊý
    unsigned int       *pu32FontWidthInfo;  //ÐèÒª¸üÐÂµÄÃ¿¸ö×Ö·ûÕ¼ÓÃµÄ¿í¶È
} SDK_PTZ_ONE_LINE_PARA;

typedef struct
{
    int             pos;
    SDK_PTZ_ONE_LINE_PARA linePara;
} SDK_PTZ_PART_LINE_PARA;

//wyf 2017-8-17
typedef struct
{
    unsigned int iMaxStep_h;
    unsigned int iCurStep_h;
    unsigned int iMaxStep_v;
    unsigned int iCurStep_v;

    double fPtzAngle_h;
    double fPtzAngle_v;
    double fZoomVal;

    unsigned int res[4];
} SDK_PTZ_UPDATE_POS_PARA;

typedef enum
{
    SDK_PTZ_FLICKER           = 0,  ///<: struct SDK_PTZ_FLICKER_PARA
    SDK_PTZ_OSD,                    ///<: struct SDK_PTZ_OSD_PARA
    SDK_PTZ_RECV_VIDEO,             ///<: int (enum SDK_PTZ_RECV_VIDEO_TYPE)
    SDK_PTZ_CRTL,                   ///<: int (enum SDK_PTZ_CRTL_TYPE)
    SDK_PTZ_UPDATE_PAGE,            ///<: void
    SDK_PTZ_ONE_LINE,               ///<: struct SDK_PTZ_ONE_LINE_PARA
    SDK_PTZ_PART_LINE,              ///<: struct SDK_PTZ_PART_LINE_PARA

    //wyf 2017-8-17
    SDK_PTZ_UPDATE_POS,             ///<: struct SDK_PTZ_UPDATE_POS_PARA
} SDK_PTZ_TYPE_PARA;

typedef enum
{
    SDK_YUVDATA_START           = 0,
    SDK_YUVDATA_STOP,
    SDK_YUVDATA_GET,
    SDK_YUVDATA_RELEASE,
    SDK_AI_YUVDATA_GET,
    SDK_AI_YUVDATA_RELEASE,
    SDK_AI_SEND_RESULT,
    SDK_AI_STRUCTURED_RESULT_PROC,
    SDK_AI_SEND_RESULT_FOR_SMART_IR     // Jesse: for Smart IR 2.0
} SDK_YUVDATA_TYPE_PARA;


typedef struct
{
    int vih;
    int res[4];
} SDK_FISHEYE_VIH_PARA;

typedef struct
{
    int flag;       //ÊÇ·ñ¿ªÆôshading¹¦ÄÜ
    int x;          //shadingÖÐÐÄµã×ø±ê
    int y;          //shadingÖÐÐÄµã×ø±ê
    int radius;     //shading°ë¾¶
    int res[4];
} SDK_FISHEYE_SHADING_PARA;

typedef struct
{
    int x;          //ÓãÑÛÍ¼ÖÐÐÄµã×ø±ê
    int y;          //
    int radius;     //ÓãÑÛÍ¼°ë¾¶
    int res[4];
} SDK_FISHEYE_IMAGE_PARA;



#define SDK_MAX_FISHETE_PTZ_NUM  4

typedef enum
{
    SDK_FISHETE_GET_INIT_PARA   = 0,
    SDK_FISHETE_SET_MODE,
    SDK_FISHETE_SET_PTZ_POS,
    SDK_FISHETE_GET_PTZ_VALID_POS,
    SDK_FISHETE_SET_PTZ_ZOOM,
    SDK_FISHETE_SET_PTZ_MOVE,  // down, up,left,right,cruise control
    SDK_FISHETE_SET_PTZ_CALIBRATE,      //±ê¶¨

    SDK_FISHETE_SET_VIH,                    //ÉèÖÃ²É¼¯Ë®Æ½Æ«ÒÆÁ¿
    SDK_FISHETE_SET_SHADING,                //ÉèÖÃshading²ÎÊý£¬°ë¾¶¡¢ÖÐÐÄµã
    SDK_FISHETE_SET_IMAGE_CENTER_RADIUS,    //ÉèÖÃ½ÃÕýºóÓãÑÛÍ¼ÖÐÐÄµã×ø±êÎ»ÖÃºÍ°ë¾¶
    SDK_FISHETE_GET_IMAGE_CENTER_RADIUS,    //»ñÈ¡½ÃÕýºóÓãÑÛÍ¼ÖÐÐÄµã×ø±êÎ»ÖÃºÍ°ë¾¶

    SDK_FISHETE_SET_ZOOM_IN_STEP,       //SDK_FISHETE_ZOOM_STEPS
    SDK_FISHETE_SET_ZOOM_OUT_STEP,
    SDK_FISHETE_GET_ZOOM_STEP,

    SDK_FISHETE_TYPE_PARA_BUTT
} SDK_FISHETE_TYPE_PARA_E;


typedef enum
{
    SDK_FISHEYE_DESKTOP_MOUNT = 0,      /* desktop mount mode */
    SDK_FISHEYE_CEILING_MOUNT,          /* ceiling mount mode */
    SDK_FISHEYE_WALL_MOUNT,         /* wall mount mode */

    SDK_FISHEYE_MOUNT_MODE_BUTT
} SDK_FISHEYE_MOUNT_MODE_E;

typedef enum
{
    SDK_FISHEYE_COMBINATION_3PTZ = 0,
    SDK_FISHEYE_COMBINATION_4PTZ,

    SDK_FISHEYE_COMBINATION_BUTT
} SDK_FISHEYE_COMBINATION_MODE_E;

typedef struct
{
    SDK_FISHEYE_MOUNT_MODE_E enMountMode;
    SDK_FISHEYE_COMBINATION_MODE_E enCombinationMode;
} SDK_FISHETE_MODE_S;

typedef struct
{
    int ptzId;
    int web_x;
    int web_y;
} SDK_FISHETE_PTZ_POS_S;

typedef enum
{
    SDK_FISHEYE_PTZ_ZOOM_STOP = 0,
    SDK_FISHEYE_PTZ_ZOOM_OUT,
    SDK_FISHEYE_PTZ_ZOOM_IN,

    SDK_FISHEYE_PTZ_ZOOM_BUTT
} SDK_FISHEYE_PTZ_ZOOM_MODE_E;

typedef struct
{
    int ptzId;
    SDK_FISHEYE_PTZ_ZOOM_MODE_E enPtzZoomMode;
} SDK_FISHETE_PTZ_ZOOM_S;

typedef struct
{
    int ptzId;
    int zoom_in_steps;
    int zoom_out_steps;
} SDK_FISHETE_ZOOM_STEPS;


typedef enum
{
    SDK_FISHEYE_MOVE_STOP = 0,
    SDK_FISHEYE_MOVE_LEFT,
    SDK_FISHEYE_MOVE_RIGHT,
    SDK_FISHEYE_MOVE_UP,
    SDK_FISHEYE_MOVE_DOWN,
    SDK_FISHEYE_MOVE_LEFT_UP,
    SDK_FISHEYE_MOVE_RIGHT_UP,
    SDK_FISHEYE_MOVE_LEFT_DOWN,
    SDK_FISHEYE_MOVE_RIGHT_DOWN,
    SDK_FISHEYE_MOVE_CRUISE,

    SDK_FISHEYE_MOVE_BUTT
} SDK_FISHEYE_MOVE_MODE_E;

typedef struct
{
    int ptzId;
    int speed;
    SDK_FISHEYE_MOVE_MODE_E enPtzMoveMode;
} SDK_FISHETE_PTZ_MOVE_S;



typedef enum
{
    SDK_AF_CTRL_GET_MODE    = 0x00,
    SDK_AF_CTRL_SET_MODE_NORMAL         = 0x01,
    SDK_AF_CTRL_SET_MODE_INTERVAL       = 0x02,
    SDK_AF_CTRL_SET_MODE_IDLE           = 0x03,
    SDK_AF_CTRL_SET_MODE_ONEPUSH        = 0x04,
    SDK_AF_CTRL_SET_MODE_ZOOMTRIGGER    = 0x05,
    SDK_AF_CTRL_SET_MODE_NEARLMT        = 0x06,
    SDK_AF_CTRL_SET_MODE_INFINITY       = 0x07,
    SDK_AF_CTRL_SET_MODE_CALIB          = 0x08,
    SDK_AF_CTRL_SET_MODE_PRESET         = 0x09,
    SDK_AF_CTRL_SET_MODE_AE_CAL           = 0x0A,
    SDK_AF_CTRL_SET_AFLIMIT                 = 0x0B,
    SDK_AF_CTRL_CALIB_CHECK                 = 0x0C,

    SDK_AF_CTRL_SET_MODE_PANTILT        = 0xf0,
    SDK_AF_CTRL_SET_MODE_DEBUG          = 0xFF,
} SDK_AF_CTRL_SET_MODE;




typedef struct
{
    SDK_AF_CTRL_SET_MODE cmd;
    unsigned int data[2];
} SDK_AF_CTRL_SET;

typedef enum _lens_status
{
    SDK_LENS_STATUS_INIT   = 0,
    SDK_LENS_STATUS_START,
    SDK_LENS_STATUS_STOP,
    SDK_LENS_STATUS_FIX_POS,
    SDK_LENS_STATUS_FIX_STEP,
} SDK_LENS_STATUS;

typedef enum _sdk_lens_direction
{
    SDK_LENS_CW     = 0,
    SDK_LENS_CCW    = 1,
} SDK_LENS_DIRECTION;

typedef enum _lens_mode
{
    SDK_LENS_AUTO = 0,
    SDK_LENS_MANALU = 2,
    //CHENGYU CHECK ????
    SDK_LENS_ADJUST,
} SDK_LENS_MODE;

typedef struct
{
    SDK_LENS_MODE mode;
    SDK_LENS_STATUS status;
    SDK_LENS_DIRECTION direction;
    int position;
    int step;
} SDK_LENS_STR;

typedef struct
{
    SDK_AF_ACTION type;
    SDK_LENS_STR str;
} SDK_LENS_RUN;

typedef enum _sdk_lens_mindis
{
    SDK_LENS_MINDIS_30CM = 0,
    SDK_LENS_MINDIS_1M   = 1,
    SDK_LENS_MINDIS_1P5M = 2,
    SDK_LENS_MINDIS_2M   = 3,
    SDK_LENS_MINDIS_3M   = 4,
    SDK_LENS_MINDIS_5M   = 5,
    SDK_LENS_MINDIS_10M  = 6,
} SDK_LENS_MINDIS;

typedef struct
{
    unsigned int    chn;
    unsigned int    width;
    unsigned int    height;
    unsigned int    frameRate;
} SDK_YUVDATA_INFO;

typedef struct IMG_YUV_FRAME {
    /*! Í¼Ïñ¸ñÊ½¡£ */
    unsigned int u32Width;
    unsigned int u32Height;

    unsigned int enPixelFormat;  //PIXEL_FORMAT_E
    unsigned int    u32Stride[3];
    union
    {
        struct
        {
            unsigned long long  u64PhyAddr[3];
            unsigned long long  u64VirAddr[3];
        } st64;
        struct
        {
            unsigned int  u32PhyAddr[3];
            unsigned int  u32VirAddr[3];
        } st32;
    } Addr;


    short   s16OffsetTop;        /* top offset of show area */
    short   s16OffsetBottom;    /* bottom offset of show area */
    short   s16OffsetLeft;        /* left offset of show area */
    short   s16OffsetRight;        /* right offset of show area */

    unsigned long long    u64PTS;
    unsigned int u32PoolId;
    unsigned int enModId;
} IMG_YUV_FRAME;

typedef struct
{
    unsigned int         chn;
    IMG_YUV_FRAME  frameInfo;
    int resv[8];
} SDK_AI_YUV_FRAME_INFO;

typedef struct
{
    unsigned int         chn;
    IPCAMERA_AV_FRAME_INFO  frameInfo;
    unsigned int         phyAddr;
    unsigned int         yuvPartPhyAddr;
    void *                  pYuvPartDataVa;
    unsigned int         yuvPartDataLen;
} SDK_YUVDATA_FRAME;


struct DetectRect {
    int x;
    int y;
    int width;  //Ëã·¨ ¼ì²â³öÀ´µÄ¿í¶È
    int height;//Ëã·¨ ¼ì²â³öÀ´µÄ¸ß¶È
    int scaleWidth; // ×¥Í¼ÐèÇóµÄ¿í¶È
    int scaleHeight;// ×¥Í¼ÐèÇóµÄ¸ß¶È
};


struct BoxDetectResult {
    struct DetectRect box;
    int score;
};

struct DetDetecResult {
    struct BoxDetectResult body;
    struct BoxDetectResult pre_frame_body;
    int classes;
    unsigned int id;
    float objectness;
};

struct CarDetectResult {
    struct DetDetecResult det;
};

struct PersonDetectResult {
    struct DetDetecResult det;
};

struct BikeDetectResult {
    struct DetDetecResult det;
};

struct ImageDetectResult {
    struct CarDetectResult* car_results;        //³µ
    int car_results_count;
    struct PersonDetectResult* person_results;  //ÈË
    int person_results_count;
    struct BikeDetectResult*bike_results;      //¶þÂÖ³µ
    int bike_results_count;
    int video_chn;                             //ÊÓÆµÍ¨µÀºÅ
    unsigned long long frame_id;               //Ö¡ºÅ
    SDK_AI_YUV_FRAME_INFO  src_frame;
};

typedef struct _sdk_jpeg_info
{
    int             iJpegWidth;
    int             iJpegHeight;
    int             iJpegLen;
    char*         pJpegData;
} SDK_JPEG_INFO;

typedef struct _sdk_ai_stuct_jpeg_mem
{
    SDK_JPEG_INFO      *pstCarJpegInfo;
    int iCarJpegCount;
    SDK_JPEG_INFO      *pstPersonJpegInfo;
    int iPersonJpegCount;
    SDK_JPEG_INFO      *pstBikeJpegInfo;
    int iBikeJpegCount;
    SDK_JPEG_INFO stSrcJpegInfo;
} SDK_AI_STRUCT_JPEG_MEM;

typedef struct  _sdk_ai_struct_info
{
    struct ImageDetectResult dtResult;
    SDK_AI_STRUCT_JPEG_MEM jpegData;
} SDK_AI_STRUCT_INFO;


typedef enum _sdk_dvs_control_type
{
    SDK_GET_DVS_VIDEOFORMAT = 1,
    SDK_GET_DVS_VIDEO_LOSS = 2,
    SDK_SET_DVS_EQUALIZER = 3,
    SDK_SET_DVS_I2C = 4,
    SDK_SET_DVS_VIDEOFORMAT = 5,
    SDK_SET_DVS_PTZ_INIT = 6,
    SDK_SET_DVS_PTZ_SET = 7, //dvs
    SDK_SET_DVS_PTZ_UP,
    SDK_SET_DVS_PTZ_DOWN,
    SDK_SET_DVS_PTZ_LEFT,
    SDK_SET_DVS_PTZ_RIGHT,
    SDK_SET_DVS_IMAGE_SHIFT,
    SDK_SET_DVS_SIGNAL_TYPE,
    SDK_SET_DVS_COC_TYPE,
    SDK_SET_DVS_ZOOM_IN,
    SDK_SET_DVS_ZOOM_OUT,
    SDK_SET_DVS_FOCUS_NEAR ,
    SDK_SET_DVS_FOCUS_FAR,
    SDK_SET_DVS_ZOOM_FOCUS_STOP,
} SDK_DVS_CONTROL_TYPE;
typedef enum _sdk_dvs_signal_type
{
    SDK_SIGNAL_AUTO = 0X0,
    SDK_SIGNAL_AHD,
    SDK_SIGNAL_TVI,
    SDK_SIGNAL_CVI,
    SDK_SIGNAL_CVBS,
} SDK_DVS_SIGNAL_TYPE;
typedef enum _sdk_dvs_coc_type
{
    SDK_COC_TVI = 0X0,
    SDK_COC_PELCO,
} SDK_DVS_COC_TYPE;

typedef struct
{
    SDK_DVS_CONTROL_TYPE controlType;
    void* pData;
    int dataLen;
} SDK_DVS_CONTROL;

typedef enum sdk_dn_check_mode_e
{
    SDK_DN_CHECK_MODE_ADC = 0,
    SDK_DN_CHECK_MODE_ISP,
    SDK_DN_CHECK_MODE_ISP_AND_ADC,
    SDK_DN_CHECK_MODE_ISP_OR_ADC,
    SDK_DN_CHECK_MODE_ERROR,
} SDK_DN_CHECK_MODE_E;

typedef enum sdk_dn_state_e
{
    SDK_DN_STATE_DAY = 0,
    SDK_DN_STATE_NIGHT,
    SDK_DN_STATE_ERROR,
} SDK_DN_STATE_E;

typedef struct sdk_dn_check_info_s
{
    int sensLevel;
    int adcD2nThr;
    int adcN2dThr;
    SDK_DN_CHECK_MODE_E d2nCheckMode;
    SDK_DN_CHECK_MODE_E n2dCheckMode;
    SDK_DN_STATE_E dnResult;
} SDK_DN_CHECK_INFO_S;

typedef struct sdk_smart_ir_cfg_s
{
    unsigned char mode;         // 0 -close, 1 - open
    unsigned char level;        // 0 - low, 1 - middle, 2 - high
    unsigned char unused[2];
} SDK_SMART_IR_CFG_S;

typedef struct
{
    int   x;
    int   y;
} SDK_POINT;

typedef enum
{
    SDK_FACE_DT_CTRL_START = 0,
    SDK_FACE_DT_CTRL_STOP,
    SDK_FACE_DT_CTRL_SET_PARA,
    SDK_FACE_DT_CTRL_GET_DATA,
    SDK_FACE_DT_CTRL_RELEASE_DATA,
    SDK_FACE_DT_SET_FACE_ADDR,

    SDK_FACE_DT_CTRL_START_HUAAN = 10,      // Æô¶¯.
    SDK_FACE_DT_CTRL_STOP_HUAAN,            // Í£Ö¹.
    SDK_FACE_DT_CTRL_SET_PARA_HUAAN,        // ÉèÖÃ²ÎÊý.
    SDK_FACE_DT_CTRL_GET_DATA_HUAAN,        // »ñÈ¡Êý¾ÝÔ´.
    SDK_FACE_DT_CTRL_RELEASE_DATA_HUAAN,    // ÊÍ·ÅÊý¾ÝÔ´.
    SDK_FACE_DT_CTRL_CROP_IMG_HUAAN,        // ¸ù¾Ý´«Èë²ÎÊý, Êä³öÏà¹ØÐÅÏ¢.

    SDK_FACE_DYUV_DATA_START = 50,      // Æô¶¯.
    SDK_FACE_DYUV_DATA_STOP,            // Í£Ö¹.
    SDK_FACE_DYUV_DATA_GET,     // »ñÈ¡Êý¾ÝÔ´.
    SDK_FACE_DYUV_DATA_RELEASE, // ÊÍ·ÅÊý¾ÝÔ´.
    SDK_FACE_DYUV_DATA_CROP_IMG,        // ¸ù¾Ý´«Èë²ÎÊý, Êä³öÏà¹ØÐÅÏ¢.

    SDK_FACE_DT_CTRL_BUTT
} SDK_FACE_DT_CTRL_E;

typedef enum
{
    SDK_FACE_DT_IMG_JPEG = 0,
    SDK_FACE_DT_IMG_YUV420,

    SDK_FACE_DT_IMG_BUTT
} SDK_FACE_DT_IMG_TYPE_E;

typedef enum
{
    SDK_FACE_DT_IMG_INVALID = 0,
    SDK_FACE_DT_IMG_VALID,
    SDK_FACE_DT_IMG_SAVED,

    SDK_FACE_DT_IMG_STATUS_BUTT
} SDK_FACE_DT_IMG_STATUS_E;

typedef struct
{
    unsigned int    srcWidth;   //:¼ì²âÔ´¿í
    unsigned int    srcHeight;  //:¼ì²âÔ´¸ß
    unsigned int    frameRate;  //:Ö¡ÂÊ
    unsigned int    pushPicInterval;    //:ÍÆËÍÈËÁ³×¥Í¼µÄ¼ä¸ôÊ±¼ä(Ãë) 0:±íÊ¾×Ô¶¯È¥ÖØ >0:±íÊ¾¹Ì¶¨¼ä¸ôÊ±¼äÉÏ´«
    int  reserved[1];
} SDK_FACE_DT_CFG;  //: start ´«ÈëÊý¾Ý½á¹¹Ìå

typedef struct
{
    SDK_POSITION   validRect;
    unsigned int maxRectWidth; //×ø±ê¿í
    unsigned int maxRectHeight; //×ø±ê¸ß
    unsigned short minFaceSize;
    unsigned short maxFaceSize;
    int ExpPriorityMode;  // 0 : ¼à¿ØÓÅÏÈ(×î´ó¿ìÃÅÊ±¼ä20ms); 1 ÈËÁ³ÓÅÏÈ(×î´ó¿ìÃÅÊ±¼ä10ms);
    int  reserved[2];
} SDK_FACE_DT_PARA;  //set paraÊý¾Ý½á¹¹Ìå

typedef struct
{
    SDK_FACE_DT_IMG_TYPE_E enType;
    SDK_FACE_DT_IMG_STATUS_E enImgSatus;
    unsigned int width;
    unsigned int height;
    unsigned int size;
    void *pData;
} SDK_FACE_IMG_DATA;


typedef struct
{
    int                  nID;            /* ID Number */
    SDK_POINT      ptLeftTop;      /* Left-Top     Face Coordinates    */
    SDK_POINT      ptRightTop;     /* Right-Top    Face Coordinates    */
    SDK_POINT      ptLeftBottom;   /* Left-Bottom  Face Coordinates    */
    SDK_POINT      ptRightBottom;  /* Right-Bottom Face Coordinates    */
    int                  nPose;          /* Face Pose                        */
    int                  nConfidence;    /* Confidence Degree                */

    int                  age;
    int                  sex;
    int                  dtFrames;

    SDK_FACE_IMG_DATA stFaceImgData;
    int                   featureSize;
    unsigned char * pFeatureData;
    SDK_POINT stPosFaceImg;         /* ÈËÁ³ÔÚ´óÍ¼µÄ×ø±ê */

    //begin ÐÂÔöËã·¨²¿·Ö¸øµÄ²ÎÊý
    float                   feature_score;//ÌØÕ÷ÖµÅÐ¶ÏÈËÁ³¿ÉÐÅ¶È0~100

    /*ÒÔÏÂ±äÁ¿ÎªIFaceDetectorEx½Ó¿ÚÖ§³Ö£¬Îª¸ºÊýÊ±ÎªÎÞÐ§×´Ì¬*/
    short                   eye_dist;     //Ë«ÑÛ¾àÀë
    short                   blur;         //Ä£ºý¶È

    char                    pose_est_score;//ÈËÁ³×ËÌ¬ÆÀ¹À·ÖÊý0~100
    char                    detect_score;  //¼ì²âËã·¨ÅÐ¶ÏÈËÁ³¿ÉÐÅ¶È0~100
    char                    illumination;  //¿ÙÍ¼ÇøÓòÆ½¾ùÁÁ¶È
    char                    faceliveness;  //»îÌå0~100

    char                    completeness;  //ÕÚµ²0~100
    char                    reserved1[3];

    float                   comprehensive_score;//×ÛºÏÆÀ·Ö [90,100)ÊÇÓÅÐã£¬ [80,90)ÊÇÁ¼ºÃ£¬[70,80)ÊÇÒ»°ã£¬[60,70)ÊÇºÏ¸ñ£¬[50,60)¿ÉÑ¡£¬[0,50)²î¡£
    //end ÐÂÔöËã·¨²¿·Ö¸øµÄ²ÎÊý

    int  reserved[9];
} SDK_FACE_DT_INFO;


typedef struct
{
    int dtInfoCount;

    LONGLONG     time;
    LONGLONG     relativeTime;

    unsigned int    DtWidth;
    unsigned int    DtHeight;

    SDK_FACE_IMG_DATA stCurSrcImgData;  // Ô­Í¼

    SDK_FACE_DT_INFO *pstDtInfo;        // Á³Í¼
    int  reserved[8];
} SDK_FACE_DT_DATA; //:resultÊý¾Ý½á¹¹Ìå

/*
  IMAGE_FORMAT_NULL        = 0,         // ¸ñÊ½Îª¿Õ
  IMAGE_FORMAT_RGB         = 0x1,       // RGB
  IMAGE_FORMAT_BGR         = 0x1 << 1,
  IMAGE_FORMAT_JPEG        = 0x1 << 2,
  IMAGE_FORMAT_H264        = 0x1 << 3,
  IMAGE_FORMAT_GRAY        = 0x1 << 4,  // µ¥Í¨µÀ»Ò¶ÈÍ¼
  IMAGE_FORMAT_NV12        = 0x1 << 5,  // YUV420SP¸ñÊ½£ºYYYYYYYY UVUV
  IMAGE_FORMAT_NV21        = 0x1 << 6,  // YUV420SP¸ñÊ½£ºYYYYYYYY VUVU
  IMAGE_FORMAT_I420        = 0x1 << 7,  // YUV420P¸ñÊ½£ºYYYYYYYY UU VV
  IMAGE_FORMAT_YV12        = 0x1 << 8,  // YUV420P¸ñÊ½£ºYYYYYYYY VV UU
  IMAGE_FORMAT_WEBP        = 0x1 << 9,
  IMAGE_FORMAT_UNDEFINED   = 0x1 << 31
*/
typedef struct
{
    int             iMemIndex;
    int             format;         // Í¼Ïñ¸ñÊ½.
    int             width;
    int             height;
    char*           data;

} SDK_SRC_IMAGE_DATA;

typedef struct
{
    int             iMemIndex;
    int             faceId;         // Á³²¿ID, ²»ÒªÐÞ¸Ä, µ±ÆäµÈÓÚ0±íÊ¾ÐèÒª¸ø³öJPEGÔ­Í¼.

    SDK_POINT       ptLeftTop;      // ×óÉÏ½Ç×ø±ê   (´«Èë²ÎÊý)
    SDK_POINT       ptRightBottom;  // ÓÒÏÂ½Ç×ø±ê   (´«Èë²ÎÊý)

    LONGLONG        llRelativeTime;

    int             iJpegWidth;
    int             iJpegHeight;
    int             iJpegLen;       // JPEGÍ¼Æ¬´óÐ¡ (´«³ö²ÎÊý)
    char*           pJpegData;      // JPEGÍ¼Æ¬Êý¾Ý (´«³ö²ÎÊý)

    int             iRgbWidth;
    int             iRgbHeight;
    int             iRgbLen;        // RGBÍ¼Æ¬´óÐ¡  (´«³ö²ÎÊý)
    char*           pRgbData;       // RGBÍ¼Æ¬Êý¾Ý  (´«³ö²ÎÊý)

} SDK_CROP_IMAGE_DATA;

typedef struct
{
    int             x;              // ×óÉÏ½ÇX×ø±ê.
    int             y;              // ×óÉÏ½ÇY×ø±ê.
    int             width;          // ÇøÓò¿í¶È.
    int             height;         // ÇøÓò¸ß¶È.

} SDK_FACE_RECT;

typedef struct
{
    int             iPersonCount;
    SDK_FACE_RECT   *pstFaceRect;

} SDK_FACE_POSITION_DATA;

// Jesse: added for face based exposure
// 2019-4-25, added by Jesse Chen, for more face information
typedef struct {
    unsigned int id;
    SDK_FACE_RECT face_rect;
    char detect_score;
    char illumination;
    char faceliveness;
    char completeness;
} SDK_FACE_INFO;

// 2019-4-25, added by Jesse Chen, for more face information
typedef struct {
    int bEnable;                 // Jesse: enable face exposure
    unsigned int faceExpStrength;    // Jesse: face exp strength
    int iPersonCount;                // Jesse: total face count
    SDK_FACE_INFO * pstFaceInfo; // Jesse:
} SDK_FACE_INFO_DATA;
// Jesse: end by Jesse Chen



typedef struct
{
    unsigned long long  u64ProduceTick;   // ½ÓÊÕµ½Êý¾ÝµÄÊ±¼ä(ºÁÃë¼¶)
    unsigned int                 iPid;
    unsigned int                 iHid;
    unsigned long long        u64CardID;
    int bitNum;
    unsigned long long        u64OriData;
    char                szRes[12];

} SDK_WIEGAND_DATA;

#define MAX_SEC_CODE_LEN  (16)

typedef struct
{
    int len;
    char  sec_code[MAX_SEC_CODE_LEN];
    int  cmd_run_with_sec_code; // linuxï¿½ï¿½ï¿½ï¿½Ö´ï¿½ï¿½ï¿½Ç·ï¿½ï¿½ï¿½ï¿½È«ï¿½ï¿½
    int  resv[2];
} SDK_SEC_CODE;

typedef struct sdk_version_s
{
    char sdkLibVersion[32];
    char afLibVersion[32];
    char imgLibVersion[32];
} SDK_VERSION_S;

typedef enum sdk_mask_type_e
{
    SDK_MASK_TYPE_MONO = 0,
    SDK_MASK_TYPE_MOSAIC,
    SDK_MASK_TYPE_ERROR,
} SDK_MASK_TYPE_E;

typedef struct
{
    SDK_EXCEPTION_MODE mode;
    int val;
} SDK_EXCEPTION_PARA;

typedef struct
{
    int index;
    int status;
} SDK_OBJECTREMOVE_PARA;

//ÒÆ¶¯Õì²âÌØÕ÷Êý¾ÝÃèÊö
/*¹ØÓÚÊý¾ÝÇø
1.´ÓÐÎÊ½ÉÏ£¬Êý¾ÝÇø¸Ä³ÉÒ»Î¬Êý×é¡£
2.´ÓÊ¹ÓÃÉÏ£¬Òª½«Ò»Î¬Êý×é»¹Ô­³É¶þÎ¬µÄÀ´Ê¹ÓÃ£¬Èç¹û»®·Ö·ÝÊýÊÇ64x64£¬ÔòÊý¾ÝÇø´æÈ¡ÏÂ£º
  data[0],  data[1],  data[2],  data[3],  data[4],  data[5],  data[6],  data[7],  //µÚ1ÐÐ£¬Õ¼8¸ö×Ö½Ú£¬¹²64Î»ÒÔ´ËÀàÍÆ¹²64ÐÐ¡£
  data[8],  data[9],  data[10], data[11], data[12], data[13], data[14], data[15], //µÚ2ÐÐ£¬
  data[16], data[17], data[18], data[19], data[20], data[21], data[22], data[23]£¬//µÚ3ÐÐ£¬
  ......
  data[504],data[505],data[506],data[507],data[508],data[509],data[510],data[511] //µÚ64ÐÐ

3.Èç¹û»®·Ö·ÝÊýÊÇ22x18£¬widthDivideNum=22£¬ heightDivedeNum=18£¬ÔòÊý¾ÝÇø´æÈ¡ÈçÏÂ£º
  data[0],   data[1],   data[2], //Ã¿ÐÐÕ¼3¸ö×Ö½Ú£¬¹²24Î»£¬×îºó2Î»ÆúÖ®²»ÓÃ(×îµÍÎ»)£¬Ö»Ê¹ÓÃÇ°22Î»¡£
  data[3],   data[4],   data[5],
  ......
  data[51],  data[52],  data[53] //µÚ18ÐÐ

4.Èç¹û»®·Ö·ÝÊýÊÇ31x31£¬widthDivideNum=31£¬ heightDivedeNum=31£¬ÔòÊý¾ÝÇø´æÈ¡ÈçÏÂ£º
  data[0],   data[1],   data[2],   data[3], //Ã¿ÐÐÕ¼4¸ö×Ö½Ú£¬¹²32Î»£¬×îºó1Î»ÆúÖ®²»ÓÃ£¬Ö»Ê¹ÓÃÇ°31Î»¡£
  data[4],   data[5],   data[6],   data[7],
  ......
  data[120], data[121], data[122], data[123]//µÚ31ÐÐ
*/
typedef struct _sdk_motion_describe
{
    unsigned long long  timeStamp;       // Ê±¼ä´Á(Ö¡Êý¾ÝÏà¶ÔÊ±¼ä)£¬ÓëÍ¬Ò»IPCËùÓÐÂëÁ÷µÄÊ±¼ä´ÁÒ»ÖÂ£¬Í¬Ò»Ä£ÄâÍ¨µÀÒ²ÀàËÆ¡£
    unsigned short      compressType;    // Ñ¹ËõÀàÐÍ£¬Ä¿Ç°ÔÝÊ±²»Ê¹ÓÃ£¬Ìî0¼´¿É
    unsigned char       widthDivideNum;  // ´ÓÒ»Ö¡ÌáÈ¡³öµÄÌØÕ÷Êý¾Ý¶ÔÍ¼Ïñ¿í¶ÈµÄÊµ¼Ê»®·Ö·ÝÊý, <= 64.
    unsigned char       heightDivideNum; // ´ÓÒ»Ö¡ÌáÈ¡³öµÄÌØÕ÷Êý¾Ý¶ÔÍ¼Ïñ¸ß¶ÈµÄÊµ¼Ê»®·Ö·ÝÊý, <= 64.
    unsigned char       data[0];         // Êý¾ÝÇø£¬°´Î»´æÈ¡£¬±ä³¤
} SDK_MOTION_DESCRIBE;

typedef struct sdk_ai_cfg_s
{
    int compat_mode;                    // ¼æÈÝÄ£Ê½
    SDK_AUDIO_INPUT compat_ai;          // ¼æÈÝÄ£Ê½ÒôÆµÊäÈëÄ£Ê½

    int left_enable;                    // ·Ç¼æÈÝÄ£Ê½ÏÂ×óÉùµÀÊÇ·ñÆôÓÃ
    int right_enable;                   // ·Ç¼æÈÝÄ£Ê½ÏÂÓÒÉùµÀÊÇ·ñÆôÓÃ
} SDK_AI_CFG_S;




//==========================Í¼ÏñÐ§¹ûÅäÖÃ=============================
typedef struct
{
    unsigned char            ucSharpenMode;                      // Èñ¶ÈÄ£Ê½         0:¹Ø  1:¿ª
    unsigned char            ucSharpen;                          //                  ÉèÖÃµÄÖµÎª0~255
} SDK_V2_SHARPEN;

typedef struct
{
    unsigned char            ucGammaMode;                        // Ù¤ÂíÄ£Ê½         0:¹Ø  1:¿ª
    unsigned char            ucGamma;                            //                  ÉèÖÃ·¶Î§Îª0~1
} SDK_V2_GAMMA;


typedef struct
{
    unsigned char            ucBright;                           // ÁÁ¶È             ÉèÖÃµÄÖµÎª0~100
    unsigned char            ucSaturation;                       // ±¥ºÍ¶È           ÉèÖÃµÄÖµÎª0~100
    unsigned char            ucContrast;                         // ¶Ô±È¶È           ÉèÖÃµÄÖµÎª0~100
    SDK_V2_SHARPEN           stSharpen;                          // Èñ¶È
    SDK_V2_GAMMA             stGamma;                            // Ù¤Âí

} SDK_V2_IMAGE_EFFECT;
//====================================================================

//=============================ÆØ¹âÅäÖÃ===============================
typedef struct
{
    unsigned char            ucExposureZoneMode;                 // ÆØ¹âÇøÓòÄ£Ê½     0:ÊÖ¶¯  1:×Ô¶¯
    unsigned char            ucExposureZone;                     // ÆØ¹âÇøÓò         0:ÉÏ  1:ÏÂ  2:×ó  3:ÓÒ  4:ÖÐ
    unsigned char            ucExposureZoneLevel;                // ÆØ¹â¼¶±ð         ÉèÖÃµÄÖµÎªx~y
} SDK_V2_EXPOSURE_ZONE;

typedef struct
{
    unsigned char            ucSlowShutterMode;                  // Âý¿ìÃÅÄ£Ê½       0:¹Ø±Õ  1:¿ªÆô
    unsigned char            ucSlowShutterVal;                   // Âý¿ìÃÅ¿ªÆôºóÉèÖÃ×îÐ¡¿ìÃÅÖµ
} SDK_V2_SLOW_SHUTTER;

typedef struct
{
    unsigned char            ucShutterMode;                      // ¿ìÃÅÄ£Ê½         0:ÊÖ¶¯  1:×Ô¶¯
    unsigned char            ucShutterValue;                     // ÊÖ¶¯Ä£Ê½ÏÂÉèÖÃ
    unsigned char            ucShutterMinVal;                    // ×Ô¶¯Ä£Ê½ÏÂÉèÖÃ×îÐ¡Öµ
    unsigned char            ucShutterMaxVal;                    // ×Ô¶¯Ä£Ê½ÏÂÉèÖÃ×î´óÖµ
} SDK_V2_SHUTTER;

typedef struct
{
    unsigned char            ucIrisMode;                         // ¹âÈ¦ÀàÐÍ         0:¹Ì¶¨¹âÈ¦  1:PIris  2:DCIris
    unsigned char            ucDCIrisMode;                       // DCIrisÄ£Ê½       0:×Ô¶¯  1:È«¿ªÄ£Ê½  2:×¨¼Ò
    unsigned char            ucDCIris_SP;                        // ×¨¼ÒÄ£Ê½ÉèÖÃ
    unsigned char            ucDCIris_SI;                        // ×¨¼ÒÄ£Ê½ÉèÖÃ
    unsigned char            ucDCIris_SD;                        // ×¨¼ÒÄ£Ê½ÉèÖÃ
    unsigned char            ucDCIris_REF;                       // ×¨¼ÒÄ£Ê½ÉèÖÃ
    unsigned char            ucPIrisMode;                        // PIrisÄ£Ê½        0:ÊÖ¶¯  1:×Ô¶¯
    unsigned char            ucPIris;                            // ÊÖ¶¯Ä£Ê½ÉèÖÃ
} SDK_V2_IRIS;

typedef struct
{
    unsigned char            ucGainMode;                         // ÔöÒæÄ£Ê½         0:ÊÖ¶¯  1:×Ô¶¯  2:×¨¼Ò
    unsigned char            ucGain;                             // ucGainMode=ÊÖ¶¯
    unsigned char            ucGainMax;                          // ucGainMode=×Ô¶¯
    unsigned char            ucGainAdvanceMode;
    unsigned char            ucGain_A;                           // ucGainMode=×¨¼Ò
    unsigned char            ucGain_D;
    unsigned char            ucGain_ISP;
} SDK_V2_GAIN;


typedef struct
{
    unsigned char            ucExpertExporsurMode;               // ×¨¼ÒÆØ¹â         0:¹Ø±Õ  1:¿ªÆô
    unsigned char            ucSceneMode;                        // ³¡¾°Ä£Ê½         0:ÊÒÍâ  1:ÊÒÄÚ
    unsigned char            ucAntiFlickerMode;                  // ¿¹ÉÁË¸Ä£Ê½       0:¹Ø±Õ  1:50HZ¿¹ÉÁ  2:60HZ¿¹ÉÁ
    unsigned char            ucExposureMode;                     // ÆØ¹âÄ£Ê½         0:ÊÖ¶¯ÆØ¹â  1:×Ô¶¯ÆØ¹â  2:¿ìÃÅÓÅÏÈ  3:ÔöÒæÓÅÏÈ  4:¹âÈ¦ÓÅÏÈ  5:ÁÁ¶ÈÓÅÏÈ
    SDK_V2_EXPOSURE_ZONE     stExposureZone;                     // ÆØ¹âÇøÓò
    SDK_V2_SHUTTER           stShutter;                          // ¿ìÃÅ
    SDK_V2_IRIS              stIris;                             // ¹âÈ¦
    SDK_V2_GAIN              stGain;                             // ÔöÒæ
    SDK_V2_SLOW_SHUTTER      stSlowShutter;                      // Âý¿ìÃÅ

} SDK_V2_EXPORSUR_SET;
//====================================================================


//===========================ÈÕÒ¹Ä£Ê½ÅäÖÃ=============================
typedef struct
{
    unsigned char            ucSmartIrMode;                      // ÖÇÄÜºìÍâÄ£Ê½     0:ÊÖ¶¯  1:×Ô¶¯  (1:Ç¿ÖÆÈ«¿ª/×î´óÁÁ¶È)
    unsigned char            ucNearGroup1_en;                    // Ä£×é1            0:¹Ø±Õ  1:¿ªÆô
    unsigned char            ucNearGroup1_grade;                 // Ä£×é1ÁÁ¶ÈÖµ
    unsigned char            ucFarGroup1_en;                     // Ä£×é2            0:¹Ø±Õ  1:¿ªÆô
    unsigned char            ucFarGroup1_grade;                  // Ä£×é2ÁÁ¶ÈÖµ
} SDK_V2_SMART_IR;


typedef struct
{
    unsigned char            ucDayNightMode;                     // ÈÕÒ¹ÇÐ»»Ä£Ê½                     0:×Ô¶¯  1:Ç¿ÖÆ²ÊÉ«  2:Ç¿ÖÆºÚ°×  3:¶¨Ê±
    //unsigned char            ucAutoSwitchSensitivity;            // ×Ô¶¯Ä£Ê½ÇÐ»»µÄÁéÃô¶È             0:µÍ  1:ÖÐ  2:¸ß
    //unsigned char            ucAutoSwitchDelayTick;              // ×Ô¶¯Ä£Ê½ÇÐ»»µÄÑÓ³ÙÊ±¼ä (µ¥Î»:ms)
    //unsigned char            szDayStartTime[32];                 // ²ÊÉ«Ä£Ê½¿ªÊ¼Ê±¼ä
    //unsigned char            szNightStartTime[32];               // ºÚ°×Ä£Ê½¿ªÊ¼Ê±¼ä
    unsigned char            ucInfraredMode;                     // (·ÇÖÇÄÜ)ºìÍâµÆÄ£Ê½               0:¹Ø±Õ  1:×Ô¶¯
    SDK_V2_SMART_IR          stSmartIr;                          // ÖÇÄÜºìÍâµÆ (Ò»°ã²»»áÓë·ÇÖÇÄÜºìÍâ¹²´æ)

} SDK_V2_DAYNIGHT_SET;
//====================================================================


//===========================ÆØ¹â²¹³¥Ä£Ê½=============================
typedef struct
{
    unsigned char            ucExposureCompenMode;               // 0:¹Ø±Õ  1:WDR  2:HLC  3:BLC  4:WDR&HLC  5:WDR&BLC  6:HLC&BLC  7:WDR&HLC&BLC
    unsigned char            ucWdrMode;                          // ¿í¶¯Ì¬Ä£Ê½           0:ÊÖ¶¯  1:×Ô¶¯
    unsigned char            ucWdrValue;                         //                      ÉèÖÃµÄÖµÎª0~2
    unsigned char            ucHlcZoneMode;                      // Ç¿¹âÒÖÖÆÄ£Ê½         0:ÊÖ¶¯  1:×Ô¶¯
    unsigned char            ucHlcZone;
    unsigned char            ucHlcLevel;                         //                      ÉèÖÃµÄÖµÎªx~y
    //unsigned char            szHlcStartTime[32];
    //unsigned char            szHlcStopTime[32];
    unsigned char            ucBlcZoneMode;                      // ±³¹â²¹³¥(ÇøÓò)Ä£Ê½   0:ÊÖ¶¯  1:×Ô¶¯
    unsigned char            ucBlcZone;                          // ±³¹â²¹³¥ÇøÓò         0:ÉÏ  1:ÏÂ  2:×ó  3:ÓÒ  4:ÖÐ
    unsigned char            ucBlcLevel;                         // ±³¹â²¹³¥µÈ¼¶         ÉèÖÃµÄÖµÎªx~y

} SDK_V2_EXPORSURE_COMPEN;
//====================================================================


//============================°×Æ½ºâÅäÖÃ==============================
typedef struct
{
    unsigned char            ucWhiteBalenceMode;                 // 0:ÊÖ¶¯  1:×Ô¶¯  2:×ÔÈ»¹â  3:ÄÆ¹âµÆ  4:°×³ãµÆ  5:ÇçÌì  6:ÒõÌì
    unsigned char            ucRGain;                            // ºìÔöÒæÖµ
    unsigned char            ucBGain;                            // À¶ÔöÒæÖµ

} SDK_V2_WHITE_BALENCE_SET;
//====================================================================


//==============================½µÔëÅäÖÃ==============================
typedef struct
{
    unsigned char            ucDenoise_2D_en;                    // 0:¹Ø±Õ  1:¿ªÆô
    unsigned char            ucDenoise_2D;                       // 2D½µÔëÖµ
} SDK_V2_DENOISE_2D;

typedef struct
{
    unsigned char            ucDenoise_3D_en;                    // 0:¹Ø±Õ  1:¿ªÆô
    unsigned char            ucDenoise_3D;                       // 3D½µÔëÖµ
} SDK_V2_DENOISE_3D;


typedef struct
{
    unsigned char            ucDenoiseMode;                      // 0:ÊÖ¶¯  1:×Ô¶¯  2:µÍÔëÉù  3:µÍÍÏÓ°
    SDK_V2_DENOISE_2D        stDenoise_2D;                       // 2D½µÔë
    SDK_V2_DENOISE_3D        stDenoise_3D;                       // 3D½µÔë

} SDK_V2_DENOISE_SET;
//====================================================================


//============================ÌØÐÔ¹¦ÄÜÅäÖÃ============================
typedef struct
{
    unsigned char            ucDeFogMode;                        // Í¸Îí             0:¹Ø±Õ  1:¿ªÆô
    unsigned char            ucDeFogLevel;                       // Í¸ÎíµÈ¼¶
} SDK_V2_DEFOG;

typedef struct
{
    unsigned char            ucDistortionCorrMode;              // Í¸Îí             0:¹Ø±Õ  1:¿ªÆô
    unsigned char            ucCorrectionLevel;                 // Í¸ÎíµÈ¼¶
} SDK_V2_DISTORTION_CORR;

typedef struct
{
    unsigned char            ucVideoStandard;                    // ÊÓÆµÖÆÊ½         0:NTSL  1:PAL
    unsigned char            ucMirrorMode;                       // ÊÓÆµ¾µÏñ         0:¹Ø±Õ  1:¿ªÆô
    unsigned char            ucFlipMode;                         // ÊÓÆµ·­×ª         0:¹Ø±Õ  1:¿ªÆô
    unsigned char            ucRotateValue;                      // ÊÓÆµÐý×ª         0:0¶È  1:90¶È  2:180¶È  3:270¶È
    unsigned char            ucHighFrameMode;                    // ¸ßÖ¡ÂÊ           0:¹Ø±Õ  1:¿ªÆô
    SDK_V2_DEFOG             stDefog;                            // Í¸Îí
    unsigned char            ucOpticalAntiShakeMode;             // ·À¶¶             0:¹Ø±Õ  1:¿ªÆô
    unsigned char            ucVisibilityEnhancerMode;           // ¿ÉÊÓÐÔÔöÇ¿       0:¹Ø±Õ  1:¿ªÆô
    unsigned char            ucDistortionCorrectionMode;         // »û±ä½ÃÕý         0:¹Ø±Õ  1:¿ªÆô

} SDK_V2_FEATUR_FUNCTION_SET;
//====================================================================

typedef struct _sdk_send_image_param
{
    SDK_IMAGE_V2_STATUS_E enSatus;
    SDK_PRODUCT_TYPE_ID    product_id;
    int  reserve[2];
} SDK_SEND_IMAGE_PARAM;

/*
ÀúÊ·ÒÅÁôÔ­Òò£¬Ó²¼þÈËÔ±¶ÔÍâ¿ÇÃüÃû·½Ê½²»¹æ·¶
1¡¢½ö´ÓÃû×Ö¶ÔÍâ¿Ç½øÐÐÊ¶±ðÈÝÒ×³ö´í
£¨1£©ÓÐµÄÍâ¿Ç·ÖÎª»ù±¾ÐÍºÍÑÜÉúÐÍ£¬±ÈÈç£¬CD45C_1ÊÇCD45CµÄÑÜÉú£¬¹¦ÄÜµã¾ùºÍCD45CÒ»ÖÂ£»
£¨2£©CR24ºÍCR24_1²»ÊÇÑÜÉúÐÍºÅ¹ØÏµ£¬ÕÕÃ÷·½°¸ÍêÈ«²»Ò»Ñù£»
2¡¢¶ÔÃ¿¸öÍâ¿Ç·ÖÅäÒ»¸öÃ¶¾ÙÖµ£¬¾ÍËãÊÇÑÜÉúÍâ¿Ç£¬Ò²¶ÀÁ¢·ÖÅä¶ÔÓ¦Ã¶¾ÙÖµ£¬²»ÄÜ½èÓÃÆäËûÍâ¿Ç½øÐÐ´¦Àí
3¡¢ÓÐµÄÍâ¿ÇÓÐ¶¨½¹¡¢±ä½¹Ö®·Ö£¬Òò´ËÃüÃûÉÏÒ²×öÁËÇø·Ö£¬±ÈÈç£º
CR24_1£º¶¨½¹
CR24_1_FZ£ºÊÖ¶¯±ä½¹
CR24_1_AZ£º×Ô¶¯±ä½¹
*/
typedef enum sdk_model_id_e
{
    SDK_MODEL_ID_NONE = 0,
    SDK_MODEL_ID_CD45 = 1,
    SDK_MODEL_ID_CD54 = 2,
    SDK_MODEL_ID_CD14 = 3,
    SDK_MODEL_ID_CD37 = 4,
    SDK_MODEL_ID_CR24 = 5,
    SDK_MODEL_ID_CR34 = 6,
    SDK_MODEL_ID_CR14 = 7,
    SDK_MODEL_ID_CR15 = 8,
    SDK_MODEL_ID_CD22 = 9,
    SDK_MODEL_ID_CR22 = 10,
    SDK_MODEL_ID_CD13C = 11,
    SDK_MODEL_ID_CB23 = 12,
    SDK_MODEL_ID_CA31 = 13,
    SDK_MODEL_ID_CR24_1_FZ = 14,
    SDK_MODEL_ID_CD71 = 15,
    SDK_MODEL_ID_CR24_1_AZ = 16,
    SDK_MODEL_ID_CR34_I4 = 17,
    SDK_MODEL_ID_CD54_DIB = 18,
    SDK_MODEL_ID_CD24 = 19,
    SDK_MODEL_ID_CR73 = 20,
    SDK_MODEL_ID_CD38 = 21,
    SDK_MODEL_ID_CR43 = 22,
    SDK_MODEL_ID_CD47 = 23,
    SDK_MODEL_ID_CR63 = 24,
    SDK_MODEL_ID_CD54E = 25,
    SDK_MODEL_ID_CD54E_1 = 26,
    SDK_MODEL_ID_CD35E = 27,
    SDK_MODEL_ID_CR34C = 28,
    SDK_MODEL_ID_CR34_1 = 29,
    SDK_MODEL_ID_CB23A = 30,
    SDK_MODEL_ID_CR63A_FZ = 31,
    SDK_MODEL_ID_CR63A_AZ = 32,
    SDK_MODEL_ID_CR24G = 33,
    SDK_MODEL_ID_CD54I = 34,
    SDK_MODEL_ID_CR24_1 = 35, //CR24_1ÖÐµÄ¶¨½¹
    SDK_MODEL_ID_CR24C = 36,
    SDK_MODEL_ID_CR24C_AZ = 37,
    SDK_MODEL_ID_CR24C_FZ = 38,
    SDK_MODEL_ID_CR34C_1 = 39,
    SDK_MODEL_ID_CD14A = 40,
    SDK_MODEL_ID_CD13A = 41,
    SDK_MODEL_ID_CD45C = 42,
    SDK_MODEL_ID_CD45C_1 = 43,
    SDK_MODEL_ID_CD54F = 44,
    SDK_MODEL_ID_CD54F_1 = 45,
    SDK_MODEL_ID_CR34_TC = 46,
    SDK_MODEL_ID_CD35F = 47,
    SDK_MODEL_ID_CD71A = 48,
    SDK_MODEL_ID_CD23 = 49,
    SDK_MODEL_ID_CD45E = 50,
    SDK_MODEL_ID_CR63A = 51, //CR63A¶¨½¹»úÐÍ
} SDK_MODEL_ID_E;

typedef enum sdk_model_lens_e
{
    SDK_MODEL_LENS_NONE = 0,
    SDK_MODEL_LENS_FZ,
    SDK_MODEL_LENS_AZ,
} SDK_MODEL_LENS_E;

#define SDK_HW_SMART_IR_MAX     (5)

typedef struct sdk_hw_smart_ir_cfg_s
{
    unsigned char mode;                         // 0 -- close, 1 - manu, 2 -- auto
    unsigned char value[SDK_HW_SMART_IR_MAX];   // 0 ~ 100
    unsigned char sync_with_zoom;               // valid for az device
    unsigned char close_far_ir;                 // valid for fz device
} SDK_HW_SMART_IR_CFG_S;


#define SDK_WHITE_LIGHT_NUM_MAX 4

typedef enum
{
    SDK_WHITE_LIGHT_MODE_CLOSE = 0,
    SDK_WHITE_LIGHT_MODE_OPEN,
    SDK_WHITE_LIGHT_MODE_AUTO ,

    SDK_WHITE_LIGHT_MODE_BUTT,
} SDK_WHITE_LIGHT_MODE_E;

typedef struct sdk_white_light_cfg_s
{
    SDK_WHITE_LIGHT_MODE_E mode;
    unsigned int strength[SDK_WHITE_LIGHT_NUM_MAX];  //  0 ~ 100
    int  reserved[4];
} SDK_WHITE_LIGHT_CFG_S;

typedef enum _sdk_osd_color_mode
{
    SDK_OSD_COLOR_MODE_STROKE = 0,   // Ä¬ÈÏ£¬Ãè±ß
    SDK_OSD_COLOR_MODE_AUTO = 1,     // ×ÔÊÊÓ¦±³¾°
    SDK_OSD_COLOR_MODE_ARGB1555 = 2, // ×Ô¶¨ÒåÑÕÉ«
} SDK_OSD_COLOR_MODE;

typedef enum _sdk_osd_align_type
{
    SDK_OSD_ALIGN_TYPE_LEFT = 0,
    SDK_OSD_ALIGN_TYPE_MIDDLE = 1,
    SDK_OSD_ALIGN_TYPE_RIGHT = 2,
} SDK_OSD_ALIGN_TYPE;

typedef struct _sdk_osd_attribute
{
    SDK_OSD_COLOR_MODE  eColorMode;
    int                 dwColorCode;     // only valid when eColorMode not equal to SDK_OSD_COLOR_MODE_AUTO
    int                 dwTransparency;  // 0~100
    SDK_OSD_ALIGN_TYPE  eAlignType;
    int                 reserved[4];
} SDK_OSD_ATTRIBUTE;

typedef struct _sdk_usb_disk_info
{
    int             iUsbStatus;     // 0Î´²åÈë  1ÒÑ¸ñÊ½»¯  2¸ñÊ½»¯ÖÐ  3Î´¸ñÊ½»¯
    unsigned int    iTotalKBytes;
    unsigned int    iUsedKBytes;
    unsigned int    iRemainKBytes;
    unsigned int    resv[4];
} SDK_USB_DISK_INFO;

//AVS PIPE JPG
typedef struct
{
    unsigned avs_pipe_id;               //PIPEµÄÍ¨µÀºÅ
    IPCAMERA_AV_FRAME_INFO FrameInfo;      //»ñÈ¡µ½µÄJPG
} AVS_PIPE_AV_FRAME;

#define MAX_SD_PART_NAME_SZIE 32 //SD¿¨·ÖÇøÃû×Ö×î´ó³¤¶È
typedef struct
{
    int existPartition;
    unsigned int part_size;
    unsigned char str_part_name[MAX_SD_PART_NAME_SZIE];
    int resv[4];
} SDK_USE_SD_PART_INFO;

typedef struct _sdk_video_norm_config
{
    SDK_VIDEO_FORMAT video_format;
    int video_src_width;
    int video_src_height;
    int video_src_fps;
    int max_video_num;
} SDK_VIDEO_NORM_CONFIG;

typedef struct
{
    int   mode;  // 0 ÊäÈë; 1 Êä³ö
    SDK_WG_BIT_MODE  enBitNum;
    int resv[8];
} SDK_WG_CFG_PARA;

#endif

