#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/prctl.h>

#include "sdk_func.h"
#include "sdk_define.h"
#include "sdk_private.h"

#include "tvt_sdk_param.h"
#include "tvt_sdk_print.h"
#include "tvt_sdk_video.h"
#include "tvt_sdk_audio.h"
#include "tvt_sdk_device.h"
#include "tvt_sdk_btn.h"
#include "tvt_sdk_flash.h"
#include "tvt_sdk_update.h"

TVT_SDK_CFG_S g_stSdkCfg = {0};

#define SDK_THREAD_SLEEP_INTERVAL_US 100000

#define WKL_FACEDT_YUV_DEBUG 0
#define WKL_FACEDT_JPEG_DEBUG 0
/**
 * @brief SDK循环函数，由AP层每200ms调用
 */
int SDKLoopFunc(void)
{
    return 0;
}

void* sdk_thread(void* pArg)
{
    unsigned int s32LoopCount = 0;
    int ret;
    int i = 0, j = 0;
    // 给线程起个名字
    prctl(PR_SET_NAME, "sdk_thread");
    while (1 == g_stSdkCfg.s32SdkThreadRun)
    {
        s32LoopCount++;
        // TIPC_PRINTF_CHK("SDK s32LoopCount = %d", s32LoopCount);
#if WKL_FACEDT_JPEG_DEBUG
        if (s32LoopCount == 90)
        {
            TIPC_PRINTF_CHK("____________________________________________________________");
            //start

            SDK_FACE_DT_CFG st_fact_dt_cfg = {0};
            st_fact_dt_cfg.srcWidth = 1920;
            st_fact_dt_cfg.srcHeight = 1080;

            SDKFaceDtControl(SDK_FACE_DT_CTRL_START_HUAAN, (void*)&st_fact_dt_cfg, sizeof(SDK_FACE_DT_CFG));

            // get capture 0:
            SDK_SRC_IMAGE_DATA st_src_image_data_0 = {0};
            SDKFaceDtControl(SDK_FACE_DT_CTRL_GET_DATA_HUAAN, (void*)&st_src_image_data_0,
                             sizeof(SDK_SRC_IMAGE_DATA));
            usleep(5 * SDK_THREAD_SLEEP_INTERVAL_US);

            //crop_img_0:
            SDK_CROP_IMAGE_DATA  stCropImgData = {0};
            stCropImgData.iMemIndex = st_src_image_data_0.iMemIndex;
            stCropImgData.faceId = 1;
            stCropImgData.ptLeftTop.x = 200;
            stCropImgData.ptLeftTop.y = 200;
            stCropImgData.ptRightBottom.x = 500;
            stCropImgData.ptRightBottom.y = 400;

            SDKFaceDtControl(SDK_FACE_DT_CTRL_CROP_IMG_HUAAN, (void*)&stCropImgData,
                             sizeof(SDK_CROP_IMAGE_DATA));

            char filename[256] = {0};
            // TIPC_PRINTF_CHK("Addr_out = %p", stCropImgData.pJpegData);

            sprintf (filename, "yuvEfmTest_0.jpeg");
            FILE * fp = fopen (filename, "wb");
            fseek(fp, 0, SEEK_SET);
            if (fp) {
                fwrite ( stCropImgData.pJpegData , stCropImgData.iJpegLen , 1 , fp);
                fflush(fp);
            }
            fclose (fp);
            usleep(5 * SDK_THREAD_SLEEP_INTERVAL_US);

            //crop_img_1:
            memset(&stCropImgData, 0, sizeof(SDK_CROP_IMAGE_DATA));
            stCropImgData.iMemIndex = st_src_image_data_0.iMemIndex;
            stCropImgData.faceId = 2;
            stCropImgData.ptLeftTop.x = 400;
            stCropImgData.ptLeftTop.y = 500;
            stCropImgData.ptRightBottom.x = 1200;
            stCropImgData.ptRightBottom.y = 1100;

            SDKFaceDtControl(SDK_FACE_DT_CTRL_CROP_IMG_HUAAN, (void*)&stCropImgData,
                             sizeof(SDK_CROP_IMAGE_DATA));

            sprintf (filename, "yuvEfmTest_1.jpeg");
            FILE * fp2 = fopen (filename, "wb");
            fseek(fp2, 0, SEEK_SET);
            if (fp2) {
                fwrite ( stCropImgData.pJpegData , stCropImgData.iJpegLen , 1 , fp2);
                fflush(fp2);
            }
            fclose (fp2);
            usleep(5 * SDK_THREAD_SLEEP_INTERVAL_US);


            // release capture 0:
            SDKFaceDtControl(SDK_FACE_DT_CTRL_RELEASE_DATA_HUAAN, (void*)&st_src_image_data_0.iMemIndex,
                             sizeof(int));
            // exit
            SDKFaceDtControl (SDK_FACE_DT_CTRL_STOP_HUAAN, NULL, 0);
            TIPC_PRINTF_CHK("____________________________________________________________");

        }
#endif

#if WKL_FACEDT_YUV_DEBUG

        typedef struct _tvt_Img_Frame
        {
            unsigned int u32Width;
            unsigned int u32Height;

            unsigned int enPixelFormat;  //PIXEL_FORMAT_E
            unsigned int u32Stride[3];
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

            unsigned long long u64PTS;
            unsigned int u32PoolId;
            unsigned int enModId;
        } tvt_Img_Frame;

        SDK_SRC_IMAGE_DATA st_src_image_data_0 = {0};
        SDK_SRC_IMAGE_DATA st_src_image_data_1 = {0};
        if (s32LoopCount == 90) {
            TIPC_PRINTF_CHK("____________________________________________________________");
            //start
            SDK_FACE_DT_CFG st_fact_dt_cfg = {0};
            st_fact_dt_cfg.srcWidth = 1920;
            st_fact_dt_cfg.srcHeight = 1080;

            SDKFaceDtControl(SDK_FACE_DT_CTRL_START_HUAAN, (void*)&st_fact_dt_cfg, sizeof(SDK_FACE_DT_CFG));

            // get capture 0 1st:
            SDKFaceDtControl(SDK_FACE_DT_CTRL_GET_DATA_HUAAN, (void*)&st_src_image_data_0,
                             sizeof(SDK_SRC_IMAGE_DATA));

            // save capture 0 1st:
            char filename[256] = {0};
            sprintf (filename, "cap_%d_1st.yuv", st_src_image_data_0.iMemIndex);

            tvt_Img_Frame* temp_img = (tvt_Img_Frame *) malloc(sizeof(tvt_Img_Frame));
            memset (temp_img, 0, sizeof(tvt_Img_Frame));
            memcpy (temp_img, st_src_image_data_0.data , sizeof(tvt_Img_Frame));

            int pitch = temp_img->u32Stride[0];
            TIPC_PRINTF_CHK("pitch = %d", pitch);
            uint8_t* y_addr = (uint8_t*) (temp_img->Addr.st64.u64VirAddr[0]);

            uint32_t luma_size = pitch * 1080;
            uint32_t chorma_size = (luma_size >> 1);

            FILE * fp = fopen (filename, "wb");
            fseek(fp, 0, SEEK_SET);
            if (fp) {
                fwrite ( y_addr , (luma_size + chorma_size) , 1 , fp);
                fflush(fp);
            }
            fclose (fp);

            usleep(5 * SDK_THREAD_SLEEP_INTERVAL_US);

            // get capture 1 1st:
            SDKFaceDtControl(SDK_FACE_DT_CTRL_GET_DATA_HUAAN, (void*)&st_src_image_data_1,
                             sizeof(SDK_SRC_IMAGE_DATA));

            usleep(5 * SDK_THREAD_SLEEP_INTERVAL_US);

            // save capture 0 2nd:
            sprintf (filename, "cap_%d_2nd.yuv", st_src_image_data_0.iMemIndex);

            fp = fopen (filename, "wb");
            fseek(fp, 0, SEEK_SET);
            if (fp) {
                fwrite ( y_addr , (luma_size + chorma_size) , 1 , fp);
                fflush(fp);
            }
            fclose (fp);

            //release capture 0 1st:
            SDKFaceDtControl(SDK_FACE_DT_CTRL_RELEASE_DATA_HUAAN, (void*)&st_src_image_data_0.iMemIndex,
                             sizeof(int));

            // get capture 0 2nd:
            memset (&st_src_image_data_0, 0, sizeof(SDK_SRC_IMAGE_DATA));
            SDKFaceDtControl(SDK_FACE_DT_CTRL_GET_DATA_HUAAN, (void*)&st_src_image_data_0,
                             sizeof(SDK_SRC_IMAGE_DATA));

            usleep(5 * SDK_THREAD_SLEEP_INTERVAL_US);

            // save capture 0 3rd:
            sprintf (filename, "cap_%d_3rd.yuv", st_src_image_data_0.iMemIndex);

            memset (temp_img, 0, sizeof(tvt_Img_Frame));
            memcpy (temp_img, st_src_image_data_0.data , sizeof(tvt_Img_Frame));

            y_addr = (uint8_t*) (temp_img->Addr.st64.u64VirAddr[0]);

            fp = fopen (filename, "wb");
            fseek(fp, 0, SEEK_SET);
            if (fp) {
                fwrite ( y_addr , (luma_size + chorma_size) , 1 , fp);
                fflush(fp);
            }
            fclose (fp);

            // release capture 0 2nd:
            SDKFaceDtControl(SDK_FACE_DT_CTRL_RELEASE_DATA_HUAAN, (void*)&st_src_image_data_0.iMemIndex,
                             sizeof(int));

            usleep(5 * SDK_THREAD_SLEEP_INTERVAL_US);

            // save capture 1 1st:

            sprintf (filename, "cap_%d_1st.yuv", st_src_image_data_1.iMemIndex);

            memset (temp_img, 0, sizeof(tvt_Img_Frame));
            memcpy (temp_img, st_src_image_data_1.data , sizeof(tvt_Img_Frame));

            y_addr = (uint8_t*) (temp_img->Addr.st64.u64VirAddr[0]);

            fp = fopen (filename, "wb");
            fseek(fp, 0, SEEK_SET);
            if (fp) {
                fwrite ( y_addr , (luma_size + chorma_size) , 1 , fp);
                fflush(fp);
            }
            fclose (fp);

            usleep(5 * SDK_THREAD_SLEEP_INTERVAL_US);

            // release capture 1 1st:
            SDKFaceDtControl(SDK_FACE_DT_CTRL_RELEASE_DATA_HUAAN, (void*)&st_src_image_data_1.iMemIndex,
                             sizeof(int));

            // exit
            SDKFaceDtControl (SDK_FACE_DT_CTRL_STOP_HUAAN, NULL, 0);
            TIPC_PRINTF_CHK("____________________________________________________________________");
        }
#endif
        for (i = 0; i < g_stSdkCfg.s32VinCnt; i++)
        {


            // 更新时间OSD
            tvt_sdk_video_ctrl(TVT_SDK_VIDEO_UPDATE_TIME_OSD, &i, sizeof(int));

            // 检查Comm OSD
            tvt_sdk_video_ctrl(TVT_SDK_VIDEO_UPDATE_COMM_OSD, &i, sizeof(int));

            // 定期更新Jpeg Buffer数据
            // TODO "CTHREAD:#13StartCapturePictureThread:547526529456:" DVSDevice.cpp:StartSDK():11230

            // for (j = TVT_SDK_VIDEO_ENC_JPEG_STREAM; j < TVT_SDK_VIDEO_ENC_STREAM_BUTT; j++)
            // {
            //     ret = 0;
            //     IPCAMERA_AV_FRAME_INFO stFrameTmp = {0};
            //     int s32StreamID = (i * TVT_SDK_VIDEO_ENC_STREAM_BUTT) + j;
            //     ret += tvt_sdk_video_getEncodeStream(s32StreamID, &stFrameTmp, NULL, NULL, 0);
            //     ret += tvt_sdk_video_releaseEncodeStream(&stFrameTmp);
            //     if (ret) TIPC_PRINTF_ERR("SDK_thread Error!");
            // }
        }

        usleep(SDK_THREAD_SLEEP_INTERVAL_US);
    }
    // TIPC_PRINTF_CHK ("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! sdk_thread return");
    return NULL;
}

int BuildSdkVersion(void)
{
    char s8Tmp = 0;

    s8Tmp = SDK_PLATFORM_VERSION;
    g_stSdkCfg.s32SDKVersion |= s8Tmp << 24;
    s8Tmp = SDK_PATCH_VERSION;
    g_stSdkCfg.s32SDKVersion |= s8Tmp << 16;
    s8Tmp = SDK_COMMFUNC_VERSION;
    g_stSdkCfg.s32SDKVersion |= s8Tmp << 8;
    s8Tmp = SDK_BUILD_VERSION;
    g_stSdkCfg.s32SDKVersion |= s8Tmp;

    return g_stSdkCfg.s32SDKVersion;
}

/**
* @brief SDK初始化，大部分SDK接口依赖于SDK初始化以后才能正常调用
* @param enInputFormat 参考枚举SDK_VIDEO_FORMAT，PAL = 0，NTSC = 1;
* @return 0：成功；非0：失败
*/
int SDKInit(SDK_VIDEO_FORMAT enInputFormat)
{
    int s32Ret = 0;

    // 打印版本
    BuildSdkVersion();
    TIPC_PRINTF_CHK( "***********************************************************");
    TIPC_PRINTF_CHK( "* IPC SDK VERSION() : *** %08x *** ", g_stSdkCfg.s32SDKVersion );
    TIPC_PRINTF_CHK( "***********************************************************");
    TIPC_PRINTF_CHK( "********%s %s******", __DATE__, __TIME__);

    // 初始化参数业务
    s32Ret = tvt_sdk_param_init();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("tvt_sdk_param_init failed[%d]!!", s32Ret);
    }

    // Init the SDK Part about FLASH Read & Write:
    s32Ret = tvt_sdk_flash_init();
    if (s32Ret) {
        TIPC_PRINTF_ERR("tvt_sdk_flash_init failed[%d]!!", s32Ret);
    }

    // Init the SDK Part about Update:
    s32Ret = tvt_sdk_update_init();
    if (s32Ret) {
        TIPC_PRINTF_ERR("tvt_sdk_update_init failed[%d]!!", s32Ret);
    }

    // 初始化外设业务
    s32Ret = tvt_sdk_device_init(&g_stSdkCfg.stDevModelInfo, tvt_sdk_param_ctrl);
    s32Ret = tvt_sdk_btn_init();

    // 初始化音频业务
    // wkl add:
    s32Ret = tvt_sdk_audio_init(&g_stSdkCfg.stDevModelInfo, tvt_sdk_param_ctrl);

    // 初始化视频相关业务
    s32Ret = SDKInitVideo(enInputFormat);

    // 创建内部线程
    g_stSdkCfg.s32SdkThreadRun = 1;
    s32Ret = pthread_create(&g_stSdkCfg.s32SdkThread, NULL, sdk_thread, (void *) NULL);
    if (s32Ret < 0)
    {
        TIPC_PRINTF_ERR("sdk_thread create failed[%d]!!", s32Ret);
        goto INIT_FAIL;
    }

    g_stSdkCfg.u8Init = 1;
    return 0;

INIT_FAIL:
    tvt_sdk_video_exit();
    return -1;
}

/**
 * @brief SDK去初始化
 * @return 0：成功；非0：失败
 */
int SDKExit(void)
{
    int s32Ret = 0;
    if (1 == g_stSdkCfg.s32SdkThreadRun)
    {
        g_stSdkCfg.s32SdkThreadRun = 0;
        pthread_join(g_stSdkCfg.s32SdkThread, NULL);
    }
    s32Ret = tvt_sdk_update_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("tvt_sdk_update_exit failed[%d]!!", s32Ret);
    }
    s32Ret = tvt_sdk_flash_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("tvt_sdk_flash_exit failed[%d]!!", s32Ret);
    }
    s32Ret = tvt_sdk_device_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("tvt_sdk_device_exit failed[%d]!!", s32Ret);
    }
    s32Ret = tvt_sdk_btn_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("tvt_sdk_btn_exit failed[%d]!!", s32Ret);
    }
    // 初始化音频业务
    s32Ret = tvt_sdk_audio_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("tvt_sdk_audio_exit failed[%d]!!", s32Ret);
    }
    //
    s32Ret = tvt_sdk_video_exit();
    if (s32Ret != 0)
    {
        TIPC_PRINTF_ERR("tvt_sdk_video_exit failed[%d]!!", s32Ret);
    }
    return s32Ret;
}

// int SDK_amba_thread_start() {
//     if (!g_stSdkCfg.s32SdkThreadRun)
//     {
//         g_stSdkCfg.s32SdkThreadRun = 1;
//         int s32Ret = pthread_create(&g_stSdkCfg.s32SdkThread, NULL, sdk_thread, (void *) NULL);
//         if (s32Ret < 0)
//         {
//             TIPC_PRINTF_ERR("sdk_thread create failed[%d]!!", s32Ret);
//             return -1;
//         }
//     }
//     return 0;
// }