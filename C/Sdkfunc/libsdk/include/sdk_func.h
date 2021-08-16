// ����˵��:
// ϵͳ����ʱ,Ӧ�ó�����ȵ�InitSDK
//  video  : �ȵ�SetVEncPara,�����е�SetVEncPara,StartVideo,StopVideoû������
//  Audio  : �ȵ�SetAudioPara,�����е�SetAudioPara,StartAudio,StopAudioû������
// iStream : ��Ƶ����iStream������,��ͬ��iStream��һ��,Ӧ�ó���ȡ����ʱ,Ҳ�Ǹ���������
//  ˵��   : �������ܻ��޸������ط�: 1 ��ȡ����Ϣ�ͱ�����ص���Ϣ  2 ȡ��Ƶ����ʱ�õ���Ƭ��Ϣ
//  �޸�   : 1 ��ǰiStream����ͬ���뷽ʽ����,����ͳһ����,iStream������SDK�ӿ���Ψһ
//           2 �ֱ�����ǰ�Ǵ�ö��ֵ,����ֱ�Ӵ����
//           3 һЩ�ӿڼ�,ȥ����û�õĲ���

#ifndef SDK_FUNC_H
#define SDK_FUNC_H

#include "sdk_define.h"

// ����ʵ�ֿպ������ص���ֵ
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



// ���� SDK ��ʼ�����˳�
int SDKInit(SDK_VIDEO_FORMAT eInputFormat);
int SDKExit(void);

// һ��֧�ּ�·��
int SDKGetStreamSupport(void);
// ��ȡ SDK ֧������
//  return  :       iCount     :  ������Ƶ��֧�ֱַ��ʵ��ܸ���
//            pVEncSupportPara :  �ǿ�ʱ����ÿ����Ƶ���ľ������
int SDKGetVEncSupport(SDK_SUPPORT_FRAME_RATE *pVEncSupportPara, SDK_VIDEO_FORMAT sdkVideoFormat);

// ��Ƶ���
int SDKSetVEncPara(int iStream, SDK_VIDEO_ENCODE_TYPE eVEncType,
                   int iWidth, int iHeight, unsigned int uFrameRate,
                   SDK_VENC_MODE eVEncMode, unsigned int uBitRate,
                   unsigned int uGop, SDK_VENC_PROFILE eProfile);
int SDKRequestIFrame(int iStream);

int SDKStartVideo(int iStream);
int SDKStopVideo(int iStream);

// OSD ���
/*
* �������˵��
* input: dwShowTimeOSD �Ƿ���ʾʱ���
*        pSDKPos       ָ��ṹ��SDK_POSITION�� ����ṹ���еĿ������
*/
int SDKSetTimeOSDPara(int iShowTimeOSD, SDK_POSITION *pSDKPos, time_dispaly_format timeFormat);

/*
* �������˵��
* input: dwOsdNo    osd��� 0 - ������ʾ����osd
*        dwShowOsd  �Ƿ���ʾosd
*        pSDKPos    osd��ʾ����������Ŀ��
*        pcOsd      osd����
*/
int SDKSetOSDPara(int iOsdNo, unsigned long dwShowOsd, SDK_POSITION *pSDKPos, char *pcOsd);

// ��Ƶ���
int SDKSetAudioPara(SDK_AUDIO_RATE eRate, SDK_AUDIO_BIT_WIDTH eBitWidth,
                    SDK_AUDIO_ENCODE_TYPE eEncType, unsigned int uPtPerFrame);
int SDKStartAudio(int iStream);
int SDKStopAudio(int iStream);

// ��ȡ��Ƶ��Ƶ����,����з�Ƭ,��Ҫ���ӿڼ�¼��Ϣ
int SDKSetWatermark(char *ucUserData, int user_len);
int SDKClearWatermark(void);
int SDKGetVideoFrame(int iStream, IPCAMERA_AV_FRAME_INFO *pFrameInfo, int *pSLice, int *pSliceBuf, int SliceBufLen);
int SDKGetAudioFrame(int iStream, IPCAMERA_AV_FRAME_INFO *pFrameInfo);
int SDKPushAudioFrame(int iStream, IPCAMERA_AV_FRAME_INFO *pFrameInfo);
int SDKGetJPEG(int iStream, IPCAMERA_AV_FRAME_INFO *pFrameInfo);
int SDKReleaseFrame(IPCAMERA_AV_FRAME_INFO *pFrameInfo);
int SDKGetHighPar(SDK_SUPPORT_HIGHPAR parm);
// ͼ�����
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

// ��ȡ��������Χ�豸
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
int SDKObjectRemove(int index, const int *pPoint, int PointNum); //pPoint�����ϣ�˳ʱ������д��ÿ���������
int SDKSetObjectRemoveSensitive(int Sensitive);

int SDKExceptionEnable(SDK_EXCEPTION_MODE mode);
int SDKExceptionDisable(SDK_EXCEPTION_MODE mode);
int SDKSetExceptionSensitive(int Sensitive);

int SDKSetOsdMask(SDK_OSD_MASK_CONFIG *pOSDInfo, int iCount);
int SDKSetROI(SDK_ROI_CONFIG *pROI, int iCount);


int SDKGetLensPara(SDK_LENS_TYPE_PARA eLensType, void *pPara, unsigned int len);
int SDKSetLensPara(SDK_LENS_TYPE_PARA eLensType, void *pPara, unsigned int  len);

// ��ȡ���ź��yuv(420)������
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
/// ���½ӿ���4.0.2.0�������ʱ��ʹ��
/* Get camera type */
int SDKGetCamerType(void);   ///<: ��Ӳ�Ʒ���������

/*
 * ���� ����һ�Σ��ָ��ϴιػ�ʱzoom��λ��
 */
int SDKSetZoomValue(int ZoomValue);

/*
 * ������ǰzoomֵ��ÿ�ε���SDKSetAF()������Ӧ�õ��ø�ֵ������
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


// ���º�����Ҫʹ�����н����IPC�����߿ͻ�������Ҫ������ΪTVT AP������
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

//////////�������λ��
int SDKMouseSetPosition( int dx, int dy );
//////////���������ʽ;lenΪ�̶�����4096
int SDKMouseSetShape( char *pdata, int len );
//////////����������λ��
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
