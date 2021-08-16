#include <stdio.h>

int main()
{
	int n,i,j,t,k,s,aa,bb,cc;
	int a[20][20];
	int b[20][20];

	scanf("%d",&n);
	for (t=0;t<n;t++)
	{
		scanf("%d %d %d",&aa,&bb,&cc);

		for (i=0;i<aa;i++)
			for (j=0;j<bb;j++)
				scanf("%d",&a[i][j]);
		for (i=0;i<bb;i++)
			for (j=0;j<cc;j++)
				scanf("%d",&b[i][j]);

		for (i=0;i<aa;i++)
		{
			for (j=0;j<cc;j++)
				{
					s=0;
					for (k=0;k<bb;k++)
						s=s+a[i][k]*b[k][j];
					printf("%d ", s);			
				}
			printf("\n");
		}

	}	
	return 0;
}