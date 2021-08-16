#include "CfgIO.h"
#include "CFG_Buffer.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

static Server_Cfg_Data g_sv_cfg_data = DEFAULT_SV_CFG_DATA;

static CFG_T g_cfg_list[CFG_NUM] = {
	{CFG_TYPE_SERVER, CFG_FILE_SERVER, &g_sv_cfg_data, sizeof(Server_Cfg_Data) },
};

inline _Bool CheckFileExist(const char * filename) {
	return	(access(filename, F_OK)) ? 0 : 1;
}

int GetCfgData (const int cfg_type , void* p_cfg) {
	if (g_cfg_list[cfg_type].data == NULL) {
		WKL_PRINTE("Get Configure Data Error! The Cfg Data is nullptr");
		return WKL_FAIL;
	}
	memcpy(p_cfg, g_cfg_list[cfg_type].data, g_cfg_list[cfg_type].data_size);

	return WKL_SUCC;
}

int SetCfgData (const int cfg_type , const  void* p_cfg_data) {
	if (p_cfg_data == NULL) {
		WKL_PRINTE("Set Configure Data Error! The Data Poniter is nullptr");
		return WKL_FAIL;
	}
	// size not equal
	if (sizeof(p_cfg_data) != g_cfg_list[cfg_type].data_size ) {
		WKL_PRINTE("Set Configure Data Error! The Data size is error");
		return WKL_FAIL;
	}
	memcpy(g_cfg_list[cfg_type].data, p_cfg_data, g_cfg_list[cfg_type].data_size);

	return WKL_SUCC;
}

int CfgInit (void) {
	int ret = 0;
	// Check The order of Cfg list
	ret += CfgInitCheck();
	if (ret) {
		WKL_PRINTE(" Configure Init Check Error, Please Check the code!");
		return WKL_FAIL;
	}

	// If cfg file exist, load it , else create it.
	for (int ind = 0 ; ind < CFG_NUM ; ind++) {
		if (CheckFileExist(g_cfg_list[ind].filename)) {
			WKL_PRINTI(" Loading Configure From File: %s", g_cfg_list[ind].filename);
			ret += LoadCfgFromFile(ind);
		} else {
			WKL_PRINTI(" Created Configure File: %s", g_cfg_list[ind].filename);
			ret += SaveCfgToFile(ind);
		}
		if (ret) {
			WKL_PRINTE(" Configure Init Error!");
			return WKL_FAIL;
		}
	}
	return WKL_SUCC;
}

int SetCfgData_Default(const int cfg_type) {
	Server_Cfg_Data sv_cfg_data = DEFAULT_SV_CFG_DATA;
	switch (cfg_type) {
	case CFG_TYPE_SERVER:
		g_sv_cfg_data = sv_cfg_data;
		break;
	default:
		WKL_PRINTE("Invalid Cfg Type!");
		return WKL_FAIL;
	}
	return WKL_SUCC;
}

int CfgInitCheck (void) {
	for (int ind = 0; ind < CFG_NUM ; ind++) {
		if (g_cfg_list[ind].type != ind) {
			WKL_PRINTE ("Error index: %d", ind );
			return WKL_FAIL;
		}
	}
	return WKL_SUCC;
}

int LoadCfgFromFile (const int cfg_type)
{
	int ret = 0;
	CFG_T* p_cfg = &g_cfg_list[cfg_type];
	const char* file_name = p_cfg->filename;
	const uint16_t size = p_cfg->data_size;
	if (CheckFileExist(file_name))
	{
		// read buffer from file
		char load_buff[CFG_BUFF_MAX_SIZE] = {0};
		ret += ReadCfgFile(file_name, load_buff);
		if (ret) {
			WKL_PRINTE("Read Data Error");
			return WKL_FAIL;
		}

		// Buffer transformed
		void* load_data = malloc (size);
		memset(load_data, 0, size);
		ret += BufferToCfgData (cfg_type, load_buff , load_data);
		if (ret) {
			free (load_data);
			WKL_PRINTE("Transform Loaded Data Error");
			return WKL_FAIL;
		}

		memcpy(p_cfg->data, load_data, size);
		free (load_data);

	} else {
		WKL_PRINTE("File not exist!");
		return WKL_FAIL;
	}
	return WKL_SUCC;
}

int SaveCfgToFile (const int cfg_type)
{
	const CFG_T* p_cfg = &g_cfg_list[cfg_type];
	if (p_cfg->data == NULL) {
		WKL_PRINTE("NULL Poniter");
		return WKL_FAIL;
	}
	int ret = 0;
	char buff[CFG_BUFF_MAX_SIZE] = {0};
	// Trans CFG Data to Buffer
	ret += CfgDataToBuffer(p_cfg->type, p_cfg->data, buff);
	if (ret) {
		WKL_PRINTE("CFG Data Trans to Buffer Error");
		return WKL_FAIL;
	}

	// Write Buffer
	ret += WriteCfgFile(p_cfg->filename, buff);
	if (ret) {
		WKL_PRINTE("Write Cfg Data Error");
		return WKL_FAIL;
	}

	return WKL_SUCC;
}

int ReadCfgFile(const char* file_name, char* buff)
{
	FILE* pfile = fopen(file_name, "rt");
	if (pfile == NULL) {
		WKL_PRINTE("fopen return NULL");
		return WKL_FAIL;
	}
	int index = 0;
	while ( ( buff[index++] = fgetc(pfile) ) != EOF );
	fclose (pfile);
	return WKL_SUCC;
}

int WriteCfgFile(const char* file_name, char* buff)
{
	FILE* pfile = fopen(file_name, "wt");
	if (pfile == NULL) {
		WKL_PRINTE("fopen return NULL");
		return WKL_FAIL;
	}

	while (*buff != '\0') {
		if (EOF == fputc(*buff, pfile)) {
			WKL_PRINTE("fputc return eof");
			return WKL_FAIL;
		}
		buff++;
	}
	fclose (pfile);
	return WKL_SUCC;
}

int BufferToCfgData (const int type, const char* buff, void * data)
{
	// int ret;
	switch (type) {
	case CFG_TYPE_SERVER:
		if (BufferToServerCfgData(buff, (Server_Cfg_Data*)data)) return WKL_FAIL;
		break;
	default:
		WKL_PRINTE("Invalid Cfg Type!");
		return WKL_FAIL;
	}
	return WKL_SUCC;
}

// The Transform refer to CFG_Buffer.h
int CfgDataToBuffer (const int type, const void *data, char* buff)
{
	// int ret;
	switch (type) {
	case CFG_TYPE_SERVER:
		ServerCfgDataToBuffer((Server_Cfg_Data*)data, buff);
		break;
	default:
		WKL_PRINTE("Invalid Cfg Type!");
		return WKL_FAIL;
	}
	return WKL_SUCC;
}

