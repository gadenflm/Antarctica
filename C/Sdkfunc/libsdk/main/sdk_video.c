#include <stdlib.h>
#include <string.h>

#include "sdk_func.h"
#include "sdk_define.h"
#include "sdk_private.h"

#include "tvt_sdk_param.h"
#include "tvt_sdk_print.h"
#include "tvt_sdk_video.h"
#include "tvt_sdk_audio.h"

extern TVT_SDK_CFG_S g_stSdkCfg;

/**
 * @brief 初始化视频业务
 * @return 0：成功；非0：失败
 */
int SDKInitVideo(SDK_VIDEO_FORMAT enInputFormat)
{
    int s32Ret = 0;
    int i = 0;

    // 获取产品Sensor配置
    TVT_SDK_PROD_CTRL_INFO stParamCtrl = {0};
    s32Ret = tvt_sdk_param_ctrl(TVT_SDK_PARAM_GET_SENSOR_CFG, &stParamCtrl, sizeof(stParamCtrl));
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get sensor cfg failed[%d]!!", s32Ret);
        return s32Ret;
    }
    int s32VinNum = stParamCtrl.s32ParamSize / sizeof(TVT_SDK_PROD_SENSOR_S);
    TVT_SDK_PROD_SENSOR_S* pstProdSenCfg = (TVT_SDK_PROD_SENSOR_S*)stParamCtrl.pParam;
    TVT_SDK_VIDEO_CFG_S* pstVideoCfg = (TVT_SDK_VIDEO_CFG_S*)malloc(s32VinNum * sizeof(TVT_SDK_VIDEO_CFG_S));
    for (i = 0; i < s32VinNum; i++)
    {
        TVT_SDK_VIDEO_CFG_S* pstVideoCfgTmp = &pstVideoCfg[i];
        TVT_SDK_PROD_SENSOR_S* pstProdSenCfgTmp = &pstProdSenCfg[i];
        pstVideoCfgTmp->enFormat = enInputFormat;
        pstVideoCfgTmp->enWorkMode = g_stSdkCfg.s32WorkMode;
        pstVideoCfgTmp->stSensorCfg.enSensorType = pstProdSenCfgTmp->enSensorType;
        pstVideoCfgTmp->stSensorCfg.u32VinDev = pstProdSenCfgTmp->u32VinDev;
        pstVideoCfgTmp->stSensorCfg.u32MaxWidth = pstProdSenCfgTmp->u32MaxWidth;
        pstVideoCfgTmp->stSensorCfg.u32MaxHeight = pstProdSenCfgTmp->u32MaxHeight;
    }
    // 初始化视频业务
    s32Ret = tvt_sdk_video_init(pstVideoCfg, s32VinNum);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("tvt_sdk_video_init failed[%d]!!", s32Ret);
        return s32Ret;
    }

    // 保存部分参数
    g_stSdkCfg.s32VinCnt = s32VinNum;

    // 创建默认Jpeg通道，固定CIF分辨率
    for (i = 0; i < s32VinNum; i++)
    {
        // 填充结构体
        TVT_SDK_VIDEO_ENC_CFG_S stEncCfg = {0};
        stEncCfg.u32Width = (unsigned int)352;
        stEncCfg.u32Height = (unsigned int)288;
        stEncCfg.s32StreamID = (i * TVT_SDK_VIDEO_ENC_STREAM_BUTT) + TVT_SDK_VIDEO_ENC_JPEG_STREAM;
#if WKL_DEBUG
        TIPC_PRINTF_CHK("[****** WKL Debug ******]  Init TVT_SDK_VIDEO_ENC_STREAM_BUTT = %d ", TVT_SDK_VIDEO_ENC_STREAM_BUTT);
        TIPC_PRINTF_CHK("[****** WKL Debug ******]  Init s32StreamID = %d ", stEncCfg.s32StreamID);
#endif
        stEncCfg.enEncType = SDK_VENC_TYPE_JPEG;
        stEncCfg.u32FrameRate = 2;

        s32Ret = tvt_sdk_video_ctrl(TVT_SDK_VIDEO_CREATE_ENCODE_STREAM, (void*)&stEncCfg, sizeof(TVT_SDK_VIDEO_ENC_CFG_S));

        if (s32Ret != 0)
        {
            TIPC_PRINTF_ERR("TVT_SDK_VIDEO_CREATE_ENCODE_STREAM failed[%d]!!", s32Ret);
            return s32Ret;
        }

        //WKL!!!!!!!!:

        TVT_SDK_VIDEO_ENC_CFG_S stEncCfg_wkl = {0};
        stEncCfg_wkl.u32Width = (unsigned int)352;
        stEncCfg_wkl.u32Height = (unsigned int)288;
        stEncCfg_wkl.s32StreamID = 4;
#if WKL_DEBUG
        TIPC_PRINTF_CHK("[****** WKL Debug ******]  Init s32StreamID = %d ", stEncCfg_wkl.s32StreamID);
#endif
        stEncCfg_wkl.enEncType = SDK_VENC_TYPE_JPEG;
        stEncCfg_wkl.u32FrameRate = 2;

        s32Ret = tvt_sdk_video_ctrl(TVT_SDK_VIDEO_CREATE_ENCODE_STREAM, (void*)&stEncCfg_wkl, sizeof(TVT_SDK_VIDEO_ENC_CFG_S));

        if (s32Ret != 0)
        {
            TIPC_PRINTF_ERR("TVT_SDK_VIDEO_CREATE_ENCODE_STREAM failed[%d]!!", s32Ret);
            return s32Ret;
        }
    }

    return s32Ret;
}

/**
* @brief 获取码流个数
* @return 0：成功；非0：失败
*/
int SDKGetStreamSupport(void)
{
    int s32Ret = 0;
    int s32TotalStreamCnt = 0;
    TVT_SDK_PROD_CTRL_INFO stParamCtrl = {0};
    s32Ret = tvt_sdk_param_ctrl(TVT_SDK_PARAM_GET_SENSOR_CFG, &stParamCtrl, sizeof(stParamCtrl));
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get sensor cfg failed[%d]!!", s32Ret);
        return -1;
    }
    int s32VinNum = stParamCtrl.s32ParamSize / sizeof(TVT_SDK_PROD_SENSOR_S);
    s32TotalStreamCnt = s32VinNum * (TVT_SDK_VIDEO_ENC_THIRD_STREAM + 1);
    return s32TotalStreamCnt;
}


/**
* @brief 获取码流能力集
* @return 0：成功；非0：失败
*/
int SDKGetVEncSupport(SDK_SUPPORT_FRAME_RATE *pVEncSupportPara, SDK_VIDEO_FORMAT sdkVideoFormat)
{
    int s32Ret = 0;
    int num;

    TVT_SDK_PROD_CTRL_INFO stParamCtrl = {0};
    int video_cfg[2];
    video_cfg[0] = (int)sdkVideoFormat;
    video_cfg[1] = 0;
    stParamCtrl.pUserData = (void*)video_cfg;
    s32Ret = tvt_sdk_param_ctrl(TVT_SDK_PARAM_GET_STREAM_CFG, &stParamCtrl, sizeof(stParamCtrl));
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get stream cfg failed[%d]!!", s32Ret);
        return -1;
    }
    if ((pVEncSupportPara != NULL) && (stParamCtrl.pParam != NULL))
    {
        memcpy(pVEncSupportPara, stParamCtrl.pParam, stParamCtrl.s32ParamSize);
    }

    num = stParamCtrl.s32ParamSize / sizeof(SDK_SUPPORT_FRAME_RATE);
    return num;
}


/**
* @brief 创建编码通道
* @return 0：成功；非0：失败
*/
int SDKSetVEncPara(int iStream, SDK_VIDEO_ENCODE_TYPE eVEncType,
                   int iWidth, int iHeight, unsigned int uFrameRate,
                   SDK_VENC_MODE eVEncMode, unsigned int uBitRate,
                   unsigned int uGop, SDK_VENC_PROFILE eProfile)
{
    int s32Ret = 0;
    // 填充结构体
    TVT_SDK_VIDEO_ENC_CFG_S stEncCfg = {0};
    stEncCfg.u32Width = (unsigned int)iWidth;
    stEncCfg.u32Height = (unsigned int)iHeight;
    stEncCfg.u32GOP = uGop;
    stEncCfg.u32Profile = (unsigned int)eProfile;
    stEncCfg.enBitRateMode = eVEncMode;
    stEncCfg.u32BitRate = uBitRate;
    stEncCfg.s32StreamID = iStream;
    stEncCfg.enEncType = eVEncType;
    stEncCfg.u32FrameRate = uFrameRate;


// WKL
    // TIPC_PRINTF_CHK(" _________________________________________________________________");
    // TIPC_PRINTF_CHK(" _________________________________________________________________");
    // TIPC_PRINTF_CHK("   Width: %u", stEncCfg.u32Width );
    // TIPC_PRINTF_CHK("   Height: %u", stEncCfg.u32Height );
    // TIPC_PRINTF_CHK("   Gop: %u", stEncCfg.u32GOP);
    // TIPC_PRINTF_CHK("   StreamID: %d", stEncCfg.s32StreamID );
    // TIPC_PRINTF_CHK("   BitRate: %u", stEncCfg.u32BitRate);
    // TIPC_PRINTF_CHK("   FrameRate: %u", stEncCfg.u32FrameRate );
    // TIPC_PRINTF_CHK("   enum of EncType: %u", stEncCfg.enEncType );
    // TIPC_PRINTF_CHK("   enum of EncMode: %u", stEncCfg.enBitRateMode );
    // TIPC_PRINTF_CHK("   enum of Profile: %u", stEncCfg.u32Profile );
    // TIPC_PRINTF_CHK(" _________________________________________________________________");
    // TIPC_PRINTF_CHK(" _________________________________________________________________");

    if ( iStream == 4 || iStream == 3) {
        stEncCfg.u32FrameRate = 2;  //WKL note: ?? this value has not be corrected
    }

    s32Ret = tvt_sdk_video_ctrl(TVT_SDK_VIDEO_CREATE_ENCODE_STREAM, (void*)&stEncCfg, sizeof(TVT_SDK_VIDEO_ENC_CFG_S));
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("TVT_SDK_VIDEO_CREATE_ENCODE_STREAM failed[%d]!!", s32Ret);
        return s32Ret;
    }
    return s32Ret;
}

// static int first_flag = -1;

/**
 * @brief 获取编码数据
 * @return 0：成功；非0：失败
 */
int SDKGetVideoFrame(int iStream, IPCAMERA_AV_FRAME_INFO *pFrameInfo, int *pSLice, int *pSliceBuf, int SliceBufLen)
{


    int s32Ret = 0;
    // TIPC_PRINTF_CHK("__________________________________________________________");
    // TIPC_PRINTF_CHK("Stream = %d", iStream);

    // TIPC_PRINTF_CHK("Width = %d", pFrameInfo->width);
    // TIPC_PRINTF_CHK("Height = %d", pFrameInfo->height);
    // TIPC_PRINTF_CHK("ucKeyFrame = %d", pFrameInfo->ucKeyFrame);
    // TIPC_PRINTF_CHK("ucEncodeType = %d", pFrameInfo->ucEncodeType);
    // TIPC_PRINTF_CHK("ucStreamIndex = %d", pFrameInfo->ucStreamIndex);
    // TIPC_PRINTF_CHK("frameIndex = %d", pFrameInfo->frameIndex);

    // TIPC_PRINTF_CHK("__________________________________________________________");

    s32Ret = tvt_sdk_video_getEncodeStream(iStream, pFrameInfo, pSLice, pSliceBuf, SliceBufLen);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get encode data failed[ % d]!!", s32Ret);
        return s32Ret;
    }

    // if (first_flag < 0 ) {
    //     if ((access("/mnt/nfs/test_start.txt", F_OK)) != -1)
    //     {
    //         TIPC_PRINTF_CHK("********************** Start save **********************");
    //         first_flag = 1;
    //     }
    // }

    // if (first_flag > 0 && iStream == 1) {
    //     if (first_flag == 1) {
    //         FILE* fp = fopen ("henv_1.h265", "wb");
    //         if (fp)
    //         {
    //             fwrite(pFrameInfo->pData, pFrameInfo->length, 1, fp);
    //             fflush(fp);
    //         }
    //         fclose(fp);
    //     }
    //     if ( first_flag > 1) {
    //         FILE* fp = fopen ("henv_1.h265", "ab");
    //         if (fp)
    //         {
    //             fwrite(pFrameInfo->pData, pFrameInfo->length, 1, fp);
    //             fflush(fp);
    //         }
    //         fclose(fp);
    //     }
    //     if (first_flag == 500)
    //     {
    //         first_flag = 0;
    //         TIPC_PRINTF_CHK ("************************************************");
    //     } else {
    //         first_flag ++;
    //     }

    // }

    return s32Ret;
}


/**
* @brief 释放编码数据
* @return 0：成功；非0：失败
*/
int SDKReleaseFrame(IPCAMERA_AV_FRAME_INFO *pFrameInfo)
{
    int s32Ret = 0;
    // if (pFrameInfo->ucStreamIndex == 1)
    // {
    //     TIPC_PRINTF_CHK("_________________________ Release __________________________");
    //     TIPC_PRINTF_CHK("idx = %u", pFrameInfo->frameIndex);
    //     TIPC_PRINTF_CHK("pts = %llu", pFrameInfo->relativeTime);
    //     TIPC_PRINTF_CHK("ucFrameType = %d", pFrameInfo->ucFrameType);
    //     TIPC_PRINTF_CHK("ucStreamIndex = %d", pFrameInfo->ucStreamIndex);
    //     TIPC_PRINTF_CHK("____________________________________________________________");
    // }

    if (IPC_FRAME_TYPE_VIDEO == pFrameInfo->ucFrameType ||
            IPC_FRAME_TYPE_PIC == pFrameInfo->ucFrameType)
    {
        if (pFrameInfo->frameIndex == 0)
        {
            TIPC_PRINTF_CHK("!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Wrong !!!!!!!!!!!!!!!!!!!!!!!!!!!");
            return 0;
        }
        s32Ret = tvt_sdk_video_releaseEncodeStream(pFrameInfo);
    }
    else if (IPC_FRAME_TYPE_AUDIO == pFrameInfo->ucFrameType)
    {
        s32Ret = tvt_sdk_audio_releaseStream(pFrameInfo);
    }
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("release encode data failed[ % d]!!", s32Ret);
        return s32Ret;
    }
    return s32Ret;
}


/**
* @brief 请求I帧
* @return 0：成功；非0：失败
*/
int SDKRequestIFrame(int iStream)
{
    int s32Ret = 0;
    // TIPC_PRINTF_CHK("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    s32Ret = tvt_sdk_video_ctrl(TVT_SDK_VIDEO_REQUEST_I_FRAME, &iStream, sizeof(iStream));
    return s32Ret;
}


/**
* @brief 使能编码
* @return 0：成功；非0：失败
*/
int SDKStartVideo(int iStream)
{

    int s32Ret = 0;
    s32Ret = tvt_sdk_video_ctrl(TVT_SDK_VIDEO_START_STREAM, &iStream, sizeof(iStream));
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("start video[ % d] failed[ % d]!!", iStream, s32Ret);
        return s32Ret;
    }
    return s32Ret;
}


/**
* @brief 关闭编码通道
* @return 0：成功；非0：失败
*/
int SDKStopVideo(int iStream)
{
    if (iStream == 4 ) {
        TIPC_PRINTF_CHK ("Find Request SDKStopVideo!!!!!!!! Stream ID = % d, this stop is rejected!", iStream);
        return 0;
    }

    int s32Ret = 0;
    s32Ret = tvt_sdk_video_ctrl(TVT_SDK_VIDEO_DESTROY_ENCODE_STREAM, &iStream, sizeof(iStream));
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("stop video[ % d] failed[ % d]!!", iStream, s32Ret);
        return s32Ret;
    }
    return s32Ret;
}


/**
* @brief 设置视频水印
* @return 0：成功；非0：失败
*/
int SDKSetWatermark(char *ucUserData, int user_len)
{
    int s32Ret = 0;
    TVT_SDK_VIDEO_WATERMARK_CFG_S stWaterMark = {0};
    stWaterMark.u8Enable = 0;
    stWaterMark.pszUserData = ucUserData;
    stWaterMark.s32DataLens = user_len;
    TIPC_PRINTF_CHK("no impl ------------------------------ > todo");
    return 0;
    s32Ret = tvt_sdk_video_ctrl(TVT_SDK_VIDEO_SET_WATERMARK, &stWaterMark, sizeof(stWaterMark));
    return s32Ret;
}

/**
* @brief 关闭视频水印
* @return 0：成功；非0：失败
*/
int SDKClearWatermark(void)
{
    int s32Ret = 0;
    TVT_SDK_VIDEO_WATERMARK_CFG_S stWaterMark = {0};
    stWaterMark.u8Enable = 0;
    TIPC_PRINTF_CHK("no impl ------------------------------ > todo");
    return 0;

    s32Ret = tvt_sdk_video_ctrl(TVT_SDK_VIDEO_SET_WATERMARK, &stWaterMark, sizeof(stWaterMark));
    return s32Ret;
}

/**
* @brief 关闭视频水印
* @return 0：成功；非0：失败
*/
int SDKGetJPEG(int iStream, IPCAMERA_AV_FRAME_INFO *pFrameInfo)
{
    int s32Ret = 0;
    s32Ret = tvt_sdk_video_getJpegBuffer(iStream, pFrameInfo);
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("get jpeg encode data failed[ % d]!!", s32Ret);
        return s32Ret;
    }
    return s32Ret;
}


/**
* @brief 不支持
* @return 0：成功；非0：失败
*/
int SDKGetHighPar(SDK_SUPPORT_HIGHPAR parm)
{
    return -1;
}


/**
* @brief 获取视频信息，不支持
* @return 0：成功；非0：失败
*/
int  SDKGetVideoInfo( SDK_VENC_MODE *enRcMode,
                      int *u32Bitrate,
                      int *u32TargetFramerate,
                      int *iWidth,
                      int *iHeight,
                      int iIndex )
{
    return -1;
}


/**
* @brief 设置帧Slice分片
* @return 0：成功；非0：失败
*/
int SDKSetVEncSlice(int iSlice)
{
    // TODO, this is empty interface
    int s32Ret = 0;
    s32Ret = tvt_sdk_video_ctrl(TVT_SDK_VIDEO_SET_SLICE, &iSlice, sizeof(int));
    return s32Ret;
}


/**
* @brief 设置时间OSD配置
* @return 0：成功；非0：失败
*/
int SDKSetTimeOSDPara(int iShowTimeOSD, SDK_POSITION *pSDKPos, time_dispaly_format timeFormat)
{

    int s32Ret = 0;
    // TIPC_PRINTF_CHK("*************** SDK Set Time OSD Para ****************");
    // TIPC_PRINTF_CHK("no impl ------------------------------ > todo");
    // return 0;
    if (NULL == pSDKPos)
    {
        TIPC_PRINTF_ERR("NULL POINTER!!");
        return -1;
    }

    int i = 0;
    for (i = 0; i < g_stSdkCfg.s32VinCnt; i++)
    {
        TVT_SDK_OSD_TIME_CFG_S stOSDTimeCfg = {0};
        stOSDTimeCfg.s32VinIDX = i;
        stOSDTimeCfg.stTimeFmt.u32Enable = iShowTimeOSD;
        stOSDTimeCfg.stTimeFmt.enDateFormatEx = (timeFormat > day_mon_year) ? TVT_SDK_OSD_DATE_CROSSBAR : TVT_SDK_OSD_DATE_CROSSBAR_TO_SLASH;
        switch (timeFormat)
        {
        case year_mon_day:
        case year_mon_day_line:
            stOSDTimeCfg.stTimeFmt.enDateFormat = TVT_SDK_OSD_DATE_YYYY_MM_DD;
            break;
        case mon_day_year:
        case mon_day_year_line:
            stOSDTimeCfg.stTimeFmt.enDateFormat = TVT_SDK_OSD_DATE_MM_DD_YYYY;
            break;
        case day_mon_year:
        case day_mon_year_line:
            stOSDTimeCfg.stTimeFmt.enDateFormat = TVT_SDK_OSD_DATE_DD_MM_YYYY;
            break;
        default:
            stOSDTimeCfg.stTimeFmt.enDateFormat = TVT_SDK_OSD_DATE_YYYY_MM_DD;
            break;
        }
        memcpy(&stOSDTimeCfg.stPosition, pSDKPos, sizeof(SDK_POSITION));
        s32Ret = tvt_sdk_video_ctrl(TVT_SDK_VIDEO_SET_TIME_OSD_CFG, &stOSDTimeCfg, sizeof(stOSDTimeCfg));
    }
    return s32Ret;
}

/**
* @brief 设置Comm OSD
* @return 0：成功；非0：失败
*/
int SDKSetOSDPara(int iOsdNo, unsigned long dwShowOsd, SDK_POSITION *pSDKPos, char *pcOsd)
{
    int s32Ret = 0;

    // if (iOsdNo == 0) {
    //     iOsdNo = 1;
    // } else {
    //     return 0;
    // }
    // TODO, Check APP parameter
    TIPC_PRINTF_CHK("no impl ------------------------------ > todo");
    return 0;
    if (NULL == pSDKPos || (NULL == pcOsd && dwShowOsd != 0))
    {
        TIPC_PRINTF_ERR("NULL POINTER!!");
        return -1;
    }

    TIPC_PRINTF_CHK("_______________________________________________________________________________");

    TIPC_PRINTF_CHK("iOsdNo = % d ", iOsdNo);
    TIPC_PRINTF_CHK("dwShowOsd = % d ", dwShowOsd);
    TIPC_PRINTF_CHK("x = % d ", pSDKPos->x);
    TIPC_PRINTF_CHK("y = % d ", pSDKPos->y);
    TIPC_PRINTF_CHK("width = % d ", pSDKPos->width);
    TIPC_PRINTF_CHK("height = % d ", pSDKPos->height);
    TIPC_PRINTF_CHK("cOsd = % p ", pcOsd);

    TIPC_PRINTF_CHK("_______________________________________________________________________________");

    int i = 0;
    for (i = 0; i < g_stSdkCfg.s32VinCnt; i++)
    {
        TVT_SDK_OSD_COMM_CFG_S stOSDCommCfg = {0};
        stOSDCommCfg.s32VinIDX = i;
        stOSDCommCfg.u32OSDIDX = iOsdNo;
        stOSDCommCfg.stOSDInfo.u32Enable = (0 == dwShowOsd) ? 0 : 1;
        // 应用层下发dwShowOsd为0x10的时候指的是图像数据
        stOSDCommCfg.stOSDInfo.enOSDType = (0x10 == dwShowOsd) ? TVT_SDK_OSD_TYPE_IMG : TVT_SDK_OSD_TYPE_CHAR;
        memcpy(&stOSDCommCfg.stPosition, pSDKPos, sizeof(SDK_POSITION));
        stOSDCommCfg.stOSDInfo.pszData = (unsigned char*)pcOsd; // 注意这里是赋值地址，需要在退出前及时拷贝

        // wkl
        // s32Ret = tvt_sdk_video_ctrl(TVT_SDK_VIDEO_UPDATE_COMM_OSD, &stOSDCommCfg, sizeof(stOSDCommCfg));
        // TIPC_PRINTF_CHK("[****** WKL_DEBUG ******]     SDKSetOSDPara_enter");
        s32Ret = tvt_sdk_video_ctrl(TVT_SDK_VIDEO_SET_COMM_OSD, &stOSDCommCfg, sizeof(stOSDCommCfg));
    }

    return s32Ret;
}


/**
* @brief 设置Mask/Mosaic
* @return 0：成功；非0：失败
*/
int SDKSetOsdMask(SDK_OSD_MASK_CONFIG *pOSDInfo, int iCount)
{
    int s32Ret = 0;
    int i = 0, j = 0;
    TIPC_PRINTF_CHK("no impl ------------------------------ > todo");
    return 0;

    if (NULL == pOSDInfo)
    {
        TIPC_PRINTF_ERR("NULL POINTER!!");
        return -1;
    }

    for (i = 0; i < g_stSdkCfg.s32VinCnt; i++)
    {
        for (j = 0; j < iCount; j++)
        {
            SDK_OSD_MASK_CONFIG* pstMaskCfg = &pOSDInfo[j];
            TVT_SDK_VIDEO_MASK_CFG_S stMaskCfg = {0};
            stMaskCfg.s32VinIDX = i;
            memcpy(&stMaskCfg.stPosition, &pstMaskCfg->rectang, sizeof(SDK_POSITION));
            stMaskCfg.u32Enable = pstMaskCfg->iShow;
            s32Ret = tvt_sdk_video_ctrl(TVT_SDK_VIDEO_SET_MASK, &stMaskCfg, sizeof(stMaskCfg));
        }
    }
    return s32Ret;
}


/**
* @brief 设置ROI
* @return 0：成功；非0：失败
*/
int SDKSetROI(SDK_ROI_CONFIG *pROI, int iCount)
{
    int s32Ret = 0;
    int i = 0, j = 0;
    // TIPC_PRINTF_CHK("no impl ------------------------------ > todo");
    // return 0;

    if (NULL == pROI)
    {
        TIPC_PRINTF_ERR("NULL POINTER!!");
        return -1;
    }

    for (i = 0; i < g_stSdkCfg.s32VinCnt; i++)
    {
        TVT_SDK_VIDEO_ROI_CFG_S stROICfg = {0};
        stROICfg.s32VinIDX = i;
        stROICfg.pstROIInfo = malloc(iCount * sizeof(TVT_SDK_VIDEO_ROI_INFO_S));
        stROICfg.s32ROINum = iCount;
        if (NULL == stROICfg.pstROIInfo)
        {
            TIPC_PRINTF_ERR("malloc failed!!");
            return -1;
        }
        for (j = 0; j < iCount; j++)
        {
            SDK_ROI_CONFIG* pstROICfg = &pROI[j];
            stROICfg.pstROIInfo[j].s32Level = pstROICfg->level;
            stROICfg.pstROIInfo[j].u32Enable = pstROICfg->iShow;
            stROICfg.pstROIInfo[j].u32ROIIDX = j;
            memcpy(&stROICfg.pstROIInfo[j].stPosition, &pstROICfg->rectang, sizeof(SDK_POSITION));
        }
        s32Ret |= tvt_sdk_video_ctrl(TVT_SDK_VIDEO_SET_ROI, &stROICfg, sizeof(stROICfg));
        if (stROICfg.pstROIInfo)
        {
            free(stROICfg.pstROIInfo);
            stROICfg.pstROIInfo = NULL;
        }
    }
    return s32Ret;
}
