#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 20001

int main()
{

	int total,n,i,j,m;
	int a[MAX_N][MAX_N] = {0};
	char b[MAX_N]={0};

	scanf("%d",&total);
	for (int ind = 0; ind < total; ++ind)
	{
		scanf ("%d",&n);
		int f = 0;
		for ( i =0 ; i < n ; ++i)
		{
			scanf ("%d",&a[i][0]);
			for (j = 1 ; j<=a[i][0] ; ++j)
				scanf ("%d",&a[i][j]);
		}
		m=0; 
		memset(b, 0, MAX_N);
		while (f < n)
		{
			for (i = 0; i < n; ++i) 
			{
				int flag = 1;
				for (j = 1 ; j<=a[i][0] ; ++j)
					if (b[a[i][j]] == 0) 
					{
						flag = 0;
						break;
					}
				if (flag) 
				{
					f++;
					b[i] = 1;
				}
			}
			m++;
		}
		printf("%d\n",m);
	}

	char ch2;
	while ((ch2= getchar()) != 'q');

	return 0;
}