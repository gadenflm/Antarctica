#ifndef _CFG_BUFFER_H_
#define _CFG_BUFFER_H_

#include <string.h>
#include <stdio.h>

#include "CfgIO.h"
#include "SDK_define.h"

#define CFG_GUIDENCE_1 "### This file is generated automatically by WKL_Server_Manager ###"
#define CFG_GUIDENCE_2 "### To change the configuration, Author recommend you using WKL_Server_Manager Program ###"
#define CFG_GUIDENCE_3 "### If you want to edit this file directly, Please be careful! ###\n"

#define LINE_MAX_SIZE 256

unsigned int buff_ind = 0;

void OutToBuffer(char* buff, const char* line ) {
	while ( (*line) != '\0' ) {
		buff[buff_ind++] = *line;
		line ++;
	}
	buff[buff_ind++] = '\n';
}

void ReadLineFromBuffer(const char* buff, char* line) {
	int ind = 0;
	do
	{
		ind = 0;
		while (buff[buff_ind] != '\n' ) {
			line[ind++] = buff[buff_ind++];
		}
		buff_ind++;
	} while ( (*line) == '#' || (*line) == '\n');
	line[ind++] = '\0';
}

void ServerCfgDataToBuffer(const Server_Cfg_Data* data, char *buff)
{
	char line[LINE_MAX_SIZE] = {0};
	buff_ind = 0;
	OutToBuffer(buff,  CFG_GUIDENCE_1);
	OutToBuffer(buff,  CFG_GUIDENCE_2);
	OutToBuffer(buff,  CFG_GUIDENCE_3);
// ipv4:
	sprintf (line, "ipv4 = %s", data->ip4_addr);
	OutToBuffer(buff,  "# Server Ipv4 Address");
	OutToBuffer(buff,  line);
// tcp buff port:
	sprintf (line, "tcp buff port = %d", data->tcp_buff_port);
	OutToBuffer(buff,  "# TCP TransBuffer Port");
	OutToBuffer(buff,  line);
// tcp buff size:
	sprintf (line, "tcp buff size = %d", data->tcp_buff_size);
	OutToBuffer(buff,  "# TCP TransBuffer Max Size");
	OutToBuffer(buff,  line);
// socket listen size:
	sprintf (line, "sv_lis_size = %d", data->listen_size);
	OutToBuffer(buff,  "# Max Client Count for One Socket");
	OutToBuffer(buff,  line);

// Final flag:
	buff [buff_ind++] = '\0';
	return ;
}

int BufferToServerCfgData(const char *buff, Server_Cfg_Data* data)
{
	char line[LINE_MAX_SIZE] = {0};
	buff_ind = 0;
// ipv4 :
	ReadLineFromBuffer(buff, line);
	// WKL_PRINTD("line: %s", line);
	if (!sscanf(line, "ipv4 = %s", data->ip4_addr )) return WKL_FAIL;

// tcp buff port:
	ReadLineFromBuffer(buff, line);
	// WKL_PRINTD("line: %s", line);
	if (!sscanf(line, "tcp buff port = %hd", &data->tcp_buff_port)) return WKL_FAIL;

// tcp buff size:
	ReadLineFromBuffer(buff, line);
	// WKL_PRINTD("line: %s", line);
	if (!sscanf(line, "tcp buff size = %hd", &data->tcp_buff_size)) return WKL_FAIL;

// socket listen size:
	ReadLineFromBuffer(buff, line);
	// WKL_PRINTD("line: %s", line);
	if (!sscanf(line, "sv_lis_size = %hhd", &data->listen_size)) return WKL_FAIL;

// Final flag:

	return WKL_SUCC;
}


#endif
