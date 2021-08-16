#include "sdk_func.h"
#include "tvt_sdk_update.h"
#include "tvt_sdk_flash.h"
#include "tvt_sdk_print.h"


/**
 * @breif  Read Cfg from Flash at specific Address(refer to tvt_sdk_flash.h)
 * @param  param_value The Configure Data need to read
 * @param  param_len   refer to tvt_sdk_flash.h @MISC_APPLICATION_PARAM_VALUE_LEN
 * @param  item_number refer to SDK_define.h @enum _flash_param_area_def_
 * @return  0 Success; others Failed;
 */
int SDKGetFlashParamItem( char* param_value, int param_len, int item_number ) {
    int s32Ret;
    tvt_sdk_flash_init();
    s32Ret = tvt_sdk_flash_GetFlashParamItem (param_value, param_len, item_number);
    return s32Ret;
}

/**
 * @breif  Write Cfg to Flash at specific Address(refer to tvt_sdk_flash.h)
 * @param  param_value The Writed Configure Data  "KEY-VALUE"
 * @param  param_len   refer to tvt_sdk_flash.h @MISC_APPLICATION_PARAM_VALUE_LEN
 * @param  item_number refer to SDK_define.h @enum _flash_param_area_def_
 * @return  0 Success; others Failed;
 */
int SDKSetFlashParamItem( char* param_value, int param_len, int item_number ) {
    int s32Ret;
    tvt_sdk_flash_init();
    s32Ret = tvt_sdk_flash_SetFlashParamItem (param_value, param_len, item_number);
    return s32Ret;
}

/**
 * @breif  Update the binary data to Flash
 * @param  pUpdateMem  Address of binary data
 * @param  iUpdateSize The data size (unit: bytes)
 * @param  iUpdateType refer to tvt_sdk_update.h @Firmware_Update_Type
 * @return  0 Success; others Failed;
 */
int SDKUpdateFlash(char *pUpdateMem, unsigned int iUpdateSize, unsigned int iUpdateType) {
    int s32Ret = 0;
    s32Ret = tvt_sdk_update_SDKUpdateFlash (pUpdateMem, iUpdateSize, iUpdateType);
    return s32Ret;
}

/**
 * @return  The sdk update flag
 */
int GetSdkUpdateFlag(void) {
    return tvt_sdk_GetSdkUpdateFlag();
}

/**
 * Get the Address of Memory used for storing update binary data.
 * @return  Address of Memory
 */
char *SDKGetUpdateMem(void) {
    return tvt_sdk_SDKGetUpdateMem();
}