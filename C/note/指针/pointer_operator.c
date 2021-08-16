/**
 * K_gadenflm 2021年5月24日21:26:44
 * 记录指针申明及操作符测试(以long long int)为例
 */
#include <stdio.h>

void sub_func(void);

int main (void) {
	//int 改成 long long int,要注意输出转义符%lld,
	//printf()输出地址用%p remains pointer
	long long int var1, var2;
	var1 = 970726;
	var2 = 970507;

	// 申明指针变量： 类型说明符表明指针所指向的类型，*表明申明的变量是一个指针
	// *与变量名之间的空格可有可无，通常申明时保留空格，作为解引用运算符时保留空格
	long long int * addr1, * addr2;
	addr1 = &var1;
	addr2 = &var2;

	//&为取地址运算符，返回为常数!!（右值）
	//*为间接运算符，根据地址返回地址中的值
	printf("In main function, var1 = %lld, addr1 = %p, var2 = %lld, addr2 = %p\n", var1, addr1, var2, addr2);
	printf("In main function, var1 = %lld, *addr1 = %lld, var2 = %lld, *addr2 = %lld\n", var1, *addr1, var2, *addr2);
	putchar('\n');

	sub_func();
	getchar();
}

void sub_func(void) {
	int var1, var2;
	var1 = 970726;
	var2 = 970507;
	int* addr1 = &var1;
	int* addr2 = &var2;

	printf("In sub_func(void), var1 = %d, addr1 = %p, var2 = %d, addr2 = %p\n", var1, addr1, var2, addr2);
}

