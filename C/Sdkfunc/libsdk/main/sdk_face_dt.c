#include "sdk_func.h"
#include "tvt_sdk_print.h"
#include "tvt_sdk_video_facedt.h"
/**
 * @breif  Interface for face detection.
 * @param  eFaceDtTypePara Enum to select whict ctrl you want. refer to @sdk_define.h
 * @param  pPara           The pointrer of parameter
 * @param  len             The size of parameter
 * @return                 0 Success; others Failed;
 */
int SDKFaceDtControl(SDK_FACE_DT_CTRL_E eFaceDtTypePara, void *pPara, unsigned int len) {
	int s32Ret;
	s32Ret = tvt_sdk_video_facedt_Ctrl (eFaceDtTypePara, pPara, len);
	return s32Ret;
}
