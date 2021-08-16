#ifndef _TVT_SDK_IMAGE_H_
#define _TVT_SDK_IMAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ��������ö��
 */
typedef enum tvt_sdk_image_cmd_e
{
    TVT_SDK_IMAGE_SET_MIRROR,                   ///< param: TVT_SDK_IMAGE_MIRROR_CFG_S
    TVT_SDK_IMAGE_SET_FLIP,                     ///< param: TVT_SDK_IMAGE_FLIP_CFG_S
    TVT_SDK_IMAGE_CMD_BUTT,
} TVT_SDK_IMAGE_CMD_E;

/**
 * @brief ���Ʒ��� set or get
 */
typedef enum tvt_sdk_image_ctrl_dir_e
{
    TVT_SDK_IMG_CTRL_GET,
    TVT_SDK_IMG_CTRL_SET,
    TVT_SDK_IMG_CTRL_DIR_BUTT,
}TVT_SDK_IMAGE_CTRL_DIR_E;

////////////////////// Image ////////////////////////////////
/**
 * @brief ����
 */
typedef struct tvt_sdk_image_mirror_cfg_s
{
    unsigned int                s32VinIDX;     ///< ��Ƶ�������
    int                         s32Enable;     ///< ʹ��
}TVT_SDK_IMAGE_MIRROR_CFG_S;

/**
 * @brief ��ת
 */
typedef TVT_SDK_IMAGE_MIRROR_CFG_S TVT_SDK_IMAGE_FLIP_CFG_S;

////////////////////////////////////////////////////////

/**
 * @brief ����ҵ����ƽӿ�
 * @param [in] s32Cmd:������
 * @param [in] s32Dir:TVT_SDK_IMAGE_CTRL_DIR_E
 * @param [in] pParam:����ָ��
 * @param [in] s32Length:��������
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_image_ctrl(int s32Cmd, int s32Dir, void* pParam, int s32Length);

/**
 * @brief ��ʼ����Ƶͼ��ҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_image_init(int s32VinNum, const int* s32VinDevID);

/**
 * @brief ȥ��ʼ����Ƶͼ��ҵ��
 * @return 0���ɹ�����0��ʧ��
 */
int tvt_sdk_image_exit();


#ifdef __cplusplus
}
#endif

#endif