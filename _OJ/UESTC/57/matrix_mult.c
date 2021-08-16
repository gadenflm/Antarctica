#include <stdio.h>

int main()
{
	int n,i,j,t,k,s;
	int a[20][20];
	int b[20][20];

	scanf("%d",&n);
	for (t=0;t<n;t++)
	{
		
		for (i=0;i<3;i++)
			for (j=0;j<3;j++)
				scanf("%d",&a[i][j]);
		for (i=0;i<3;i++)
			for (j=0;j<3;j++)
				scanf("%d",&b[i][j]);

		for (i=0;i<3;i++)
		{
			for (j=0;j<3;j++)
				{
					s=0;
					for (k=0;k<3;k++)
						s=s+a[i][k]*b[k][j];
					printf("%d ", s);			
				}
			printf("\n");
		}

	}	
	return 0;
}