#include <stdio.h>
#include <stdint.h>
#include <string.h>

double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size) {
	int flag = 0, index = 0, i = 0, j = 0;
	int a[2001] = {0};
	double ret = 0;
	if ( (nums1Size + nums2Size) % 2 == 0 ) {
		flag = 1;
		index = (nums1Size + nums2Size) / 2;
	} else {
		index = (nums1Size + nums2Size) / 2 + 1;
	}
	if (nums1Size == 0) {
		for (j = 0 ; j < nums2Size; j++) {
			if (i + j + 1 == index) {
				if (flag) {
					ret = ((double)nums2[j] + (double)nums2[j + 1]) / 2;
				} else {
					ret = nums2[j];
				}
				return ret;
			}
		}
	}

	if (nums2Size == 0) {
		for (i = 0 ; i < nums1Size; i++) {
			if (i + j + 1 == index) {
				if (flag) {
					ret = ((double)nums1[i] + (double)nums1[i + 1]) / 2;
				} else {
					ret = nums1[i];
				}
				return ret;
			}
		}
	}
	int ind = 0;
	int* p1 = &nums1[0];
	int* p2 = &nums2[0];
	while (ind < nums1Size + nums2Size) {
		while (i < nums1Size && (*p1) < (*p2)) {
			a[ind++] = (*p1);
			i++;
			if (i < nums1Size) p1++;
		}
		if (i == nums1Size || (*p1) < (*p2)) {
			while (j < nums2Size) {
				a[ind++] = (*p2);
				j++;
				if (j < nums2Size) p2++;
			}
		}
		while (j < nums2Size && (*p1) >= (*p2)) {
			a[ind++] = (*p2);
			j++;
			if (j < nums2Size) p2++;
		}
		if (j == nums2Size || (*p1) >= (*p2)) {
			while (i < nums1Size) {
				a[ind++] = (*p1);
				i++;
				if (i < nums1Size) p1++;
			}
		}

		// for (int ii = 0 ; ii < nums1Size + nums2Size; ii++) printf("%d ", a[ii]);
		// putchar('\n');
	}
	if (flag) {
		printf("%d \n", index);
		return ((double)a[index - 1] + (double)a[index ]) / 2;
	} else {
		return (double)a[index - 1];
	}
}

int main() {
	int num1[] = {1, 2};
	int num2[] = {3, 4};

	printf("%llf\n", findMedianSortedArrays(num1, 2, num2, 2));
	char ch;
	while ( (ch = getchar() != 'q') ) ;
}