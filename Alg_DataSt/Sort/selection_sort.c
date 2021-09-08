/**
 * Selection Sort.c by K_gadenflm
 *
 * @Time Complexity		O(n^2)
 * @breif				Selection sort is a stable sort Alg. , generally quick than bubble sort.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM 		10
#define RAND_START  0
#define RAND_END	99

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

static void selection_sort() {
	int temp = 0;
	for (int i = 0; i < NUM - 1; i++)
		for (int j = i + 1; j < NUM; j++) {
			if (arr[i] > arr[j]) {
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}

	return;
}

int main()
{
	array_init();
	printf("Here is a random array[%u]:\n", NUM);
	for (int i = 0; i < NUM; i++) {
		printf("%d  ", arr[i]);
	}
	putchar('\n');
	putchar('\n');

	selection_sort();

	printf("Selection sorted array[%u]:\n", NUM);
	for (int i = 0; i < NUM; i++) {
		printf("%d  ", arr[i]);
	}
	putchar('\n');
	return 0;
}