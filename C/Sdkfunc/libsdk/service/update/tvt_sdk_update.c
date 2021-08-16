#include "tvt_sdk_update.h"
#include "tvt_sdk_flash.h"
#include "sdk_func.h"
#include "tvt_sdk_print.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/ioctl.h>

static TVT_SDK_UPDATE_INFO_S* g_pstSdkUpdateInfo = NULL;

// #define UP_FLASH_UBOOT_SIZE    (g_pstSdkUpdateInfo->uboot_size)
// #define UP_FLASH_UBOOT_NO_PARAM_SIZE  (g_pstSdkUpdateInfo->uboot_no_param_size)
#define UP_FLASH_KERNEL_SIZE   (g_pstSdkUpdateInfo->kernel_size)
#define UP_FLASH_FILESYS_SIZE  (g_pstSdkUpdateInfo->filesys_size)
#define UP_FLASH_APPFS_SIZE    (g_pstSdkUpdateInfo->appfs_size)
#define UP_FLASH_MIN_FIRMWARE_SIZE (g_pstSdkUpdateInfo->min_firmware_size)
#define UP_FLASH_MAX_FIRMWARE_SIZE (g_pstSdkUpdateInfo->max_firmware_size)

#define UP_FLASH_UPDATE_MEM_SIZE (g_pstSdkUpdateInfo->update_mem_size)
#define UP_FLASH_UPDATE_MEM_TYPE (g_pstSdkUpdateInfo->update_mem_type)

// #define UP_FLASH_DEV_UBOOT_NAME (g_pstSdkUpdateInfo->dev_uboot_name)
#define UP_FLASH_DEV_KENREL_NAME (g_pstSdkUpdateInfo->dev_kernel_name)
#define UP_FLASH_DEV_FILESYS_NAME (g_pstSdkUpdateInfo->dev_filesys_name)
#define UP_FLASH_DEV_APPFS_NAME (g_pstSdkUpdateInfo->dev_appfs_name)
// #define UP_FLASH_DEV_ALGO_NAME (g_pstSdkUpdateInfo->dev_algo_name)

// #define UP_FLASH_FL_UBOOT_TYPE (g_pstSdkUpdateInfo->fl_type_uboot)
#define UP_FLASH_FL_KENREL_TYPE (g_pstSdkUpdateInfo->fl_type_kernel)
#define UP_FLASH_FL_FILESYS_TYPE (g_pstSdkUpdateInfo->fl_type_filesys)
#define UP_FLASH_FL_APPFS_TYPE (g_pstSdkUpdateInfo->fl_type_appfs)
// #define UP_FLASH_FL_ALGO_TYPE (g_pstSdkUpdateInfo->fl_type_algo)

// #define UP_FLASH_USED_NEW_FUNC (g_pstSdkUpdateInfo->up_used_new)
#define UP_FLASH_USED_EXIT_TYPE (g_pstSdkUpdateInfo->used_exit_type_val)

#define ERR_EXIT {\
					SDKFreeMMZ();\
					return -1;\
			    }

char *pVirAddr = NULL;
uint64_t PhyAddr = 0;
int update_flag = 0;
int mem_alloc_from_mmz = 0;

void SetSdkUpdateFlag(int flag)
{
	update_flag = flag;
}

int tvt_sdk_GetSdkUpdateFlag(void)
{
	return update_flag;
}

void SDKFreeMMZ(void)
{
	int flash_type = misc_get_system_flash_type();
	if (!pVirAddr)
	{
		TIPC_PRINTF_INFO("pVirAddr is null, return");
		return;
	}

	if (( TUSED_FLASH_TYPE_EMMC == flash_type ) || (1 == mem_alloc_from_mmz))
	{
		// TODO
		// HI_MPI_SYS_MmzFree(PhyAddr, pVirAddr);
	}
	else
	{
		free( pVirAddr );
	}

	pVirAddr = NULL;
}



void SDKSetXzErrorStop(int val)
{
	int fd = open("/dev/xzerrorctrl", O_RDWR);
	if ( fd > 0 )
	{
		ioctl( fd, 1, val );
		close( fd );
		fd = -1;
	}

	return;
}

int SDKDebugFlagCtrl( char *pDebugMem )
{
	char debug_use_flag[17] = "aa55aa55aa55aa55";
	char debug_no_flag[17] = "aaaa5555aaaa5555";
	char debug_info[17] = "debug";
	char debug_prod_type[7] = "a5a5";
	FILE* fd = NULL;

	if ( 0 == memcmp( pDebugMem, debug_use_flag, 16 ) )
	{
		fd = fopen( DEBUG_FLAG_FILE_NAME, "w+b" );
		if ( fd )
		{
			TIPC_PRINTF_CHK( "write debug info" );
			fwrite( debug_info, 1, 5, fd );
			fclose( fd );
		}
	}
	else if ( 0 == memcmp( pDebugMem, debug_no_flag, 16 ) )
	{
		fd = fopen( DEBUG_FLAG_FILE_NAME, "w+b" );
		if ( fd )
		{
			TIPC_PRINTF_CHK( "clean debug info" );
			fclose( fd );
		}
	}
	else if ( 0 == memcmp( pDebugMem, debug_prod_type, 4 ) )
	{
		char prod_inf[32 * 11];

		memset( prod_inf, 0, 32 * 11 );
		memcpy( prod_inf + 32 * (IPC_FLASH_PARAM_PRODUCT_TYPE - IPC_FLASH_PARAM_CUSTOM_ID), &pDebugMem[4], 12 );
		memcpy( prod_inf + 32 * (IPC_FALSE_PARAM_LENS_TYPE - IPC_FLASH_PARAM_CUSTOM_ID), &pDebugMem[16], 8 );
		memcpy( prod_inf + 32 * (IPC_FLASH_PARAM_CUSTOM_ID - IPC_FLASH_PARAM_CUSTOM_ID), &pDebugMem[24], 8 );
		SDKSetFlashParamItem( prod_inf, 32 * 11, IPC_FLASH_PARAM_CUSTOM_ID );

		//memset( prod_inf, 0, 128 );
		//memcpy( prod_inf, pDebugMem, 17 );
		//printf( "set flash param %s\n", prod_inf );
	}
	return 0;
}

static int SdkCommUpdateKernel(char *pUpdateMem, unsigned int iUpdateSize)
{
	int ret = -1;
	int flash_type;

	flash_type = UP_FLASH_FL_KENREL_TYPE;
	if ( iUpdateSize > UP_FLASH_KERNEL_SIZE)
	{
		TIPC_PRINTF_ERR("kernel size[=%dM] is bigger kernel[=%dM], return",
		                iUpdateSize >> 20,  UP_FLASH_KERNEL_SIZE >> 20);
		return -1;
	}
	TIPC_PRINTF_CHK( "flashcpm kernel %d,%s", iUpdateSize, UP_FLASH_DEV_KENREL_NAME );
	ret = flashcpm_main(pUpdateMem, UP_FLASH_UPDATE_MEM_SIZE, iUpdateSize, UP_FLASH_DEV_KENREL_NAME,
	                    UP_FLASH_KERNEL_SIZE, flash_type);

	if (ret != 0)
	{
		TIPC_PRINTF_ERR("flash kernel fail");
		return -1;
	}

	return 0;
}

static int SdkCommUpdateAppfs(char *pUpdateMem, unsigned int iUpdateSize)
{
	int ret = -1;
	int flash_type;

	flash_type = UP_FLASH_FL_APPFS_TYPE;
	if ( iUpdateSize > UP_FLASH_APPFS_SIZE)
	{
		TIPC_PRINTF_ERR("appfs size[=%dM] is bigger appfs[=%dM], return",
		                iUpdateSize >> 20,  UP_FLASH_APPFS_SIZE >> 20);
		return -1;
	}
	TIPC_PRINTF_CHK( "flashcpm EMMC appfs %d,%s,%d", iUpdateSize, UP_FLASH_DEV_APPFS_NAME, flash_type);
	ret = flashcpm_main(pUpdateMem, UP_FLASH_UPDATE_MEM_SIZE, iUpdateSize, UP_FLASH_DEV_APPFS_NAME,
	                    UP_FLASH_APPFS_SIZE, flash_type);

	if (ret != 0)
	{
		TIPC_PRINTF_ERR("flash appfs fail");
		return -1;
	}

	return 0;
}

static int SdkCommUpdateFilesystem(char *pUpdateMem, unsigned int iUpdateSize)
{
	int ret = -1;
	int flash_type;

	flash_type = UP_FLASH_FL_FILESYS_TYPE;
	if ( iUpdateSize > UP_FLASH_FILESYS_SIZE)
	{
		TIPC_PRINTF_ERR("filesys size[=%dM] is bigger filesys[=%dM], return",
		                iUpdateSize >> 20,  UP_FLASH_FILESYS_SIZE >> 20);
		return -1;
	}
	TIPC_PRINTF_CHK( "flashcpm filesystem %d,%s", iUpdateSize, UP_FLASH_DEV_FILESYS_NAME);
	ret = flashcpm_main(pUpdateMem, UP_FLASH_UPDATE_MEM_SIZE, iUpdateSize, UP_FLASH_DEV_FILESYS_NAME,
	                    UP_FLASH_FILESYS_SIZE, flash_type);

	if (ret != 0)
	{
		TIPC_PRINTF_ERR("flash filesys fail");
		return -1;
	}

	return 0;
}


int SdkCommWriteDataToFlash(char *pUpdateMem, unsigned int iUpdateSize, unsigned int iUpdateType)
{
	if (FIRMWARE_TYPE_UBOOT == iUpdateType)
	{
		///TODO
	}

	if (FIRMWARE_TYPE_KERNEL == iUpdateType)
	{
		if (SdkCommUpdateKernel(pUpdateMem, iUpdateSize ) != 0)
		{
			TIPC_PRINTF_ERR("flash kernel fail");
			ERR_EXIT
		}
	}

	if (FIRMWARE_TYPE_FILESYSTEM == iUpdateType)
	{
		if (SdkCommUpdateFilesystem(pUpdateMem, iUpdateSize ) != 0)
		{
			TIPC_PRINTF_ERR("flash Filesystem fail");
			ERR_EXIT
		}
	}

	if (FIRMWARE_TYPE_APPFS == iUpdateType)
	{
		// if (IPC_PROD_HISI_A3_800M_9X8X_CA == gp_prod_type_val)
		// {
		// 	if (SdkCommUpdateAppfsWithBackup(pUpdateMem, iUpdateSize ) != 0)
		// 	{
		// 		TIPC_PRINTF_ERR("flash appfs fail");
		// 		ERR_EXIT
		// 	}
		// }
		// else
		// {
		if (SdkCommUpdateAppfs(pUpdateMem, iUpdateSize ) != 0)
		{
			TIPC_PRINTF_ERR("flash appfs fail");
			ERR_EXIT
		}
		// }
	}

	if (iUpdateType == (FIRMWARE_TYPE_FILESYSTEM | FIRMWARE_TYPE_KERNEL | FIRMWARE_TYPE_UBOOT | FIRMWARE_TYPE_APPFS))
	{
		if (UP_FLASH_MAX_FIRMWARE_SIZE != iUpdateSize)
		{
			TIPC_PRINTF_ERR("Total flash size[=%dM] is not Max Size, return", iUpdateSize >> 20);
			return -1;
		}

		int block_pos = 0x1300000; //Kernel Start Pos
		int block_size = 0;

		if (FIRMWARE_TYPE_UBOOT & iUpdateType) {
			///TODO
		}

		if (FIRMWARE_TYPE_KERNEL & iUpdateType)
		{
			block_pos += 0;
			block_size = UP_FLASH_KERNEL_SIZE;
			if (SdkCommUpdateKernel(pUpdateMem + block_pos,  block_size ) != 0)
			{
				TIPC_PRINTF_ERR("flash kernel fail");
				ERR_EXIT
			}
		}

		if (FIRMWARE_TYPE_APPFS & iUpdateType)
		{
			block_pos += UP_FLASH_KERNEL_SIZE + UP_FLASH_FILESYS_SIZE;
			block_size = UP_FLASH_APPFS_SIZE;
			// if (IPC_PROD_HISI_A3_800M_9X8X_CA == gp_prod_type_val)
			// {
			// 	if (SdkCommUpdateAppfsWithBackup(pUpdateMem + block_pos,  block_size ) != 0)
			// 	{
			// 		TIPC_PRINTF_ERR("flash appfs fail");
			// 		ERR_EXIT
			// 	}
			// }
			// else
			// {
			if (SdkCommUpdateAppfs(pUpdateMem + block_pos,  block_size ) != 0)
			{
				TIPC_PRINTF_ERR("flash Appfs fail");
				ERR_EXIT
			}
			// }
		}

		if (FIRMWARE_TYPE_FILESYSTEM & iUpdateType)
		{
			block_pos += UP_FLASH_KERNEL_SIZE;
			block_size = UP_FLASH_FILESYS_SIZE;
			SDKSetXzErrorStop(3);
			if (SdkCommUpdateFilesystem(pUpdateMem + block_pos,  block_size ) != 0)
			{
				TIPC_PRINTF_ERR("flash Filesystem fail");
				ERR_EXIT
			}
		}

	}

// SDKFreeMMZ();

	return 0;
}

int tvt_sdk_update_SdkCommUpdateFlash(char *pUpdateMem, unsigned int iUpdateSize, unsigned int iUpdateType)
{
	int ret = 0;
	int bksize = iUpdateSize;
	// char flash_mask = 0;
	char debug_head_flag[17] = "5a5a5a5a_aaaaaaa";

	if (!pUpdateMem)
	{
		TIPC_PRINTF_ERR("pUpdateMem is NULL, return");
		return -1;
	}

	TIPC_PRINTF_CHK("update size %d", iUpdateSize );
	if ( 32 == iUpdateSize  )
	{
		if (0 == memcmp( debug_head_flag, pUpdateMem, 16 ))
		{
			TIPC_PRINTF_CHK( "SDKDebugFlagCtrl info" );
			return SDKDebugFlagCtrl( pUpdateMem + 16 );
		}
		else
		{
			TIPC_PRINTF_CHK( "SDKDebugFlagCtrlx info" );
			return SDKDebugFlagCtrl( pUpdateMem );
		}
	}

	// TODO : encryption
	// iUpdateSize = encry_file_cmd( 0, pUpdateMem, iUpdateSize, 0, &flash_mask, NULL, 0 );
	// if ( -1 == iUpdateSize )
	// {
	// 	TIPC_PRINTF_ERR("deencry file error, return");
	// 	return -1;
	// }
	// iUpdateType = flash_mask;

	TIPC_PRINTF_CHK( "update src len = %d, deenc len = %d, mask %x", bksize, iUpdateSize, iUpdateType );

	if ((iUpdateSize < UP_FLASH_MIN_FIRMWARE_SIZE) || (iUpdateSize > UP_FLASH_MAX_FIRMWARE_SIZE))
	{
		TIPC_PRINTF_ERR("iUpdateSize EMMC=%#x [min=%#x max=%#x], return",
		                iUpdateSize, UP_FLASH_MIN_FIRMWARE_SIZE, UP_FLASH_MAX_FIRMWARE_SIZE);
		ERR_EXIT
	}

	TIPC_PRINTF_CHK( "[****** WKL Debug ******] memset size:%d", (UP_FLASH_UPDATE_MEM_SIZE - iUpdateSize) );
	memset( pUpdateMem + iUpdateSize, 0xff, (UP_FLASH_UPDATE_MEM_SIZE - iUpdateSize) );

	SDKSetXzErrorStop(1);

	TIPC_PRINTF_CHK( "[****** WKL Debug ******] Before Write To" );

	ret = SdkCommWriteDataToFlash( pUpdateMem, iUpdateSize, iUpdateType );

	return ret;
}


int tvt_sdk_update_SDKUpdateFlash(char *pUpdateMem, unsigned int iUpdateSize, unsigned int iUpdateType)
{
	int s32Ret = -1;
	//lock:
	TIPC_MUTEX_LOCK(&g_pstSdkUpdateInfo->s32Mutex, g_pstSdkUpdateInfo->szMutexName,
	                &g_pstSdkUpdateInfo->u32MutexLockCnt, 0);
	// Set:
	s32Ret = tvt_sdk_update_SdkCommUpdateFlash( pUpdateMem, iUpdateSize, iUpdateType );
	// unlock:
	TIPC_MUTEX_UNLOCK(&g_pstSdkUpdateInfo->s32Mutex, g_pstSdkUpdateInfo->szMutexName,
	                  &g_pstSdkUpdateInfo->u32MutexLockCnt, 0);
	return s32Ret ;
}

char * tvt_sdk_SDKGetUpdateMem(void) {
	// int s32Ret = 0;
	int mem_size = UP_FLASH_UPDATE_MEM_SIZE;
	int mem_type = UP_FLASH_UPDATE_MEM_TYPE;
	// int exit_type = UP_FLASH_USED_EXIT_TYPE;

	SetSdkUpdateFlag(1);

	if (!pVirAddr)
	{
		if (1 == mem_type)
		{
			// TODO
		}
		else
		{
			// TODO
			pVirAddr = (char*)malloc( mem_size );
		}
	}

	if (!pVirAddr)
	{
		printf("pVirAddr is null, malloc failed %d\n", mem_size);
		return NULL;
	}

	memset( pVirAddr, 0, mem_size );
	TIPC_PRINTF_INFO("end get mem");

	return (char *)pVirAddr;
}

///////////////////////////////////////////////////////////////////////////////////////

int tvt_sdk_update_init(void)
{
	if (g_pstSdkUpdateInfo != NULL)
	{
		if (g_pstSdkUpdateInfo->u8Init != 0)
		{
			// has been inited:
			TIPC_PRINTF_ERR("update service is running");
			return -1;
		}
	}
	else
	{
		// create the memory of info struct
		g_pstSdkUpdateInfo = (TVT_SDK_UPDATE_INFO_S* )malloc(sizeof(TVT_SDK_UPDATE_INFO_S));
		if (NULL == g_pstSdkUpdateInfo)
		{
			TIPC_PRINTF_ERR("flash service malloc failed");
			return -1;
		}
		memset(g_pstSdkUpdateInfo, 0, sizeof(TVT_SDK_UPDATE_INFO_S));
	}

	// Default Init The Parameter with EMMC:
	// The Constant Value is given by guess without check
	//
	g_pstSdkUpdateInfo->kernel_size = (15 * 1024 * 1024);
	g_pstSdkUpdateInfo->filesys_size = (4 * 1024 * 1024);
	g_pstSdkUpdateInfo->appfs_size = (48 * 1024 * 1024);
	g_pstSdkUpdateInfo->min_firmware_size = (100 * 1024);
	g_pstSdkUpdateInfo->update_mem_size = ((48 * 1024 + 1) * 1024);
	g_pstSdkUpdateInfo->max_firmware_size = ((48 * 1024 + 1) * 1024);

	g_pstSdkUpdateInfo->fl_type_kernel = TUSED_FLASH_TYPE_EMMC;
	sprintf( g_pstSdkUpdateInfo->dev_kernel_name, "/dev/mmcblk0p5" );

	g_pstSdkUpdateInfo->fl_type_filesys = TUSED_FLASH_TYPE_EMMC;
	sprintf( g_pstSdkUpdateInfo->dev_filesys_name, "/dev/mmcblk0p6" );

	g_pstSdkUpdateInfo->fl_type_appfs = TUSED_FLASH_TYPE_EMMC;
	sprintf( g_pstSdkUpdateInfo->dev_appfs_name, "/dev/mmcblk0p7" );

	g_pstSdkUpdateInfo->used_exit_type_val = 0;
	g_pstSdkUpdateInfo->update_mem_type = 0;

	// init the mutex:
	pthread_mutex_init(&g_pstSdkUpdateInfo->s32Mutex, NULL);
	sprintf(g_pstSdkUpdateInfo->szMutexName, "tvt_sdk_update_lock");

	g_pstSdkUpdateInfo->u8Init = 1;
	TIPC_PRINTF_INFO ("[****** WKL Debug ******] The update has been inited");
	return 0;
}

int tvt_sdk_update_exit(void)
{
	if (NULL == g_pstSdkUpdateInfo)
	{
		TIPC_PRINTF_ERR("update service is not running");
		return -1;
	}

	if (g_pstSdkUpdateInfo->u8Init != 1)
	{
		TIPC_PRINTF_ERR("update service is not running");
		free((void*)g_pstSdkUpdateInfo);
		g_pstSdkUpdateInfo = NULL;
		return -1;
	}

	pthread_mutex_destroy(&g_pstSdkUpdateInfo->s32Mutex);

	free((void*)g_pstSdkUpdateInfo);
	g_pstSdkUpdateInfo = NULL;

	return 0;
}