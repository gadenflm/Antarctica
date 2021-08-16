/**
 * sizeof.c -- 输出不同变量类型的占用字节。思考对应大小及内存分配
 * K_gadenflm 2021年5月1日21:32:09
 */
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <complex.h>
int main () {
	// sizeof 为一元运算符（一个运算对象），返回类型size_t

	size_t int_size = sizeof(int);
	size_t uint_size = sizeof(unsigned int);
	size_t long_size = sizeof(long);

	// sizeof(int):4 illustrates that int type has 4 Byte, 32 bit. （64位电脑）
	printf("******************************************************* Integer *******************************************************\n");
	printf("sizeof(int): %zu\n", int_size);
	printf("max value of int: %zu\n", INT_MAX);
	printf("4 Bytes (32 bits, 2^32): %lf\n", pow(2, 32) );
	printf("max value of uint: %zu\n", UINT_MAX);
	putchar('\n');

	printf("sizeof(short int): %hd\n", sizeof(short int)); // short int 转义符为 %hd -32768~32767;
	printf("sizeof(unsigned int): %zu\n", uint_size);
	printf("sizeof(long): %zu\n", long_size);
	printf("sizeof(long long): %zu\n", sizeof(long long));
	putchar('\n');
	putchar('\n');

	printf("******************************************************* Float *******************************************************\n");
	// float 最大值推算: 4 Bytes, 32 bits, 1位符号位, 8位指数位， 23位尾数位
	// 0 11111111 11111111111111111111111
	// 指数位: 8, 2^8=256 ==> (-128,128)
	// 2^128 ==> ( -3.4E38 , 3.4E38 )
	// 尾数位: 23,对应7(6)位十进制数，所以有效数字是7(6)位
	printf("sizeof(float): %zu\n", sizeof(float));
	printf("max value of float: %f\n", FLT_MAX);

	putchar('\n');

	// float 最大值推算: 8 Bytes, 64 bits, 1位符号位, 11位指数位， 52位尾数位
	printf("sizeof(double): %zu\n", sizeof(double));
	printf("max value of doulbe: %llf\n", DBL_MAX);
	printf("sizeof(long double): %zu\n", sizeof(long double));
	putchar('\n');
	putchar('\n');

	printf("******************************************************* Other *******************************************************\n");
	printf("sizeof(char): %zu\n", sizeof(char));
	printf("sizeof(_Bool): %zu\n", sizeof(_Bool));
	printf("sizeof(size_t): %zu\n", sizeof(size_t));
	putchar('\n');
	putchar('\n');

	printf("******************************************************* Complex *******************************************************\n");
	// include "complex.h" 后可以使用复数类型，
	// float complex, double complex, long double complex，分别对应 两个 float 数...(real,imaginary)
	printf("sizeof(float complex): %zu\n", sizeof(float complex));
	printf("sizeof(long double complex): %zu\n", sizeof(long double complex));
	getchar();
}