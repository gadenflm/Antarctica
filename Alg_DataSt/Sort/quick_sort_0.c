/**
 * Quick Sort.c by K_gadenflm
 *
 * @Time Complexity		O(nlg(n))
 * @breif				Quick Sort is a unstable sort Alg.
 *
 *           			Bad performance for short array.
 *
 *           			If the array is sorted or local-sorted, Quick Sort Alg. will take
 *           			much time. In the worst case, Quick Sort will take O(n^2).
 *
 *						The mid pointer for each times is always set as right,
 *						So search the arrry begin from left.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define NUM 		15
#define RAND_START  0
#define RAND_END	99
#define DEBUG_FLAG	1

static int arr[NUM] = {0};

static int rand_int(int start, int end) {
	int range = end - start + 1;
	return rand() % range + start;
}

static void array_init() {
	srand((unsigned)time(NULL));
	for (int i = 0; i < NUM; i++) {
		arr[i] = rand_int(RAND_START, RAND_END);
	}
	return;
}

static void quick_sort(int* a, int left, int right) {
	if (left >= right) return;

	int mid = a[right];
	int ll = left;
	int rr = right;

	while (left < right) {
		while (left < right && a[left] <= mid) left++;
		a[right] = a[left];
		while (left < right && a[right] >= mid) right --;
		a[left] = a[right];
	}
	a[left] = mid;

#if DEBUG_FLAG
	for (int i = 0; i < NUM; i++) {
		if (i == left)  printf ("\033[1;31m%d  \033[0m", a[i]);
		else if (i == ll)  printf ("\033[1;32m%d  \033[0m", a[i]);
		else if (i == rr)  printf ("\033[1;34m%d  \033[0m", a[i]);
		else printf ("%d  ", a[i]);
	}
	putchar('\n');
#endif

	quick_sort(a, ll, left - 1);
	quick_sort(a, left + 1 , rr);
	return;
}

int main()
{
#if DEBUG_FLAG
	printf ("\033[1;32m Left \033[0m \t \033[1;31m Mid \033[0m \t \033[1;34m Right \033[0m \n");
#endif
	array_init();
	printf("Here is a random array[%u]:\n", NUM);
	for (int i = 0; i < NUM; i++) {
		printf("%d  ", arr[i]);
	}
	putchar('\n');
	putchar('\n');

	quick_sort(arr, 0, NUM - 1);

	printf("\nQuick sorted array[%u]:\n", NUM);
	for (int i = 0; i < NUM; i++) {
		printf("%d  ", arr[i]);
	}
	putchar('\n');
	return 0;
}