/**
 * operator.c 运算符的一些记录
 * K_gadenflm 2021年5月1日21:43:24
 */
#include <stdio.h>
int main() {
	// 进制自动转换:
	int decimal_var = 0B01001100;
	char char_var = 0B01001100;
	printf("decimal_var = 0B01001100: %d\n", decimal_var);
	printf("char_var = 0B01001100: %c\n", char_var);

	printf("%%o: %o\n", char_var);
	printf("%%x: %x\n", char_var);

	putchar('\n');

	// 赋值运算符：把值存储到申请的内存位置处（执行顺序从右往左）。
	// 赋值表达式会返回赋值运算符左侧的值。
	int var1, var2, var3;
	printf("var1=var2=var3=19970726: %d\n", var1 = var2 = var3 = 19970726);
	putchar('\n');

	//逗号可做运算符和分隔符，此处为运算符逗号
	//运算符逗号
	//1. 被分隔的表达式从左往右运算，左侧项的副作用一定在程序执行右侧项之前发生。
	//2. 整个逗号表达式的值是右侧项的值。
	int var_dot;
	printf("var_dot=(507,726): %d\n", var_dot = (507, 726));
	// 常用于for循环
	int i, j;
	for (i = 0, j = 1; (i <= 5) && (j <= 5) ; i++, j++) {
		printf("i: %d, j: %d.\n", i, j);
	}

	getchar();
}
