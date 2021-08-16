#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "tvt_sdk_video_facedt.h"
#include "tvt_sdk_print.h"
#include "sdk_define.h"
#include "tvt_sdk_chip.h"

#define MAX_RESULT_MEM_NUM  8  // refer to amba_vproc.c @stIavSysRes.canvas_cfg[s32YuvCanvasID].feed_frame_num 
#define MAX_DT_FACE_NUM     32 //

#define HUAAN_DT_MIN(x,y) ((x) > (y) ? (y) : (x))
#define HUAAN_DT_MAX(x,y) ((x) > (y) ? (x) : (y))

typedef struct tvt_sdk_facedt_info_s
{
	char                        u8Init;                                 ///< init flag
	char  						u8Start;								///< start flag

	char  						needZoom;
	char                        getYuvFlag;

	unsigned int                yuv_width;
	unsigned int                yuv_height;
	// unsigned int                yuv_fps ; TODO
	unsigned int                dt_width;
	unsigned int                dt_height;

	CHIP_YUV_INFO_S 			stCurVideoFrame;
	CHIP_YUV_INFO_S 			astVideoFrame[MAX_RESULT_MEM_NUM];
	int 						allocFlag[MAX_RESULT_MEM_NUM];
	int  						curFaceNum[MAX_RESULT_MEM_NUM];

	SDK_JPEG_INFO 				panoramaJpegMem[MAX_RESULT_MEM_NUM];
	SDK_JPEG_INFO 				faceJpegMem[MAX_RESULT_MEM_NUM][MAX_DT_FACE_NUM];

	unsigned int 				indexMem;
	tvt_Img_Frame 				TvtDtImgFrame[MAX_RESULT_MEM_NUM]; //有可能缩放

	pthread_mutex_t             s32Mutex;                               ///< mutex
	char                        szMutexName[32];                        ///< mutex name
	unsigned int                u32MutexLockCnt;                        ///< count for mutex lock times

} TVT_SDK_FACEDT_INFO_S;

static TVT_SDK_FACEDT_INFO_S* g_pstSdkFaceDtInfo = NULL;

void trans_frame_format(CHIP_YUV_INFO_S*pSrc, tvt_Img_Frame*pFrame)
{
	pFrame->u32Width = pSrc->width;
	pFrame->u32Height = pSrc->height;
	pFrame->u32Stride[0] = pSrc->pitch; //y ptich
	pFrame->u32Stride[1] = pSrc->pitch; //u ptich
	pFrame->u32Stride[2] = pSrc->pitch; //v ptich

	pFrame->Addr.st64.u64PhyAddr[0] = pSrc->y_phy_addr;
	pFrame->Addr.st64.u64PhyAddr[1] = pSrc->uv_phy_addr;
	pFrame->Addr.st64.u64PhyAddr[2] = pSrc->uv_phy_addr;

	pFrame->Addr.st64.u64VirAddr[0] = pSrc->y_addr;
	pFrame->Addr.st64.u64VirAddr[1] = pSrc->uv_addr;
	pFrame->Addr.st64.u64VirAddr[2] = pSrc->uv_addr;

	pFrame->u64PTS = pSrc->pts;

	pFrame->u32PoolId = 7;
	// not uesd
	pFrame->enPixelFormat = 0;

	pFrame->s16OffsetTop = 0;
	pFrame->s16OffsetBottom = 0;
	pFrame->s16OffsetLeft = 0;
	pFrame->s16OffsetRight = 0;

	pFrame->enModId = 0;

	return;
}


////////////////////////////////////////////////////////////////////////////////
int giTvtfaceMaxNumber = 8;
void tvt_face_set_framebuf_max_number( int number )
{
	if ( number < 0 )
	{
		TIPC_PRINTF_INFO("error frame buffer number = %d, set default 8", number );
		giTvtfaceMaxNumber = 8;
	}
	else if ( number > MAX_RESULT_MEM_NUM )
	{
		TIPC_PRINTF_INFO("error frame buffer number = %d, set max number", number );
		giTvtfaceMaxNumber = MAX_RESULT_MEM_NUM;
	}
	else
	{
		giTvtfaceMaxNumber = number;
	}
}
inline int tvt_face_get_framebuf_number(void)
{
	return giTvtfaceMaxNumber;
}
///////////////////////////////////////////////////////////////////////////////
static int tvt_getFreeResultMemIndex()
{
	int i;
	int index = -1;
	TVT_SDK_FACEDT_INFO_S* pstCfg = g_pstSdkFaceDtInfo;
	if ( !pstCfg )
	{
		TIPC_PRINTF_ERR("NULL PTR!!!");
		return -1;
	}

	if (pstCfg->needZoom) {
		//TODO
	}
	else {
		for (i = 0; i < tvt_face_get_framebuf_number(); i++) {
			if (0 == pstCfg->allocFlag[i])
			{
				index = i;
				pstCfg->allocFlag[i] = 1;
				break;
			}
		}
	}
	return index;
}

static int tvt_face_releaseYuvData(CHIP_YUV_INFO_S *pstVideoFrame)
{
	TVT_SDK_FACEDT_INFO_S* pstCfg = g_pstSdkFaceDtInfo;
	if ( !pstCfg )
	{
		TIPC_PRINTF_ERR("NULL PTR!!!");
		return -1;
	}
	if (NULL == pstVideoFrame)
	{
		TIPC_PRINTF_ERR("NULL PTR!!!");
		return -1;
	}

	if ((pstVideoFrame->width > 0) && (pstVideoFrame->height > 0))
	{
		// if (AiGetSnapJpegSizeMode())
		// {
		chip_video_ReleaseYuvFrame(pstVideoFrame->index);
		// }
		// else
		// {
		// }
	}
	else
	{
		TIPC_PRINTF_INFO("Invalid Frame!");
	}

	pstCfg->getYuvFlag = 0;
	memset(pstVideoFrame, 0, sizeof(CHIP_YUV_INFO_S));

	//TIPC_PRINTF_INFO("");

	return 0;
}

static int tvt_freeJpegMem(SDK_JPEG_INFO * pJpegMem)
{
	if (NULL == pJpegMem)
	{
		TIPC_PRINTF_ERR("pJpegMem = NULL !!!");
		return -1;
	}

	pJpegMem->iJpegWidth = 0;
	pJpegMem->iJpegHeight = 0;
	pJpegMem->iJpegLen = 0;

	if (pJpegMem->pJpegData)
	{
		free(pJpegMem->pJpegData);
		pJpegMem->pJpegData = NULL;
	}

	return 0;
}

static int tvt_face_releaseJpegData(const unsigned int index)
{
	TVT_SDK_FACEDT_INFO_S* pstCfg = g_pstSdkFaceDtInfo;
	tvt_freeJpegMem(&pstCfg->panoramaJpegMem[index]);

	for (int i = 0; i < MAX_DT_FACE_NUM; i++)
	{
		tvt_freeJpegMem(&pstCfg->faceJpegMem[index][i]);
	}
	return 0;
}

int HUAAN_face_start(SDK_FACE_DT_CFG *pstFaceDtCfg)
{
	//TODO: The Struct SDK_FACE_DT_CFG is not uesd !! only saved the width and height
	//but the YUV width and height is defined as 1920x1080 and can not changed

	// int i = 0;
	int ret = 0;
	TVT_SDK_FACEDT_INFO_S *pstInfo = g_pstSdkFaceDtInfo;
	if ( !pstInfo)
	{
		TIPC_PRINTF_ERR("NULL PTR!!!");
		return -1;
	}

	TIPC_MUTEX_LOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
	                &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);

	if (pstInfo->u8Init)
	{
		if (!pstInfo->u8Start)
		{
			int s32Src_w = 1920;
			int s32Src_h = 1080;

			// TODO
			// getVideoSrcW_H(&s32Src_w, &s32Src_h);
			// if (AiGetSnapJpegSizeMode())
			// {
			// 	if (pstFaceDtCfg->srcWidth < pstFaceDtCfg->srcHeight) {// 走廊模式
			// 	} else {}
			// } else {
			// 	if (pstFaceDtCfg->srcWidth < pstFaceDtCfg->srcHeight) {// 走廊模式
			// 		if ((IPC_PROD_PF_200M_E2128 == gp_prod_type_val ) && customer_cnvp09_proc())  // 云从定制
			// 		}
			// 	else {
			// 		if ((IPC_PROD_PF_200M_E2128 == gp_prod_type_val ) && customer_cnvp09_proc())  // 云从定制
			// 		}
			// }

			pstInfo->yuv_width = s32Src_w;//HUAAN_YUV_WIDTH;
			pstInfo->yuv_height = s32Src_h;//HUAAN_YUV_HEIGHT;

			// From arg
			pstInfo->dt_width = pstFaceDtCfg->srcWidth;
			pstInfo->dt_height = pstFaceDtCfg->srcHeight;

			memset(pstInfo->curFaceNum, 0, sizeof(pstInfo->curFaceNum));

			pstInfo->needZoom = 0;

			TIPC_PRINTF_INFO("yuv_width=%d, yuv_height=%d, dt_width=%d, dt_height=%d", \
			                 pstInfo->yuv_width, pstInfo->yuv_height, pstInfo->dt_width, pstInfo->dt_height);

			if ((pstInfo->dt_width != pstInfo->yuv_width) || (pstInfo->dt_height != pstInfo->yuv_height))
			{
				// 分配zoom帧
				// TODO
				// pstInfo->needZoom = 1;
				TIPC_PRINTF_ERR("The zoom part has not finished");
				return -1;
			}

			// if (AiGetSnapJpegSizeMode())
			// {
			// 	// 不需要启动扩展通道
			// }
			// else
			// {
			ret = chip_video_YuvInit();
			// }

			if (ret < 0)
			{
				TIPC_PRINTF_ERR("Start Yuv failed, ret=%d", ret);
				TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
				                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);
				return -1;
			}
			// }
			pstInfo->getYuvFlag = 0;
			pstInfo->u8Start = 1;
		}
		else
		{
			TIPC_PRINTF_ERR("face started !!!");
			TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
			                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);
			return -1;
		}
	}
	else
	{
		TIPC_PRINTF_ERR("not init!!!");
		TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
		                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);
		return -1;
	}

	TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
	                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);

	return 0;
}

int HUAAN_face_stop()
{
	int i = 0;
	TVT_SDK_FACEDT_INFO_S * pstCfg = g_pstSdkFaceDtInfo;
	if ( !pstCfg )
	{
		TIPC_PRINTF_ERR("NULL PTR!!!");
		return -1;
	}

	TIPC_MUTEX_LOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
	                &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);

	if (1 == pstCfg->u8Init)
	{
		if (1 == pstCfg->u8Start)
		{
			for (i = 0; i < tvt_face_get_framebuf_number(); i++)
			{
				tvt_face_releaseYuvData(&pstCfg->astVideoFrame[i]);
				tvt_face_releaseJpegData(i);
				memset(&pstCfg->TvtDtImgFrame[i], 0 , sizeof(tvt_Img_Frame));
				pstCfg->allocFlag[i] = 0;
				pstCfg->curFaceNum[i] = 0;
			}

			// if (pstCfg->needZoom)
			// {
			// 	//TODO
			// }

			pstCfg->u8Start = 0;
			chip_video_YuvExit();
		}
		else
		{
			TIPC_PRINTF_ERR("face stoped !!!");
			TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
			                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);
			return -1;
		}
	}
	else
	{
		TIPC_PRINTF_ERR("not init!!!");
		TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
		                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);
		return -1;
	}

	TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
	                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);

	return 0;
}

int HUAAN_face_getDataEx(SDK_SRC_IMAGE_DATA * pstSrcImgData)
{
	int ret = 0;
	int index_mem = 0;
	TVT_SDK_FACEDT_INFO_S * pstCfg = g_pstSdkFaceDtInfo;
	tvt_Img_Frame TempImgFrame = {0};
	if ( !pstCfg )
	{
		TIPC_PRINTF_ERR("NULL PTR!!!");
		return -1;
	}

	if (NULL == pstSrcImgData)
	{
		TIPC_PRINTF_ERR("NULL ptr !!!");
		return -1;
	}

	TIPC_MUTEX_LOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
	                &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);

	if (0 == pstCfg->u8Init)
	{
		TIPC_PRINTF_ERR("not init!!!");
		TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
		                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);
		return -1;
	}

	if (0 == pstCfg->u8Start)
	{
		TIPC_PRINTF_ERR("not started!!!");
		TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
		                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);
		return -1;
	}

	//get Mem Index
	index_mem = tvt_getFreeResultMemIndex();
	if ((0 > index_mem) || (tvt_face_get_framebuf_number() <= index_mem))
	{
		TIPC_PRINTF_ERR("Result memory alloc error!!!, index=%d", index_mem);
		tvt_face_releaseYuvData(&pstCfg->stCurVideoFrame);
		TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
		                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);
		return -1;
	}

	// Get yuv Capture
	ret =  chip_video_YuvGetCapture(index_mem, &pstCfg->stCurVideoFrame);
	if (ret) {
		TIPC_PRINTF_ERR("Error, Get yuv Capture Failed");
		TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
		                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);
		return -1;
	}

	if (pstCfg->needZoom)
	{
		//TODO
	} else {
		trans_frame_format(&pstCfg->stCurVideoFrame, &TempImgFrame);
	}

	pstCfg->getYuvFlag = 1;
	pstCfg->indexMem = index_mem;
	pstCfg->curFaceNum[index_mem] = 0;

// 保存yuv frame 结构体
	memcpy(&pstCfg->astVideoFrame[index_mem], &pstCfg->stCurVideoFrame, sizeof(CHIP_YUV_INFO_S));
	memcpy(&pstCfg->TvtDtImgFrame[index_mem], &TempImgFrame, sizeof(tvt_Img_Frame));

	pstSrcImgData->iMemIndex = index_mem;
	pstSrcImgData->format = 0x1; //IMAGE_FORMAT_RGB
	pstSrcImgData->width = pstCfg->dt_width;
	pstSrcImgData->height = pstCfg->dt_height;
	pstSrcImgData->data = ( char * )&pstCfg->TvtDtImgFrame[index_mem];

	TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
	                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);

	return 0;
}

int HUAAN_face_releaseResultMem(int index)
{
	TVT_SDK_FACEDT_INFO_S * pstCfg = g_pstSdkFaceDtInfo;
	TIPC_MUTEX_LOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
	                &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);

	tvt_face_releaseYuvData(&pstCfg->astVideoFrame[index]);
	tvt_face_releaseJpegData(index);
	memset(&pstCfg->TvtDtImgFrame[index], 0 , sizeof(tvt_Img_Frame));
	pstCfg->allocFlag[index] = 0;
	pstCfg->curFaceNum[index] = 0;

	TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
	                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);

	return 0;
}

static int tvt_GetVehicePosOfDtInfo(int yuv_width, int yuv_height, int dt_width, int dt_height, SDK_CROP_IMAGE_DATA * pstCropImgData, SDK_POSITION * pstPos)
{
	int zoom_w;
	int zoom_H;

	int max_x, max_y;
	int min_x, min_y;

	zoom_w = yuv_width * 100 / dt_width;
	zoom_H = yuv_height * 100 / dt_height;

	// x 位置起点  和 x 位置的终点
	max_x = pstCropImgData->ptRightBottom.x * zoom_w / 100;
	min_x = pstCropImgData->ptLeftTop.x * zoom_w / 100;

	// y 位置起点  和 y 位置的终点
	max_y = pstCropImgData->ptRightBottom.y * zoom_H / 100;
	min_y = pstCropImgData->ptLeftTop.y * zoom_H / 100;

	pstPos->width = max_x - min_x;
	pstPos->height = max_y - min_y;

	// 车牌抓原始图像
	pstPos->x = min_x;
	pstPos->y = min_y;

	if (pstPos->x < 0)
	{
		pstPos->x = 0;
	}
	else if ( pstPos->x > yuv_width )
	{
		pstPos->x = yuv_width;
	}

	if ((pstPos->x & 0xF) > 0x8)
	{
		pstPos->x += 0x8;
	}

	if ((pstPos->y ) < 0)
	{
		pstPos->y = 0;
	}
	else if ( pstPos->y > yuv_height )
	{
		pstPos->y = yuv_height;
	}

	if ( pstPos->width > yuv_width )
	{
		pstPos->width = yuv_width;
	}
	else if ( pstPos->width < 16 )
	{
		pstPos->width = 16;
	}

	if ( pstPos->height > yuv_height )
	{
		pstPos->height = yuv_height;
	}
	else if ( pstPos->height < 4 )
	{
		pstPos->height = 4;
	}

	pstPos->width = (pstPos->width + 15) & 0xFFF0;
	pstPos->height = (pstPos->height + 3) & 0xFFFC;


	if ((pstPos->x + pstPos->width) > yuv_width)
	{
		pstPos->x = (yuv_width - pstPos->width);
	}

	if ((pstPos->y + pstPos->height) > yuv_height)
	{
		pstPos->y = (yuv_height - pstPos->height);
	}

	// 16对齐
	pstPos->x = (pstPos->x) & 0xfffffff0;
	// 4对齐
	pstPos->y = (pstPos->y) & 0xfffffffC;

	return 0;
}

static int tvt_GetPosOfDtInfo(int yuv_width, int yuv_height, int dt_width, int dt_height, SDK_CROP_IMAGE_DATA * pstCropImgData, SDK_POSITION * pstPos)
{
	int zoom_w;
	int zoom_H;

	int max_x; //, max_y;
	int min_x, min_y;

	zoom_w = yuv_width * 100 / dt_width;
	zoom_H = yuv_height * 100 / dt_height;

	max_x = pstCropImgData->ptRightBottom.x * zoom_w / 100;
	min_x = pstCropImgData->ptLeftTop.x * zoom_w / 100;
	min_y = pstCropImgData->ptLeftTop.y * zoom_H / 100;

	//TIPC_PRINTF_INFO("min_x=%d, min_y=%d, max_x=%d",  min_x, min_y, max_x);

	pstPos->width = max_x - min_x;

	pstPos->x = min_x - pstPos->width / 4;
	pstPos->y = min_y - pstPos->width / 4;
	pstPos->width += pstPos->width / 2;

	if ((pstPos->x ) < 0)
	{
		pstPos->x = 0;
	}

	if ((pstPos->y ) < 0)
	{
		pstPos->y = 0;
	}

	if (pstPos->width > HUAAN_DT_MIN(yuv_height, yuv_width))
	{
		pstPos->width = HUAAN_DT_MIN(yuv_height, yuv_width);
	}

	pstPos->width = (pstPos->width) & 0xFFFC;
	pstPos->height = pstPos->width;

	if ((pstPos->x & 0xF) > 0x8)
	{
		pstPos->x += 0x8;
	}

	if ((pstPos->x + pstPos->width) > yuv_width)
	{
		pstPos->x = (yuv_width - pstPos->width);
	}

	if ((pstPos->y + pstPos->height) > yuv_height)
	{
		pstPos->y = (yuv_height - pstPos->height);
	}

	pstPos->x =  ((pstPos->x) & 0xFFF0);
	pstPos->y =  ((pstPos->y) & 0xFFFE);

	TIPC_PRINTF_CHK("x=%d, y=%d, w=%d, h=%d", pstPos->x, pstPos->y, pstPos->width, pstPos->height);

	return 0;
}

int HUAAN_face_cropImg(SDK_CROP_IMAGE_DATA * pstCropImgData)
{
	int ret = 0;
	TVT_SDK_FACEDT_INFO_S * pstCfg = g_pstSdkFaceDtInfo;
	CHIP_YUV_INFO_S* pstCurVideoFrame = NULL;
	int indexMem = -1;
	if ( !pstCfg )
	{
		TIPC_PRINTF_ERR("NULL PTR!!!");
		return -1;
	}
	if (NULL == pstCropImgData)
	{
		TIPC_PRINTF_ERR("NULL PTR !!!");
		TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
		                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);

		return -1;
	}

	if (0 == pstCfg->u8Init)
	{
		TIPC_PRINTF_ERR("not init!!!");
		TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
		                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);

		return -1;
	}


	if (0 == pstCfg->u8Start)
	{
		TIPC_PRINTF_ERR("not start!!!");
		TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
		                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);

		return -1;
	}

	TIPC_MUTEX_LOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
	                &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);

	indexMem = pstCropImgData->iMemIndex;

	if ((0 > indexMem) || (tvt_face_get_framebuf_number() <= indexMem))
	{
		TIPC_PRINTF_ERR("not get result memory, indexMem=%d !!!", indexMem);
		TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
		                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);
		return -1;
	}

	pstCurVideoFrame = &pstCfg->astVideoFrame[indexMem];

	if (0 == pstCropImgData->faceId)
	{

		ret = chip_video_YuvEncJpeg( pstCurVideoFrame->index, NULL, NULL, \
		                             &pstCfg->panoramaJpegMem[indexMem], 1);
		if (0 != ret)
		{
			TIPC_PRINTF_ERR("TVT_ai_jpeg_chn_start_enc failed, ret=%d", ret);
			TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
			                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);
			return -1;
		}

		pstCropImgData->iJpegWidth = pstCfg->panoramaJpegMem[indexMem].iJpegWidth;
		pstCropImgData->iJpegHeight = pstCfg->panoramaJpegMem[indexMem].iJpegHeight;
		pstCropImgData->iJpegLen = pstCfg->panoramaJpegMem[indexMem].iJpegLen;
		pstCropImgData->pJpegData = pstCfg->panoramaJpegMem[indexMem].pJpegData;
		pstCropImgData->llRelativeTime = pstCurVideoFrame->pts;

		// 原图yuv
		pstCropImgData->iRgbWidth = pstCfg->dt_width;//pstCfg->yuv_width;
		pstCropImgData->iRgbHeight = pstCfg->dt_height;//pstCfg->yuv_height;
		pstCropImgData->iRgbLen = ((pstCfg->dt_width + 15) & 0xFFF0) * pstCfg->dt_height * 3 / 2; //((pstCfg->yuv_width+15)&0xFFF0)*pstCfg->yuv_height*3/2;
		pstCropImgData->pRgbData = (char *)&pstCfg->TvtDtImgFrame[indexMem];//(char*)&pstCfg->stTvtSrcImgFrame;

	}
	else if (0 < pstCropImgData->faceId)
	{
		SDK_POSITION pos = {0};
		SDK_POSITION stDestEncPos = {0};
		int iFaceNum = pstCfg->curFaceNum[indexMem];
		int cap_jpeg_type = 0;

		if (iFaceNum >=  MAX_DT_FACE_NUM)
		{
			TIPC_PRINTF_ERR("Error! curFaceNum=%d", iFaceNum);
			TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
			                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);
			return -1;
		}

		TIPC_PRINTF_CHK("faceId=%x", pstCropImgData->faceId);

		if ( 0x5a5a5a5a == pstCropImgData->iRgbLen )
		{
			// 车牌处理
			cap_jpeg_type = 1;
			pstCropImgData->iRgbLen = 0;
		}

		if ( 0 == cap_jpeg_type )
		{
			tvt_GetPosOfDtInfo(pstCfg->yuv_width, pstCfg->yuv_height, pstCfg->dt_width, pstCfg->dt_height, pstCropImgData, &pos);
		}
		else
		{
			tvt_GetVehicePosOfDtInfo(pstCfg->yuv_width, pstCfg->yuv_height, pstCfg->dt_width, pstCfg->dt_height, pstCropImgData, &pos);
		}

		if ( 0 == cap_jpeg_type )
		{
			memcpy( &stDestEncPos, &pos, sizeof(SDK_POSITION) );
		}
		else
		{
			// 车牌的数据位置处理
			stDestEncPos.x = 0;
			stDestEncPos.y = 0;
			stDestEncPos.width = pos.width;
			stDestEncPos.height = pos.height;

			if ( pos.width > pstCropImgData->iJpegWidth )
			{
				stDestEncPos.width = pstCropImgData->iJpegWidth;
			}

			if ( pos.height > pstCropImgData->iJpegHeight )
			{
				stDestEncPos.height = pstCropImgData->iJpegHeight;
			}

		}

		ret = chip_video_YuvEncJpeg( pstCurVideoFrame->index, &pos, &stDestEncPos, \
		                             &pstCfg->faceJpegMem[indexMem][iFaceNum], 0);
		if (0 != ret)
		{
			TIPC_PRINTF_ERR("TVT_ai_jpeg_chn_start_enc failed, ret=%d", ret);
			TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
			                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);
			return -1;
		}

		pstCropImgData->ptLeftTop.x = pos.x;
		pstCropImgData->ptLeftTop.y = pos.y;
		pstCropImgData->ptRightBottom.x = pos.x + pos.width;
		pstCropImgData->ptRightBottom.y = pos.y + pos.height;

		pstCropImgData->iJpegWidth = pstCfg->faceJpegMem[indexMem][iFaceNum].iJpegWidth;
		pstCropImgData->iJpegHeight = pstCfg->faceJpegMem[indexMem][iFaceNum].iJpegHeight;
		pstCropImgData->iJpegLen = pstCfg->faceJpegMem[indexMem][iFaceNum].iJpegLen;
		pstCropImgData->pJpegData = pstCfg->faceJpegMem[indexMem][iFaceNum].pJpegData;

		TIPC_PRINTF_CHK("x=%d, y=%d, w=%d, h=%d--%d,%d--%d,%d,%d",
		                pos.x, pos.y, pos.width, pos.height,
		                stDestEncPos.width, stDestEncPos.height,
		                pstCropImgData->iJpegLen,
		                pstCropImgData->iJpegWidth, pstCropImgData->iJpegHeight );

		pstCropImgData->llRelativeTime = pstCurVideoFrame->pts;

		pstCfg->curFaceNum[indexMem]++;
	}
	else
	{
		TIPC_PRINTF_ERR("invalid faceId: %d", pstCropImgData->faceId);
		TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
		                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);
		return -1;
	}

	TIPC_MUTEX_UNLOCK(&g_pstSdkFaceDtInfo->s32Mutex, g_pstSdkFaceDtInfo->szMutexName,
	                  &g_pstSdkFaceDtInfo->u32MutexLockCnt, 0);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////		 InterFace		 //////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

int tvt_sdk_facedt_init(int s32VinNum, const int* s32VinDevID) {
	int s32Ret = -1;
	// int i = 0;
	if (g_pstSdkFaceDtInfo != NULL)
	{
		if (g_pstSdkFaceDtInfo->u8Init != 0)
		{
			TIPC_PRINTF_ERR("video face detection service is running");
			return 0;
		}
	}
	else
	{
		// malloc
		g_pstSdkFaceDtInfo = (TVT_SDK_FACEDT_INFO_S* )malloc(sizeof(TVT_SDK_FACEDT_INFO_S));
		if (NULL == g_pstSdkFaceDtInfo)
		{
			TIPC_PRINTF_ERR("video roi service malloc failed");
			return s32Ret;
		}
		// memset
		memset(g_pstSdkFaceDtInfo, 0, sizeof(TVT_SDK_FACEDT_INFO_S));

	}
	// Init the Parameter
	g_pstSdkFaceDtInfo->u8Start = 0;

	// init the mutex
	pthread_mutex_init(&g_pstSdkFaceDtInfo->s32Mutex, NULL);
	sprintf(g_pstSdkFaceDtInfo->szMutexName, "tvt_sdk_facedt_lock");
	g_pstSdkFaceDtInfo->u8Init = 1;

	return 0;
}

int tvt_sdk_facedt_exit(void) {
	if (NULL == g_pstSdkFaceDtInfo)
	{
		TIPC_PRINTF_ERR("video face detection service is not running");
		return -1;
	}

	if (g_pstSdkFaceDtInfo->u8Init != 1)
	{
		TIPC_PRINTF_ERR("video face detection service is not running");
		free((void*)g_pstSdkFaceDtInfo);
		g_pstSdkFaceDtInfo = NULL;
		return -1;
	}
	else
	{
		pthread_mutex_destroy(&g_pstSdkFaceDtInfo->s32Mutex);
		g_pstSdkFaceDtInfo->u8Init = 0;
		free((void*)g_pstSdkFaceDtInfo);
		g_pstSdkFaceDtInfo = NULL;
	}
	return 0;
}

int tvt_sdk_video_facedt_Ctrl (SDK_FACE_DT_CTRL_E eFaceDtTypePara, void *pPara, unsigned int len)
{
	int ret = -1;

	// global Flag, TODO
	// if ( !gp_mod_var->b_func_enable_face_detect )
	// {
	// 	return -1;
	// }

	if ((NULL == pPara) && (len > 0))
	{
		TIPC_PRINTF_ERR("SDKFaceDtControl, cmd %d, data is NULL, iLenght=%d\n", (int)eFaceDtTypePara, len);
		return ret;
	}

	switch (eFaceDtTypePara)
	{
	// TODO
	case SDK_FACE_DT_CTRL_START:
		break;

	case SDK_FACE_DT_CTRL_STOP:
		break;

	case SDK_FACE_DT_CTRL_SET_PARA:
		break;

	case SDK_FACE_DT_CTRL_GET_DATA:
		break;

	case SDK_FACE_DT_CTRL_RELEASE_DATA:
		break;

	case SDK_FACE_DT_SET_FACE_ADDR:
		break;


	case SDK_FACE_DT_CTRL_START_HUAAN:
	{
		SDK_FACE_DT_CFG *pstFaceDtCfg = (SDK_FACE_DT_CFG *)pPara;
		ret = HUAAN_face_start(pstFaceDtCfg);
	}
	break;

	case SDK_FACE_DT_CTRL_STOP_HUAAN:
	{
		ret = HUAAN_face_stop();
	}
	break;

	case SDK_FACE_DT_CTRL_SET_PARA_HUAAN:
		break;

	case SDK_FACE_DT_CTRL_GET_DATA_HUAAN:
	{
		SDK_SRC_IMAGE_DATA * pstSrcImgData = (SDK_SRC_IMAGE_DATA *)pPara;
		ret = HUAAN_face_getDataEx(pstSrcImgData);
	}
	break;

	case SDK_FACE_DT_CTRL_CROP_IMG_HUAAN:
	{
		SDK_CROP_IMAGE_DATA * pstCropImgData = (SDK_CROP_IMAGE_DATA * )pPara;
		// tvt_set_ui_cfg_osd_flag(0);  		No Osd
		ret = HUAAN_face_cropImg(pstCropImgData);
	}
	break;

	// case SDK_FACE_DT_CTRL_CROP_IMG_HUAAN_OSD:
	// 	break;


	case SDK_FACE_DT_CTRL_RELEASE_DATA_HUAAN:
	{

		int * pInedx = (int *)pPara;
		if (NULL == pInedx)
		{
			//ret = HUAAN_face_releaseYuvData();
			ret = 0; //这里不释放yuv 数据,人脸比对完才释放
		}
		else
		{
			//TIPC_PRINTF_INFO(" === >>>>free index = %d", * pInedx);
			ret = HUAAN_face_releaseResultMem( *pInedx);
		}
	}
	break;

	case SDK_FACE_DYUV_DATA_START:

		break;

	case SDK_FACE_DYUV_DATA_STOP:
		break;

	case SDK_FACE_DYUV_DATA_GET:
		break;

	case SDK_FACE_DYUV_DATA_RELEASE:
		break;

	case SDK_FACE_DYUV_DATA_CROP_IMG:
		break;

	// case SDK_FACE_DT_SET_IR_FACE_ADDR:	// Jesse: set face rect coordinates in IR image
	// 	break;

	// case SDK_FACE_DT_GET_IR_FACE_EXP_STATUS:  // Jesse: get status for IR face rect based exposure
	// 	break;

	default:
	{
		TIPC_PRINTF_INFO("invalid command define %d", eFaceDtTypePara);
	}
	break;
	}

	return ret;
}
