#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <time.h>
#include <unistd.h>

static int a[51];
static int out[51][3];
static int b[51] = {0};

/*
现场写的思路有错误,

新思路
排序a[i] (nlogn ~ n^2)

sort k 次 (kn ~ n^2)
如果 前 i-1 个值都在对的位置 则
把 第i个值 交换到对的位置. 

k 最多 n 次

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

		ret = 0;
		flag = 1;
		while (flag) 
		{
			i = 0;
			while ( (i < n-1) && (a[i+1] >= a[i]) ) ++i;
			if (i == n-1) 
			{
				flag = 0;
			}	
			if (flag) {
				out[ret][0] = i+1;
				out[ret][1] = a[i];
				++i;
				while ( (i < n-1) && (a[i+1] <= out[ret][1]) ) ++i;
				// while ( (i < n-1) && (a[i+1] >= a[i]) ) ++i;
				out[ret][1] = i+1;

				// printf("[kl] %d %d\n",out[ret][0], out[ret][1]);
				// sleep(1);
				
				d = 0; 
				int ii = out[ret][0]-1;
				int jj = out[ret][1]-1;
				while ((d < out[ret][1] - out[ret][0]) && (a[jj] <= a[ii])) {
					++ii;
					++d; 
				}
				shift (&a[out[ret][0]-1],out[ret][1] - out[ret][0] + 1, d);
				out[ret][2] = d;
	
				for (int iii = 0; iii < n; ++iii)
				{
					printf("%d ",a[iii]);
				}
				putchar('\n');

				ret++;
			}
		}
		printf("%d\n", ret);
		for (i = 0; i < ret; ++i )
			printf("%d %d %d\n",out[i][0], out[i][1], out[i][2]);
	}

    char ch2;
    while ((ch2= getchar())  != 'q');
	return 0;
}