#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <time.h>
// #include <unistd.h>

static int a[51];
static int out[51][3];
static int b[51] = {0};

static int quicksort(int* aa, int left, int right) 
{
	if (left >= right) return 0;
	int ll = left, rr = right;
	int mid = aa[right];
	while (left < right)
	{
		while (left < right && aa[left] <= aa[right]) ++left;
		aa[right] = aa[left];
		while (left < right && aa[right] >= aa[left]) --right;
		aa[left] = aa[right];
	}
	aa[left] = mid;
	quicksort(aa, ll+1, left);
	quicksort(aa, left, rr-1);
}

/*
1
9
8 7 3 59 3 2 34 -1 29
 */

static inline int shift(int* aa, int length, int d)
{
	memcpy(b,aa,sizeof(int)*(d));
	memcpy(aa,aa+d,sizeof(int)*(length-d));
	memcpy(aa+(length-d),b,sizeof(int)*(d));
	return 0;
}

int main()
{
	int tt,index;

	scanf("%d",&tt);
	for (int index = 0; index < tt ; ++ index) 
	{
		int n, i, flag, ret, d;
		scanf ("%d",&n);
		for (i = 0; i< n; ++i) {
			scanf("%d",&a[i]);
		}
		quicksort(a,0,n-1);
		for (i = 0; i < n; ++i) 
		{
			printf("%d\n",a[i]);
		}
	}

    char ch2;
    while ((ch2= getchar())  != 'q');
	return 0;
}