/**
 * Author: K_gadenflm
 * Description: A header for Socket
 */
#ifndef _SOCKET_FUNC_H_
#define _SOCKET_FUNC_H_

#include "SDK_define.h"

typedef struct sock_node_t
{
	const char* ip_addr;
	int port;
	int type;
	int fd; //file descriptor
} Sock_Node;

int ServerSocketInit ();
int ServerSocketRelease ();

#endif