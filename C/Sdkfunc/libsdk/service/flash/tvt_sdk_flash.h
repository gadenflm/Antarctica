#ifndef _TVT_SDK_FLASH_H_
#define _TVT_SDK_FLASH_H_

#include "sdk_define.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

struct erase_info_user {
	uint32_t start;
	uint32_t length;
};

struct mtd_info_user {
	uint8_t type;
	uint32_t flags;
	uint32_t size;	 // Total size of the MTD
	uint32_t erasesize;
	uint32_t oobblock;  // Size of OOB blocks (e.g. 512)
	uint32_t oobsize;   // Amount of OOB data per block (e.g. 16)
	uint32_t ecctype;
	uint32_t eccsize;
};

struct region_info_user {
	uint32_t offset;		/* At which this region starts,
							* from the beginning of the MTD */
	uint32_t erasesize;		/* For this region */
	uint32_t numblocks;		/* Number of blocks in this region */
	uint32_t regionindex;
	uint64_t memaddr;
};

#define MEMGETINFO              _IOR('M', 1, struct mtd_info_user)
#define MEMERASE                _IOW('M', 2, struct erase_info_user)
#define MEMUNLOCK               _IOW('M', 6, struct erase_info_user)
#define MEMGETREGIONCOUNT	    _IOR('M', 7, int)
#define MEMGETREGIONINFO	    _IOWR('M', 8, struct region_info_user)

#define TUSED_FLASH_TYPE_SPI_NOR 1
#define TUSED_FLASH_TYPE_SPI_NAND 2
#define TUSED_FLASH_TYPE_EMMC 3

typedef struct mtd_info_user mtd_info_t;
typedef struct erase_info_user erase_info_t;
typedef struct region_info_user region_info_t;

int tvt_sdk_flash_init(void);
int tvt_sdk_flash_exit(void);
int tvt_sdk_flash_GetFlashParamItem( char* param_value, int param_len, int item_number );
int tvt_sdk_flash_SetFlashParamItem( char* param_value, int param_len, int item_number );

// Saved Param Size
#define CFG_FLASH_APP_PARAM_SIZE 0x10000
// Saved OFFSET_POS
#define ENV_FLASH_APP_PARAM_OFFSET_POS 0x300000
// Backup OFFSET = Size + OFFSET_POS
#define ENV_FLASH_APPBK_PARAM_OFFSETPOS 0x310000

#define MISC_APPLICATION_PARAM_VALUE_LEN 32
#define MISC_SDK_PARAM_BEGIN_ITEM 10
#define MSIC_SDK_PARM_END_ITEM 1536

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////flash_cpm////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void misc_set_boot_param_flash_type( int flash_type );
int misc_get_boot_param_flash_type(void);
void misc_set_system_flash_type( int flash_type );
int misc_get_system_flash_type(void);

int flashcpm_main(char *filebuf, int new_update_method_inbuflen, int filesize,  char *devicename, int device_blk_size, int flash_type );
int flashcpm_logo_image(char *pDataBuf, int iDataLen, int iOffsetSize,  char *devicename, int flash_type);
int flashcpm_set_check_memaddr( char* pCheckMemAddr, int mem_len );

#ifdef __cplusplus
}
#endif

#endif