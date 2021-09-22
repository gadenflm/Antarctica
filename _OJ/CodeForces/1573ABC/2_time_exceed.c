#include <stdio.h>
#include <stdlib.h>

int main()
{
	int total,n,m;
	int a[100001]={0};
	int b[100001]={0};
	register int i,j;
	scanf("%d",&total);
	for (register int ind = 0; ind < total; ++ind)
	{
		scanf ("%d",&n);
		for (i=0; i<n; ++i)
			scanf ("%d",&a[i]);
		for (i=0; i<n; ++i)
			scanf ("%d",&b[i]);
		m=2*n+1;

		for (i=0; i<n; ++i)
		{
			if (i>=m ) break;
			for (j=0; j<n; ++j)
			{
				if (j>=m) break;
				if (a[i]<b[j]) 
				{
					if ((i+j)<m)
					{
						m=i+j;

					}
				}
			}
		}
		printf("%d\n",m);
	}
	// char ch2;
	// while ((ch2= getchar()) != 'q');

	return 0;
}