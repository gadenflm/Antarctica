#include "CfgIO.h"
#include "Socket_Func.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

Sock_Node* g_sock = NULL;

int ServerSocketInit () {
	if (g_sock != NULL) {
		WKL_PRINTE("Init socket error, The socket has been inited!");
		return WKL_FAIL;
	}

	// Get Cfg:
	Server_Cfg_Data * sv_cfg = (Server_Cfg_Data*) malloc (sizeof(Server_Cfg_Data) );
	if (GetCfgData(CFG_TYPE_SERVER, (void*)sv_cfg) ) return WKL_FAIL;

	// Create Socket:
	g_sock = (Sock_Node* ) malloc (sizeof(Sock_Node));
	g_sock->ip_addr = NULL;
	g_sock->port = sv_cfg->tcp_buff_port;
	g_sock->type = SOCK_STREAM;                   // TCP
	g_sock->fd = socket(AF_INET, g_sock->type, 0);  // IPv4
	if ( g_sock->fd == -1 ) {
		WKL_PRINTE("Create sever socket error!");
		return WKL_FAIL;
	}
	WKL_PRINTI("Create Server Socket Success! Socket.fd=%d", g_sock->fd);

	//Set Sever Address:
	struct sockaddr_in server_addr;                     // the struct is used for socket addrss init
	memset(&server_addr, 0, sizeof(server_addr));       // init sockaddr_in with 0
	server_addr.sin_family = AF_INET;                   // IPv4
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // 0.0.0.0, All netboard address will be set
	server_addr.sin_port = htons(g_sock->port);           // Port Number

	// Bind
	int ret = bind(g_sock->fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in));
	if (ret == -1 ) {
		WKL_PRINTE("Bind socket error! return:%d", ret);
		return WKL_FAIL;
	}
	WKL_PRINTI("Bind socket OK!");
	free (sv_cfg);
	return WKL_SUCC;
}


int ServerSocketRelease () {
	if (g_sock == NULL) {
		WKL_PRINTE("Release socket error, The socket pointer is NUll!");
		return WKL_FAIL;
	}
	close(g_sock->fd);  // unistd.h
	free(g_sock);
	g_sock = NULL;
	WKL_PRINTI("Close the socket file descriptor FD finished.");
	return WKL_SUCC;
}