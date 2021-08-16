#ifndef _TVT_SDK_IMAGE_H_
#define _TVT_SDK_IMAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 控制命令枚举
 */
typedef enum tvt_sdk_image_cmd_e
{
    TVT_SDK_IMAGE_SET_MIRROR,                   ///< param: TVT_SDK_IMAGE_MIRROR_CFG_S
    TVT_SDK_IMAGE_SET_FLIP,                     ///< param: TVT_SDK_IMAGE_FLIP_CFG_S
    TVT_SDK_IMAGE_CMD_BUTT,
} TVT_SDK_IMAGE_CMD_E;

/**
 * @brief 控制方向 set or get
 */
typedef enum tvt_sdk_image_ctrl_dir_e
{
    TVT_SDK_IMG_CTRL_GET,
    TVT_SDK_IMG_CTRL_SET,
    TVT_SDK_IMG_CTRL_DIR_BUTT,
}TVT_SDK_IMAGE_CTRL_DIR_E;

////////////////////// Image ////////////////////////////////
/**
 * @brief 镜像
 */
typedef struct tvt_sdk_image_mirror_cfg_s
{
    unsigned int                s32VinIDX;     ///< 视频输入序号
    int                         s32Enable;     ///< 使能
}TVT_SDK_IMAGE_MIRROR_CFG_S;

/**
 * @brief 翻转
 */
typedef TVT_SDK_IMAGE_MIRROR_CFG_S TVT_SDK_IMAGE_FLIP_CFG_S;

////////////////////////////////////////////////////////

/**
 * @brief 外设业务控制接口
 * @param [in] s32Cmd:命令字
 * @param [in] s32Dir:TVT_SDK_IMAGE_CTRL_DIR_E
 * @param [in] pParam:参数指针
 * @param [in] s32Length:参数长度
 * @return 0：成功；非0：失败
 */
int tvt_sdk_image_ctrl(int s32Cmd, int s32Dir, void* pParam, int s32Length);

/**
 * @brief 初始化视频图像业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_image_init(int s32VinNum, const int* s32VinDevID);

/**
 * @brief 去初始化视频图像业务
 * @return 0：成功；非0：失败
 */
int tvt_sdk_image_exit();


#ifdef __cplusplus
}
#endif

#endif