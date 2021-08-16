// 调用说明:
// 系统启动时,应用程序会先调InitSDK
//  video  : 先调SetVEncPara,运行中调SetVEncPara,StartVideo,StopVideo没有限制
//  Audio  : 先调SetAudioPara,运行中调SetAudioPara,StartAudio,StopAudio没有限制
// iStream : 视频流的iStream独立开,不同流iStream不一样,应用程序取数据时,也是根据它区分
//  说明   : 后续可能会修改两个地方: 1 获取流信息和编码相关的信息  2 取视频数据时得到分片信息
//  修改   : 1 以前iStream按不同编码方式区分,现在统一区分,iStream在整个SDK接口里唯一
//           2 分辨率以前是传枚举值,现在直接传宽高
//           3 一些接口简化,去掉了没用的参数

#ifndef SDK_FUNC_H
#define SDK_FUNC_H

#include "sdk_define.h"

// 增加实现空函数返回的数值
#define IPC_SDK_FUNC_NO_SUPPORT -100001

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#ifdef __FUNC_SUPPORT_CAR_PLATE__
typedef enum
{
    SDK_CAR_NULL = 0,
    SDK_CAR_REGISTER_EVENT_CALLBACKFUN ,
    SDK_CAR_REGISTER_OSD_FONT,
    SDK_CAR_SET_TEST,
    SDK_CAR_SET_OSD_INFO,
    SDK_CAR_SET_TRRIGER,
} SDK_CAR_RECGNIZE_TYPE;
#ifdef TEST_SDK
typedef void(*SDKCarcallbackFun)(void* para1);
void SDKRegisterCallbackFun(SDKCarcallbackFun *pFunc, void *para);
#else
typedef int(*SDKCarcallbackFun)(void* para1);
void SDKRegisterCallbackFun(SDKCarcallbackFun *pFunc, SDK_CAR_RECGNIZE_TYPE eType, void *para);
#endif
int SDKCarSetParam(SDK_CAR_RECGNIZE_TYPE eType,  void *pPara, int iParaLen);
#endif
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////



// 整个 SDK 初始化和退出
int SDKInit(SDK_VIDEO_FORMAT eInputFormat);
int SDKExit(void);

// 一共支持几路流
int SDKGetStreamSupport(void);
// 获取 SDK 支持属性
//  return  :       iCount     :  所有视频流支持分辨率的总个数
//            pVEncSupportPara :  非空时返回每个视频流的具体参数
int SDKGetVEncSupport(SDK_SUPPORT_FRAME_RATE *pVEncSupportPara, SDK_VIDEO_FORMAT sdkVideoFormat);

// 视频相关
int SDKSetVEncPara(int iStream, SDK_VIDEO_ENCODE_TYPE eVEncType,
                   int iWidth, int iHeight, unsigned int uFrameRate,
                   SDK_VENC_MODE eVEncMode, unsigned int uBitRate,
                   unsigned int uGop, SDK_VENC_PROFILE eProfile);
int SDKRequestIFrame(int iStream);

int SDKStartVideo(int iStream);
int SDKStopVideo(int iStream);

// OSD 相关
/*
* 个别参数说明
* input: dwShowTimeOSD 是否显示时间戳
*        pSDKPos       指向结构体SDK_POSITION， 这个结构体中的宽高无用
*/
int SDKSetTimeOSDPara(int iShowTimeOSD, SDK_POSITION *pSDKPos, time_dispaly_format timeFormat);

/*
* 个别参数说明
* input: dwOsdNo    osd序号 0 - 最大可显示几块osd
*        dwShowOsd  是否显示osd
*        pSDKPos    osd显示的起点和区域的宽高
*        pcOsd      osd点阵
*/
int SDKSetOSDPara(int iOsdNo, unsigned long dwShowOsd, SDK_POSITION *pSDKPos, char *pcOsd);

// 音频相关
int SDKSetAudioPara(SDK_AUDIO_RATE eRate, SDK_AUDIO_BIT_WIDTH eBitWidth,
                    SDK_AUDIO_ENCODE_TYPE eEncType, unsigned int uPtPerFrame);
int SDKStartAudio(int iStream);
int SDKStopAudio(int iStream);

// 获取视频音频数据,如果有分片,需要扩接口记录信息
int SDKSetWatermark(char *ucUserData, int user_len);
int SDKClearWatermark(void);
int SDKGetVideoFrame(int iStream, IPCAMERA_AV_FRAME_INFO *pFrameInfo, int *pSLice, int *pSliceBuf, int SliceBufLen);
int SDKGetAudioFrame(int iStream, IPCAMERA_AV_FRAME_INFO *pFrameInfo);
int SDKPushAudioFrame(int iStream, IPCAMERA_AV_FRAME_INFO *pFrameInfo);
int SDKGetJPEG(int iStream, IPCAMERA_AV_FRAME_INFO *pFrameInfo);
int SDKReleaseFrame(IPCAMERA_AV_FRAME_INFO *pFrameInfo);
int SDKGetHighPar(SDK_SUPPORT_HIGHPAR parm);
// 图像参数
int SDKSetImagePara(SDK_IMAGE_TYEP_PARA eType, void *pData, int iLength);
int SDKGetImagePara(SDK_IMAGE_TYEP_PARA eType, void *pData, int iLength);

int  SDKGetVideoInfo( SDK_VENC_MODE *enRcMode,
                      int *u32Bitrate,
                      int *u32TargetFramerate,
                      int *iWidth,
                      int *iHeight,
                      int iIndex );
//  input : iSlice : 0 not slice; 1 slice
int SDKSetVEncSlice(int iSlice);

// 获取和设置外围设备
//  return : success 0; fail : -1
int SDKGetDeviceState(SDK_GET_DEVICE_TYPE_PARA eDevPara, void *pData, int iBufLen);
int SDKSetDeviceState(SDK_SET_DEVICE_TYPE_PARA eDevPara, const void *pData, int iDataLen);

// AP will call this func per 200ms
int SDKLoopFunc(void);


int SDKMotionDetectEnable(void);
int SDKMotionDetectDisable(void);
int SDKGetMotionStatus(unsigned char *pStatus);
int SDKGetMotionGrid(unsigned long *pWidthGrid, unsigned long *pHeightGrid);
int SDKSetMotion(int iMotionSensitive, const ULONGLONG *pArea);

int SDKObjectRemoveEnable(void);
int SDKObjectRemoveDisable(void);
int SDKObjectRemove(int index, const int *pPoint, int PointNum); //pPoint：左上，顺时针依次写入每个点的坐标
int SDKSetObjectRemoveSensitive(int Sensitive);

int SDKExceptionEnable(SDK_EXCEPTION_MODE mode);
int SDKExceptionDisable(SDK_EXCEPTION_MODE mode);
int SDKSetExceptionSensitive(int Sensitive);

int SDKSetOsdMask(SDK_OSD_MASK_CONFIG *pOSDInfo, int iCount);
int SDKSetROI(SDK_ROI_CONFIG *pROI, int iCount);


int SDKGetLensPara(SDK_LENS_TYPE_PARA eLensType, void *pPara, unsigned int len);
int SDKSetLensPara(SDK_LENS_TYPE_PARA eLensType, void *pPara, unsigned int  len);

// 获取缩放后的yuv(420)的数据
int SDKGetScaleYuvData(int iWidth, int iHeight, IPCAMERA_AV_FRAME_INFO *pFrameInfo);
int SDKYuvDataControl(SDK_YUVDATA_TYPE_PARA eYuvDataTypePara, void *pPara, unsigned int len);

int SDKGetPtzPara(SDK_PTZ_TYPE_PARA ePtzType, void *pPara, unsigned int len);
int SDKSetPtzPara(SDK_PTZ_TYPE_PARA ePtzType, void *pPara, unsigned int len);


char *SDKGetUpdateMem(void);
int SDKUpdateFlash(char *pUpdateMem, unsigned int iUpdateSize, unsigned int iUpdateType);
int GetSdkUpdateFlag(void);

int SDKGetFlashParamItem( char* param_value, int param_len, int item_number );
int SDKSetFlashParamItem( char* param_value, int param_len, int item_number );


int SDKFishEyeControl(SDK_FISHETE_TYPE_PARA_E eType, void *pData, int iLength);

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/// 以下接口在4.0.2.0的软件暂时不使用
/* Get camera type */
int SDKGetCamerType(void);   ///<: 添加产品相关配置中

/*
 * 开机 运行一次，恢复上次关机时zoom的位置
 */
int SDKSetZoomValue(int ZoomValue);

/*
 * 保留当前zoom值，每次调用SDKSetAF()函数后都应该调用该值并保存
 */
int SDKGetZoomValue(void);

int SDKSetAF(SDK_AF_ACTION type, void * value);

// add tvt ptz osd process begin
// start ptz config page
int SDKStartTVTPTZControl(void);
// stop ptz config page
int SDKStopTVTPTZControl(void);
// update whole page
int SDKUpdatePage(void);
// input :     iLine   : line, 0, 1, 2..10
//         pPTZOSDLine : osd content
int SDKSetOneLine(int iLine, SDK_POSITION *pSDKPos, char *pcOsd);
// input :     iLine   : line, 0, 1, 2..10
//          iPosition  : the position start to update, 0, 1, 2...10
int SDKUpdatePartOfLine(int iLine, int iPosition, SDK_POSITION *pSDKPos, char *pcOsd);
// input :   bFlicker  : true, flicker;false ,not flicker
//             iLine   : line, 0, 1, 2..10
//          iPosition  : the position start to flicker, 0, 1, 2...10
//          iChCount   : flicker charactor count
int SDKSetFlicker(int iFlicker, int iLine, int iPosition, int iChCount);
int SDKStartTVTPTZOSD(int iType);
int SDKStopTVTPTZOSD(int iType);
int SDKClearTVTPTZOSD(void);
// add tvt ptz osd process end

int SDKFaceDtControl(SDK_FACE_DT_CTRL_E eFaceDtTypePara, void *pPara, unsigned int len);

int SDKAudioControl(SDK_AUDIO_CTRL_E enAudioCtrl, void *pPara, unsigned int len);

int SDKSetOSDAttribute(SDK_OSD_ATTRIBUTE *pstOsdAttri, int iOsdNo);


// 如下函数主要使用于有界面的IPC，或者客户定制需要，不作为TVT AP的需求。
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

//////////设置鼠标位置
int SDKMouseSetPosition( int dx, int dy );
//////////设置鼠标样式;len为固定长度4096
int SDKMouseSetShape( char *pdata, int len );
//////////设置鼠标更改位置
int SDKMouseChangeScreen( int x, int y, int w, int h );

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

int SDKSetOsMemSize( int iOsMem );
int SDKGetBuildVersionInfo(unsigned char* pucBuildVersionInfo, int buf_len );

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

// wkl note: for amba sdk_thread
int SDK_amba_thread_start(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
