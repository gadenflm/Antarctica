/**
 *  echo_eof.c -- 配合bat文件演示重定向过程，将输入输出分别重定向到test.txt
 *  K_gadenflm 2021年5月2日21:35:59
 */
#include <stdio.h>
int main () {
	// char -> 1Byte, 8bit, 0~255. 不能去负数, EOF 对应-1,所以不能用char
	int ch;
	// MS-windows 下 EOF 对应Ctrl-Z
	fflush(stdin);
	while ( (ch = getchar()) != EOF) putchar (ch);
}