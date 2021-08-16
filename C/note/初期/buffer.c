/**
 * buffer.c 缓冲区的一些记录
 * K_gadenflm 2021年5月1日22:34:39
 */

/*如果没有自行设置缓冲区的话，系统会默认为标准输入输出设置一个缓冲区，
这个缓冲区的大小通常是512个字节的大小。
缓冲区大小由 stdio.h 头文件中的宏 BUFSIZ 定义

scanf( ) 、getchar( ) 是带有缓冲区的。遇到 scanf( ) 和getchar( )函数，
程序会先检查输入缓冲区中是否有数据。如果有数据，scanf( ) 会直接读取，不会等待用户输入；*/

#include <stdio.h>
#include <conio.h>
int main () {
	//getchar(): 从输入队列中返回下一个字符， getchar 与 putchar 速度快于scanf 和 printf.
	//scanf会忽视空格、换行，getchar()不会！
	char ch;
	//输出系统缓冲区默认大小
	printf ("The default Buffer Size: %d\n", BUFSIZ);

	// 行缓冲I/O，按下enter键刷新缓冲区，对应还有完全缓冲I/O,缓冲区被填满时才刷新，多见于File I/O.
	printf ("Input any characters, # to skip! Output with buffer.\n");
	while ((ch = getchar()) != '#') putchar(ch);
	putchar('\n');

	//清空缓冲区，否则上一次输出的"\r\n"会对后续输入造成影响.在 Linux GCC 下可能无效.
	//while ( ( ch=getchar() ) != '\n') continue; 也可以，持续读取流文件中的 \n.
	fflush(stdin);

	//getche()直接读入刷新缓冲区
	printf ("Input any characters, Press Enter to exit! Output without buffer, using gerche().\n");
	while ((ch = getche()) != '\r') putchar(ch);

}