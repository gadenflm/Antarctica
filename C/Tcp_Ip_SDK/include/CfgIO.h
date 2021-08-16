/**
 * Author: K_gadenflm
 * Description: Header for Read and Write Configure File.
 */
#ifndef _CFG_IO_H_
#define _CFG_IO_H_

#include "SDK_define.h"
#include <unistd.h>
#include <fcntl.h>

typedef struct server_cfg_data_t
{
	char ip4_addr[16];
	uint16_t tcp_buff_port;
	uint16_t tcp_buff_size;
	uint8_t listen_size;
} Server_Cfg_Data;

#define DEFAULT_SV_CFG_DATA {"0.0.0.0",\
								8888,\
								1024,\
								20}
#define	CFG_FILE_SERVER "server.cfg"

// Saved File Buffer's Max Bytes Count
#define CFG_BUFF_MAX_SIZE 8192
//CFG Types Count
#define CFG_NUM 1


/**
 * Configure Type, For Using API Interface.
 * !! The order must be same as g_cfg_list (defined in CfgIO.c) !!
 */
typedef enum cfg_type_e {
	CFG_TYPE_SERVER = 0,
} Cfg_Type_e;

typedef struct cfg_t {
	Cfg_Type_e type;
	const char* filename;
	void* data;
	uint16_t data_size;
} CFG_T;

// Interface:

int CfgInit (void);
int CfgInitCheck (void);

int GetCfgData (const int cfg_type , void* p_cfg);
int SetCfgData (const int cfg_type , const void* p_cfg_data);
int SetCfgData_Default (const int cfg_type);

int LoadCfgFromFile (const int cfg_type);
int SaveCfgToFile (const int cfg_type);

// Self Function:

int ReadCfgFile(const char* file_name, char* buff);
int WriteCfgFile(const char* file_name, char* buff);
int BufferToCfgData (const int type, const char* buff, void * data);
int CfgDataToBuffer (const int type, const void *data, char* buff);

/////////////////////////////////////////////////////////////////////////////
/// Inline func:

/**
 * @return          [0] file not exist  [1] file exist
 */
_Bool CheckFileExist(const char * filename);

#endif