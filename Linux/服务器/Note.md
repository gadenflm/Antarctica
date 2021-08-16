

[TOC]

## 字节序

字节序是指字节在设备上存储的顺序。及多字节数据在计算机内存中存储或者网络传输时各字节的存储顺序。

分为：

#### 主机字节序(HBO)

字节数据在计算机内存中存储顺序，主要有两种:

1. Little-endian(LE 小端) 将低序字节存储在起始地址
2. Big-endian (BE 大端) 将高序字节存储在起始地址

无符号数 1 Byte -> `0x 00 ~ 0x FF`

将 `0x1234abcd` 写入到 以 `0x0000` 开始的内存中

```
         big-endian    little-endian
0x0000      0x12          0xcd
0x0001      0x34          0xab
0x0002      0xab          0x34
0x0003      0xcd          0x12
```

#### 网络字节序(NBO)

__网络字节序是TCP/IP中规定好的一种数据表示格式，它与具体的CPU类型、操作系统等无关,从而可以保证数据在不同主机(字节序不同)之间传输时能够被正确解释。__网络字节顺序采用big endian排序方式。

#### 编程

在C\C++ Socket编程中，用四个函数来完成主机字节顺序格式和网络字节顺序格式之间的转换。

`netinet/in.h` 申明

* `htons` Host to Net (short) 
* `htonl` Host to Net (long)
* `ntohs` Net to Host (short)
* `ntohl` Net to Host (long)

分别完成 16位无符号数 和 32位无符号数 下 主机字节序和网络字节序的 相互转换。

例如端口号范围为 (0 ~ 65535)，正好为16位无符号数( 2Byte <--> shor int )
`server_addr.s_port = htons(sock->port);`


## socket 初始化 

---

#### 申明 `int socket(int domain, int type, int protocol);`

`sys/socket.h` 中定义，返回标记fd(文件描述符)

`int domian`: 指定使用何种的地址类型,一般如下(PF、AF非开源时期用以区分，基本可以混用)

* PF_INET, AF_INET： Ipv4网络协议
* PF_INET6, AF_INET6： Ipv6网络协议

`int type`:设置通信的协议类型,常用如下

* `SOCK_STREAM` 提供面向连接的稳定数据传输,即TCP协议 __(常用)__
* `SOCK_DGRAM` 使用不连续不可靠的数据包连接,基于UDP __(常用)__
* `SOCK_SEQPACKET` 提供连续可靠的数据包连接。 SCTP协议
* `SOCK_RAW` 提供原始网络协议存取。  ICMP
* `SOCK_RDM` 提供可靠的数据包连接。  基于UDP
* `SOCK_PACKET` 与网络驱动程序直接通信。 物理层协议

`int protocol` 指定具体协议号，若无特殊需求，一般都设定为0

---

#### 地址处理 `struct sockaddr_in` 和 `struct sockaddr`

`struct sockaddr_in` 在 `netinet/in.h` 定义
`struct sockaddr` 在 `sys/socket.h` 定义

```C 
//非源码，为理解后改写 
struct sockadd_in {
    sa_family_t     sin_family;      // Address Family, 地址族(与通信协议位数对应)
    unsigned short sin_port;    //16位TCP/UDP端口号
    struct in_addr sin_addr;    //32位IP地址,结构体里只有一个变量 s_addr
    unsigned char sin_zero[8];  //没有实际意义,只是为了跟SOCKADDR结构在内存中对齐
};

sin_addr.s_addr = htonl (INADDR_ANY);
/*
INADDR_ANY 为0.0.0.0 表示不确定地址, 泛指本机所有IP。 如本机有三块网卡，对应三个地址，则用该种初始化方式可以绑定所有网卡的对应端口，若给与特定地址，则只绑定单一网卡的对应端口。
 */
```
二者空间都是16字节，可以直接相互转化！

一般先把sockaddr_in变量赋值后，强制类型转换后传入用sockaddr做参数的函数：
__sockaddr_in用于socket定义和赋值；sockaddr用于函数参数__

`sin_family` 取值：
* AF_INET(Ipv4)
* AF_INET6(Ipv6)
* AF_LOCAL(本地通信中采用的Unix协议的地址族)

---

#### 绑定`bind` (套接字命名)

__对于服务器端，`bind()`是必须要做的事情，服务器启动时需要绑定指定的端口来提供服务(以便于客户向指定的端口发送请求)。__

__对于客户端，一般而言无需主动调用bind()，一切由操作系统来完成。在发送数据前，操作系统会为套接字随机分配一个可用的端口，同时将该套接字和本地地址信息绑定。__

```C 
#include <sys/socket.h>
int bind(int socket, const struct sockaddr *address, socklen_t address_len);

//个人习惯：

int ret = (sock->fd, (struct sockaddr*) &server_sock_adrr_in, sizeof(struct sockaddr_in));
```

* `int socket` 通过申明获得的socket fd.
* `const struct sockaddr *address` 含有具体信息,一般通过上述 `struct sockaddr_in` 初始化参数后，取地址并转成 `struct sockaddr *` 传入。
*  arg2 的指针所指数据大小，通过`sizeof(struct sockaddr_in)` 传入即可

## socket 功能实现

### 服务器端

#### 设定套接字为被动式socket `listen()`

使用`socket()`创建的socket默认是主动socket，主动的socket可以调用`connect()`跟一个被动socket建立一个连接。    
被动socket是一个通过调用`listen()`监听要发起连接的socket，当被动socket接受一个连接通常称为被动打开

```C 
#include <sys/socket.h>
int listen(int sockfd, int backlog);
```

__listen()将对应socket标记成一个被动式的socket,也就是说可以用accept()函数去接受进来的连接请求__，此函数设置于 `bind()` 之后， `accept()` 之前。

* `int sockfd` 创建socket返回的文件描述符
* `int backlog` 维护连接队列最大值的参数，指定的是已经连接(完成三次握手)但等待accept的队列的最大值(并非直接对应，相对影响)

__如果当此连接队列已经满了，此时一个连接请求到达服务端，此时客户端会收到一个ECONNREFUSED的错误码，或者如果此协议支持重传，那么服务端会直接忽略掉这个连接请求以使得重传到直到连接成功。__

__底层理解`listen()`调用后，维护了两个队列，已完成连接的队列(三次握手)和未完成连接的队列。__

1. SYN队列（半连接队列）：  <font color="red"> __三次握手:__ 当服务器端收到客户端的SYN报文时，会响应SYN/ACK报文，然后连接就会进入SYN RECEIVED状态，处于SYN RECEIVED状态的连接被添加到SYN队列，并且当它们的状态改变为ESTABLISHED时，即当接收到3次握手中的ACK分组时，将它们移动到ACC队列。</font> SYN队列的大小由内核参数/proc/sys/net/ipv4/tcp_max_syn_backlog设置。

2. ACC队列（完全连接队列）：ACC队列存放的是已经完成TCP三次握手的连接，而`accept()`系统调用只是简单地从ACC队列中取出连接而已，并不是调用accept函数才会完成TCP三次握手，ACC队列的大小通过`int backlog`参数控制。 

__`accept()`函数不参与三次握手，而只负责从完全连接队列中取出一个连接__

---

#### `accept()`

``` C 
#include <sys/socket.h>
int accept(int sockfd,struct sockaddr *addr,socklen_t *addrlen);
```

`accept()` 提取出所监听套接字的等待连接队列中第一个连接请求，创建一个新的套接字，并返回指向该套接字的文件描述符。新建立的套接字不在监听状态，原来所监听的套接字也不受该系统调用的影响。

