/**
 * K_gadenflm 2021年5月24日21:34:07
 * 利用指针为函数参数，示例：交换传入参数的值
 */
#include <stdio.h>
void swap_int(int * p_int1, int * p_int2);

int main() {
	int var1, var2;
	var1 = 1;
	var2 = 2;
	printf("The value of var1: %d, var2: %d. \n", var1, var2 );
	swap_int(&var1, &var2);
	printf("After swap, the value of var1: %d, var2: %d.\n", var1, var2);
	getchar();
}

void swap_int(int * p_int1, int * p_int2) {
	int temp;
	temp = *p_int2;
	*p_int2 = *p_int1;
	*p_int1 = temp;
	return;
}