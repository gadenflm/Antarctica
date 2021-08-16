#ifndef _TVT_SDK_VIDEO_FACEDT_H_
#define _TVT_SDK_VIDEO_FACEDT_H_

#include "sdk_define.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _tvt_Img_Frame
{
	unsigned int u32Width;
	unsigned int u32Height;

	unsigned int enPixelFormat;  //PIXEL_FORMAT_E
	unsigned int	u32Stride[3];
	union
	{
		struct
		{
			unsigned long long	u64PhyAddr[3];
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
} tvt_Img_Frame;


int tvt_sdk_facedt_init(int s32VinNum, const int* s32VinDevID);
int tvt_sdk_facedt_exit(void);


/**
 * @breif  Interface for face detection.
 * @param  eFaceDtTypePara Enum to select whict ctrl you want. refer to @sdk_define.h
 * @param  pPara           The pointrer of parameter
 * @param  len             The size of parameter
 * @return                 0 Success; others Failed;
 */
int tvt_sdk_video_facedt_Ctrl (SDK_FACE_DT_CTRL_E eFaceDtTypePara, void *pPara, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif