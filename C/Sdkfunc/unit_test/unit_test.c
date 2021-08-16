#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>

#include "sdk_define.h"
#include "sdk_func.h"

#define MASK_NUM 4
#define ROI_NUM 8

#define SAVE_ENC 0
#define RUN_OSD  0
#define RUN_MASK 0
#define RUN_AUDIO 1
#define RUN_ROI 0

#define __TIC__()                                                               \
    struct timeval __timing_start, __timing_end;                                \
    gettimeofday(&__timing_start, NULL);

#define __TOC__()                                                               \
    do {                                                                        \
        gettimeofday(&__timing_end, NULL);                                      \
        double __timing_gap =                                                   \
            (__timing_end.tv_sec - __timing_start.tv_sec) * 1000.0 +            \
            (__timing_end.tv_usec - __timing_start.tv_usec) / 1000.0;           \
        fprintf(stdout, "TIME(ms): %lf\n", __timing_gap);                       \
    } while (0)

int g_s32Run = 300;
// int g_s32Run = 300 * 100;

#if RUN_AUDIO
pthread_t g_s32AudioThread;
int g_s32AudioThdFlag = 0;
#endif

void exitProg(int signo)
{
    if (SIGINT == signo || SIGTSTP == signo || SIGTERM == signo)
    {
        g_s32Run = 0;
        sleep(10);

        printf("\033[0;31mprogram exit!\033[0;39m\n");
    }

    // exit(0);
}

#if RUN_AUDIO
/**
 * @brief “Ù∆µœﬂ≥Ã
 * @return
 */
void* test_audio_service(void* pData)
{
    FILE* pFp = NULL;
    pFp = fopen("sdk.snd", "wb");
    unsigned char u8AudioBuff[644] = {0};
    while (g_s32AudioThdFlag)
    {
        usleep(40000);
#if RUN_AUDIO

        int s32Ret = 0;
        IPCAMERA_AV_FRAME_INFO stFrame = {0};
        memset(&stFrame, 0, sizeof(stFrame));
        s32Ret = SDKGetAudioFrame(0, &stFrame);
        if (s32Ret == 0)
        {
            // printf("1 get size %d\n", stFrame.length);
            if (pFp)
            {
                fwrite(stFrame.pData + 4, stFrame.length - 4, 1, pFp);
                fflush(pFp);
            }
            memcpy(u8AudioBuff, stFrame.pData, stFrame.length);
            SDKReleaseFrame(&stFrame);
            stFrame.pData = u8AudioBuff;
            s32Ret = SDKPushAudioFrame(0, &stFrame);
            if (s32Ret != 0)
            {
                printf("SDKPushAudioFrame err\n");
            }
        }
#endif
    }
    if (pFp)
    {
        fclose(pFp);
        pFp = NULL;
    }
    return NULL;
}

#endif

int check_set_cfg(void)
{
    int param_len = 32;
    // only for debug
    int item_number = IPC_FLASH_PARAM_ENCRY_VER;
    char param_value[32 + 1] = {'\0'};

    sprintf(param_value, "%s%c", "Hello_World!-", 'N');
    int ret = SDKSetFlashParamItem(param_value, param_len, item_number);
    printf ("SDKSetFlashParamItem return: %d \n", ret);
    return 0;
}

int check_get_cfg(void)
{
    int param_len = 32;
    char chPintOpen = '\0';
    // only for debug
    int item_number = IPC_FLASH_PARAM_ENCRY_VER;
    char param_value[32 + 1] = {'\0'};

    int ret = SDKGetFlashParamItem(param_value, param_len, item_number);
    printf ("SDKGetFlashParamItem return: %d \n", ret);

    if (NULL == strstr(param_value, "Hello_World!-"))
    {
        printf("Error!, %s\n", param_value);
        return -1;
    }

    chPintOpen = *(param_value + strlen("Hello_World!-"));
    if ('Y' == chPintOpen)
    {
        printf("Pint = Y\n");
    }
    else if ('N' == chPintOpen)
    {
        printf("Pint = N\n");
    }
    else
    {
        printf("Error!, %s\n", param_value);
        return -1;
    }

    return 0;
}

typedef enum __Firmware_Update_Type
{
    FIRMWARE_TYPE_UBOOT  = 0x1,
    FIRMWARE_TYPE_KERNEL = (0x1 << 1),
    FIRMWARE_TYPE_FILESYSTEM = (0x1 << 2),
    FIRMWARE_TYPE_APPFS = (0x1 << 3),
} Firmware_Update_Type;

int check_update(void)
{
    int ret = 0;
    unsigned int size = 48 * 1024 * 1024;

    char* mem = (char *) malloc (size + 1);
    memset (mem, 0, size + 1);
    FILE * fp = fopen ("E4H_4M_squashfs_rootfs", "rb");
    fseek(fp, 0, SEEK_SET);

    fread ( mem, 2 , 2142208 , fp);

    for (int ind = 0 ; ind < 32 ; ind++) {
        printf("%02x " , mem[ind]);
    }
    putchar ('\n');

    ret = SDKUpdateFlash(mem, size, FIRMWARE_TYPE_APPFS);
    printf ("SDKUpdateFlash return: %d \n", ret);
    free (mem);
    fclose (fp);
    return 0;
}

int main()
{
    int s32Ret = 0;
    int i = 0;
    signal(SIGINT, exitProg);
    signal(SIGTERM, exitProg);

    s32Ret = SDKInit(SDK_VIDEO_INPUT_PAL);
    if (s32Ret != 0)
    {
        printf("SDKInit errrrrr\n");
    }
    else
    {
        printf("SDKInit success\n");
    }

    // printf ("************************* FLASH CFG *************************\n");

    // check_set_cfg();
    // check_get_cfg();

    // printf ("************************* ********* *************************\n");

    // printf ("************************** UPDATE ***************************\n");

    // check_update();

    // printf ("************************* ********* *************************\n");

    FILE* pFp0 = NULL;
    FILE* pFp1 = NULL;
    FILE* pFp2 = NULL;
#if SAVE_ENC
    // pFp0 = fopen("sdk0.h265", "wb");
    // pFp1 = fopen("sdk1.h265", "wb");
    // pFp2 = fopen("sdk2.h265", "wb");
#endif


////////////////////////// Stream 0
    // APP
    // s32Ret += SDKSetVEncPara(0, SDK_VENC_TYPE_H265, 2592, 1520, 25, SDK_VENC_MODE_VBR, 3072, 50, SDK_VENC_PROFILE_HIGH);
    // unit_test
    // s32Ret += SDKSetVEncPara(0, SDK_VENC_TYPE_H265, 2592, 1520, 25, SDK_VENC_MODE_VBR, 3072, 50, SDK_VENC_PROFILE_HIGH);
    // sleep(1);



////////////////////////// Stream 1
    // OK
    // s32Ret += SDKSetVEncPara(1, SDK_VENC_TYPE_H265, 1280, 720, 25, SDK_VENC_MODE_VBR, 2048, 50, SDK_VENC_PROFILE_HIGH);

    // APP
    // s32Ret += SDKSetVEncPara(1, SDK_VENC_TYPE_H265, 704, 576, 25, SDK_VENC_MODE_VBR, 512, 50, SDK_VENC_PROFILE_HIGH);
    // s32Ret += SDKSetVEncPara(1, SDK_VENC_TYPE_H265, 768, 576, 25, SDK_VENC_MODE_VBR, 512, 50, SDK_VENC_PROFILE_HIGH);
    // sleep(1);

////////////////////////// Stream 2
    // APP
    // s32Ret += SDKSetVEncPara(2, SDK_VENC_TYPE_H265, 704, 576, 25, SDK_VENC_MODE_CBR, 512, 50, SDK_VENC_PROFILE_HIGH);
    // OK
    // s32Ret += SDKSetVEncPara(2, SDK_VENC_TYPE_H265, 1280, 720, 25, SDK_VENC_MODE_VBR, 2048, 50, SDK_VENC_PROFILE_HIGH);

    // s32Ret += SDKSetVEncPara(2, SDK_VENC_TYPE_H265, 768, 576, 25, SDK_VENC_MODE_CBR, 512, 50, SDK_VENC_PROFILE_HIGH);
    // sleep(1);

////////////////////////// Stream 3
    // APP
    // s32Ret += SDKSetVEncPara(4, SDK_VENC_TYPE_MJPEG, 704, 576, 0, SDK_VENC_MODE_BUTT, 0, 0, SDK_VENC_PROFILE_BASELINE);
    // sleep(1);

////////////////////////// Stream 4
    // APP
    // s32Ret += SDKSetVEncPara(3, SDK_VENC_TYPE_MJPEG, 352, 288, 0, SDK_VENC_MODE_BUTT, 0, 0, SDK_VENC_PROFILE_BASELINE);

    if (s32Ret != 0)
    {
        printf("SDKSetVEncPara errrrrr 11\n");
        SDKExit();
        return s32Ret;
    }
#if RUN_AUDIO
    printf("run audio\n");
    s32Ret = SDKSetAudioPara(SDK_AUDIO_RATE_8000, SDK_AUDIO_BIT_WIDTH_16, SDK_AENC_TYPE_G711A, 320);
    if (s32Ret != 0)
    {
        printf("SDKSetAudioPara err\n");
    }
    else
    {
        // g_s32AudioThdFlag = 1;
        // if (pthread_create(&g_s32AudioThread, NULL, (void*)test_audio_service, NULL) != 0)
        // {
        //     printf("pthread_create failed\n");
        // }
    }

#endif

#if RUN_OSD
    printf("run osd\n");
    SDK_POSITION stTimePos = {0};
    SDK_POSITION stCommPos = {0};
    s32Ret += SDKSetTimeOSDPara(1, &stTimePos, year_mon_day);
    if (s32Ret != 0)
    {
        printf("SDKSetTimeOSDPara err\n");
    }
    stCommPos.x = 200;
    stCommPos.y = 200;
    stCommPos.width = 20;
    stCommPos.height = 20;

    char cOsd_data_test[8] = {0};
    s32Ret += SDKSetOSDPara(1, 1, &stCommPos, cOsd_data_test);
    if (s32Ret != 0)
    {
        printf("SDKSetCommOSDPara err\n");
    }
    SDKLoopFunc();
#endif

#if RUN_MASK
    printf("run mask\n");
    SDK_OSD_MASK_CONFIG stMaskCfg[MASK_NUM] = {0};
    for (i = 0; i < MASK_NUM; i++)
    {
        stMaskCfg[i].iShow = 1;
        stMaskCfg[i].rectang.width = 64;
        stMaskCfg[i].rectang.height = 64;
        stMaskCfg[i].rectang.x = i * 64;
        stMaskCfg[i].rectang.y = i * 64;
    }
    s32Ret = SDKSetOsdMask(stMaskCfg, MASK_NUM);
    if (s32Ret != 0)
    {
        printf("SDKSetOsdMask err\n");
    }
#endif


    while (g_s32Run > 0)
    {
        g_s32Run--;
        usleep(1000);
        // usleep(1000 * 10000);
        // __TIC__();
        IPCAMERA_AV_FRAME_INFO stFrame = {0};

        if (pFp0)
        {
            memset(&stFrame, 0, sizeof(stFrame));
            s32Ret = SDKGetVideoFrame(0, &stFrame, NULL, NULL, 0);
            if (s32Ret == 0)
            {
                // printf("0 get size %d %d %d\n", stFrame.length, stFrame.width, stFrame.height);

                fwrite(stFrame.pData, stFrame.length, 1, pFp0);
                fflush(pFp0);

                SDKReleaseFrame(&stFrame);
            }
        }
        if (pFp1)
        {
            memset(&stFrame, 0, sizeof(stFrame));
            s32Ret = SDKGetVideoFrame(1, &stFrame, NULL, NULL, 0);
            if (s32Ret == 0)
            {
                // printf("1 get size %d %d %d\n", stFrame.length, stFrame.width, stFrame.height);


                fwrite(stFrame.pData, stFrame.length, 1, pFp1);
                fflush(pFp1);

                SDKReleaseFrame(&stFrame);
            }
        }
        if (pFp2) {
            memset(&stFrame, 0, sizeof(stFrame));
            s32Ret = SDKGetVideoFrame(2, &stFrame, NULL, NULL, 0);
            if (s32Ret == 0)
            {
                // printf("1 get size %d %d %d\n", stFrame.length, stFrame.width, stFrame.height);


                fwrite(stFrame.pData, stFrame.length, 1, pFp2);
                fflush(pFp2);

                SDKReleaseFrame(&stFrame);
            }
        }
#if RUN_ROI
        if (g_s32Run == 100)
        {
            printf("run roi\n");
            SDK_ROI_CONFIG stROI[ROI_NUM] = {0};
            for (i = 0; i < ROI_NUM; i++)
            {
                stROI[i].iShow = 1;
                stROI[i].level = 10;
                stROI[i].rectang.width = 1000;
                stROI[i].rectang.height = 1000;
                stROI[i].rectang.x = i * 1000;
                stROI[i].rectang.y = i * 1000;
            }
            s32Ret = SDKSetROI(stROI, ROI_NUM);
            if (s32Ret != 0)
            {
                printf("SDKSetROI err\n");
            }
        }
#endif

        // if (g_s32Run == 100)
        // {
        //     IPCAMERA_AV_FRAME_INFO stFrame2 = {0};
        //     FILE* pFp2 = fopen("sdk.jpeg", "wb");
        //     s32Ret = SDKGetJPEG(3, &stFrame2);
        //     if (s32Ret == 0)
        //     {
        //         if (pFp2)
        //         {
        //             fwrite(stFrame2.pData, stFrame2.length, 1, pFp2);
        //             fflush(pFp2);
        //             fclose(pFp2);
        //         }
        //         SDKReleaseFrame(&stFrame2);
        //     }
        //     else
        //     {
        //         printf("get jpeg failed %d\n", s32Ret);
        //     }
        // }

        // __TOC__();
    }

    if (pFp0)
    {
        fclose(pFp0);
    }
    if (pFp1)
    {
        fclose(pFp1);
    }
    if (pFp2)
    {
        fclose(pFp2);
    }
#if RUN_AUDIO
    if (g_s32AudioThdFlag)
    {
        g_s32AudioThdFlag = 0;
        pthread_join(g_s32AudioThread, NULL);
    }
#endif
    printf("exit\n");
    SDKExit();

    return 0;

}