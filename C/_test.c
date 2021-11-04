#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

int func(int n)
{
	int k = n-1;
	while (n % k == 0) --k;
	return k;
}

int main() 
{
	int t,ind,n,i,m;
	char c;
	char s[300001];
	char a[300001];
	scanf("%d",&t);
	for (ind = 0; ind < t; ++ind)
	{
		scanf("%d %c",&n,&c);
		scanf("%s",s);
		m = 0;
		for (i = 0; i < n; ++i)
		{
			if (s[i] == c)
			{
				a[i]  = 0;
			} else 
			{
				a[i] = 1;
				++m;
			}
		}
		if (m == 0) 
		{
			printf ("0\n"); continue;
		}

		if (m == 1)
		{
			printf ("1\n");
			if (a[n-1] == 1) 
				m = func(n);
			else 
				m = n;
			printf ("%d\n",m);
			continue;
		}

		if (a[n-1] == 1) 
		{
			printf ("2\n");
			printf ("%d %d\n",n,func(n));	
		}
		else{
			printf ("1\n");
			printf ("%d\n",n);
		}			
	}	
	// char ch;
	// while ((ch = getchar())!='q');
	return 0;
}