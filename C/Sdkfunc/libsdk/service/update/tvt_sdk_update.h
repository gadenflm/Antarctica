#ifndef _TVT_SDK_UPDATE_H_
#define _TVT_SDK_UPDATE_H_

#include "sdk_define.h"
#include <pthread.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BST_EMMC_BLOCK 	 		"/dev/mmcblk0p1"
#define KERNEL_EMMC_BLOCK 	 		"/dev/mmcblk0p5"
#define FILESYS_EMMC_BLOCK	 		"/dev/mmcblk0p6"
#define APPFS_EMMC_BLOCK            "/dev/mmcblk0p7"

#define DEBUG_FLAG_FILE_NAME "/mnt/cfg/ext/debug.dat"


typedef struct tvt_sdk_update_info_s
{
	char u8Init;
	// int up_used_new;
	int used_exit_type_val;

	// int uboot_size;
	// int uboot_no_param_size;
	int kernel_size;
	int filesys_size;
	int appfs_size;
	int min_firmware_size;
	int max_firmware_size;

	int update_mem_type;
	int update_mem_size;

	// int fl_type_uboot;
	// char dev_uboot_name[32];
	int fl_type_kernel;
	char dev_kernel_name[32];
	int fl_type_filesys;
	char dev_filesys_name[32];
	int fl_type_appfs;
	char dev_appfs_name[32];
	// int fl_type_algo;
	// char dev_algo_name[32];

	pthread_mutex_t                      s32Mutex;                          ///< 业务锁
	char                                 szMutexName[32];                   ///< 业务锁名称
	unsigned int                         u32MutexLockCnt;                   ///< 业务锁锁定次数
} TVT_SDK_UPDATE_INFO_S;

typedef enum __Firmware_Update_Type
{
	FIRMWARE_TYPE_UBOOT  = 0x1,
	FIRMWARE_TYPE_KERNEL = (0x1 << 1),
	FIRMWARE_TYPE_FILESYSTEM = (0x1 << 2),
	FIRMWARE_TYPE_APPFS = (0x1 << 3),
} Firmware_Update_Type;

int tvt_sdk_update_init(void);
int tvt_sdk_update_exit(void);
int tvt_sdk_update_SDKUpdateFlash(char *pUpdateMem, unsigned int iUpdateSize, unsigned int iUpdateType);
int tvt_sdk_update_SdkCommUpdateFlash(char *pUpdateMem, unsigned int iUpdateSize, unsigned int iUpdateType);
int tvt_sdk_GetSdkUpdateFlag(void);
char * tvt_sdk_SDKGetUpdateMem(void);

#ifdef __cplusplus
}
#endif

#endif