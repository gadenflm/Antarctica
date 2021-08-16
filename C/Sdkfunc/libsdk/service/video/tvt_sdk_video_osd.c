#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "tvt_sdk_video_osd.h"
#include "tvt_sdk_print.h"
#include "tvt_sdk_chip.h"
#include "GregorianToPersian.h"

/// TODO: 不同平台下支持OverLay的格式会有差异，
//        若业务层统一成一种格式，再下发到硬件层，由硬件层转换的话可能比较影响性能以及带来开发难度
//        因此尽量在业务层内决定位图格式后在传入硬件层直接叠加，目前是使用了平台宏区分，支持ARGB1555以及CLUT8
//        后续可以由参数文件决定用的什么格式(包括对齐)，这边再兼容

/*!
 * @macros ROUND_UP(x,d)
 * @brief round up func.
 */
#define ROUND_UP(x, n)  ( ((x)+(n)-1u) & ~((n)-1u) )

#define TVT_SDK_OSD_DEBUG 0

#ifdef CHIP_AMBA
#define OSD_ALIGN_W 32
#define OSD_ALIGN_H 4
#else
#define OSD_ALIGN_W 16
#define OSD_ALIGN_H 2
#endif

#define TVT_SDK_OSD_MAX_LINE 2
#define TVT_SDK_TIME_STR_MAX_LENGTH   128

#define ARGB1555_COLOR_WHITE 0xFFFF
#define ARGB1555_COLOR_BLACK 0x8000
#define ARGB1555_COLOR_NULL 0x0
#define CLUT8_COLOR_NULL ARGB1555_COLOR_NULL
#define CLUT8_COLOR 0x1
#define CLUT8_COLOR_BLACK 0x2

#define TIME_OSD_REF_PIC_W  352     // 不缩放时间戳所在画面宽
#define TIME_OSD_REF_PIC_H  240     // 不缩放时间戳所在画面高

#define APP_OSD_REF_PIC_W  10000    // 应用层下发的位置所在画面宽
#define APP_OSD_REF_PIC_H  10000    // 应用层下发的位置所在画面高

#define TIME_OSD_FONT_W  8
#define TIME_OSD_FONT_H  12

#define COMM_OSD_FONT_W  8

#define TVT_SDK_OSD_SIGNDATA_NUM 17
static const unsigned char g_szSignData[ ][TIME_OSD_FONT_H + 1] =
{
    {0x00, 0x00, 0x3c, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00, '0'}, /*'0'*/
    {0x00, 0x00, 0x18, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00, '1'}, /*'1'*/
    {0x00, 0x00, 0x3c, 0x66, 0x66, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x7e, 0x00, '2'}, /*'2'*/
    {0x00, 0x00, 0x3c, 0x66, 0x06, 0x1c, 0x06, 0x06, 0x06, 0x66, 0x3c, 0x00, '3'}, /*'3'*/
    {0x00, 0x00, 0x0c, 0x1c, 0x3c, 0x6c, 0x6c, 0xcc, 0x7c, 0x0c, 0x1e, 0x00, '4'}, /*'4'*/
    {0x00, 0x00, 0x7e, 0x60, 0x60, 0x7c, 0x66, 0x06, 0x66, 0x66, 0x3c, 0x00, '5'}, /*'5'*/
    {0x00, 0x00, 0x1e, 0x36, 0x60, 0x7c, 0x76, 0x66, 0x66, 0x66, 0x3c, 0x00, '6'}, /*'6'*/
    {0x00, 0x00, 0x7e, 0x6c, 0x0c, 0x0c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, '7'}, /*'7'*/
    {0x00, 0x00, 0x3c, 0x66, 0x66, 0x3c, 0x3c, 0x66, 0x66, 0x66, 0x3c, 0x00, '8'}, /*'8'*/
    {0x00, 0x00, 0x3c, 0x66, 0x66, 0x66, 0x6e, 0x3e, 0x06, 0x6c, 0x78, 0x00, '9'}, /*'9'*/
    {0x00, 0x00, 0x0c, 0x0c, 0x18, 0x18, 0x30, 0x30, 0x60, 0x60, 0xc0, 0x00, '/'}, /*'/'*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x00, ':'}, /*':'*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, ' '}, /*' '*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, '-'}, /*'-'*/
    {0x00, 0x00, 0x00, 0x38, 0x38, 0x6C, 0x6C, 0x6C, 0xFE, 0xC6, 0xC6, 0x00, 'a'}, /*a*/
    {0x00, 0x00, 0x00, 0xFC, 0xC6, 0xC6, 0xC6, 0xFC, 0xC0, 0xC0, 0xC0, 0x00, 'p'}, /*p*/
    {0x00, 0x00, 0x00, 0xE7, 0xE7, 0xFF, 0xFF, 0xDB, 0xDB, 0xC3, 0xC3, 0x00, 'm'}, /*m*/
};


/**
 * @brief 业务状态
 */
typedef enum tvt_sdk_osd_status_e
{
    TVT_SDK_OSD_RUN = 0,
    TVT_SDK_OSD_PAUSE,
    TVT_SDK_OSD_STATUS_BUTT,
} TVT_SDK_OSD_STATUS_E;

/**
 * @brief 显示位图填充信息
 */
typedef struct tvt_sdk_osd_fill_info_s
{
    CHIP_OSD_BUFF_TYPE_E    enFillType; ///< 填充格式
    int             s32SrcW;    ///< 点阵数据宽
    int             s32SrcH;    ///< 点阵数据高
    int             s32DstW;    ///< 目标位图宽
    int             s32DstH;    ///< 目标位图高
    unsigned int    u32Color;   ///< 填充具体颜色
    unsigned char   u8OutLine;  ///< 描边
} TVT_SDK_OSD_FILL_INFO_S;

/**
 * @brief OSD叠加内容信息
 */
typedef struct tvt_sdk_osd_overlay_s
{
    int                         s32Handle;       ///< OSD句柄
    SDK_POSITION                stDisOSDPos;     ///< 最终显示的位置
    unsigned char*              pBuffAddr;       ///< 不同编码流下的位图内容首地址(虚拟地址)
    unsigned long long          u64BuffPhyAddr;  ///< 不同编码流下的位图内容首地址(物理地址)
    unsigned int                u32BuffSize;     ///< 位图长度
} TVT_SDK_OSD_OVERLAY_S;


/**
 * @brief OSD内容信息
 */
typedef struct tvt_sdk_osd_data_info_s
{
    unsigned int                u32IsConfigured;                    ///< 已配置
    unsigned int                u32Enable;                          ///< 使能
    unsigned int                u32NeedFresh;                       ///< 刷新配置标志位
    TVT_SDK_OSD_TYPE_E          enOSDType;                          ///< 类型
    unsigned char               szOSDContent[TVT_SDK_TIME_STR_MAX_LENGTH];  ///< 点阵对应字符串内容
    unsigned int                u32StrLen;                          ///< 字符长度
    unsigned char*              pu8BitBuff;                         ///< 点阵数据首地址
    SDK_POSITION                stSrcOSDPos;                        ///< 相对位置
    TVT_SDK_OSD_OVERLAY_S       stOverLayInfo[CHIP_MAX_VENC_NUM_PER_DEV + CHIP_MAX_VO_NUM];   ///< 叠加信息
} TVT_SDK_OSD_DATA_INFO_S;



/**
 * @brief OSD业务全局配置
 */
typedef struct tvt_sdk_osd_cfg_s
{
    int                         s32VinDev;          ///< OSD所在视频输入设备号
    TVT_SDK_OSD_TIME_FMT_S      stTimeFmt;          ///< 时间OSD配置
    unsigned char               u8TransLevel;       ///< 0:不使能透明；1-255:透明度
    unsigned char               u8Flick;            ///< 0:不使能闪烁；other:每2秒闪烁
    unsigned char               u8AdaptBg;          ///< 自适应黑白 0:不使能；1:使能
    TVT_SDK_OSD_SIZE_E          enSize;             ///< OSD 字体大小
    unsigned int                u32Colour;          ///< OSD 颜色，与自适应黑白互斥
    TVT_SDK_OSD_ALIGN_MODE_E    enAlignMode;        ///< 对齐模式
    unsigned int                u32Margins;         ///< 国标对齐模式边距，单位：OSD Size
    TVT_SDK_OSD_DATA_INFO_S     stOSDInfoArr[TVT_SDK_OSD_INDEX_BUTT];   ///< 每个OSD的具体信息
} TVT_SDK_OSD_CFG_S;


/**
 * @brief 业务结构体
 */
typedef struct tvt_sdk_osd_info_s
{
    char                        u8Init;                                 ///< 初始化标志位
    char                        u8Exit;                                 ///< 去初始化标志位，线程退出标志
    TVT_SDK_OSD_STATUS_E        enStatus;                               ///< 业务运行状态
    pthread_mutex_t             s32Mutex;                               ///< 业务锁
    char                        szMutexName[32];                        ///< 业务锁名称
    unsigned int                u32MutexLockCnt;                        ///< 业务锁锁定次数
    TVT_SDK_OSD_CFG_S           stOSDCfg[CHIP_MAX_VI_NUM];              ///< OSD业务全局配置
} TVT_SDK_OSD_INFO_S;

static TVT_SDK_OSD_INFO_S* g_pstSdkOSDInfo = NULL;


/**
 * @brief 获取字符对应的点阵数据
 */
static inline const unsigned char* getSignData(unsigned char u8Ch)
{
    const unsigned char* pu8SignPosition = g_szSignData[12]; // 默认给空格

    int i = 0;
    for (i = 0; i < TVT_SDK_OSD_SIGNDATA_NUM; i++)
    {
        unsigned char u8SignChar = g_szSignData[i][TIME_OSD_FONT_H];
        if (u8Ch == u8SignChar)
        {
            pu8SignPosition = g_szSignData[i];
            break;
        }
    }

    return pu8SignPosition;
}

/**
 * @brief 获取模块结构体指针
 */
static TVT_SDK_OSD_INFO_S* tvt_sdk_osd_getHandle(void)
{
    return g_pstSdkOSDInfo;
}

/**
 * @brief 获取时间字符串
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_osd_getTimeStr(const TVT_SDK_OSD_TIME_FMT_S* pstTimeFmt, char* pszTimeString)
{
    time_t stNow;
    struct tm stNowTmTime ;
    struct tm* pstNowTime = &stNowTmTime;
    char szTempApm[3] = {'\0'};
    if (NULL == pszTimeString)
    {
        TIPC_PRINTF_ERR("NULL pointer");
    }

    // 获取当前时间
    time(&stNow);
    localtime_r(&stNow, &stNowTmTime);
    pstNowTime->tm_year += 1900;
    pstNowTime->tm_mon += 1;
    if (TVT_SDK_OSD_TIME_PERSIAN == pstTimeFmt->enCalendar)
    {
        // 公历转波斯历
        unsigned int u32Year;
        unsigned int u32Month;
        unsigned int u32Day;
        GregorianToPersian(pstNowTime->tm_year, pstNowTime->tm_mon, pstNowTime->tm_mday, \
                           &u32Year, &u32Month, &u32Day);
        pstNowTime->tm_year = u32Year;
        pstNowTime->tm_mon = u32Month;
        pstNowTime->tm_mday = u32Day;
    }

    // 生成字符串
    char s8ConnectSymbol = '-';
    // 决定横杆还是斜杆
    /// TODO: 横杆转“年月日”未实现，支持的话如何判断添加的是英文的还是中文的？
    switch (pstTimeFmt->enDateFormatEx)
    {
    case TVT_SDK_OSD_DATE_CROSSBAR:
        s8ConnectSymbol = '-';
        break;
    case TVT_SDK_OSD_DATE_CROSSBAR_TO_SLASH:
        s8ConnectSymbol = '/';
        break;
    default:
        break;
    }
    // 根据当前12/24小时制确定Hour
    if (TVT_SDK_OSD_TIME_12HOUR_FORMAT == pstTimeFmt->enTimeFormat)
    {
        if (pstNowTime->tm_hour >= 12 && pstNowTime->tm_hour < 24)
        {
            int s32TempHour = 0;
            s32TempHour = (pstNowTime->tm_hour > 12) ? (pstNowTime->tm_hour - 12) : pstNowTime->tm_hour;
            pstNowTime->tm_hour = s32TempHour;
            strcpy(szTempApm, "pm");
        }
        else if (pstNowTime->tm_hour >= 0 && pstNowTime->tm_hour < 12)
        {
            int s32TempHour = 0;
            s32TempHour = (0 == pstNowTime->tm_hour) ? 12 : pstNowTime->tm_hour;
            pstNowTime->tm_hour = s32TempHour;
            strcpy(szTempApm, "am");
        }
        else
        {
            TIPC_PRINTF_ERR("error: hour=%d", pstNowTime->tm_hour);
        }
    }
    // 决定排列
    switch (pstTimeFmt->enDateFormat)
    {
    case TVT_SDK_OSD_DATE_MM_DD_YYYY:
    {
        snprintf(pszTimeString, TVT_SDK_TIME_STR_MAX_LENGTH, "%.2d%c%.2d%c%.4d %.2d:%.2d:%.2d", pstNowTime->tm_mday, s8ConnectSymbol,
                 pstNowTime->tm_mon, s8ConnectSymbol, pstNowTime->tm_year, pstNowTime->tm_hour, pstNowTime->tm_min, pstNowTime->tm_sec);
    }
    break;
    case TVT_SDK_OSD_DATE_YYYY_MM_DD:
    {
        snprintf(pszTimeString, TVT_SDK_TIME_STR_MAX_LENGTH, "%.4d%c%.2d%c%.2d %.2d:%.2d:%.2d", pstNowTime->tm_year, s8ConnectSymbol,
                 pstNowTime->tm_mon, s8ConnectSymbol, pstNowTime->tm_mday, pstNowTime->tm_hour, pstNowTime->tm_min, pstNowTime->tm_sec);
    }
    break;
    case TVT_SDK_OSD_DATE_DD_MM_YYYY:
    {
        snprintf(pszTimeString, TVT_SDK_TIME_STR_MAX_LENGTH, "%.2d%c%.2d%c%.4d %.2d:%.2d:%.2d", pstNowTime->tm_mon, s8ConnectSymbol,
                 pstNowTime->tm_mday, s8ConnectSymbol, pstNowTime->tm_year, pstNowTime->tm_hour, pstNowTime->tm_min, pstNowTime->tm_sec);
    }
    break;
    default :
    {
        snprintf(pszTimeString, TVT_SDK_TIME_STR_MAX_LENGTH, "%.2d%c%.2d%c%.4d %.2d:%.2d:%.2d", pstNowTime->tm_mday, s8ConnectSymbol,
                 pstNowTime->tm_mon, s8ConnectSymbol, pstNowTime->tm_year, pstNowTime->tm_hour, pstNowTime->tm_min, pstNowTime->tm_sec);
    }
    break;
    }

    // 决定是否填充am pm
    if (TVT_SDK_OSD_TIME_12HOUR_FORMAT == pstTimeFmt->enTimeFormat)
    {
        strcat(pszTimeString, szTempApm);
    }

    return 0;
}

/**
 * @brief 计算更新OSD位图数据位置
 * @param pstVencSize       对应编码通道的分辨率
 * @param pstSrcPos         应用层传入相对的位置
 * @param pstBitDataSize    点阵数据的尺寸
 * @param pstDstPos         返回计算后OSD位图数据的位置
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_osd_caluDispOSDPos(const SDK_POSITION* pstVencSize, const SDK_POSITION* pstSrcPos,
                                      const SDK_POSITION* pstBitDataSize, SDK_POSITION* pstDstPos)
{
    float f32FontZoomW = 1.0, f32FontZoomH = 1.0, f32ConvW = 0.0, f32ConvH = 0.0;
    int s32ConvW = 0, s32ConvH = 0;

    // 点阵数据的宽高
    int s32OrigW = pstBitDataSize->width;
    int s32OrigH = pstBitDataSize->height;

    // 编码流的宽高
    int s32PicW = pstVencSize->width;
    int s32PicH = pstVencSize->height;

    // 计算宽高比
    float f32ImageRate = (float)s32PicW / (float)s32PicH;
    if (f32ImageRate > 1.5)
    {
        f32FontZoomH = (float)s32PicH / TIME_OSD_REF_PIC_H;
        f32FontZoomW = f32FontZoomH; // 保持比例不拉伸
    }
    else
    {
        f32FontZoomW = (float)s32PicW / TIME_OSD_REF_PIC_W;
        f32FontZoomH = f32FontZoomW; // 保持比例不拉伸
    }

    // 再缩小一些，这里是按照之前定制项目的经验值
    f32FontZoomW = f32FontZoomW / 1.2;
    f32FontZoomH = f32FontZoomH / 1.2;

    // 至少也要保持一倍，否则不好看
    f32FontZoomW = (f32FontZoomW < 1) ? 1 : f32FontZoomW;
    f32FontZoomH = (f32FontZoomH < 1) ? 1 : f32FontZoomH;

    // 得到转换后的宽高
    f32ConvW = f32FontZoomW * s32OrigW;
    f32ConvH = f32FontZoomH * s32OrigH;

    s32ConvW = (int)f32ConvW + 1;
    s32ConvH = (int)f32ConvH + 1;

    // 字节对齐
    // int s32Align = s32OrigW / TIME_OSD_FONT_W;
    // s32ConvW = (s32ConvW + ((s32Align * 2) - s32ConvW % (s32Align * 2)) % (s32Align * 2));
    s32ConvW = ROUND_UP(s32ConvW, OSD_ALIGN_W);
    s32ConvH = ROUND_UP(s32ConvH, OSD_ALIGN_H);

    if (s32ConvW > s32PicW || s32ConvH > s32PicH)
    {
        TIPC_PRINTF_ERR("convert size [%d x %d] large than encode size [%d x %d]", s32ConvW, s32ConvH, s32PicW, s32PicH);
        return -1;
    }

    int s32SrcPosX = pstSrcPos->x;
    int s32SrcPosY = pstSrcPos->y;
    int s32DstPosX = ((s32SrcPosX) * (s32PicW)) / APP_OSD_REF_PIC_W;
    int s32DstPosY = ((s32SrcPosY) * (s32PicH)) / APP_OSD_REF_PIC_H;

    // 防越界
    if ((s32DstPosX + s32ConvW) > s32PicW)
    {
        s32DstPosX = s32PicW - s32ConvW;
    }
    if ((s32DstPosY + s32ConvH) > s32PicH)
    {
        s32DstPosY = s32PicH - s32ConvH;
    }
    s32DstPosX = (s32DstPosX < 0) ? 0 : s32DstPosX;
    s32DstPosY = (s32DstPosY < 0) ? 0 : s32DstPosY;

#ifdef CHIP_AMBA
    s32DstPosX = (s32DstPosX < 4) ? 4 : s32DstPosX;
    s32DstPosY = (s32DstPosY < 4) ? 4 : s32DstPosY;
#endif

    // 4字节对齐
    s32DstPosX &= 0xFFFFFFFC;
    s32DstPosY &= 0xFFFFFFFC;

    // 赋值
    pstDstPos->x = s32DstPosX;
    pstDstPos->y = s32DstPosY;
    pstDstPos->width = s32ConvW;
    pstDstPos->height = s32ConvH;

    return 0;
}

/**
 * @brief 填充时间OSD 显示位图数据
 * @param s32StartIDX    填充的显示位图数据所属字符串偏移量
 * @param s32CharLens    显示位图数据所属字符串长度
 * @param pu8ImgData     显示位图数据数据头
 * @param pszBitData     显示位图数据对应字符串点阵图
 * @param pstFillInfo    填充信息
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_osd_fillTimeOSDBuff(int s32StartIDX, int s32CharLens, void* pImgData, const unsigned char* pszBitData,
                                       TVT_SDK_OSD_FILL_INFO_S* pstFillInfo)
{
    int s32StartPoint = 0;
    int i = 0, j = 0;

    int s32BitDataW = pstFillInfo->s32SrcW;
    int s32BitDataH = pstFillInfo->s32SrcH;

    int s32Width = pstFillInfo->s32DstW;
    int s32Height = pstFillInfo->s32DstH;

    if (s32CharLens <= 0)
    {
        TIPC_PRINTF_ERR("length of char %d err", s32CharLens);
        return -1;
    }

    unsigned char* pu8ImgData = (unsigned char*)pImgData;
    unsigned short* pu16ImgData = (unsigned short*)pImgData;

    /*
        对于每个字符，判断它的10个char型的每一位，如果是1,
        设置这一位对应的扩大区域为颜色(0xXXXX)
        否则，设置其对应区域为透明(0)
    */
    // 计算CLUT8数据起始位置
    s32StartPoint = s32StartIDX * s32Width / s32CharLens;

    // 计算放大倍数
    float f32WidthRate = (float)s32Width / (float)(s32CharLens * s32BitDataW);
    float f32HeightRate = (float)s32Height / (float)s32BitDataH;

    // 填充
    int s32ClutCharWTmp = s32Width / s32CharLens; /// 位图数据内，这个字符的长度
    for (i = 0; i < s32Height; i++)
    {
        for (j = 0; j < s32ClutCharWTmp; j++)
        {
            // 计算对应点阵的点
            float f32CurBitHeight = i / f32HeightRate;
            int s32CurBitHeight = (int)f32CurBitHeight;
            if (s32CurBitHeight >= s32BitDataH)
            {
                // 防越界
                s32CurBitHeight = (s32BitDataH - 1);
            }

            float f32CurBitWidth = j / f32WidthRate;
            int s32CurBitWidth = (int)f32CurBitWidth;
            if (s32CurBitWidth >= s32BitDataW)
            {
                // 防越界
                s32CurBitWidth = (s32BitDataW - 1);
            }

            // 根据点阵的点的值判断
            if (pszBitData[s32CurBitHeight] & (0x01 << (7 - s32CurBitWidth)))
            {
                if (CHIP_OSD_BUFF_CLUT8 == pstFillInfo->enFillType)
                {
                    pu8ImgData[s32StartPoint + (i * s32Width) + j] = CLUT8_COLOR;
                }
                else
                {
                    pu16ImgData[s32StartPoint + (i * s32Width) + j] = (unsigned short)pstFillInfo->u32Color;
                }
            }
            else
            {
                if (CHIP_OSD_BUFF_CLUT8 == pstFillInfo->enFillType)
                {
                    pu8ImgData[s32StartPoint + (i * s32Width) + j] = CLUT8_COLOR_NULL;
                }
                else
                {
                    pu16ImgData[s32StartPoint + (i * s32Width) + j] = ARGB1555_COLOR_NULL;
                }
            }

        }
    }

    // 描边处理
    if (0 == pstFillInfo->u8OutLine)
    {
        return 0;
    }
    if ((CHIP_OSD_BUFF_ARGB1555 == pstFillInfo->enFillType) && (ARGB1555_COLOR_WHITE == (unsigned short)pstFillInfo->u32Color))
    {
        return 0;
    }

    /// TODO: 后面可能需要根据传来的s32OutLine值确认描边的厚度
    /// TODO: ugly code
    for (i = 1; i < (s32Height - 1); i++)
    {
        for (j = 1; j < (s32ClutCharWTmp - 1); j++)
        {
            if (CHIP_OSD_BUFF_CLUT8 == pstFillInfo->enFillType)
            {
                if (pu8ImgData[s32StartPoint + i * s32Width + j] != CLUT8_COLOR_NULL)
                {
                    continue;
                }
                if ( (CLUT8_COLOR == pu8ImgData[s32StartPoint + (s32Width * i) +  j + 1])
                        || (CLUT8_COLOR == pu8ImgData[s32StartPoint + (s32Width * i) +  j - 1])
                        || (CLUT8_COLOR == pu8ImgData[s32StartPoint + (s32Width * (i - 1)) + j - 1])
                        || (CLUT8_COLOR == pu8ImgData[s32StartPoint + (s32Width * (i - 1)) + j + 0])
                        || (CLUT8_COLOR == pu8ImgData[s32StartPoint + (s32Width * (i - 1)) + j + 1])
                        || (CLUT8_COLOR == pu8ImgData[s32StartPoint + (s32Width * (i + 1)) +  j - 1])
                        || (CLUT8_COLOR == pu8ImgData[s32StartPoint + (s32Width * (i + 1)) + j + 0])
                        || (CLUT8_COLOR == pu8ImgData[s32StartPoint + (s32Width * (i + 1)) + j + 1]))
                {
                    pu8ImgData[s32StartPoint + (i * s32Width) + j] = CLUT8_COLOR_BLACK;
                }
            }
            else
            {
                if (pu16ImgData[s32StartPoint + i * s32Width + j] != ARGB1555_COLOR_NULL)
                {
                    continue;
                }
                if ( (ARGB1555_COLOR_WHITE == pu16ImgData[s32StartPoint + (s32Width * i) +  j + 1])
                        || (ARGB1555_COLOR_WHITE == pu16ImgData[s32StartPoint + (s32Width * i) +  j - 1])
                        || (ARGB1555_COLOR_WHITE == pu16ImgData[s32StartPoint + (s32Width * (i - 1)) + j - 1])
                        || (ARGB1555_COLOR_WHITE == pu16ImgData[s32StartPoint + (s32Width * (i - 1)) + j + 0])
                        || (ARGB1555_COLOR_WHITE == pu16ImgData[s32StartPoint + (s32Width * (i - 1)) + j + 1])
                        || (ARGB1555_COLOR_WHITE == pu16ImgData[s32StartPoint + (s32Width * (i + 1)) +  j - 1])
                        || (ARGB1555_COLOR_WHITE == pu16ImgData[s32StartPoint + (s32Width * (i + 1)) + j + 0])
                        || (ARGB1555_COLOR_WHITE == pu16ImgData[s32StartPoint + (s32Width * (i + 1)) + j + 1]))
                {
                    pu16ImgData[s32StartPoint + (i * s32Width) + j] = ARGB1555_COLOR_BLACK;
                }
            }
        }
    }

    i = 0;
    for (j = 0; j < s32ClutCharWTmp; ++j)
    {
        if (CHIP_OSD_BUFF_CLUT8 == pstFillInfo->enFillType)
        {
            if (CLUT8_COLOR == pu8ImgData[s32StartPoint + (i * s32Width) + j])
            {
                pu8ImgData[s32StartPoint + (i * s32Width) + j] = CLUT8_COLOR_BLACK;
            }
        }
        else
        {
            if (ARGB1555_COLOR_WHITE == pu16ImgData[s32StartPoint + (i * s32Width) + j])
            {
                pu16ImgData[s32StartPoint + (i * s32Width) + j] = ARGB1555_COLOR_BLACK;
            }
        }
    }

    i = s32Height - 1;
    for (j = 0; j < s32ClutCharWTmp; ++j)
    {
        if (CHIP_OSD_BUFF_CLUT8 == pstFillInfo->enFillType)
        {
            if (CLUT8_COLOR == pu8ImgData[s32StartPoint + (i * s32Width) + j])
            {
                pu8ImgData[s32StartPoint + (i * s32Width) + j] = CLUT8_COLOR_BLACK;
            }
        }
        else
        {
            if (ARGB1555_COLOR_WHITE == pu16ImgData[s32StartPoint + (i * s32Width) + j])
            {
                pu16ImgData[s32StartPoint + (i * s32Width) + j] = ARGB1555_COLOR_BLACK;
            }
        }
    }

    j = 0;
    for (i = 0; i < (s32Height - 1); ++i)
    {
        if (CHIP_OSD_BUFF_CLUT8 == pstFillInfo->enFillType)
        {
            if (CLUT8_COLOR == pu8ImgData[s32StartPoint + (i * s32Width) + j])
            {
                pu8ImgData[s32StartPoint + (i * s32Width) + j] = CLUT8_COLOR_BLACK;
            }
        }
        else
        {
            if (ARGB1555_COLOR_WHITE == pu16ImgData[s32StartPoint + (i * s32Width) + j])
            {
                pu16ImgData[s32StartPoint + (i * s32Width) + j] = ARGB1555_COLOR_BLACK;
            }
        }
    }

    j = s32ClutCharWTmp - 1;
    for (i = 0; i < (s32Height - 1); ++ i)
    {
        if (CHIP_OSD_BUFF_CLUT8 == pstFillInfo->enFillType)
        {
            if (CLUT8_COLOR == pu8ImgData[s32StartPoint + (i * s32Width) + j])
            {
                pu8ImgData[s32StartPoint + (i * s32Width) + j] = CLUT8_COLOR_BLACK;
            }
        }
        else
        {
            if (ARGB1555_COLOR_WHITE == pu16ImgData[s32StartPoint + (i * s32Width) + j])
            {
                pu16ImgData[s32StartPoint + (i * s32Width) + j] = ARGB1555_COLOR_BLACK;
            }
        }
    }

    return 0;
}

/**
 * @brief 更新时间OSD显示数据
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_osd_updateTimeOSDBuff(int s32NeedFresh, TVT_SDK_OSD_CFG_S* pstOSDCfg, int* ps32UpdateFlag)
{
    int s32Ret = 0;
    int i = 0;
    char szTimeString[TVT_SDK_TIME_STR_MAX_LENGTH] = {'\0'};
    const unsigned char* pszSignPosition = NULL;

    TVT_SDK_OSD_DATA_INFO_S* pstTimeInfo = &pstOSDCfg->stOSDInfoArr[TVT_SDK_OSD_TIME];
    unsigned char* pszOSDContent = pstTimeInfo->szOSDContent;

    s32Ret = tvt_sdk_osd_getTimeStr(&pstOSDCfg->stTimeFmt, szTimeString);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("getTimeStr failed");
        return -1;
    }

    // TIPC_PRINTF_CHK(" [WKL_DEBUG ] !!!!!!!!!!!!! tvt_sdk_osd_getTimeStr return: %s", szTimeString);

    // 需要重新配置字符串
    if (1 == s32NeedFresh)
    {
        memset(pszOSDContent, '\0', TVT_SDK_TIME_STR_MAX_LENGTH);
    }

    // 获取更新点阵图
    int s32TimeChangeCnt = 0;
    int s32StrLen = strlen(szTimeString);
    int s32StreamIDX = 0;
    for (i = 0; i < s32StrLen; i++)
    {
        if (szTimeString[i] != pszOSDContent[i])
            /// TODO: 若支持黑白反色，需要在此判断是否此字需要更新反色
        {
            // 获取字符对应的点阵内存
            pszSignPosition = getSignData(szTimeString[i]);
            s32TimeChangeCnt++;
            for (s32StreamIDX = 0; s32StreamIDX < TVT_SDK_VIDEO_ENC_STREAM_BUTT; s32StreamIDX++)
            {
                int s32Stride = pstTimeInfo->stOverLayInfo[s32StreamIDX].stDisOSDPos.width;
                int s32Height = pstTimeInfo->stOverLayInfo[s32StreamIDX].stDisOSDPos.height;
                if (s32Stride <= 0)
                {
                    // 还未更新位置信息
                    continue;
                }
                if (1 == s32NeedFresh)
                {
                    // 需要重新释放分配内存
                    if (pstTimeInfo->stOverLayInfo[s32StreamIDX].pBuffAddr != NULL)
                    {
                        free(pstTimeInfo->stOverLayInfo[s32StreamIDX].pBuffAddr);
                        pstTimeInfo->stOverLayInfo[s32StreamIDX].pBuffAddr = NULL;
                    }
                }
                if (NULL == pstTimeInfo->stOverLayInfo[s32StreamIDX].pBuffAddr)
                {
                    // 分配内存，直接按照u16的大小分配
                    // 这里不考虑对齐问题，在计算显示位置的时候考虑好了
                    int s32MallocSize = s32Stride * s32Height * 2;
                    pstTimeInfo->stOverLayInfo[s32StreamIDX].pBuffAddr = malloc(s32MallocSize);
                    if (NULL == pstTimeInfo->stOverLayInfo[s32StreamIDX].pBuffAddr)
                    {
                        TIPC_PRINTF_ERR("=====ERR malloc size[%d] failed!!!=======", s32MallocSize);
                        continue;
                    }
                }
                TVT_SDK_OSD_FILL_INFO_S stFillInfo = {0};
                stFillInfo.s32SrcW = TIME_OSD_FONT_W;
                stFillInfo.s32SrcH = TIME_OSD_FONT_H;
                stFillInfo.s32DstW = s32Stride;
                stFillInfo.s32DstH = s32Height;
                stFillInfo.u32Color = pstOSDCfg->u32Colour;
                stFillInfo.u8OutLine = 1;
#ifdef CHIP_AMBA
                // 安霸仅支持CLUT8索引数据
                stFillInfo.enFillType = CHIP_OSD_BUFF_CLUT8;
                unsigned char* pu8ImgData = (unsigned char*)pstTimeInfo->stOverLayInfo[s32StreamIDX].pBuffAddr;
                tvt_sdk_osd_fillTimeOSDBuff(i, s32StrLen, pu8ImgData, pszSignPosition, &stFillInfo);
#else
                // 其他平台使用ARGB1555
                stFillInfo.enFillType = CHIP_OSD_BUFF_ARGB1555;
                unsigned short* pu16ImgData = (unsigned short*)pstTimeInfo->stOverLayInfo[s32StreamIDX].pBuffAddr;
                tvt_sdk_osd_fillTimeOSDBuff(i, s32StrLen, pu16ImgData, pszSignPosition, &stFillInfo);
#endif
                /// TODO: VO OSD
            }
        }
    }

    *ps32UpdateFlag = s32TimeChangeCnt;

    return 0;
}


/**
 * @brief 设置时间OSD格式位置
 * @return 0：成功；非0：失败
 */
int tvt_sdk_osd_setTimeOSDCfg(int s32VinIDX, const TVT_SDK_OSD_TIME_FMT_S* pstTimeFmt, const SDK_POSITION* pstPos)
{
    int s32StreamIDX = 0;
    TVT_SDK_OSD_INFO_S* pstHandle = tvt_sdk_osd_getHandle();
    if (NULL == pstHandle || NULL == pstTimeFmt || NULL == pstPos)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    if (s32VinIDX >= CHIP_MAX_VI_NUM)
    {
        TIPC_PRINTF_ERR("err Vin Index[%d]", s32VinIDX);
        return -1;
    }
    int s32VinDevID = pstHandle->stOSDCfg[s32VinIDX].s32VinDev;
    if (s32VinDevID < 0)
    {
        TIPC_PRINTF_ERR("err Vin DevID[%d] with Vin Index[%d] ", s32VinDevID, s32VinIDX);
        return -1;
    }
    // 判断业务状态
    if (pstHandle->enStatus != TVT_SDK_OSD_RUN)
    {
        TIPC_PRINTF_ERR("err osd service status[%d]", pstHandle->enStatus);
        return -1;
    }

    TVT_SDK_OSD_CFG_S* pstOSDCfg = &pstHandle->stOSDCfg[s32VinIDX];
    TVT_SDK_OSD_DATA_INFO_S* pstTimeDataInfo = &pstOSDCfg->stOSDInfoArr[TVT_SDK_OSD_TIME];
    // 加锁
    TIPC_MUTEX_LOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
    // 检查参数
    int s32CmpFmt = memcmp(&pstOSDCfg->stTimeFmt, pstTimeFmt, sizeof(TVT_SDK_OSD_TIME_FMT_S));
    int s32CmpPos = 0;
    if (pstTimeDataInfo->stSrcOSDPos.x != pstPos->x || pstTimeDataInfo->stSrcOSDPos.y != pstPos->y)
    {
        s32CmpPos = 1;
    }
    if (s32CmpFmt != 0 || s32CmpPos != 0)
    {
        // 根据传来的位置计算各个码流的叠加位置
        for (s32StreamIDX = 0; s32StreamIDX < TVT_SDK_VIDEO_ENC_STREAM_BUTT; s32StreamIDX++)
        {
            // 获取编码流的宽高
            CHIP_VIDEO_ENC_CFG_S stEncCfg = {0};
            stEncCfg.s32VinDevID = s32VinDevID;
            stEncCfg.s32StreamIDX = s32StreamIDX;
            chip_video_getChnCfg(&stEncCfg);
            if ((int)stEncCfg.u32Width <= 0 || (int)stEncCfg.u32Height <= 0)
            {
                // 视为通道未创建
                continue;
            }
            // 获取点阵数据的宽高
            char szTimeString[TVT_SDK_TIME_STR_MAX_LENGTH] = {'\0'};
            tvt_sdk_osd_getTimeStr(pstTimeFmt, szTimeString);
            int s32BitDataW = strlen(szTimeString) * TIME_OSD_FONT_W;
            SDK_POSITION stBitDataSize = {0};
            stBitDataSize.width = s32BitDataW;
            stBitDataSize.height = TIME_OSD_FONT_H;
            SDK_POSITION stVencSize = {0};
            stVencSize.width = (int)stEncCfg.u32Width;
            stVencSize.height = (int)stEncCfg.u32Height;
            SDK_POSITION stDispSize = {0};
            // 计算目的位置
            tvt_sdk_osd_caluDispOSDPos(&stVencSize, pstPos, &stBitDataSize, &stDispSize);
            if (stDispSize.width <= 0 || stDispSize.height <= 0)
            {
                continue;
            }
            // 更新业务结构体数据
            memcpy(&pstTimeDataInfo->stOverLayInfo[s32StreamIDX].stDisOSDPos, &stDispSize, sizeof(SDK_POSITION));
        }
        // 更新配置
        pstTimeDataInfo->u32NeedFresh = 1;
        pstTimeDataInfo->u32Enable = pstTimeFmt->u32Enable;
        memcpy(&pstOSDCfg->stTimeFmt, pstTimeFmt, sizeof(TVT_SDK_OSD_TIME_FMT_S));

        // 更新显示位图的宽高和位置
        pstTimeDataInfo->stSrcOSDPos.x = pstPos->x;
        pstTimeDataInfo->stSrcOSDPos.y = pstPos->y;
    }
    // 解锁
    TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
    return 0;
}

/**
 * @brief 刷新时间OSD内容
 * @return 0：成功；非0：失败
 */
int tvt_sdk_osd_updateTimeOSD(int s32VinIDX)
{
    int s32Ret = 0;
    int i = 0;

    TVT_SDK_OSD_INFO_S* pstHandle = tvt_sdk_osd_getHandle();
    if (NULL == pstHandle)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }

    if (s32VinIDX >= CHIP_MAX_VI_NUM)
    {
        TIPC_PRINTF_ERR("err Vin Index[%d]", s32VinIDX);
        return -1;
    }

    int s32VinDevID = pstHandle->stOSDCfg[s32VinIDX].s32VinDev;
    if (s32VinDevID < 0)
    {
        TIPC_PRINTF_ERR("err Vin DevID[%d] with Vin Index[%d] ", s32VinDevID, s32VinIDX);
        return -1;
    }

    // 判断业务状态
    if (pstHandle->enStatus != TVT_SDK_OSD_RUN)
    {
        TIPC_PRINTF_ERR("err osd service status[%d]", pstHandle->enStatus);
        return -1;
    }

    TVT_SDK_OSD_CFG_S* pstOSDCfg = &pstHandle->stOSDCfg[s32VinIDX];
    TVT_SDK_OSD_DATA_INFO_S* pstTimeDataInfo = &pstOSDCfg->stOSDInfoArr[TVT_SDK_OSD_TIME];

    if (0 == pstTimeDataInfo->u32NeedFresh && 0 == pstTimeDataInfo->u32Enable)
    {
        // 无需更新


        return s32Ret;
    }

    // 加锁
    TIPC_MUTEX_LOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
    // TIPC_PRINTF_CHK ("[ WKL_DEBUG ]!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Refresh Osd");
    if (0 == pstTimeDataInfo->u32Enable)
    {
        // 直接关闭
        CHIP_OSD_INFO_S stTimeOSDInfo = {0};
        stTimeOSDInfo.s32Enable = 0;
        stTimeOSDInfo.s32VinDevID = s32VinDevID;
        stTimeOSDInfo.s32OSDIDX = TVT_SDK_OSD_TIME;
        for (i = 0; i < TVT_SDK_VIDEO_ENC_STREAM_BUTT; i++)
        {
            // 将该画面下所有编码流的OSD都关闭
            stTimeOSDInfo.s32StreamIDX = i;
            s32Ret = chip_video_setOSD(&stTimeOSDInfo);
            if (s32Ret != 0)
            {
                // TIPC_PRINTF_ERR("chip_video_setOSD failed[%d]", s32Ret);
                TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
                return -1;
            }
        }

        // 解锁
        TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
        return s32Ret;
    }
    int s32UpdateFlag = 0;
    s32Ret = tvt_sdk_osd_updateTimeOSDBuff(pstTimeDataInfo->u32NeedFresh, pstOSDCfg, &s32UpdateFlag);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("update Time OSD Buffer failed[%d]", s32Ret);
        TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
        return s32Ret;
    }

    if (0 == s32UpdateFlag)
    {
        // 无需更新内容
        TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
        return 0;
    }

    // 调用硬件接口实现
    CHIP_OSD_INFO_S stOSDInfo = {0};
#ifdef CHIP_AMBA
    stOSDInfo.enBuffType = CHIP_OSD_BUFF_CLUT8;
#else
    stOSDInfo.enBuffType = CHIP_OSD_BUFF_ARGB1555;
#endif
    stOSDInfo.s32Alpha = (255 - pstOSDCfg->u8TransLevel);
    stOSDInfo.s32Color = (int)pstOSDCfg->u32Colour;
    stOSDInfo.s32Enable = (int)pstTimeDataInfo->u32Enable;
    stOSDInfo.s32OSDIDX = TVT_SDK_OSD_TIME;
    stOSDInfo.s32VinDevID = s32VinDevID;
    int s32StreamIDX = 0;

    for (s32StreamIDX = 0; s32StreamIDX < TVT_SDK_VIDEO_ENC_STREAM_BUTT; s32StreamIDX++)
    {
        stOSDInfo.pu8BuffAddr = (unsigned char*)pstTimeDataInfo->stOverLayInfo[s32StreamIDX].pBuffAddr;
        stOSDInfo.s32StreamIDX = s32StreamIDX;
        stOSDInfo.s32W = pstTimeDataInfo->stOverLayInfo[s32StreamIDX].stDisOSDPos.width;
        stOSDInfo.s32H = pstTimeDataInfo->stOverLayInfo[s32StreamIDX].stDisOSDPos.height;
#if WKL_DEBUG
        TIPC_PRINTF_CHK("[****** WKL Debug ******]      Before chip_video_setOSD()");
        TIPC_PRINTF_CHK("[****** WKL Debug ******]      stOSDInfo.s32W = %d ", stOSDInfo.s32W);
        TIPC_PRINTF_CHK("[****** WKL Debug ******]      stOSDInfo.s32H = %d ", stOSDInfo.s32H);
#endif
        stOSDInfo.s32X = pstTimeDataInfo->stOverLayInfo[s32StreamIDX].stDisOSDPos.x;
        stOSDInfo.s32Y = pstTimeDataInfo->stOverLayInfo[s32StreamIDX].stDisOSDPos.y;

// wkl add:
        if ( stOSDInfo.s32W <= 0 || stOSDInfo.s32H <= 0) {
#if WKL_DEBUG
            TIPC_PRINTF_CHK("[****** WKL Debug ******]      s32StreamIDX = %d", s32StreamIDX);
#endif
            continue;
        }
        s32Ret = chip_video_setOSD(&stOSDInfo);
        if (s32Ret != 0)
        {
            TIPC_PRINTF_ERR("set OSD failed[%d] [%d]", s32Ret, s32StreamIDX);
        }
    }

    /// TODO: VO OSD

    pstTimeDataInfo->u32NeedFresh = 0;

    // 解锁
    TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
    return 0;
}


////////////////////////////// Comm OSD //////////////////////
/**
 * @brief 填充Comm OSD 显示位图数据
 * @param pu8ImgData     显示位图数据数据头
 * @param pszBitData     显示位图数据对应字符串点阵图
 * @param pstFillInfo    填充信息
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_osd_fillCommOSDBuff(void* pImgData, unsigned char* pszBitData, TVT_SDK_OSD_FILL_INFO_S* pstFillInfo)
{
    int s32StartPoint = 0;
    int i = 0, j = 0;
    unsigned char* pu8TmpData = NULL;

    int s32BitDataW = pstFillInfo->s32SrcW;
    int s32BitDataH = pstFillInfo->s32SrcH;

    int s32DataW = ROUND_UP(s32BitDataW, COMM_OSD_FONT_W); //(s32BitDataW + 7) / 8;

    int s32Width = pstFillInfo->s32DstW;
    int s32Height = pstFillInfo->s32DstH;

    unsigned char* pu8ImgData = (unsigned char*)pImgData;
    unsigned short* pu16ImgData = (unsigned short*)pImgData;

    /*
        对于每个字符，判断它的10个char型的每一位，如果是1,
        设置这一位对应的扩大区域为颜色(0xXXXX)
        否则，设置其对应区域为透明(0)
    */

    // 计算放大倍数
    float f32WidthRate = (float)s32Width / (float)s32BitDataW;
    float f32HeightRate = (float)s32Height / (float)s32BitDataH;

    // 填充
    for (i = 0; i < s32Height; i++)
    {
        for (j = 0; j < s32Width; j++)
        {
            char s8OutOfEdge = 0;
            // 计算对应点阵的点
            float f32CurBitHeight = i / f32HeightRate;
            int s32CurBitHeight = (int)f32CurBitHeight;
            if (s32CurBitHeight >= s32BitDataH)
            {
                s8OutOfEdge = 1;
                // 防越界
                s32CurBitHeight = (s32BitDataH - 1);
            }

            float f32CurBitWidth = j / f32WidthRate;
            int s32CurBitWidth = (int)f32CurBitWidth;

            pu8TmpData = &pszBitData[s32CurBitHeight * s32DataW];

            // 根据点阵的点的值判断
            int s32OriPos = s32CurBitWidth / COMM_OSD_FONT_W;
            if (s32OriPos >= s32DataW)
            {
                s8OutOfEdge = 1;
                s32OriPos = (s32DataW - 1);
            }
            float f32OriBitPos = s32CurBitWidth - (s32OriPos * 8);
            int s32OriBitPos = (int)f32OriBitPos;
            if (s32OriBitPos >= COMM_OSD_FONT_W)
            {
                s8OutOfEdge = 1;
                s32OriBitPos = COMM_OSD_FONT_W - 1;
            }

            if (pu8TmpData[s32CurBitHeight] & (0x01 << (7 - s32CurBitWidth)) && (s8OutOfEdge == 0))
            {
                if (CHIP_OSD_BUFF_CLUT8 == pstFillInfo->enFillType)
                {
                    pu8ImgData[s32StartPoint + (i * s32Width) + j] = CLUT8_COLOR;
                }
                else
                {
                    pu16ImgData[s32StartPoint + (i * s32Width) + j] = (unsigned short)pstFillInfo->u32Color;
                }
            }
            else
            {
                if (CHIP_OSD_BUFF_CLUT8 == pstFillInfo->enFillType)
                {
                    pu8ImgData[s32StartPoint + (i * s32Width) + j] = CLUT8_COLOR_NULL;
                }
                else
                {
                    pu16ImgData[s32StartPoint + (i * s32Width) + j] = ARGB1555_COLOR_NULL;
                }
            }

        }
    }

    // 描边处理
    if (0 == pstFillInfo->u8OutLine)
    {
        return 0;
    }
    if ((CHIP_OSD_BUFF_ARGB1555 == pstFillInfo->enFillType) && (ARGB1555_COLOR_WHITE == (unsigned short)pstFillInfo->u32Color))
    {
        return 0;
    }

    /// TODO: 后面可能需要根据传来的s32OutLine值确认描边的厚度
    /// TODO: ugly code
    for (i = 1; i < (s32Height - 1); i++)
    {
        for (j = 1; j < (s32Width - 1); j++)
        {
            if (CHIP_OSD_BUFF_CLUT8 == pstFillInfo->enFillType)
            {
                if (pu8ImgData[i * s32Width + j] != CLUT8_COLOR_NULL)
                {
                    continue;
                }
                if ( (CLUT8_COLOR == pu8ImgData[(s32Width * i) +  j + 1])
                        || (CLUT8_COLOR == pu8ImgData[(s32Width * i) +  j - 1])
                        || (CLUT8_COLOR == pu8ImgData[(s32Width * (i - 1)) + j - 1])
                        || (CLUT8_COLOR == pu8ImgData[(s32Width * (i - 1)) + j + 0])
                        || (CLUT8_COLOR == pu8ImgData[(s32Width * (i - 1)) + j + 1])
                        || (CLUT8_COLOR == pu8ImgData[(s32Width * (i + 1)) +  j - 1])
                        || (CLUT8_COLOR == pu8ImgData[(s32Width * (i + 1)) + j + 0])
                        || (CLUT8_COLOR == pu8ImgData[(s32Width * (i + 1)) + j + 1]))
                {
                    pu8ImgData[(i * s32Width) + j] = CLUT8_COLOR_BLACK;
                }
            }
            else
            {
                if (pu16ImgData[i * s32Width + j] != ARGB1555_COLOR_NULL)
                {
                    continue;
                }
                if ( (ARGB1555_COLOR_WHITE == pu16ImgData[(s32Width * i) +  j + 1])
                        || (ARGB1555_COLOR_WHITE == pu16ImgData[(s32Width * i) +  j - 1])
                        || (ARGB1555_COLOR_WHITE == pu16ImgData[(s32Width * (i - 1)) + j - 1])
                        || (ARGB1555_COLOR_WHITE == pu16ImgData[(s32Width * (i - 1)) + j + 0])
                        || (ARGB1555_COLOR_WHITE == pu16ImgData[(s32Width * (i - 1)) + j + 1])
                        || (ARGB1555_COLOR_WHITE == pu16ImgData[(s32Width * (i + 1)) +  j - 1])
                        || (ARGB1555_COLOR_WHITE == pu16ImgData[(s32Width * (i + 1)) + j + 0])
                        || (ARGB1555_COLOR_WHITE == pu16ImgData[(s32Width * (i + 1)) + j + 1]))
                {
                    pu16ImgData[(i * s32Width) + j] = ARGB1555_COLOR_BLACK;
                }
            }
        }
    }

    i = 0;
    for (j = 0; j < s32Width; ++j)
    {
        if (CHIP_OSD_BUFF_CLUT8 == pstFillInfo->enFillType)
        {
            if (CLUT8_COLOR == pu8ImgData[(i * s32Width) + j])
            {
                pu8ImgData[(i * s32Width) + j] = CLUT8_COLOR_BLACK;
            }
        }
        else
        {
            if (ARGB1555_COLOR_WHITE == pu16ImgData[(i * s32Width) + j])
            {
                pu16ImgData[(i * s32Width) + j] = ARGB1555_COLOR_BLACK;
            }
        }
    }

    i = s32Height - 1;
    for (j = 0; j < s32Width; ++j)
    {
        if (CHIP_OSD_BUFF_CLUT8 == pstFillInfo->enFillType)
        {
            if (CLUT8_COLOR == pu8ImgData[(i * s32Width) + j])
            {
                pu8ImgData[(i * s32Width) + j] = CLUT8_COLOR_BLACK;
            }
        }
        else
        {
            if (ARGB1555_COLOR_WHITE == pu16ImgData[(i * s32Width) + j])
            {
                pu16ImgData[(i * s32Width) + j] = ARGB1555_COLOR_BLACK;
            }
        }
    }

    j = 0;
    for (i = 0; i < (s32Height - 1); ++i)
    {
        if (CHIP_OSD_BUFF_CLUT8 == pstFillInfo->enFillType)
        {
            if (CLUT8_COLOR == pu8ImgData[(i * s32Width) + j])
            {
                pu8ImgData[(i * s32Width) + j] = CLUT8_COLOR_BLACK;
            }
        }
        else
        {
            if (ARGB1555_COLOR_WHITE == pu16ImgData[(i * s32Width) + j])
            {
                pu16ImgData[(i * s32Width) + j] = ARGB1555_COLOR_BLACK;
            }
        }
    }

    j = s32Width - 1;
    for (i = 0; i < (s32Height - 1); ++ i)
    {
        if (CHIP_OSD_BUFF_CLUT8 == pstFillInfo->enFillType)
        {
            if (CLUT8_COLOR == pu8ImgData[(i * s32Width) + j])
            {
                pu8ImgData[(i * s32Width) + j] = CLUT8_COLOR_BLACK;
            }
        }
        else
        {
            if (ARGB1555_COLOR_WHITE == pu16ImgData[(i * s32Width) + j])
            {
                pu16ImgData[(i * s32Width) + j] = ARGB1555_COLOR_BLACK;
            }
        }
    }

    return 0;
}

/**
 * @brief 更新Comm OSD显示数据
 * @return 0：成功；非0：失败
 */
static int tvt_sdk_osd_updateCommOSDBuff(int s32OSDIDX, TVT_SDK_OSD_CFG_S* pstOSDCfg)
{
    int s32StreamIDX = 0;
    TVT_SDK_OSD_DATA_INFO_S* pstCommDataInfo = &pstOSDCfg->stOSDInfoArr[s32OSDIDX];
    for (s32StreamIDX = 0; s32StreamIDX < TVT_SDK_VIDEO_ENC_STREAM_BUTT; s32StreamIDX++)
    {
        int s32Stride = pstCommDataInfo->stOverLayInfo[s32StreamIDX].stDisOSDPos.width;
        int s32Height = pstCommDataInfo->stOverLayInfo[s32StreamIDX].stDisOSDPos.height;
        if (s32Stride <= 0)
        {
            // 还未更新位置信息
            continue;
        }

        // 重新释放分配内存
        if (pstCommDataInfo->stOverLayInfo[s32StreamIDX].pBuffAddr != NULL)
        {
            free(pstCommDataInfo->stOverLayInfo[s32StreamIDX].pBuffAddr);
            pstCommDataInfo->stOverLayInfo[s32StreamIDX].pBuffAddr = NULL;
        }
        // 分配内存，直接按照u16的大小分配
        // 这里不考虑对齐问题，在计算显示位置的时候考虑好了
        int s32MallocSize = s32Stride * s32Height * 2;
        pstCommDataInfo->stOverLayInfo[s32StreamIDX].pBuffAddr = malloc(s32MallocSize);
        if (NULL == pstCommDataInfo->stOverLayInfo[s32StreamIDX].pBuffAddr)
        {
            TIPC_PRINTF_ERR("=====ERR malloc size[%d] failed!!!=======", s32MallocSize);
            continue;
        }
        unsigned char* pszBitData = pstCommDataInfo->pu8BitBuff;
        TVT_SDK_OSD_FILL_INFO_S stFillInfo = {0};
        stFillInfo.s32SrcW = pstCommDataInfo->stSrcOSDPos.width;
        stFillInfo.s32SrcH = pstCommDataInfo->stSrcOSDPos.height;
        stFillInfo.s32DstW = s32Stride;
        stFillInfo.s32DstH = s32Height;
        stFillInfo.u32Color = pstOSDCfg->u32Colour;
        stFillInfo.u8OutLine = 1;
#ifdef CHIP_AMBA
        // 安霸仅支持CLUT8索引数据
        stFillInfo.enFillType = CHIP_OSD_BUFF_CLUT8;
        unsigned char* pu8ImgData = (unsigned char*)pstCommDataInfo->stOverLayInfo[s32StreamIDX].pBuffAddr;
        tvt_sdk_osd_fillCommOSDBuff(pu8ImgData, pszBitData, &stFillInfo);
#else
        // 其他平台使用ARGB1555
        stFillInfo.enFillType = CHIP_OSD_BUFF_ARGB1555;
        unsigned short* pu16ImgData = (unsigned short*)pstCommDataInfo->stOverLayInfo[s32StreamIDX].pBuffAddr;
        tvt_sdk_osd_fillCommOSDBuff(pu16ImgData, pszBitData, &stFillInfo);
#endif
    }
    /// TODO: VO OSD
    return 0;
}

/**
 * @brief 刷新Comm OSD显示数据
 * @return 0：成功；非0：失败
 */
int tvt_sdk_osd_updateCommOSD(int s32VinIDX, int s32OSDIDX)
{
    int s32Ret = 0;
    int i = 0;
    TVT_SDK_OSD_INFO_S* pstHandle = tvt_sdk_osd_getHandle();
    if (NULL == pstHandle)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    if (s32VinIDX >= CHIP_MAX_VI_NUM)
    {
        TIPC_PRINTF_ERR("err Vin Index[%d]", s32VinIDX);
        return -1;
    }
    if (s32OSDIDX >= TVT_SDK_OSD_INDEX_BUTT)
    {
        TIPC_PRINTF_ERR("err OSD Index[%d]", s32OSDIDX);
        return -1;
    }
    int s32VinDevID = pstHandle->stOSDCfg[s32VinIDX].s32VinDev;
    if (s32VinDevID < 0)
    {
        TIPC_PRINTF_ERR("err Vin DevID[%d] with Vin Index[%d] ", s32VinDevID, s32VinIDX);
        return -1;
    }

    // 判断业务状态
    if (pstHandle->enStatus != TVT_SDK_OSD_RUN)
    {
        TIPC_PRINTF_ERR("err osd service status[%d]", pstHandle->enStatus);
        return -1;
    }

    TVT_SDK_OSD_CFG_S* pstOSDCfg = &pstHandle->stOSDCfg[s32VinIDX];
    TVT_SDK_OSD_DATA_INFO_S* pstCommDataInfo = &pstOSDCfg->stOSDInfoArr[s32OSDIDX];

    if (0 == pstCommDataInfo->u32NeedFresh)
    {
        // 无需更新
        return s32Ret;
    }

    // 加锁
    TIPC_MUTEX_LOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
    if (0 == pstCommDataInfo->u32Enable)
    {
        // 直接关闭
        CHIP_OSD_INFO_S stCommOSDInfo = {0};
        stCommOSDInfo.s32Enable = 0;
        stCommOSDInfo.s32VinDevID = s32VinDevID;
        stCommOSDInfo.s32OSDIDX = s32OSDIDX;
        for (i = 0; i < TVT_SDK_VIDEO_ENC_STREAM_BUTT; i++)
        {
            // 将该画面下所有编码流的OSD都关闭
            stCommOSDInfo.s32StreamIDX = i;
            s32Ret = chip_video_setOSD(&stCommOSDInfo);
            if (s32Ret != 0)
            {
                // TIPC_PRINTF_ERR("chip_video_setOSD failed[%d]", s32Ret);
                TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
                return -1;
            }
        }

        // 解锁
        TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
        return s32Ret;
    }

    if (TVT_SDK_OSD_TYPE_CHAR == pstCommDataInfo->enOSDType)
    {
        // 点阵数据-->显示数据
        s32Ret = tvt_sdk_osd_updateCommOSDBuff(s32OSDIDX, pstOSDCfg);
    }
    else
    {
        // 显示数据缩放
        /// TODO: 待补充
        s32Ret = -1;
    }
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("update Comm OSD Buffer failed[%d]", s32Ret);
        TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
        return s32Ret;
    }

    // 调用硬件接口实现
    CHIP_OSD_INFO_S stOSDInfo = {0};
#ifdef CHIP_AMBA
    stOSDInfo.enBuffType = CHIP_OSD_BUFF_CLUT8;
#else
    stOSDInfo.enBuffType = CHIP_OSD_BUFF_ARGB1555;
#endif
    if (TVT_SDK_OSD_TYPE_IMG == pstCommDataInfo->enOSDType)
    {
        stOSDInfo.enBuffType = CHIP_OSD_BUFF_ARGB1555;
    }
    stOSDInfo.s32Alpha = (255 - pstOSDCfg->u8TransLevel);
    stOSDInfo.s32Color = (int)pstOSDCfg->u32Colour;
    stOSDInfo.s32Enable = (int)pstCommDataInfo->u32Enable;
    stOSDInfo.s32OSDIDX = s32OSDIDX;
    stOSDInfo.s32VinDevID = s32VinDevID;
    int s32StreamIDX = 0;
    for (s32StreamIDX = 0; s32StreamIDX < TVT_SDK_VIDEO_ENC_STREAM_BUTT; s32StreamIDX++)
    {
        stOSDInfo.pu8BuffAddr = (unsigned char*)pstCommDataInfo->stOverLayInfo[s32StreamIDX].pBuffAddr;
        stOSDInfo.s32StreamIDX = s32StreamIDX;
        stOSDInfo.s32W = pstCommDataInfo->stOverLayInfo[s32StreamIDX].stDisOSDPos.width;
        stOSDInfo.s32H = pstCommDataInfo->stOverLayInfo[s32StreamIDX].stDisOSDPos.height;
        stOSDInfo.s32X = pstCommDataInfo->stOverLayInfo[s32StreamIDX].stDisOSDPos.x;
        stOSDInfo.s32Y = pstCommDataInfo->stOverLayInfo[s32StreamIDX].stDisOSDPos.y;
        // wkl add:
        if ( stOSDInfo.s32W <= 0 || stOSDInfo.s32H <= 0) {
#if WKL_DEBUG
            TIPC_PRINTF_CHK("[****** WKL Debug ******]      s32StreamIDX = %d", s32StreamIDX);
#endif
            continue;
        }
        s32Ret = chip_video_setOSD(&stOSDInfo);
        if (s32Ret != 0)
        {
            TIPC_PRINTF_ERR("set OSD failed[%d] [%d]", s32Ret, s32StreamIDX);
        }
    }

    /// TODO: VO OSD

    pstCommDataInfo->u32NeedFresh = 0;

    // 解锁
    TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
    return s32Ret;
}


/**
 * @brief 设置Comm OSD内容
 * @return 0：成功；非0：失败
 */
int tvt_sdk_osd_setCommOSD(int s32VinIDX, int s32OSDIDX, const TVT_SDK_OSD_COMM_INFO_S* pstCfg, const SDK_POSITION* pstPos)
{
    int s32StreamIDX = 0;
    TVT_SDK_OSD_INFO_S* pstHandle = tvt_sdk_osd_getHandle();
    if (NULL == pstHandle || NULL == pstCfg || NULL == pstPos)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    if (s32VinIDX >= CHIP_MAX_VI_NUM)
    {
        TIPC_PRINTF_ERR("err Vin Index[%d]", s32VinIDX);
        return -1;
    }
    if (s32OSDIDX >= TVT_SDK_OSD_INDEX_BUTT)
    {
        TIPC_PRINTF_ERR("err OSD Index[%d]", s32OSDIDX);
        return -1;
    }
    int s32VinDevID = pstHandle->stOSDCfg[s32VinIDX].s32VinDev;
    if (s32VinDevID < 0)
    {
        TIPC_PRINTF_ERR("err Vin DevID[%d] with Vin Index[%d] ", s32VinDevID, s32VinIDX);
        return -1;
    }

    // 判断业务状态
    if (pstHandle->enStatus != TVT_SDK_OSD_RUN)
    {
        TIPC_PRINTF_ERR("err osd service status[%d]", pstHandle->enStatus);
        return -1;
    }

    TVT_SDK_OSD_CFG_S* pstOSDCfg = &pstHandle->stOSDCfg[s32VinIDX];
    TVT_SDK_OSD_DATA_INFO_S* pstCommDataInfo = &pstOSDCfg->stOSDInfoArr[s32OSDIDX];
    // 加锁
    TIPC_MUTEX_LOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
    // 这里不用检查数据是否有更新，直接刷新结构体内容就是了
    pstCommDataInfo->enOSDType = pstCfg->enOSDType;
    pstCommDataInfo->u32Enable = pstCfg->u32Enable;
    memcpy(&pstCommDataInfo->stSrcOSDPos, pstPos, sizeof(SDK_POSITION));
    if (pstCommDataInfo->pu8BitBuff != NULL)
    {
        free(pstCommDataInfo->pu8BitBuff);
        pstCommDataInfo->pu8BitBuff = NULL;
    }
    int s32BitBuffSize = 0;
    if (TVT_SDK_OSD_TYPE_IMG == pstCfg->enOSDType)
    {
        // ARGB1555数据
        s32BitBuffSize = pstPos->width * pstPos->height * 2;
    }
    else
    {
        // 点阵数据
        int s32Stride = ROUND_UP(pstPos->width, COMM_OSD_FONT_W);
        s32BitBuffSize = s32Stride * pstPos->height;
    }
    pstCommDataInfo->pu8BitBuff = (unsigned char*)malloc(s32BitBuffSize);
    if (NULL == pstCommDataInfo->pu8BitBuff)
    {
        TIPC_PRINTF_ERR("=====ERR malloc size[%d] failed!!!=======", s32BitBuffSize);
        // 解锁
        TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
        return -1;
    }

    // 拷贝点阵/位图数据
    memcpy(pstCommDataInfo->pu8BitBuff, pstCfg->pszData, s32BitBuffSize);

    // 根据传来的位置计算各个码流的叠加位置
    for (s32StreamIDX = 0; s32StreamIDX < TVT_SDK_VIDEO_ENC_STREAM_BUTT; s32StreamIDX++)
    {
        // 获取编码流的宽高
        CHIP_VIDEO_ENC_CFG_S stEncCfg = {0};
        stEncCfg.s32VinDevID = s32VinDevID;
        stEncCfg.s32StreamIDX = s32StreamIDX;
        chip_video_getChnCfg(&stEncCfg);
        if ((int)stEncCfg.u32Width <= 0 || (int)stEncCfg.u32Height <= 0)
        {
            // 视为通道未创建
            continue;
        }
        // 获取点阵数据的宽高
        SDK_POSITION stBitDataSize = {0};
        stBitDataSize.width = pstPos->width;
        stBitDataSize.height = pstPos->height;
        SDK_POSITION stVencSize = {0};
        stVencSize.width = (int)stEncCfg.u32Width;
        stVencSize.height = (int)stEncCfg.u32Height;
        SDK_POSITION stDispSize = {0};
        // 计算目的位置
        tvt_sdk_osd_caluDispOSDPos(&stVencSize, pstPos, &stBitDataSize, &stDispSize);
        if (stDispSize.width <= 0 || stDispSize.height <= 0)
        {
            continue;
        }
        // 更新业务结构体数据
        memcpy(&pstCommDataInfo->stOverLayInfo[s32StreamIDX].stDisOSDPos, &stDispSize, sizeof(SDK_POSITION));
    }

    pstCommDataInfo->u32NeedFresh = 1; // 更新状态

    // 解锁
    TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
    return 0;
}

/////////////////////////////////////////////////////////////////////////////

/**
 * @brief 关闭显示所有OSD
 * @return 0：成功；非0：失败
 */
int tvt_sdk_osd_stopDispALLOSD()
{
    int s32Ret = 0;
    int s32StreamIDX = 0;
    int s32VinIDX = 0;
    int s32OSDIDX = 0;
    TVT_SDK_OSD_INFO_S* pstHandle = tvt_sdk_osd_getHandle();
    if (NULL == pstHandle)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    // 加锁
    TIPC_MUTEX_LOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
    for (s32VinIDX = 0; s32VinIDX < CHIP_MAX_VI_NUM; s32VinIDX++)
    {
        int s32VinDevID = pstHandle->stOSDCfg[s32VinIDX].s32VinDev;
        if (s32VinDevID < 0)
        {
            continue;
        }
        for (s32OSDIDX = 0; s32OSDIDX < TVT_SDK_OSD_INDEX_BUTT; s32OSDIDX++)
        {
            // 直接在此关闭显示
            CHIP_OSD_INFO_S stOSDInfo = {0};
            stOSDInfo.s32Enable = 0;
            stOSDInfo.s32VinDevID = s32VinDevID;
            stOSDInfo.s32OSDIDX = s32OSDIDX;
            for (s32StreamIDX = 0; s32StreamIDX < TVT_SDK_VIDEO_ENC_STREAM_BUTT; s32StreamIDX++)
            {
                // 将该画面下所有编码流的OSD都关闭
                stOSDInfo.s32StreamIDX = s32StreamIDX;
                s32Ret = chip_video_setOSD(&stOSDInfo);
                if (s32Ret != 0)
                {
                    // TIPC_PRINTF_ERR("chip_video_setOSD failed[%d]", s32Ret);
                }
            }
            // 设置业务状态
            g_pstSdkOSDInfo->enStatus = TVT_SDK_OSD_PAUSE;
        }
    }
    // 解锁
    TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
    return 0;
}

/**
 * @brief 重新显示所有OSD
 * @return 0：成功；非0：失败
 */
int tvt_sdk_osd_startDispALLOSD()
{
    int s32VinIDX = 0;
    int s32OSDIDX = 0;
    TVT_SDK_OSD_INFO_S* pstHandle = tvt_sdk_osd_getHandle();
    if (NULL == pstHandle)
    {
        TIPC_PRINTF_ERR("NULL Pointer");
        return -1;
    }
    // 加锁
    TIPC_MUTEX_LOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
    for (s32VinIDX = 0; s32VinIDX < CHIP_MAX_VI_NUM; s32VinIDX++)
    {
        int s32VinDevID = pstHandle->stOSDCfg[s32VinIDX].s32VinDev;
        if (s32VinDevID < 0)
        {
            continue;
        }
        TVT_SDK_OSD_CFG_S* pstOSDCfg = &pstHandle->stOSDCfg[s32VinIDX];
        for (s32OSDIDX = 0; s32OSDIDX < TVT_SDK_OSD_INDEX_BUTT; s32OSDIDX++)
        {
            TVT_SDK_OSD_DATA_INFO_S* pstDataInfo = &pstOSDCfg->stOSDInfoArr[s32OSDIDX];
            // 直接强制刷新即可
            pstDataInfo->u32NeedFresh = 1;
        }
        // 设置业务状态
        g_pstSdkOSDInfo->enStatus = TVT_SDK_OSD_RUN;
    }
    // 解锁
    TIPC_MUTEX_UNLOCK(&pstHandle->s32Mutex, pstHandle->szMutexName, &pstHandle->u32MutexLockCnt, TVT_SDK_OSD_DEBUG);
    return 0;
}

/**
 * @brief 初始化视频OSD业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_osd_init(int s32VinNum, const int* s32VinDevID)
{
    int s32Ret = -1;
    int i = 0;
    if (g_pstSdkOSDInfo != NULL)
    {
        if (g_pstSdkOSDInfo->u8Init != 0)
        {
            // 已初始化
            TIPC_PRINTF_ERR("video osd service is running");
            return 0;
        }
    }
    else
    {
        // 创建结构体
        g_pstSdkOSDInfo = (TVT_SDK_OSD_INFO_S* )malloc(sizeof(TVT_SDK_OSD_INFO_S));
        if (NULL == g_pstSdkOSDInfo)
        {
            TIPC_PRINTF_ERR("video osd service malloc failed");
            return s32Ret;
        }
        // 初始化参数
        memset(g_pstSdkOSDInfo, 0, sizeof(TVT_SDK_OSD_INFO_S));
        for (i = 0; i < CHIP_MAX_VI_NUM; i++)
        {
            g_pstSdkOSDInfo->stOSDCfg[i].s32VinDev = -1;
        }
    }

    // 初始化模块锁
    pthread_mutex_init(&g_pstSdkOSDInfo->s32Mutex, NULL);
    sprintf(g_pstSdkOSDInfo->szMutexName, "tvt_sdk_osd_lock");
    g_pstSdkOSDInfo->u8Init = 1;
    g_pstSdkOSDInfo->enStatus = TVT_SDK_OSD_RUN;
    for (i = 0; i < s32VinNum; i++)
    {
        g_pstSdkOSDInfo->stOSDCfg[i].s32VinDev = s32VinDevID[i];
    }
    return 0;
}


/**
 * @brief 去初始化视频OSD业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_osd_exit()
{
    if (NULL == g_pstSdkOSDInfo)
    {
        // 未初始化
        TIPC_PRINTF_ERR("video osd service is not running");
        return -1;
    }

    if (g_pstSdkOSDInfo->u8Init != 1)
    {
        // 未初始化
        TIPC_PRINTF_ERR("video osd service is not running");
        free((void*)g_pstSdkOSDInfo);
        g_pstSdkOSDInfo = NULL;
        return -1;
    }
    else
    {
        pthread_mutex_destroy(&g_pstSdkOSDInfo->s32Mutex);
        g_pstSdkOSDInfo->u8Init = 0;
        free((void*)g_pstSdkOSDInfo);
        g_pstSdkOSDInfo = NULL;
    }
    return 0;
}