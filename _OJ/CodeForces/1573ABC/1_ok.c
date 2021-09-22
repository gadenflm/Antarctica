#include <stdio.h>
#include <stdlib.h>

int main()
{
	int total,n,i,m;
	char s[100];
	char ch;
	scanf("%d",&total);
	for (register int ind = 0; ind < total; ++ind)
	{
		scanf("%d",&n);
		scanf("%s",s);
		m=0;
		for (i=0; i < n; i++)
		{
			ch = s[i];
			if (i<n-1 && ch != '0') ++m;
			m=m+(ch-'0');
		}
		printf("%d\n",m);
	}
	return 0;
}