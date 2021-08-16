#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>

#include "tvt_sdk_print.h"

#include "amba_yuv.h"
#include "amba_venc.h"

#ifndef AM_IOCTL
#define AM_IOCTL(_filp, _cmd, _arg) \
        do {                        \
            if (ioctl(_filp, _cmd, _arg) < 0) { \
                perror(#_cmd);      \
                TIPC_PRINTF_ERR("%s err [%d]", #_cmd, errno); \
                return -1;      \
            }                       \
        } while (0)
#endif

#define YUV_CANVAS_ID 7
#define YUV_PASS_ID  0
#define YUV_BUFF_ID  IAV_SRCBUF_5
#define YUV_STREAM_ID 5
#define YUV_MAX_CACHED_BUFF_NUM 8

typedef struct amba_yuv_canvas_buff_s
{
	uint8_t							get_data_flag;
	int								pts;
	struct iav_querydesc 			query_desc;
} AMBA_YUV_CANVAS_BUFF_S;

typedef struct amba_yuv_cfg_s
{
	int             				s32Fd;                               ///< 设备文件描述符
	int             				s32VinNum;                           ///< Sensor个数

	uint8_t 	    				u8Init;                              ///< 初始化标志

	uint64_t						Canvas_Pool_PhyAddr;
	uint8_t*						Canvas_Pool_VirAddr;
	uint32_t						Canvas_Pool_Size;
	uint8_t							Canvas_Pool_InitFlag;

	AMBA_YUV_CANVAS_BUFF_S  		Canvas_Buff[YUV_MAX_CACHED_BUFF_NUM];

	struct iav_efm_get_pool_info 	efm_pool_info;

	// uint32_t	 	 YuvDMADataSize;         	          ///< Amba dma 内存区大小
	// uint8_t	* 		 pu8YuvDMAAddr;            	          ///< Amba dma 内存区首地址
	// int 			 YuvDMAfd;							  ///< Amba dma File Discriptor, this memory is uesd fo copy

	uint8_t							 u8DspMapInit;
	uint8_t	*   					 DspAddr;						  ///< Amba Dsp Partition Address
	uint32_t  						 DspSize;						  ///< Amba Dsp Partition Size
	uint64_t		 				 DspPhyAddr;

} AMBA_YUV_CFG_S;

AMBA_YUV_CFG_S g_stYuvCfg = {0};

// this interface is only for debug
static int show_efm_stream_status() {

	struct iav_queryinfo stIavQueryInfo = {0};
	struct iav_stream_info* pstStreamInfo = NULL;
	stIavQueryInfo.qid = IAV_INFO_STREAM;
	pstStreamInfo = &stIavQueryInfo.arg.stream;
	pstStreamInfo->id = YUV_STREAM_ID;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_QUERY_INFO, &stIavQueryInfo);
	TIPC_PRINTF_CHK("EFM Stream, stream[%d] status[%d]\n", YUV_STREAM_ID, pstStreamInfo->state);
	return 0;
}


static int wait_idle() {
	struct iav_queryinfo stIavQueryInfo = {0};
	struct iav_stream_info* pstStreamInfo = NULL;
	while (1)
	{
		memset (&stIavQueryInfo, 0, sizeof(struct iav_queryinfo));
		stIavQueryInfo.qid = IAV_INFO_STREAM;
		pstStreamInfo = &stIavQueryInfo.arg.stream;
		pstStreamInfo->id = YUV_STREAM_ID;
		AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_QUERY_INFO, &stIavQueryInfo);
		if (pstStreamInfo->state == IAV_STREAM_STATE_IDLE) {
			break;
		}
		usleep (1000);
	}
	return 0;
}


static int start_efm_stream(const unsigned int enc_x, const unsigned int enc_y, \
                            const unsigned int enc_width, const unsigned int enc_height)
{
	// Get Stream iav cfg
	struct iav_stream_cfg stIavStreamCfg = {0};
	struct iav_stream_format* pstIavStreamFormat = NULL;
	stIavStreamCfg.id = YUV_STREAM_ID;	 // Stream id 5
	stIavStreamCfg.cid = IAV_STMCFG_FORMAT;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_GET_STREAM_CONFIG, &stIavStreamCfg);

	TIPC_PRINTF_CHK("(%u,%u) %ux%u", enc_x, enc_y, enc_width, enc_height);

	// Set encode format
	pstIavStreamFormat = &stIavStreamCfg.arg.format;
	pstIavStreamFormat->type = IAV_STREAM_TYPE_MJPEG; // For jpeg
	pstIavStreamFormat->enc_win.x = enc_x;
	pstIavStreamFormat->enc_win.y = enc_y;
	pstIavStreamFormat->enc_win.width = enc_width;
	pstIavStreamFormat->enc_win.height = enc_height;
	pstIavStreamFormat->enc_src_id = 16;  // 16 for efm
	// pstIavStreamFormat->duration = 1;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_SET_STREAM_CONFIG, &stIavStreamCfg);

	//Set Mjpeg Cfg
	struct iav_mjpeg_cfg stMjpegCfg = {0};
	// struct iav_queryinfo stQueryInfo = {0};
	struct iav_stream_info* pstStreamInfo = NULL;

	stMjpegCfg.id = YUV_STREAM_ID;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_GET_MJPEG_CONFIG, &stMjpegCfg);
	// Jpeg quality: defined in amba_video.c
	stMjpegCfg.quality = 90;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_SET_MJPEG_CONFIG, &stMjpegCfg);


	// check stream State
	struct iav_queryinfo stIavQueryInfo;
	pstStreamInfo = NULL;
	memset(&stIavQueryInfo, 0, sizeof(stIavQueryInfo));
	stIavQueryInfo.qid = IAV_INFO_STREAM;
	pstStreamInfo = &stIavQueryInfo.arg.stream;
	pstStreamInfo->id = stIavStreamCfg.id;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_QUERY_INFO, &stIavQueryInfo);
	TIPC_PRINTF_CHK("Before Start EFM Stream, stream[%d] status[%d]\n", YUV_STREAM_ID, pstStreamInfo->state);

	// start encoding
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_START_ENCODE, (1 << stIavStreamCfg.id));

	return 0;
}

static int stop_efm_stream(void)
{
	struct iav_queryinfo query_info;
	struct iav_stream_info *stream_info;

	memset(&query_info, 0, sizeof(query_info));
	query_info.qid = IAV_INFO_STREAM;
	stream_info = &query_info.arg.stream;
	stream_info->id = YUV_STREAM_ID;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_QUERY_INFO, &query_info);

	if (stream_info->state != IAV_STREAM_STATE_ENCODING && stream_info->state != IAV_STREAM_STATE_STARTING ) {
		TIPC_PRINTF_CHK("Stream[%d] current status[%d]", YUV_STREAM_ID, stream_info->state);
	} else {
		TIPC_PRINTF_CHK("Stop The Efm Stream[%d]!", YUV_STREAM_ID);
		AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_STOP_ENCODE, (1 << YUV_STREAM_ID));
	}

	memset(&query_info, 0, sizeof(query_info));
	query_info.qid = IAV_INFO_STREAM;
	stream_info = &query_info.arg.stream;
	stream_info->id = YUV_STREAM_ID;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_QUERY_INFO, &query_info);

	if (stream_info->state != IAV_STREAM_STATE_ENCODING && stream_info->state != IAV_STREAM_STATE_STOPPING)
	{
		TIPC_PRINTF_CHK("Stream[%d] current status[%d]", YUV_STREAM_ID, stream_info->state);
	}
	else
	{
		AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_ABORT_ENCODE, (1 << YUV_STREAM_ID));
	}
	// show_efm_stream_status();
	return 0;
}

static int MapCanvasPoolAddr(void)
{
	int dsp_state;
	uint8_t * canvas_pool_addr  = NULL;
	struct iav_querymem query_mem;
	struct iav_mem_part_info *part_info;

	// check state
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_GET_IAV_STATE, &dsp_state);
	if ((dsp_state != IAV_STATE_PREVIEW) && (dsp_state != IAV_STATE_ENCODING)) {
		TIPC_PRINTF_ERR("IAV is not in preview / encoding state, cannot get yuv buf!");
		return -1;
	}

	if (!g_stYuvCfg.s32Fd) {
		TIPC_PRINTF_ERR("Please init the fd");
		return -1;
	}

	//query CanvasPool buffer start address
	memset(&query_mem, 0, sizeof(query_mem));
	query_mem.mid = IAV_MEM_PARTITION;
	part_info = &query_mem.arg.partition;
	part_info->pid = IAV_PART_CANVAS_POOL;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_QUERY_MEMBLOCK, &query_mem);

	// mmap
	canvas_pool_addr = (uint8_t *)mmap(NULL, part_info->mem.length , PROT_READ, MAP_SHARED,
	                                   g_stYuvCfg.s32Fd,  part_info->mem.addr);

	if (canvas_pool_addr == MAP_FAILED || canvas_pool_addr == NULL) {
		TIPC_PRINTF_ERR ("CanvasPool buffer start Address Mmap Error!!");
		return -1;
	}

	g_stYuvCfg.Canvas_Pool_PhyAddr = part_info->mem.addr;
	g_stYuvCfg.Canvas_Pool_Size = part_info->mem.length;
	g_stYuvCfg.Canvas_Pool_VirAddr = canvas_pool_addr;
	g_stYuvCfg.Canvas_Pool_InitFlag = 1;
	return 0;
}

static int amba_yuv_FeedCanvas(void) {
	struct iav_feed_canvas feed_canvas;
	feed_canvas.canvas_id = YUV_CANVAS_ID;
	AM_IOCTL(g_stYuvCfg.s32Fd , IAV_IOC_FEED_CANVAS_BUF, &feed_canvas);
	return 0;
}

static int get_efm_pool_info(u32 StreamId)
{
	struct iav_efm_get_pool_info *pool_info = NULL;

	pool_info = &g_stYuvCfg.efm_pool_info;
	memset(pool_info, 0, sizeof(struct iav_efm_get_pool_info));
	pool_info->stream_id = StreamId;

	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_EFM_GET_POOL_INFO, pool_info);

	return 0;
}


static int fill_yuv_from_capture(const uint32_t index, struct iav_efm_request_frame* yuv_request)
{

	if (!g_stYuvCfg.Canvas_Buff[index].get_data_flag ) {
		TIPC_PRINTF_ERR (" The Canvas is empty !");
		return -1;
	}
	int i = 0;
	uint8_t* src = NULL;
	uint8_t* dest = NULL;
	struct iav_yuv_cap* yuv_src;

	yuv_src = &g_stYuvCfg.Canvas_Buff[index].query_desc.arg.canvas.yuv;

	src = g_stYuvCfg.Canvas_Pool_VirAddr + yuv_src->y_addr_offset;
	dest = g_stYuvCfg.DspAddr + yuv_request->yuv_luma_offset;
	for (i = 0; i < yuv_src->height; i++) {
		memcpy(dest + i * g_stYuvCfg.efm_pool_info.yuv_pitch,
		       src + i * yuv_src->pitch, yuv_src->width);
	}

	src = g_stYuvCfg.Canvas_Pool_VirAddr + yuv_src->uv_addr_offset;
	dest = g_stYuvCfg.DspAddr + yuv_request->yuv_chroma_offset;

	for (i = 0; i < (yuv_src->height >> 1); i++) {
		memcpy(dest + i * g_stYuvCfg.efm_pool_info.yuv_pitch,
		       src + i * yuv_src->pitch, yuv_src->width);

	}
	return 0;
}

static int amba_yuv_MapDspMem(void) {

	// check state
	int dsp_state;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_GET_IAV_STATE, &dsp_state);

	if ((dsp_state != IAV_STATE_PREVIEW) && (dsp_state != IAV_STATE_ENCODING)) {
		TIPC_PRINTF_ERR("IAV is not in preview / encoding state, cannot get yuv buf!\n");
		return -1;
	}

	//map dsp buffer
	struct iav_querymem query_mem;
	struct iav_mem_part_info *part_info;

	memset(&query_mem, 0, sizeof(query_mem));
	query_mem.mid = IAV_MEM_PARTITION;
	part_info = &query_mem.arg.partition;
	part_info->pid = IAV_PART_DSP;

	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_QUERY_MEMBLOCK, &query_mem);

	// mmap
	uint8_t * dsp_data_mem  = NULL;

	dsp_data_mem = (uint8_t *)mmap(NULL, part_info->mem.length , PROT_READ | PROT_WRITE, MAP_SHARED,
	                               g_stYuvCfg.s32Fd,  part_info->mem.addr);

	if (dsp_data_mem == MAP_FAILED || dsp_data_mem == NULL) {
		TIPC_PRINTF_ERR ("Dsp Address Mmap Error!!");
		return -1;
	}
	g_stYuvCfg.DspSize = part_info->mem.length;
	g_stYuvCfg.DspAddr =  dsp_data_mem;
	g_stYuvCfg.DspPhyAddr = part_info->mem.addr;
	g_stYuvCfg.u8DspMapInit = 1;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////		 InterFace		 //////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

int amba_yuv_GetFrame(const unsigned int index, AMBA_YUV_FRAMEINFO_S* pstYUVFrameInfo)
{
	if (!g_stYuvCfg.Canvas_Pool_InitFlag || !g_stYuvCfg.u8Init) {
		TIPC_PRINTF_ERR ("The Yuv Service is not init");
		return -1;
	}

	if (index >= YUV_MAX_CACHED_BUFF_NUM) {
		TIPC_PRINTF_ERR ("The index is bigger than max");
		return -1;
	}

	if (g_stYuvCfg.Canvas_Buff[index].get_data_flag) {
		TIPC_PRINTF_ERR ("The Canvas has a frame supposed to be released first, index:[%d]", index);
		return -1;
	}

	//Feed canvas one frame
	amba_yuv_FeedCanvas();

	//Query canvas buffer
	struct iav_canvasdesc* canvas;

	memset(&g_stYuvCfg.Canvas_Buff[index].query_desc, 0, sizeof(struct iav_querydesc));
	g_stYuvCfg.Canvas_Buff[index].query_desc.qid = IAV_DESC_CANVAS;

	canvas = &g_stYuvCfg.Canvas_Buff[index].query_desc.arg.canvas;
	canvas->canvas_id = YUV_CANVAS_ID;
	canvas->yuv_use_dma_buf_fd = 0 ;  //no copy
	canvas->query_extra_raw = 0;      //no extra raw data
	canvas->skip_cache_sync = 1;

	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_QUERY_DESC, &g_stYuvCfg.Canvas_Buff[index].query_desc);

	//Get Yuv Capture Info
	struct iav_yuv_cap* yuv_cap = &canvas->yuv;

	if (yuv_cap->format != IAV_YUV_FORMAT_YUV420) {
		TIPC_PRINTF_ERR ("yuv_cap->format != IAV_YUV_FORMAT_YUV420");
		return -1;
	}

	TIPC_PRINTF_INFO("Get Frame Index[%d] y_offset: %u, uv_addr_offset:%u", index, yuv_cap->y_addr_offset, yuv_cap->uv_addr_offset);
	TIPC_PRINTF_INFO("Get Frame Index[%d] width: %u, pitch:%u", index, yuv_cap->width, yuv_cap->pitch);

	// Set frame info
	AMBA_YUV_FRAMEINFO_S* pstYUVFrameInfo_Tmp = (AMBA_YUV_FRAMEINFO_S*) malloc (sizeof (AMBA_YUV_FRAMEINFO_S));
	memset (pstYUVFrameInfo_Tmp, 0, sizeof(AMBA_YUV_FRAMEINFO_S));

	pstYUVFrameInfo_Tmp->pitch = yuv_cap->pitch;
	pstYUVFrameInfo_Tmp->width = yuv_cap->width;
	pstYUVFrameInfo_Tmp->height = yuv_cap->height;
	pstYUVFrameInfo_Tmp->pts = yuv_cap->dsp_pts;

	pstYUVFrameInfo_Tmp->y_phy_addr = g_stYuvCfg.Canvas_Pool_PhyAddr + yuv_cap->y_addr_offset;
	pstYUVFrameInfo_Tmp->uv_phy_addr = g_stYuvCfg.Canvas_Pool_PhyAddr + yuv_cap->uv_addr_offset;

	pstYUVFrameInfo_Tmp->y_vir_addr = g_stYuvCfg.Canvas_Pool_VirAddr + yuv_cap->y_addr_offset;
	pstYUVFrameInfo_Tmp->uv_vir_addr = g_stYuvCfg.Canvas_Pool_VirAddr + yuv_cap->uv_addr_offset;

	memcpy(pstYUVFrameInfo, pstYUVFrameInfo_Tmp, sizeof(AMBA_YUV_FRAMEINFO_S));
	g_stYuvCfg.Canvas_Buff[index].get_data_flag = 1;
	g_stYuvCfg.Canvas_Buff[index].pts = yuv_cap->dsp_pts;
	free (pstYUVFrameInfo_Tmp);

	return 0;
}

int amba_yuv_ReleaseFrame(const unsigned int index)
{
	if (index >= YUV_MAX_CACHED_BUFF_NUM) {
		TIPC_PRINTF_ERR ("The index is bigger than max");
		return -1;
	}
	struct iav_canvasdesc* canvas;
	canvas = &g_stYuvCfg.Canvas_Buff[index].query_desc.arg.canvas;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_RELEASE_CANVAS_BUF, canvas);

	g_stYuvCfg.Canvas_Buff[index].get_data_flag = 0;
	g_stYuvCfg.Canvas_Buff[index].pts = -1;
	memset (&g_stYuvCfg.Canvas_Buff[index].query_desc, 0, sizeof(struct iav_querydesc));
	return 0;
}

int amba_yuv_ReleaseAllFrame(void)
{
	struct iav_querydesc query_desc;
	struct iav_canvasdesc* canvas;

	memset(&query_desc, 0, sizeof(query_desc));
	query_desc.qid = IAV_DESC_CANVAS;
	canvas = &query_desc.arg.canvas;
	canvas->canvas_id = YUV_CANVAS_ID;

	/*release all possible locked canvas buffers */
	query_desc.arg.canvas.feed_seq_num = -1;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_RELEASE_CANVAS_BUF, &query_desc.arg.canvas);

	for (int ind = 0 ; ind < YUV_MAX_CACHED_BUFF_NUM; ind++) {
		g_stYuvCfg.Canvas_Buff[ind].get_data_flag = 0;
		g_stYuvCfg.Canvas_Buff[ind].pts = -1;
		memset (&g_stYuvCfg.Canvas_Buff[ind].query_desc, 0, sizeof(struct iav_querydesc));
	}
	return 0;
}

int amba_yuv_EncToJpeg (const unsigned int index, const unsigned int pos_x, const unsigned int pos_y,
                        const unsigned int enc_width, const unsigned int enc_height, const unsigned int dest_width,
                        const unsigned int dest_height, AMBA_YUV_JPEGINFO_S* jpeg_info)
{
	//Get bsb mem addr:
	char* pbsb_mem = amba_venc_getBsbMem();
	if (pbsb_mem == NULL ) {
		TIPC_PRINTF_ERR ("Null pointer");
		return -1;
	}

	if (jpeg_info == NULL) {
		TIPC_PRINTF_ERR ("Null pointer");
		return -1;
	}

	if (stop_efm_stream()) {
		TIPC_PRINTF_ERR("Stop efm stream Error");
		return -1;
	}

	// show_efm_stream_status();

	wait_idle();
	start_efm_stream(pos_x, pos_y, enc_width, enc_height);

	// get efm pool info
	int ret = 0;
	ret = get_efm_pool_info(YUV_STREAM_ID);
	if (ret) {
		TIPC_PRINTF_ERR ("get_efm_pool_info Error!");
	}

	//feed yuv
	// first: requset to get data offset

	struct iav_efm_request_frame yuv_request;
	memset(&yuv_request, 0, sizeof(struct iav_efm_request_frame));
	yuv_request.stream_id = YUV_STREAM_ID;
	yuv_request.skip_cache_sync = 0;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_EFM_REQUEST_FRAME, &yuv_request);

	ret = fill_yuv_from_capture(index, &yuv_request);

	// TIPC_PRINTF_CHK("Request idx: %d", yuv_request.frame_idx);
	// TIPC_PRINTF_CHK("Request y offset: %u", yuv_request.yuv_luma_offset);
	// TIPC_PRINTF_CHK("Request uv offset: %u", yuv_request.yuv_chroma_offset);

	//handshake
	struct iav_efm_handshake_frame handshake = {0};
	handshake.stream_id = YUV_STREAM_ID;
	handshake.frame_idx = yuv_request.frame_idx;
	handshake.frame_pts = g_stYuvCfg.Canvas_Buff[index].pts;
	handshake.hold_frame = 1;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_EFM_HANDSHAKE_FRAME, &handshake);


	struct iav_querydesc stQueryDesc = {0};
	struct iav_framedesc* pstIavFrameDesc = NULL;
	// Query Desc
	while (1) {
		memset (&stQueryDesc, 0, sizeof (struct iav_querydesc));
		pstIavFrameDesc = &stQueryDesc.arg.frame;
		stQueryDesc.qid = IAV_DESC_FRAME;
		pstIavFrameDesc->id = YUV_STREAM_ID;
		pstIavFrameDesc->time_ms = 1000;
		AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_QUERY_DESC, &stQueryDesc);

		TIPC_PRINTF_CHK("[****** WKL ******] Frame IDX: %d" , pstIavFrameDesc->frame_num);

		// 宽高
		TIPC_PRINTF_CHK("[****** WKL ******] Frame size: %d" , pstIavFrameDesc->size);
		TIPC_PRINTF_CHK("[****** WKL ******] Frame Width: %d" , pstIavFrameDesc->reso.width);
		TIPC_PRINTF_CHK("[****** WKL ******] Frame Height: %d" , pstIavFrameDesc->reso.height);
		// 时间戳
		TIPC_PRINTF_CHK("[****** WKL ******] Frame PTS: %d" , pstIavFrameDesc->dsp_pts);

		// ensure the pts is same as yuv.
		if ((uint32_t)pstIavFrameDesc->dsp_pts == (uint32_t)g_stYuvCfg.Canvas_Buff[index].pts) {
			break;
		}
	}


	char* Jpeg_buffer_addr = pbsb_mem + pstIavFrameDesc->data_addr_offset;
	uint32_t jpeg_size = pstIavFrameDesc->size;

	if (pstIavFrameDesc->reso.width == dest_width && pstIavFrameDesc->reso.height == dest_height)
	{
		jpeg_info->JpegWidth = pstIavFrameDesc->reso.width;
		jpeg_info->JpegHeight = pstIavFrameDesc->reso.height;
		jpeg_info->JpegSize = jpeg_size;
		jpeg_info->JpegAddr = (char *)malloc(jpeg_size * sizeof(char));
		memcpy(jpeg_info->JpegAddr, Jpeg_buffer_addr, jpeg_size);
		// TIPC_PRINTF_CHK("Addr = %p", jpeg_info->JpegAddr);
	} else
	{
		jpeg_info->JpegWidth = pstIavFrameDesc->reso.width;
		jpeg_info->JpegHeight = pstIavFrameDesc->reso.height;
		jpeg_info->JpegSize = jpeg_size;
		jpeg_info->JpegAddr = (char *)malloc(jpeg_size * sizeof(char));
		memcpy(jpeg_info->JpegAddr, Jpeg_buffer_addr, jpeg_size);
		// TIPC_PRINTF_CHK("Addr = %p", jpeg_info->JpegAddr);
		// TODO need zoom
	}
	// release all efm frame:
	struct iav_efm_release_all_frame release_cfg = {0};
	// memset( &release_cfg, 0, sizeof(struct iav_efm_release_all_frame));
	release_cfg.stream_id = YUV_STREAM_ID;
	release_cfg.no_prefetch_flag = 0;
	AM_IOCTL(g_stYuvCfg.s32Fd , IAV_IOC_EFM_RELEASE_ALL_FRAMES, &release_cfg);

	// release queryframe
	AM_IOCTL(g_stYuvCfg.s32Fd , IAV_IOC_RELEASE_FRAMEDESC, pstIavFrameDesc);


	return 0;
}

int SetYuvBuffSize(unsigned int width, unsigned int height) {

	struct iav_video_proc stIavVproc = {0};
	struct iav_dptz* pstIavDptz = &stIavVproc.arg.dptz;
	struct iav_apply_flag stApplyFlag[IAV_VIDEO_PROC_NUM] = {0};
	stIavVproc.cid = IAV_VIDEO_PROC_DPTZ;

	// Get cfg
	pstIavDptz->channel_id = 0;   // only for sensor 0
	pstIavDptz->pass_id = YUV_PASS_ID;
	pstIavDptz->buf_id = YUV_BUFF_ID;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_GET_VIDEO_PROC, &stIavVproc);

	// TIPC_PRINTF_CHK("[****** WKL ******]Before, The Get Size : % dx % d ", pstIavDptz->buf_cfg.output.width, pstIavDptz->buf_cfg.output.height);

	// Set cfg
	pstIavDptz->buf_cfg.output.width = width;
	pstIavDptz->buf_cfg.output.height = height;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_CFG_VIDEO_PROC, &stIavVproc);

	// Apply
	stApplyFlag[IAV_VIDEO_PROC_DPTZ].param = (1 << 0);
	stApplyFlag[IAV_VIDEO_PROC_DPTZ].apply = 1;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_APPLY_VIDEO_PROC, stApplyFlag);

	return 0;
}

int amba_yuv_init(int s32IavFD, int s32VinNum) {
	int s32Ret = 0;
	if (s32IavFD < 0)
	{
		TIPC_PRINTF_ERR("iav fd err[ %d]", s32IavFD);
		return -1;
	}

	// Init Parameter
	g_stYuvCfg.s32Fd = s32IavFD;
	g_stYuvCfg.u8Init = 0;
	g_stYuvCfg.Canvas_Pool_InitFlag = 0;

	g_stYuvCfg.s32VinNum = s32VinNum;

	for (int ind = 0 ; ind < YUV_MAX_CACHED_BUFF_NUM; ind++) {
		g_stYuvCfg.Canvas_Buff[ind].get_data_flag = 0;
	}

	s32Ret += MapCanvasPoolAddr();
	s32Ret += amba_yuv_MapDspMem();
	if ( s32Ret ) {
		TIPC_PRINTF_ERR("amba yuv init Error");
		return s32Ret;
	}

	g_stYuvCfg.u8Init = 1;
	return s32Ret;
}

int amba_yuv_exit() {
	int s32Ret;
	if (!g_stYuvCfg.u8Init) {
		TIPC_PRINTF_ERR("The Service is not running");
		return -1;
	}

	s32Ret = amba_yuv_ReleaseAllFrame();
	if (s32Ret) {
		TIPC_PRINTF_ERR("Release All Frame Error");
		return -1;
	}
	s32Ret = stop_efm_stream();
	if (s32Ret) {
		TIPC_PRINTF_ERR("Stop efm stream Error");
		return -1;
	}
	if (g_stYuvCfg.Canvas_Pool_VirAddr) {
		if ( munmap(g_stYuvCfg.Canvas_Pool_VirAddr, g_stYuvCfg.Canvas_Pool_Size) ) {
			TIPC_PRINTF_ERR("munmap memory failed");
		}

		g_stYuvCfg.Canvas_Pool_VirAddr = NULL;
		g_stYuvCfg.Canvas_Pool_PhyAddr = 0;
		g_stYuvCfg.Canvas_Pool_Size = 0;
		g_stYuvCfg.Canvas_Pool_InitFlag = 0;
	}

	if (g_stYuvCfg.DspAddr) {
		if ( munmap(g_stYuvCfg.DspAddr, g_stYuvCfg.DspSize) ) {
			TIPC_PRINTF_ERR("munmap memory failed");
		}
		g_stYuvCfg.DspAddr = NULL;
		g_stYuvCfg.DspPhyAddr = 0;
		g_stYuvCfg.DspSize = 0;
		g_stYuvCfg.u8DspMapInit = 0;
	}
	memset(&g_stYuvCfg, 0, sizeof(g_stYuvCfg));
	return 0;
}



///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Not Uesd Interface ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

/*
int amba_yuv_GetCapture_old(const unsigned char u8_copy_flag, AMBA_YUV_FRAMEINFO_S * pstYUVFrameInfo)
{
	if (!g_stYuvCfg.u8DspMapInit || !g_stYuvCfg.u8Init) {
		TIPC_PRINTF_ERR ("The Yuv Service is not init");
		return -1;
	}

	if (pstYUVFrameInfo == NULL) {
		TIPC_PRINTF_ERR ("Error, the pstYUVFrameInfo is NULL");
		return -1;
	}

	//Query Canvas Desc
	struct iav_querydesc query_desc;
	struct iav_canvasdesc *canvas;
	struct iav_yuv_cap *yuv_cap;
	memset(&query_desc, 0, sizeof(query_desc));

	query_desc.qid = IAV_DESC_CANVAS;
	canvas = &query_desc.arg.canvas;
	canvas->canvas_id = YUV_CANVAS_ID;  // 7
	canvas->query_extra_raw = 0;
	canvas->skip_cache_sync = 1;
	canvas->yuv_use_dma_buf_fd = u8_copy_flag;
	canvas->non_block_flag = 0;
	AM_IOCTL(g_stYuvCfg.s32Fd , IAV_IOC_QUERY_DESC, &query_desc);

	// get the yuv capture info
	yuv_cap = &canvas->yuv;

	if ((yuv_cap->y_addr_offset == 0) ||
	        (yuv_cap->uv_addr_offset == 0)) {
		TIPC_PRINTF_ERR ("YUV buffer offset is 0");
		return -1;
	}

	if (yuv_cap->format != IAV_YUV_FORMAT_YUV420) {
		TIPC_PRINTF_ERR ("yuv_cap->format != IAV_YUV_FORMAT_YUV420");
		return -1;
	}

	// set size info
	AMBA_YUV_FRAMEINFO_S* pstYUVFrameInfo_Tmp = (AMBA_YUV_FRAMEINFO_S*) malloc (sizeof (AMBA_YUV_FRAMEINFO_S));
	memset (pstYUVFrameInfo_Tmp, 0, sizeof(AMBA_YUV_FRAMEINFO_S));

	pstYUVFrameInfo_Tmp->pitch = yuv_cap->pitch;
	pstYUVFrameInfo_Tmp->width = yuv_cap->width;
	pstYUVFrameInfo_Tmp->height = yuv_cap->height;
	pstYUVFrameInfo_Tmp->pts = yuv_cap->dsp_pts;

	pstYUVFrameInfo_Tmp->y_phy_addr = g_stYuvCfg.u32DspPhyAddr + yuv_cap->y_addr_offset;
	pstYUVFrameInfo_Tmp->uv_phy_addr = g_stYuvCfg.u32DspPhyAddr + yuv_cap->uv_addr_offset;

	if (u8_copy_flag)
	{
		uint32_t luma_size =  yuv_cap->width * yuv_cap->height;
		// For YUV420NV12, The UV size is half of Y size
		// uint32_t chorma_size =  luma_size >> 1;

		// gdma copy allocate
		if (g_stYuvCfg.YuvDMAfd < 0 ) {
			if (alloc_gdma_dst_buf(luma_size)) {
				TIPC_PRINTF_ERR ("alloc_gdma_dst_buf error");
				return -1;
			}
		}
		// copy
		if (copy_yuv_data(canvas->yuv_dma_buf_fd, yuv_cap, 1, 0) < 0) {
			TIPC_PRINTF_ERR ("Failed to copy yuv data");
			return -1;
		}
		pstYUVFrameInfo_Tmp->copy_flag = 1;
		pstYUVFrameInfo_Tmp->y_addr = g_stYuvCfg.pu8YuvDMAAddr;
		pstYUVFrameInfo_Tmp->uv_addr = (g_stYuvCfg.pu8YuvDMAAddr + luma_size);
	} else {
		pstYUVFrameInfo_Tmp->copy_flag = 0;
		pstYUVFrameInfo_Tmp->y_addr = g_stYuvCfg.pu8DspAddr + yuv_cap->y_addr_offset;
		pstYUVFrameInfo_Tmp->uv_addr = g_stYuvCfg.pu8DspAddr + yuv_cap->uv_addr_offset;
	}
	memcpy(pstYUVFrameInfo, pstYUVFrameInfo_Tmp, sizeof(AMBA_YUV_FRAMEINFO_S));

	free (pstYUVFrameInfo_Tmp);

	return 0;
}*/

// Not Used ( if use this function, remember to unmmap and free memory !)
// static int amba_yuv_mapCanvas(void)
// {

// 	struct iav_querymem stQueryMem;
// 	struct iav_mem_canvas_info* pstCanvasInfo;
// 	unsigned char* pu8MapAddr = NULL;

// 	int s32MemLength = 0;
// 	int s32Fd = g_stYuvCfg.s32Fd;
// 	// int s32VinNum = g_stYuvCfg.s32VinNum;
// 	if (s32Fd <= 0)
// 	{
// 		TIPC_PRINTF_ERR("fd of /dev/iav err[%d]", s32Fd);
// 		return -1;
// 	}

// 	//Set IAV_MEM_CANVAS
// 	memset(&stQueryMem, 0, sizeof(stQueryMem));
// 	stQueryMem.mid = IAV_MEM_CANVAS;
// 	//  0 refer to Main Buffer( Failed )
// 	//  1 Main Encode Stream, OK
// 	//  3 Encode Stream, OK
// 	//	5 Jpeg Stream, OK
// 	//  6 refer to VOut ( Failed )
// 	stQueryMem.arg.canvas.id_map = 1;

// 	pstCanvasInfo = &stQueryMem.arg.canvas;
// 	AM_IOCTL(s32Fd, IAV_IOC_QUERY_MEMBLOCK, &stQueryMem);

// 	// IAV_MAX_CANVAS_BUF_NUM = 12
// 	// yuv[11] Error
// 	// yuv[1] Error
// 	if (pstCanvasInfo->yuv[0].length == 0)
// 	{
// 		TIPC_PRINTF_ERR("pstCanvasInfo->yuv[0].length == 0.");
// 		return -1;
// 	}

// 	//mmap
// 	s32MemLength = pstCanvasInfo->yuv[0].length;
// 	pu8MapAddr = mmap(NULL, s32MemLength, PROT_READ, MAP_SHARED, s32Fd, pstCanvasInfo->yuv[0].addr);

// 	if ((MAP_FAILED == pu8MapAddr) || (NULL == pu8MapAddr))
// 	{
// 		TIPC_PRINTF_ERR("mmap failed!!");
// 		return -1;
// 	}

// 	// global var
// 	g_stYuvCfg.s32YuvDataSize = s32MemLength;
// 	g_stYuvCfg.pu8YuvDMAAddr = pu8MapAddr;
// 	g_stYuvCfg.u32YuvDataOffset = pstCanvasInfo->yuv[0].offset;

// 	return 0;
// }

/*
static int alloc_gdma_dst_buf(u32 size)
{
	struct iav_alloc_mem_part_fd alloc_mem_part;

	// For yuv420,Needed Memory is Y size + 1/2 Y size(UV),So alloc 2 * Y size
	alloc_mem_part.length = size * 2;
	alloc_mem_part.enable_cache = 1;

	if (ioctl(g_stYuvCfg.s32Fd , IAV_IOC_ALLOC_ANON_MEM_PART_FD, &alloc_mem_part) < 0) {
		perror("IAV_IOC_ALLOC_ANON_MEM_PART_FD");
		return -1;
	}

	g_stYuvCfg.YuvDMAfd = alloc_mem_part.dma_buf_fd;
	g_stYuvCfg.YuvDMADataSize = lseek(g_stYuvCfg.YuvDMAfd, 0, SEEK_END);
	if (g_stYuvCfg.YuvDMADataSize)
	{
		g_stYuvCfg.pu8YuvDMAAddr = mmap(NULL, g_stYuvCfg.YuvDMADataSize, PROT_READ | PROT_WRITE, MAP_SHARED,
		                                g_stYuvCfg.YuvDMAfd, 0);

		if (g_stYuvCfg.pu8YuvDMAAddr == MAP_FAILED) {
			perror("mmap dma buffer failed\n");
			return -1;
		}
	}
	return 0;
}

static int copy_yuv_data(int dmabuf_fd, struct iav_yuv_cap *yuv_cap, u8 is_canvas, u8 is_manual_feed)
{
	struct iav_gdma_copy gdma_copy = {0};
	int rval = 0;

	// The parameter is set refer to official sample test_yubcap.c
	gdma_copy.src_skip_cache_sync = 1;
	gdma_copy.src_offset = yuv_cap->y_addr_offset;
	gdma_copy.dst_offset = 0;
	gdma_copy.src_pitch = yuv_cap->pitch;
	gdma_copy.dst_pitch = yuv_cap->width;
	gdma_copy.width = yuv_cap->width;
	gdma_copy.height = yuv_cap->height;

	if (dmabuf_fd > 0) {
		gdma_copy.src_dma_buf_fd = dmabuf_fd;
		gdma_copy.src_use_dma_buf_fd = 1;
	} else {
		if (is_canvas) {
			gdma_copy.src_mmap_type = is_manual_feed ? IAV_PART_CANVAS_POOL : IAV_PART_DSP;
		} else {
			gdma_copy.src_mmap_type = is_manual_feed ? IAV_PART_PYRAMID_POOL : IAV_PART_DSP;
		}
	}
	gdma_copy.dst_dma_buf_fd = g_stYuvCfg.YuvDMAfd;
	gdma_copy.dst_use_dma_buf_fd = 1;

	// TIPC_PRINTF_CHK ("*************** %d", g_stYuvCfg.YuvDMAfd);

	if (ioctl(g_stYuvCfg.s32Fd, IAV_IOC_GDMA_COPY, &gdma_copy) < 0) {
		perror("IAV_IOC_GDMA_COPY");
		rval = -1;
		goto COPY_YUV_DATA_EXIT;
	}


	gdma_copy.src_offset = yuv_cap->uv_addr_offset;
	gdma_copy.dst_offset = yuv_cap->width * yuv_cap->height;
	if (yuv_cap->format == IAV_YUV_FORMAT_YUV420) {
		gdma_copy.height = yuv_cap->height / 2;
	} else {
		gdma_copy.height = yuv_cap->height;
	}
	// TIPC_PRINTF_CHK ("*************** %d", yuv_cap->uv_addr_offset);
	if (ioctl(g_stYuvCfg.s32Fd, IAV_IOC_GDMA_COPY, &gdma_copy) < 0) {
		perror("IAV_IOC_GDMA_COPY");
		rval = -1;
		goto COPY_YUV_DATA_EXIT;
	}

COPY_YUV_DATA_EXIT:
	return rval;
}*/


/*int amba_yuv_MapDspMem(void) {

	// check state
	int dsp_state;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_GET_IAV_STATE, &dsp_state);

	if ((dsp_state != IAV_STATE_PREVIEW) && (dsp_state != IAV_STATE_ENCODING)) {
		TIPC_PRINTF_ERR("IAV is not in preview / encoding state, cannot get yuv buf!\n");
		return -1;
	}

	//map dsp buffer
	struct iav_querymem query_mem;
	struct iav_mem_part_info *part_info;

	memset(&query_mem, 0, sizeof(query_mem));
	query_mem.mid = IAV_MEM_PARTITION;
	part_info = &query_mem.arg.partition;
	part_info->pid = IAV_PART_DSP;

	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_QUERY_MEMBLOCK, &query_mem);

	// mmap
	uint8_t * dsp_data_mem  = NULL;

	dsp_data_mem = (uint8_t *)mmap(NULL, part_info->mem.length , PROT_READ | PROT_WRITE, MAP_SHARED,
	                               g_stYuvCfg.s32Fd,  part_info->mem.addr);

	if (dsp_data_mem == MAP_FAILED || dsp_data_mem == NULL) {
		TIPC_PRINTF_ERR ("Dsp Address Mmap Error!!");
		return -1;
	}
	g_stYuvCfg.u32DspSize = part_info->mem.length;
	g_stYuvCfg.u32DspPhyAddr = part_info->mem.addr;
	g_stYuvCfg.pu8DspAddr =  dsp_data_mem;
	g_stYuvCfg.u8DspMapInit = 1;
	return 0;
}*/


/*int amba_yuv_SaveYUVtoFile(const AMBA_YUV_FRAMEINFO_S * pstYUVFrameInfo)
{
	if (pstYUVFrameInfo == NULL) {
		TIPC_PRINTF_ERR("Error, The Yuv Info Pointer is NULL");
		return -1;
	}

	char filename [256] = {0};
	uint8_t * yuv_data_mem = NULL;
	uint32_t luma_size = pstYUVFrameInfo->width * pstYUVFrameInfo->height;
	// For YUV420NV12, The UV size is half of Y size
	uint32_t chorma_size =  luma_size >> 1;

	if (pstYUVFrameInfo->copy_flag) {
		yuv_data_mem = pstYUVFrameInfo->y_addr;

		sprintf (filename, "yuv_copy_capture.yuv");
		FILE * fp = fopen (filename, "wb");
		fseek(fp, 0, SEEK_SET);
		if (fp) {
			fwrite ( yuv_data_mem , (chorma_size + luma_size) , 1 , fp);
			fflush(fp);
		}
		fclose (fp);
	}
	else {

		uint8_t * pu8_yuv_data_tmp = (uint8_t *) malloc( (chorma_size + luma_size) * sizeof(uint8_t ));
		memset ( pu8_yuv_data_tmp, 0, (chorma_size + luma_size));
		yuv_data_mem = pu8_yuv_data_tmp;

		// solve the pitch > width for y
		uint8_t * pu8_y_data = pstYUVFrameInfo->y_addr;
		for (int i = 0; i < pstYUVFrameInfo->height; i++) {
			memcpy(pu8_yuv_data_tmp, pu8_y_data, pstYUVFrameInfo->width);
			pu8_y_data += pstYUVFrameInfo-> pitch;
			pu8_yuv_data_tmp += pstYUVFrameInfo-> width;
		}
		// solve the pitch > width for uv
		uint8_t * pu8_uv_data = pstYUVFrameInfo->uv_addr;
		for (int i = 0; i < ( pstYUVFrameInfo->height / 2) ; i++) {
			memcpy(pu8_yuv_data_tmp, pu8_uv_data, pstYUVFrameInfo->width);
			pu8_uv_data += pstYUVFrameInfo-> pitch;
			pu8_yuv_data_tmp += pstYUVFrameInfo-> width;
		}

		sprintf (filename, "yuv_nocopy_capture.yuv");
		FILE * fp = fopen (filename, "wb");
		fseek(fp, 0, SEEK_SET);
		if (fp) {
			fwrite ( yuv_data_mem , (chorma_size + luma_size) , 1 , fp);
			fflush(fp);
		}
		fclose (fp);

	}
	return 0;
}*/

/*static int gdma_copy(uint64_t src, u32 src_pitch, uint64_t dst, u32 dst_pitch,
                     u32 width, u32 height)
{
	struct iav_gdma_copy param = {0};

	// Aligned to DSP physical base for GDMA copy
	param.src_offset = (u32)(src - g_stYuvCfg.DspPhyAddr);
	param.dst_offset = (u32)(dst - g_stYuvCfg.DspPhyAddr);

	param.src_mmap_type = IAV_PART_DSP;
	param.dst_mmap_type = IAV_PART_DSP;

	param.src_pitch = src_pitch;
	param.dst_pitch = dst_pitch;
	param.width = width;
	param.height = height;

	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_GDMA_COPY, &param);

	return 0;
}*/


/*int jpeg_stream_efm_test(int index)
{

	TIPC_PRINTF_CHK("/////////////////////////////// EFM ///////////////////////////////");
	int s32StreamID = 5;
	/////////////////////////////////// Map BSB ///////////////////////////////////

	// TODO :add check()
	char* pbsb_mem = amba_venc_getBsbMem();

	//////////////////////////////////// Set Stream ////////////////////////////////
	// Get Stream iav cfg
	struct iav_stream_cfg stIavStreamCfg = {0};
	struct iav_stream_format* pstIavStreamFormat = NULL;
	stIavStreamCfg.id = s32StreamID;	 // Stream id 5
	stIavStreamCfg.cid = IAV_STMCFG_FORMAT;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_GET_STREAM_CONFIG, &stIavStreamCfg);

	// Set encode format
	pstIavStreamFormat = &stIavStreamCfg.arg.format;
	pstIavStreamFormat->type = IAV_STREAM_TYPE_MJPEG; // For jpeg
	pstIavStreamFormat->enc_win.x = 0;
	pstIavStreamFormat->enc_win.y = 0;
	pstIavStreamFormat->enc_win.width = 1920;
	pstIavStreamFormat->enc_win.height = 1080;
	pstIavStreamFormat->enc_src_id = 1; //Source Canvas Id ??????????????????? TODO
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_SET_STREAM_CONFIG, &stIavStreamCfg);

	//Set Mjpeg Cfg

	struct iav_mjpeg_cfg stMjpegCfg = {0};
	struct iav_queryinfo stQueryInfo = {0};
	struct iav_stream_info* pstStreamInfo = NULL;
	struct iav_stream_cfg stStreamCfg = {0};
	int s32IavFD = g_stYuvCfg.s32Fd;

	memset(&stQueryInfo, 0, sizeof(stQueryInfo));
	stQueryInfo.qid = IAV_INFO_STREAM;
	pstStreamInfo = &stQueryInfo.arg.stream;
	pstStreamInfo->id = s32StreamID;
	AM_IOCTL(s32IavFD, IAV_IOC_QUERY_INFO, &stQueryInfo);

	if (pstStreamInfo->state != IAV_STREAM_STATE_ENCODING)
	{
		stMjpegCfg.id = s32StreamID;
		AM_IOCTL(s32IavFD, IAV_IOC_GET_MJPEG_CONFIG, &stMjpegCfg);
		// Jpeg quality: defined in amba_video.c
		stMjpegCfg.quality = 90;
		AM_IOCTL(s32IavFD, IAV_IOC_SET_MJPEG_CONFIG, &stMjpegCfg);
	}
	else
	{
		memset(&stStreamCfg, 0, sizeof(stStreamCfg));
		stStreamCfg.id = s32StreamID;
		stStreamCfg.cid = IAV_MJPEG_CFG_QUALITY;
		// Jpeg quality: defined in amba_video.c
		stStreamCfg.arg.mjpeg_quality = 90;
		AM_IOCTL(s32IavFD, IAV_IOC_SET_STREAM_CONFIG, &stStreamCfg);
	}

	// framerate
	// stIavStreamCfg.id = s32StreamID;
	// stIavStreamCfg.cid = IAV_STMCFG_FPS;
	// AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_GET_STREAM_CONFIG, &stIavStreamCfg);
	// stIavStreamCfg.arg.fps.abs_fps_enable = 1;
	// // fps: 6 for jpeg
	// stIavStreamCfg.arg.fps.abs_fps = 6;
	// AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_SET_STREAM_CONFIG, &stIavStreamCfg);

	// start encoding
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_START_ENCODE, (1 << stIavStreamCfg.id));

	// check stream State
	struct iav_queryinfo stIavQueryInfo;
	pstStreamInfo = NULL;
	memset(&stIavQueryInfo, 0, sizeof(stIavQueryInfo));
	stIavQueryInfo.qid = IAV_INFO_STREAM;
	pstStreamInfo = &stIavQueryInfo.arg.stream;
	pstStreamInfo->id = stIavStreamCfg.id;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_QUERY_INFO, &stIavQueryInfo);
	TIPC_PRINTF_CHK("Before EFM, stream[%d] status[%d]\n", s32StreamID, pstStreamInfo->state);

	// Get Stream iav cfg
	memset(&stIavStreamCfg, 0, sizeof(struct iav_stream_cfg ));
	stIavStreamCfg.id = s32StreamID;	 // Stream id 5
	stIavStreamCfg.cid = IAV_STMCFG_FORMAT;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_GET_STREAM_CONFIG, &stIavStreamCfg);

	// Set encode format
	pstIavStreamFormat = &stIavStreamCfg.arg.format;
	// pstIavStreamFormat->type = IAV_STREAM_TYPE_MJPEG; // For jpeg
	pstIavStreamFormat->enc_win.x = 300;
	pstIavStreamFormat->enc_win.y = 300;
	pstIavStreamFormat->enc_win.width = 300;
	pstIavStreamFormat->enc_win.height = 200;
	// pstIavStreamFormat->enc_src_id = 1; // Canvas Id
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_SET_STREAM_CONFIG, &stIavStreamCfg);

	// get efm pool info
	int ret = 0;
	ret = get_efm_pool_info(s32StreamID);
	if (ret) {
		TIPC_PRINTF_ERR ("get_efm_pool_info Error!");
	}

	//feed yuv
	// first: requset to get data offset
	struct iav_efm_request_frame yuv_request;
	memset(&yuv_request, 0, sizeof(struct iav_efm_request_frame));
	yuv_request.stream_id = s32StreamID;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_EFM_REQUEST_FRAME, &yuv_request);

	ret = fill_yuv_from_capture(index, &yuv_request);


	//handshake
	struct iav_efm_handshake_frame handshake;

	memset(&handshake, 0, sizeof(handshake));
	handshake.stream_id = s32StreamID;
	handshake.frame_idx = yuv_request.frame_idx;
	handshake.frame_pts = g_stYuvCfg.Canvas_Buff[index].query_desc.arg.canvas.yuv.dsp_pts;
	handshake.hold_frame = 1;

	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_EFM_HANDSHAKE_FRAME, &handshake);

	// Show Stream status
	memset(&stIavQueryInfo, 0, sizeof(stIavQueryInfo));
	stIavQueryInfo.qid = IAV_INFO_STREAM;
	pstStreamInfo = &stIavQueryInfo.arg.stream;
	pstStreamInfo->id = stIavStreamCfg.id;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_QUERY_INFO, &stIavQueryInfo);

	TIPC_PRINTF_CHK("After EFM, stream[%d] status[%d]\n", s32StreamID, pstStreamInfo->state);

	struct iav_querydesc stQueryDesc = {0};
	struct iav_framedesc* pstIavFrameDesc = NULL;
	// Query Desc
	memset(&stQueryDesc, 0, sizeof(stQueryDesc));
	pstIavFrameDesc = &stQueryDesc.arg.frame;
	stQueryDesc.qid = IAV_DESC_FRAME;
	pstIavFrameDesc->id = s32StreamID;

	// 超时时间
	pstIavFrameDesc->time_ms = 1000;
	AM_IOCTL(g_stYuvCfg.s32Fd, IAV_IOC_QUERY_DESC, &stQueryDesc);

	char* Jpeg_buffer_addr = pbsb_mem + pstIavFrameDesc->data_addr_offset;

	if ((IAV_PIC_TYPE_I_FRAME == pstIavFrameDesc->pic_type) || (IAV_PIC_TYPE_IDR_FRAME == pstIavFrameDesc->pic_type))
	{
		TIPC_PRINTF_CHK("[****** WKL ******] This is I Frame");
	}
	// 帧序号
	TIPC_PRINTF_CHK("[****** WKL ******] Frame Index: %d" , pstIavFrameDesc->frame_num);
	// 宽高
	TIPC_PRINTF_CHK("[****** WKL ******] Frame Width: %d" , pstIavFrameDesc->reso.width);
	TIPC_PRINTF_CHK("[****** WKL ******] Frame Height: %d" , pstIavFrameDesc->reso.height);
	// 时间戳
	TIPC_PRINTF_CHK("[****** WKL ******] Frame PTS: %d" , pstIavFrameDesc->arm_pts);

	uint32_t jpeg_size = pstIavFrameDesc->size;

	char filename[256] = {0};

	sprintf (filename, "yuvEfmTest.jpeg");
	FILE * fp = fopen (filename, "wb");
	fseek(fp, 0, SEEK_SET);
	if (fp) {
		fwrite ( Jpeg_buffer_addr , jpeg_size , 1 , fp);
		fflush(fp);
	}
	fclose (fp);

	Jpeg_buffer_addr = NULL;

	// TODO IAV_IOC_RELEASE_EFM

	//Unmutex TODO

	TIPC_PRINTF_CHK("/////////////////////////////// EFM END ///////////////////////////////");
	return 0;
}
*/