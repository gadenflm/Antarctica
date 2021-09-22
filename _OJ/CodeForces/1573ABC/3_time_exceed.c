#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 200000
static int* a[MAX_N] = {0};
int main()
{

	int total,n,i,j,m,k;

	char b[MAX_N]={0};

	scanf("%d",&total);
	for (int ind = 0; ind < total; ++ind)
	{
		scanf ("%d",&n);
		int f = 1;
		for ( i =0 ; i < n ; ++i)
		{
			scanf ("%d",&k);
			if (k == 0) f = 0;
			a[i] = (int*) malloc (sizeof(int) * (k+1));
			memset(a[i], 0 , sizeof(int) * (k+1));
			a[i][0] = k;
			for (j = 1 ; j<=a[i][0] ; ++j)
				scanf ("%d",&a[i][j]);
		}
		if (f == 1) 
		{
			printf("%d\n",-1);
			continue;
		}
		m=0; 
		memset(b, 0, MAX_N);
		while (f < n)
		{
			for (i = 0; i < n; ++i) 
			{
				if (b[i] != 0) continue;
				int flag = 1;
				for (j = 1 ; j<=a[i][0] ; ++j)
					if (b[a[i][j]-1] == 0) 
					{
						// printf("%d: %d %d\n",m,i,j);
						flag = 0;
						break;
					}
				if (flag) 
				{
					// printf("%d: %d\n",m,i);
					f++;
					b[i] = 1;
				}
			}
			m++;
		}
		printf("%d\n",m);
	}

	// char ch2;
	// while ((ch2= getchar()) != 'q');

	return 0;
}