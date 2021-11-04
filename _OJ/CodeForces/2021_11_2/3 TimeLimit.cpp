#include <iostream>
#include <cmath>
#include <cstring>
#include <stdint.h>

using namespace std;
static int* aa = (int*)malloc(200001 * sizeof(int));

static void SelectionSort(int *arr, int num) 
{
	int temp = 0;
	for (register int i = 0; i < num - 1; ++i)
		for (register int j = i + 1; j < num; ++j) 
		{
			if (arr[i] > arr[j]) 
			{
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	return;
}
static void QuickSort(int* a, int left ,int right)
{
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

	QuickSort(a, ll, left - 1);
	QuickSort(a, left + 1 , rr);
	return;
}

static void MinimumExtration()
{
	int n, i, max;
	int* a = nullptr;

	a = aa;
	scanf("%d",&n);
	for (i = 0; i < n ; ++i)
		scanf("%d",&a[i]);
	if (n < 30)
		SelectionSort(a,n);
	else
		QuickSort(a,0,n-1);  // n2
	// if (a[0] <=0 && (n > 1))
	// {
	// 	int m = 0, j = 0;
	// 	while ( (j < n - 1) && (a[j] <= 0))
	// 	{
	// 		if (a[j+1] > 0)
	// 		{
	// 			m -= a[j];
	// 			break;
	// 		}
	// 		++j;
	// 	}
	// 	a = a + (j + 1);
	// 	n = n - (j + 1);
	// 	for (i = 0; i < n; ++i)
	// 		a[i] += m;
	// }
	max = a[0];
	while (n > 1)
	{
		if (a[0] > 0 && (a[n-1] <= max + 1))
		{
			break;
		}
		for (i = 1; i < n; ++i)
			a[i] -= a[0];
		a = a + 1;
		--n;
		if (a[0] > max)
			max = a[0];
	}
	// if (a[0] > max)
	// 	max = a[0];
	printf("%d\n",max);
	return;
}

int main ()
{
	int tt;
	cin >> tt;
	for (register int ind = 0; ind < tt ; ++ind)
	{
		MinimumExtration();
	}
	free(aa);
 	// char ch;
 	// while((ch = getchar())!='q');
 	return 0;
}