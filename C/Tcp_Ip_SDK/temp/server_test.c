#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>

#define TCP_PORT 8888
#define BUFF_SIZE 1024
#define LISTEN_SIZE 20

#define WKL_SUCC 0
#define WKL_FAIL 1

struct sock_node
{
	char* ip_addr;
	int port;
	int type;
	int fd; //file descriptor
};

int BindSocket (struct sock_node* sock);
int SocketInit (struct sock_node* sock);
int SocketRelease (struct sock_node* sock);
int	ErrorExit(const char *format, ...);
int echo(int fd);

int main(int argc, char** argv) {
	struct sock_node* server_sock = (struct sock_node*)malloc(sizeof(struct sock_node));

	if (!SocketInit(server_sock)) {

		if (!BindSocket (server_sock)) {
			struct sockaddr_in fsin;
			unsigned int alen;
			int fd;
			fd_set rfds;// read file descriptor set
			fd_set afds;// active file descriptor set
			int nfds = getdtablesize();
			FD_ZERO(&afds);
			FD_SET(server_sock->fd, &afds);

			while (1) {
				memcpy(&rfds, &afds, sizeof(rfds));
				if (select(nfds, &rfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0) < 0) {
					ErrorExit("Select: %s\n", strerror(errno));
				}

				if (FD_ISSET(server_sock->fd, &rfds)) {
					alen = sizeof(fsin);
					int ssock = accept(server_sock->fd, (struct sockaddr *)&fsin, &alen);
					if (ssock < 0)
						ErrorExit("Accept: %s\n", strerror(errno));
					else
						printf("Accept clinet %d\n", ssock);
					FD_SET(ssock, &afds);
				}
				for (fd = 0; fd < nfds; ++fd) {
					if (fd != server_sock->fd && FD_ISSET(fd, &rfds)) {
						if (echo(fd) <= 0) {
							(void) close(fd);
							FD_CLR(fd, &afds);
						}
					}
				}
			}
		}
		SocketRelease (server_sock);
	}

	return WKL_SUCC;
}

int SocketInit ( struct sock_node* sock) {
	sock->ip_addr = NULL;
	sock->port = TCP_PORT;
	sock->type = SOCK_STREAM;
	sock->fd = socket(AF_INET, sock->type, 0);
	printf("sock.fd=%d\n", sock->fd);
	if ( sock->fd == -1 ) {
		printf("[ERROR] Create socket error!\n");
		return WKL_FAIL;
	}

	return WKL_SUCC;
}

int BindSocket (struct sock_node* sock) {
	//Set Sever Address
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr)); //init the sockaddr_in with 0.
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(sock->port);

	// Bind
	int ret = bind(sock->fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in));
	if (ret == -1 ) {
		printf("[ERROR] Bind socket error!return:%d\n", ret);
		perror("bind");
		return WKL_FAIL;
	} else {
		printf("[Info] Bind socket OK!\n");
		if (listen(sock->fd, LISTEN_SIZE) == -1) {
			printf("[ERROR] Set listen error\n");
			return WKL_FAIL;
		}
		printf("[Info] Tcp server start\n");

		return WKL_SUCC;
	}

}

int echo(int fd) {
	char buf[BUFF_SIZE];
	int	ret = read(fd, buf, sizeof(buf));
	if (ret > 0) {
		printf("Receive msg from client %d : %s\n", fd, buf);
	} else if (ret == 0)
		printf("Client %d disconnect\n", fd);
	else
		printf("Read from client %d error, close!\n", fd);
	return ret;
}

int ErrorExit(const char *format, ...) {
	va_list	args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	exit(1);
}

int SocketRelease (struct sock_node* sock) {
	close(sock->fd);
	free (sock);
	sock = NULL;
	return WKL_SUCC;
}