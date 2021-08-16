/**
 * K_gadenflm 2021年6月8日21:56:52 指针与数组
 */
#include <stdio.h>
#define SIZE 4
/**
 * 输出指针加法与数组对应关系
 */
void plt_array_addr(void);
void plt_double_arr_addr(void);
int main (void) {

	plt_array_addr();
	plt_double_arr_addr();
	getchar();
}

//ar[ind] 与 *(ar+ind) 等价 !!!!!!!!!!!!!!!
//数组名与该数组首元素的地址等价!!!!!!!!!!!!
void plt_array_addr(void) {
	short s_arr[SIZE] = {0, 1, 2, 3};
	double d_arr[SIZE] = {0, 1, 2, 3};

	// s_arr == &s_arr[0]
	// 数组名与该数组首元素的地址等价！
	short* p_s = s_arr;
	double* p_d = d_arr;

	//指针 加1 运算指的是加一个存储单元， 所以申明指针必指定所指向对象类型。!!
	printf("sizeof(short):%u, sizeof(double):%u\n", sizeof(short), sizeof(double));
	putchar('\n');
	printf("%23s %10s\n", "short", "double");
	for (int index = 0 ; index < SIZE ; index++) {
		printf ("pointers + %d: %10p %10p\n", index, p_s + index, p_d + index);
	}

	for (int index = 0 ; index < SIZE ; index++) {
		printf ("values   + %d: %10d %10lf\n", index, *(p_s++), *(p_d++));
	}

	// 指针作差返回 相差 类型长度的个数
	printf ("pointer of array last item - &arr[0]:%d\n", --p_s - s_arr);
	return;
}

/**
 * 就int arr[4][2] 而言
 * arr == & arr[0]   二维数组类型的首个地址，指向的每个元素都是内含两个int类型的一维数组！！！！
 * *arr == arr[0] == &arr[0][0]  二维数组类型的首个地址，指向是int类型的变量！！！！
 * **arr == arr[0][0]   二维数组的首个int元素的值。
 *
 * arr + 2  二维数组类型三个元素的地址，指向的每个元素都是内含两个int类型的一维数组
 * *(arr + 2) 二维数组第三个元素的首元素地址，指向是int类型的变量
 * *(arr + 2) + 1 二维数组第三个元素的第二个int 类型元素的地址
 * *(*(arr + 2) + 1) == arr[2][1] !!
 */
void plt_double_arr_addr(void) {
	putchar ('\n');
	short int arr[SIZE][2] = {{0, 1}, {2, 3}, {4, 5}, {6, 7}};
	int (* p_arr)[2]; 	//对应指针变量要这样申明，解释见note.md
	printf("sizeof ( short int ):%u, Here is a short int arr[4][2]:\n", sizeof (short int) );
	putchar ('\n');
	printf ("  arr = %p,   arr + 1 = %p\n", arr, arr + 1);
	printf (" *arr = %p,  *arr + 1 = %p\n", *arr, *arr + 1);
	printf ("arr[0]= %p,  arr[0] + 1 = %p\n", *arr, *arr + 1);

	printf (" *(*(arr + 3) + 1) = %d\n", *(*(arr + 3) + 1));
	return;
}