/**
 * logic.c 逻辑表达式的一些记录
 * K_gadenflm 2021年5月1日22:13:07
 */
#include <stdio.h>
#include <math.h>
/*
判断两浮点数是否相等不要用==,用下面的函数形式
 */
_Bool float_is_equal (float var_1, float var_2) {
	// function fabs() from math.h, return the absolute value of float variable.
	return ( fabs(var_1 - var_2) < 0.00001 );
}


int main() {
	// _Bool 类型本质为 Short int, 只能用%d打印, 1 == true, 0 == false.
	printf("%f equals %f: %d\n", 0.5, 0.5, float_is_equal(0.5, 0.5));

	// Boolean_express? Value1 : Value2 ;
	int max = (2 > 3) ? 2 : 3;
	printf("max of {2,3}: %d\n", max);

	//C语言 保证逻辑表达式的求值顺序从左往右，且一旦发现某个元素让整个表达式无效，则立即返回！
	// ;为空语句
	int var1;
	printf ("Input 0 or 6 to continue.\n");
	// 先判断var1是否为0 ，等于0 则整体表达式返回。 防止除0
	while (scanf("%d", &var1), (var1 != 0 && 12 / var1 != 2)) ;

	printf ("Input Space or N to exit.\n");
	char c;
	//先从输入队列中返回得到c，执行c！=' '时保证c已经获得值.
	while ( (c = getchar()) != ' ' && c != 'N');

}