#ifndef _AMBA_YUV_H_
#define _AMBA_YUV_H_

#include "iav_ioctl.h"
#include <basetypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct amba_yuv_frameinfo_s {
	unsigned char* 		y_vir_addr;
	unsigned char* 		uv_vir_addr;
	unsigned int        pitch;
	unsigned int        width;
	unsigned int        height;
	unsigned int        pts;
	unsigned long long  y_phy_addr;
	unsigned long long  uv_phy_addr;
} AMBA_YUV_FRAMEINFO_S;

typedef struct amba_yuv_jpeginfo_s {
	unsigned int             JpegWidth;
	unsigned int             JpegHeight;
	unsigned int             JpegSize;
	char*        			 JpegAddr;
} AMBA_YUV_JPEGINFO_S;


int amba_yuv_init(const int s32IavFD, const int s32VinNum);
int amba_yuv_exit();

int amba_yuv_GetFrame(const unsigned int index, AMBA_YUV_FRAMEINFO_S* pstYUVFrameInfo);
int amba_yuv_ReleaseFrame(const unsigned int index);
int amba_yuv_ReleaseAllFrame(void);

int SetYuvBuffSize(const unsigned int width, const unsigned int height);

int amba_yuv_EncToJpeg (const unsigned int index, const unsigned int pos_x, const unsigned int pos_y,
                        const unsigned int enc_width, const unsigned int enc_height, const unsigned int dest_width,
                        const unsigned int dest_height, AMBA_YUV_JPEGINFO_S* jpeg_info);

#ifdef __cplusplus
}
#endif

#endif