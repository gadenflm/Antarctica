#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define SERVER_IP_ADDR "66.112.212.123"
#define TCP_PORT 8888
#define BUFF_SIZE 1024

#define WKL_SUCC 0
#define WKL_FAIL 1

struct sock_node
{
	char* ip_addr;
	int port;
	int type;
	int fd; //file descriptor
};

int ConnectSocket (struct sock_node* sock);
int SocketInit ( struct sock_node* sock);
int SocketRelease ( struct sock_node* sock);
void WriteToServer_test(struct sock_node* sock);

int main(int argc, char** argv) {
	struct sock_node* server_sock = (struct sock_node*) malloc (sizeof (struct sock_node));
	if (!SocketInit(server_sock)) {
		if (!ConnectSocket (server_sock)) {
			WriteToServer_test(server_sock);
		}
		SocketRelease (server_sock);
	}

	return WKL_SUCC;
}

int SocketInit ( struct sock_node* sock) {
	sock->ip_addr = SERVER_IP_ADDR;
	sock->port = TCP_PORT;
	sock->type = SOCK_STREAM;
	sock->fd = socket(AF_INET, sock->type, 0);
	if ( sock->fd == -1 ) {
		printf("[ERROR] create socket error!\n");
		return WKL_FAIL;
	}
	return WKL_SUCC;
}

int ConnectSocket (struct sock_node* sock) {
	//Set Sever Address
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(sock->port);
	inet_pton(AF_INET, sock->ip_addr, &server_addr.sin_addr);

	// Connect
	int ret = connect(sock->fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in));
	if (ret == -1 ) {
		printf("[ERROR] connect socket error!\n");
		return WKL_FAIL;
	} else {
		printf("[Info] connect socket OK!\n");
		return WKL_SUCC;
	}
}

void WriteToServer_test(struct sock_node* sock) {
	char data[BUFF_SIZE];
	memset(data, 0, sizeof(data));
	scanf("%s", data);
	if (-1 == write(sock->fd, data, sizeof(data))) {
		printf("[ERROR] Write to server error\n");
	} else {
		printf("[Info] Write to server OK!\n");
	}
	return ;
}

int SocketRelease ( struct sock_node* sock) {
	close(sock->fd);
	free (sock);
	sock = NULL;
	return WKL_SUCC;
}
