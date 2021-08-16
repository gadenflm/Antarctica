#include "sdk_func.h"
#include "tvt_sdk_flash.h"
#include "tvt_sdk_print.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/resource.h>

// not used:
/*#define misc_isxdigit(c)	(('0' <= (c) && (c) <= '9') \
			 || ('a' <= (c) && (c) <= 'f') \
			 || ('A' <= (c) && (c) <= 'F'))

#define misc_isdigit(c)	('0' <= (c) && (c) <= '9')
#define misc_islower(c)	('a' <= (c) && (c) <= 'z')
#define misc_toupper(c)	(misc_islower(c) ? ((c) - 'a' + 'A') : (c))
*/

typedef struct tvt_sdk_flash_info_s {
	char u8Init;
	int boot_param_flash_type;
	int system_flash_type;

	pthread_mutex_t                      s32Mutex;                          ///< 业务锁
	char                                 szMutexName[32];                   ///< 业务锁名称
	unsigned int                         u32MutexLockCnt;                   ///< 业务锁锁定次数
} TVT_SDK_FLASH_INFO_S;

static TVT_SDK_FLASH_INFO_S* g_pstSdkFlashInfo = NULL;

int g_misc_check_flash_param_value_flag = 0;
int g_misc_checked_flash_type = 0;

void misc_set_boot_param_flash_type( int flash_type )
{
	g_pstSdkFlashInfo->boot_param_flash_type = flash_type;
}

int misc_get_boot_param_flash_type(void)
{
	return g_pstSdkFlashInfo->boot_param_flash_type;
}

void misc_set_system_flash_type( int flash_type )
{
	g_pstSdkFlashInfo->system_flash_type = flash_type;
}

int misc_get_system_flash_type(void)
{
	return g_pstSdkFlashInfo->system_flash_type;
}

char* misc_get_mtd_name()
{
	char* emmc_name = "/dev/mmcblk0";
	// SPI TODO:
	// char* spi_name = "/dev/mtd0";

	if ( TUSED_FLASH_TYPE_EMMC == misc_get_boot_param_flash_type() )
	{
		return emmc_name;
	}
	else
	{
		return "";
	}
}

int misc_flash_open_data( char* flash_buf, int flash_len, int flash_offset, char* fl_dev_name )
{
	int fd;
	int tmp;

	fd = open( fl_dev_name, O_RDWR ); //打开flash的第一个分区
	if (-1 == fd)
	{
		TIPC_PRINTF_ERR( " open device %s error", fl_dev_name);
		return -1;
	}

	tmp = lseek(fd, flash_offset, SEEK_SET);
	if ( tmp < 0 )
	{
		close(fd);
		TIPC_PRINTF_ERR( " lseek device %d error", flash_offset);
		return -1;
	}

	tmp = read( fd, flash_buf, flash_len );   //读取环境变量的内容
	if ( tmp != flash_len )
	{
		perror(NULL);
		close(fd);
		TIPC_PRINTF_ERR( " read device %d error", CFG_FLASH_APP_PARAM_SIZE );
		return -1;
	}
	//关闭
	close(fd);
	fd = -1;
	return 0;
}

int misc_flash_repeat_write( char* flash_buf, int flash_len, int flash_offset, char* fl_dev_name )
{
	int fd = -1;
	// used for SPI:
	// mtd_info_t meminfo;
	// erase_info_t erase;
	int tmp;
	int rp_cnt = 0;

	fd = open( fl_dev_name, O_RDWR ); //打开flash的第一个分区
	if (-1 == fd)
	{
		// perror(NULL);
		TIPC_PRINTF_ERR( " open device %s error", fl_dev_name);
		return -1;
	}

	tmp = lseek(fd, flash_offset, SEEK_SET);
	if ( tmp < 0 )
	{
		close(fd);
		// perror(NULL);
		TIPC_PRINTF_ERR(" lseek device SEEK_SET %d error", flash_offset);
		return -1;
	}

	// if ( TUSED_FLASH_TYPE_EMMC == misc_get_boot_param_flash_type() )
	// {
	// 	//todo ???
	// }
	// else
	// {
	// 	///TODO:
	// }

	rp_cnt = 8;
	//rok = -1;
	while ( rp_cnt-- > 0 )
	{
		// erase.start = flash_offset;
		// erase.length = flash_len;

		// if ( TUSED_FLASH_TYPE_EMMC == misc_get_boot_param_flash_type() )
		// {
		// 	// todo ???
		// }
		// else
		// {
		// 	///TODO:
		// }

		tmp = lseek(fd, flash_offset, SEEK_SET);
		if ( tmp < 0 )
		{
			// perror(NULL);
			TIPC_PRINTF_ERR(" lseek device SEEK_SET %d error", flash_offset);
			usleep(1000);
			continue;
		}

		tmp = write(fd,  flash_buf, flash_len );
		if ( tmp != flash_len )
		{
			// perror(NULL);
			TIPC_PRINTF_ERR("error number = %d", rp_cnt );
			usleep(1000);
			continue;
		}
		//rok = 1;
		break;
	}

	close(fd);
	return 0;
}

int misc_flash_application_param( int param_number, int is_get_flag, char* param_value, int param_len, int ver_type) {
	char *szBuff = NULL;
	int len;
	int offset_flash_pos = 0;
	// int tmp_rtu = 0;

	TIPC_PRINTF_CHK( "------ctrl= %d------current param type == %d", is_get_flag, param_number );

	// check the param
	if ( 0 == ver_type ) {
		if ( param_number >= MSIC_SDK_PARM_END_ITEM || param_number < MISC_SDK_PARAM_BEGIN_ITEM) {
			TIPC_PRINTF_ERR( "param number too big %d error ", param_number);
			return -1;
		}
		if ( param_len & (MISC_APPLICATION_PARAM_VALUE_LEN - 1) ) {
			TIPC_PRINTF_ERR( "param number too len %d error ", param_len);
			return -1;
		}
		offset_flash_pos = MISC_APPLICATION_PARAM_VALUE_LEN * param_number;
	} else {
		return -1;
	}

	szBuff = (char*)malloc( CFG_FLASH_APP_PARAM_SIZE + 1024 );
	if ( !szBuff ) {
		TIPC_PRINTF_ERR( "alloc flash env buffer error");
		return -1;
	}
	memset( szBuff, 0, CFG_FLASH_APP_PARAM_SIZE + 1024 );

	len = misc_flash_open_data( szBuff, CFG_FLASH_APP_PARAM_SIZE, ENV_FLASH_APP_PARAM_OFFSET_POS, misc_get_mtd_name());

	if ( len != 0 )
	{
		free( szBuff );
		TIPC_PRINTF_ERR( "flash read data  %d error", CFG_FLASH_APP_PARAM_SIZE);
		return -1;
	}

	// encryption ///TODO:
	// tmp_rtu = xdecry_flash_param( szBuff, CFG_FLASH_APP_PARAM_SIZE );
	// if ( 0 != tmp_rtu )
	// {
	// 	free( szBuff );
	// 	printf( "dencry flash param error %d\n", tmp_rtu );
	// 	return -1;
	// }

	if ( 0 == is_get_flag )	{
		// 设置头，尾标志
		memset( szBuff, 0xa5, MISC_APPLICATION_PARAM_VALUE_LEN );
		memset( szBuff + CFG_FLASH_APP_PARAM_SIZE - MISC_APPLICATION_PARAM_VALUE_LEN, 0x5a, MISC_APPLICATION_PARAM_VALUE_LEN );

		// 拷贝数据
		memcpy( szBuff + offset_flash_pos, param_value, param_len );

		// encryption ///TODO:
		// tmp_rtu = xdecry_flash_param( szBuff, CFG_FLASH_APP_PARAM_SIZE );
		// if ( 0 != tmp_rtu )
		// {
		// 	free( szBuff );
		// 	printf( "dencry flash param error %d\n", tmp_rtu );
		// 	return -1;
		// }

		len = misc_flash_repeat_write( szBuff, CFG_FLASH_APP_PARAM_SIZE, ENV_FLASH_APP_PARAM_OFFSET_POS, misc_get_mtd_name() );
		if ( len != 0 ) {
			free( szBuff );
			TIPC_PRINTF_ERR(" flash write env error");
			return -1;
		}

		len = misc_flash_repeat_write( szBuff, CFG_FLASH_APP_PARAM_SIZE, ENV_FLASH_APPBK_PARAM_OFFSETPOS, misc_get_mtd_name() );
		if ( len != 0 ) {
			free( szBuff );
			TIPC_PRINTF_ERR(" flash write env backup error");
			return -1;
		}
	} else {
		memcpy( param_value, szBuff + offset_flash_pos, param_len );
	}
	free( szBuff );
	return 0;
}

int misc_check_flash_app_param_chg( void ) {
	char *szBuff = NULL;
	char *szBkBuf = NULL;
	char *pTailFlag = NULL;
	char *pHeadFlag = NULL;
	//int tmp;
	int len;
	int ret;

	if ( g_misc_check_flash_param_value_flag ) {
		return 0;
	}

	szBuff = (char*)malloc( CFG_FLASH_APP_PARAM_SIZE * 2 );
	if ( !szBuff ) {
		TIPC_PRINTF_ERR("alloc check flash env param buffer error");
		return -1;
	}
	memset( szBuff, 0, CFG_FLASH_APP_PARAM_SIZE * 2 );
	szBkBuf = szBuff + CFG_FLASH_APP_PARAM_SIZE;

	len = misc_flash_open_data( szBuff, CFG_FLASH_APP_PARAM_SIZE, ENV_FLASH_APP_PARAM_OFFSET_POS, misc_get_mtd_name() );
	if ( len != 0 ) {
		free( szBuff );
		TIPC_PRINTF_ERR(" flash read begin env param %x error", ENV_FLASH_APP_PARAM_OFFSET_POS);
		return -1;
	}

	len = misc_flash_open_data( szBkBuf, CFG_FLASH_APP_PARAM_SIZE, ENV_FLASH_APPBK_PARAM_OFFSETPOS, misc_get_mtd_name() );
	if ( len != 0 ) {
		free( szBuff );
		TIPC_PRINTF_ERR(" flash read begin env param %x error", ENV_FLASH_APP_PARAM_OFFSET_POS);
		return -1;
	}

	ret = memcmp( szBuff, szBkBuf, CFG_FLASH_APP_PARAM_SIZE );
	if ( 0 != ret ) {
		char head_flag[MISC_APPLICATION_PARAM_VALUE_LEN];
		char tail_flag[MISC_APPLICATION_PARAM_VALUE_LEN];
		int cmp1valhead = 0;
		int cmp1valtail = 0;
		int cmp2valhead = 0;
		int cmp2valtail = 0;

		memset( head_flag, 0xa5, MISC_APPLICATION_PARAM_VALUE_LEN );
		memset( tail_flag, 0x5a, MISC_APPLICATION_PARAM_VALUE_LEN );

		pHeadFlag = szBuff;
		pTailFlag = szBuff + CFG_FLASH_APP_PARAM_SIZE - MISC_APPLICATION_PARAM_VALUE_LEN;
		cmp1valhead = memcmp( pHeadFlag, head_flag, MISC_APPLICATION_PARAM_VALUE_LEN );
		cmp1valtail = memcmp( pTailFlag, tail_flag, MISC_APPLICATION_PARAM_VALUE_LEN );

		pHeadFlag = szBkBuf;
		pTailFlag = szBkBuf + CFG_FLASH_APP_PARAM_SIZE - MISC_APPLICATION_PARAM_VALUE_LEN;
		cmp2valhead = memcmp( pHeadFlag, head_flag, MISC_APPLICATION_PARAM_VALUE_LEN );
		cmp2valtail = memcmp( pTailFlag, tail_flag, MISC_APPLICATION_PARAM_VALUE_LEN );

		if ( ( 0 == cmp1valhead &&  0 == cmp1valtail ) && ( 0 != cmp2valhead || 0 != cmp2valtail ) ) {
			len = misc_flash_repeat_write( szBuff, CFG_FLASH_APP_PARAM_SIZE, ENV_FLASH_APPBK_PARAM_OFFSETPOS, misc_get_mtd_name() );
			if ( 0 != len ) {
				free( szBuff );
				TIPC_PRINTF_ERR(" flash write backup env param %x error", ENV_FLASH_APPBK_PARAM_OFFSETPOS);
				return -1;
			}
			TIPC_PRINTF_CHK( "current begin param write to backup flash param" );
		}
		else if ( ( 0 == cmp2valhead &&  0 == cmp2valtail ) && ( 0 != cmp1valhead || 0 != cmp1valtail ) ) {
			len = misc_flash_repeat_write( szBkBuf, CFG_FLASH_APP_PARAM_SIZE, ENV_FLASH_APP_PARAM_OFFSET_POS, misc_get_mtd_name() );
			if ( 0 != len ) {
				free( szBuff );
				TIPC_PRINTF_ERR(" flash write backup env param %x error", ENV_FLASH_APP_PARAM_OFFSET_POS);
				return -1;
			}
			TIPC_PRINTF_CHK( "current backup flash param write to begin flash param " );
		}
		else if ( ( 0 == cmp2valhead &&  0 == cmp2valtail ) && ( 0 == cmp1valhead && 0 == cmp1valtail ) ) {
			len = misc_flash_repeat_write( szBuff, CFG_FLASH_APP_PARAM_SIZE, ENV_FLASH_APPBK_PARAM_OFFSETPOS, misc_get_mtd_name() );
			if ( 0 != len ) {
				free( szBuff );
				TIPC_PRINTF_ERR("current support sssss flash write backup env param %x error", ENV_FLASH_APPBK_PARAM_OFFSETPOS);
				return -1;
			}

			TIPC_PRINTF_CHK("current support sssss begin param write to backup flash param" );
		}
		else
		{
			pHeadFlag = szBuff;
			pTailFlag = szBuff + CFG_FLASH_APP_PARAM_SIZE - MISC_APPLICATION_PARAM_VALUE_LEN;
			memset( szBuff, 0xff, CFG_FLASH_APP_PARAM_SIZE );
			memcpy( pHeadFlag, head_flag, MISC_APPLICATION_PARAM_VALUE_LEN );
			memcpy( pTailFlag, tail_flag, MISC_APPLICATION_PARAM_VALUE_LEN );

			len = misc_flash_repeat_write( szBuff, CFG_FLASH_APP_PARAM_SIZE, ENV_FLASH_APP_PARAM_OFFSET_POS, misc_get_mtd_name() );
			if ( 0 != len ) {
				free( szBuff );
				TIPC_PRINTF_ERR(" flash write 123 env param %x error", ENV_FLASH_APPBK_PARAM_OFFSETPOS);
				return -1;
			}

			len = misc_flash_repeat_write( szBuff, CFG_FLASH_APP_PARAM_SIZE, ENV_FLASH_APPBK_PARAM_OFFSETPOS, misc_get_mtd_name() );
			if ( 0 != len ) {
				free( szBuff );
				TIPC_PRINTF_ERR(" flash write 123 backup env param %x error", ENV_FLASH_APPBK_PARAM_OFFSETPOS);
				return -1;
			}

			TIPC_PRINTF_CHK( "current begin and backup flash param all error , write empty" );
		}
	}
	else
	{
		TIPC_PRINTF_CHK(  "current flash param == " );
	}

	free( szBuff );
	g_misc_check_flash_param_value_flag = 1;
	return 0;
}

void msic_check_flash_param(void)
{
	misc_check_flash_app_param_chg();
}

int tvt_sdk_flash_GetFlashParamItem( char* param_value, int param_len, int item_number ) {
	int s32Ret = -1;
	//lock:
	TIPC_MUTEX_LOCK(&g_pstSdkFlashInfo->s32Mutex, g_pstSdkFlashInfo->szMutexName,
	                &g_pstSdkFlashInfo->u32MutexLockCnt, 0);
	// Set:
	misc_check_flash_app_param_chg();

	if ( param_len & (MISC_APPLICATION_PARAM_VALUE_LEN - 1) )
	{
		// TVT_MUTEX_UN_LOCK( &g_lock_flash_param );
		TIPC_MUTEX_UNLOCK(&g_pstSdkFlashInfo->s32Mutex, g_pstSdkFlashInfo->szMutexName,
		                  &g_pstSdkFlashInfo->u32MutexLockCnt, 0);
		TIPC_PRINTF_ERR( "current param len %d mast 32x", param_len );
		return -1;
	}
	s32Ret  = misc_flash_application_param( item_number, 1, param_value, param_len, 0 );
	// unlock:
	TIPC_MUTEX_UNLOCK(&g_pstSdkFlashInfo->s32Mutex, g_pstSdkFlashInfo->szMutexName,
	                  &g_pstSdkFlashInfo->u32MutexLockCnt, 0);
	return s32Ret ;
}
int tvt_sdk_flash_SetFlashParamItem( char* param_value, int param_len, int item_number ) {
	int s32Ret = -1;
	//lock:
	TIPC_MUTEX_LOCK(&g_pstSdkFlashInfo->s32Mutex, g_pstSdkFlashInfo->szMutexName,
	                &g_pstSdkFlashInfo->u32MutexLockCnt, 0);
	// Set:
	misc_check_flash_app_param_chg();
	if ( param_len & (MISC_APPLICATION_PARAM_VALUE_LEN - 1) )
	{
		// TVT_MUTEX_UN_LOCK( &g_lock_flash_param );
		TIPC_MUTEX_UNLOCK(&g_pstSdkFlashInfo->s32Mutex, g_pstSdkFlashInfo->szMutexName,
		                  &g_pstSdkFlashInfo->u32MutexLockCnt, 0);
		TIPC_PRINTF_ERR( "current param len %d mast 32x", param_len );
		return -1;
	}
	s32Ret = misc_flash_application_param( item_number, 0, param_value, param_len, 0 );
	// unlock:
	TIPC_MUTEX_UNLOCK(&g_pstSdkFlashInfo->s32Mutex, g_pstSdkFlashInfo->szMutexName,
	                  &g_pstSdkFlashInfo->u32MutexLockCnt, 0);
	return s32Ret ;
}


/**
 * @breif Init the SDK Flash Part
 * @return 0 Success; others Failed
 */
int tvt_sdk_flash_init(void) {
	if (g_pstSdkFlashInfo != NULL)
	{
		if (g_pstSdkFlashInfo->u8Init )
		{
			// has been inited:
			TIPC_PRINTF_INFO("flash service is running");
			return 0;
		}
	}
	else
	{
		// create the memory of info struct
		g_pstSdkFlashInfo = (TVT_SDK_FLASH_INFO_S* )malloc(sizeof(TVT_SDK_FLASH_INFO_S));
		if (NULL == g_pstSdkFlashInfo)
		{
			TIPC_PRINTF_ERR("flash service malloc failed");
			return -1;
		}
		memset(g_pstSdkFlashInfo, 0, sizeof(TVT_SDK_FLASH_INFO_S));
	}

	// Default Init with EMMC:
	g_pstSdkFlashInfo->boot_param_flash_type = TUSED_FLASH_TYPE_EMMC;
	g_pstSdkFlashInfo->system_flash_type = TUSED_FLASH_TYPE_EMMC;

	// init the mutex:
	pthread_mutex_init(&g_pstSdkFlashInfo->s32Mutex, NULL);
	sprintf(g_pstSdkFlashInfo->szMutexName, "tvt_sdk_flash_lock");

	g_pstSdkFlashInfo->u8Init = 1;
	TIPC_PRINTF_INFO ("[****** WKL Debug ******] The flash has been inited");
	return 0;
}

/**
 * @breif Release Memory and Mutex of SDK Flash Part
 * @return 0 Success; others Failed
 */
int tvt_sdk_flash_exit(void) {
	if (NULL == g_pstSdkFlashInfo)
	{
		TIPC_PRINTF_ERR("flash service is not running");
		return -1;
	}

	if (g_pstSdkFlashInfo->u8Init != 1)
	{
		TIPC_PRINTF_ERR("flash service is not running");
		free((void*)g_pstSdkFlashInfo);
		g_pstSdkFlashInfo = NULL;
		return -1;
	}

	pthread_mutex_destroy(&g_pstSdkFlashInfo->s32Mutex);

	free((void*)g_pstSdkFlashInfo);
	g_pstSdkFlashInfo = NULL;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////flash_cpm////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

static char* g_pCheckFlashMemAddr = NULL;
static int g_iCheckFlashMemLen = 0;

inline int xopen3(const char *pathname, int flags, int mode)
{
	int ret;

	ret = open(pathname, flags, mode);
	if (ret < 0) {
		TIPC_PRINTF_ERR("can't open '%s'", pathname);
	}
	return ret;
}

// Die if we can't open a file and return a fd.
inline int  xopen(const char *pathname, int flags)
{
	return xopen3(pathname, flags, 0666);
}

inline int xclose( int fd )
{
	if ( fd > 0 ) {
		close( fd );
	}
	return 0;
}

inline off_t  xlseek(int fd, off_t offset, int whence)
{
	off_t off = lseek(fd, offset, whence);
	if (off == (off_t) - 1) {
		if (whence == SEEK_SET) {
			TIPC_PRINTF_ERR("lseek(%"OFF_FMT"u)", offset);
			return -1;
		}
		TIPC_PRINTF_ERR("lseek");
		return -1;
	}
	return off;
}

ssize_t safe_read(int fd, void *buf, size_t count) {
	ssize_t n;
	do {
		n = read(fd, buf, count);
	} while (n < 0 && errno == EINTR);
	return n;
}

ssize_t full_read(int fd, void *buf, size_t len) {
	ssize_t cc;
	ssize_t total;
	total = 0;
	while (len) {
		cc = safe_read(fd, buf, len);
		if (cc < 0) {
			if (total) {
				/* we already have some! */
				/* user can do another read to know the error code */
				return total;
			}
			return cc; /* read() returns -1 on failure. */
		}
		if (cc == 0)
			break;
		buf = ((char *)buf) + cc;
		total += cc;
		len -= cc;
	}
	return total;
}

inline int  xread(int fd, void *buf, size_t count) {
	if (count) {
		ssize_t size = full_read(fd, buf, count);
		if ((size_t)size != count)
		{
			TIPC_PRINTF_ERR("short read");
			return -1;
		}
	}
	return 0;
}

ssize_t  safe_write(int fd, const void *buf, size_t count)
{
	ssize_t n;
	do {
		n = write(fd, buf, count);
	} while (n < 0 && errno == EINTR);
	return n;
}

ssize_t  full_write(int fd, const void *buf, size_t len)
{
	ssize_t cc;
	ssize_t total;
	total = 0;
	while (len) {
		cc = safe_write(fd, buf, len);
		if (cc < 0) {
			if (total) {
				/* we already wrote some! */
				/* user can do another write to know the error code */
				return total;
			}
			return cc;  /* write() returns -1 on failure. */
		}
		total += cc;
		buf = ((const char *)buf) + cc;
		len -= cc;
	}
	return total;
}


int flashcpm_chect_data_error( char *filebuf, int filesize, int fd_d )
{
	int mtd_flash_block_size = 0;
	char * filecontent = filebuf;
	int pPos = 0;

	mtd_flash_block_size = 256 * 1024;
	if ( g_pCheckFlashMemAddr && g_iCheckFlashMemLen >= mtd_flash_block_size * 2 )
	{
		char* buf = NULL;
		char* buf2 = NULL;
		int cmp_size = 0;

		mtd_flash_block_size = ((g_iCheckFlashMemLen / (mtd_flash_block_size * 2)) * mtd_flash_block_size);
		buf = g_pCheckFlashMemAddr;
		if ( buf )
		{
			memset( buf, 0, mtd_flash_block_size * 2 );
			buf2 = buf + mtd_flash_block_size;

			pPos = 0;
			xlseek(fd_d, 0, SEEK_SET);
			while ( pPos < filesize )
			{
				if ( pPos + mtd_flash_block_size > filesize )
				{
					cmp_size = filesize - pPos;
				}
				else
				{
					cmp_size = mtd_flash_block_size;
				}

				memcpy(buf, filecontent + pPos, cmp_size);
				xread(fd_d, buf2, mtd_flash_block_size);
				if (memcmp(buf, buf2, cmp_size))
				{
					TIPC_PRINTF_ERR("verification mismatch at %x", pPos);
					return -1;
				}
				pPos += mtd_flash_block_size;
			}
		}
	}
	return 0;
}


int flashcpm_set_check_memaddr( char* pCheckMemAddr, int mem_len )
{
	g_pCheckFlashMemAddr = pCheckMemAddr;
	g_iCheckFlashMemLen = mem_len;
	return 0;
}


int flashcpm_main(char *filebuf, int new_update_method_inbuflen, int filesize,
                  char *devicename, int device_blk_siz, int flash_type)
{
	int fd_d;
	struct mtd_info_user mtd;
	// struct erase_info_user e;
	// struct region_info_user erase_write_info;
	int ret_len = 0;
	int times = 0;
	int wr_real_size = filesize;
	int erase_type = 0;

	fd_d = xopen(devicename, O_SYNC | O_RDWR);
	if ( fd_d <= 0 )
	{
		TIPC_PRINTF_ERR("xopen %s error", devicename);
		return -1;
	}

	if ( TUSED_FLASH_TYPE_EMMC == flash_type )
	{
		mtd.erasesize = 512;
		erase_type = 0;
	}
	else
	{
		///TODO:
	}

	wr_real_size = ((filesize + (mtd.erasesize - 1)) / mtd.erasesize) * mtd.erasesize;

	// 这个必需要处理，清空无效数据
	if ( TUSED_FLASH_TYPE_EMMC == flash_type )
	{
		if ( new_update_method_inbuflen > 0  )
		{
			// 主要是处理：分配升级的缓冲区长度 < 升级的分区的大小 的时候，
			if (device_blk_siz > new_update_method_inbuflen)
			{
				memset( filebuf + filesize, 0xff, new_update_method_inbuflen - filesize );
			}
			else
			{
				memset( filebuf + filesize, 0xff, device_blk_siz - filesize );
			}
		}
		else
		{
			memset( filebuf + filesize, 0xff, device_blk_siz - filesize );
		}
	}
	else
	{
		///TODO
	}
	if ( 0 != setpriority( PRIO_PROCESS, 0, -20 ) )
	{
		TIPC_PRINTF_ERR( "xxx set priority error %d", errno );
	}

	for ( times = 0; times < 1; times++ )
	{
		TIPC_PRINTF_CHK( "begin erase flash %d erase_type = %d", times, erase_type );
		xlseek(fd_d, 0, SEEK_SET);

		if ( TUSED_FLASH_TYPE_EMMC == flash_type )
		{
			ret_len = full_write(fd_d, filebuf, wr_real_size);
			if ( ret_len != wr_real_size )
			{
				xclose( fd_d );
				TIPC_PRINTF_ERR("write error at on %s, write returned %d", devicename, ret_len);
				return -1;
			}
		}
		else
		{
			///TODO
		}

	}

	xclose( fd_d );

	TIPC_PRINTF_CHK("end update flashcp main" );

	return 0;

}

int flashcpm_logo_image(char *pDataBuf, int iDataLen, int iOffsetSize,  char *devicename, int flash_type)
{
	int fd_d;
	struct mtd_info_user mtd;
	// struct erase_info_user e;
	// struct region_info_user erase_write_info;
	int ret_len = 0;
	int times = 0;
	int wr_real_size = iDataLen;
	int erase_type = 0;

	fd_d = xopen(devicename, O_SYNC | O_RDWR);
	if ( fd_d <= 0 )
	{
		TIPC_PRINTF_ERR("xopen %s error", devicename);
		return -1;
	}

	if ( TUSED_FLASH_TYPE_EMMC == flash_type )
	{
		mtd.erasesize = 512;
		erase_type = 0;
	}
	else
	{
		///TODO:
	}

	wr_real_size = ((iDataLen + (mtd.erasesize - 1)) / mtd.erasesize) * mtd.erasesize;

	for ( times = 0; times < 1; times++ )
	{
		TIPC_PRINTF_CHK( "begin erase flash %d erase_type = %d", times, erase_type );
		xlseek(fd_d, iOffsetSize, SEEK_SET);

		if ( TUSED_FLASH_TYPE_EMMC == flash_type )
		{
			ret_len = full_write(fd_d, pDataBuf, wr_real_size);
			if ( ret_len != wr_real_size )
			{
				xclose( fd_d );
				TIPC_PRINTF_ERR("write error at on %s, write returned %d", devicename, ret_len);
				return -1;
			}
		}
		else
		{
			///TODO:
		}

	}

	xclose( fd_d );
	TIPC_PRINTF_CHK( "end update flashcp main" );
	return 0;

}

