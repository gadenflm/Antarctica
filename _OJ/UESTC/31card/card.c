#include <stdio.h>

int main()
{
	int n,m,i,j,t,max;
	long sum;
	int value[1100];
	scanf ("%d",&n);
 	while (n!=0)
 	{
 		for (i=0;i<1001;i++)
 		{
 			value[i]=0;
  		}
  		max=0;
  		sum=0;
		for (i=0;i<n;i++)
		{
			scanf("%d",&t);
			sum=sum+t;
			for (j=1000;j>0;j--)
			{
				if  (value[j] > 0 ) value[t+j]=1;	
			}
			value[t]=1;
			if (t>max) max=t;
		}
		scanf("%d",&m);

		if (m>=6) 
		{
			for (i=m-5;i>0;i--)
			{
				if  (value[i] > 0 ) 
				{
					if (i==sum) max=0;
					m=m-i;
					break;
				}
			}
		}
		printf("%d\n",m-max);


		scanf ("%d",&n);
	}
	return 0;
}