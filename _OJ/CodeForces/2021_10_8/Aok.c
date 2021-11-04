#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main() 
{
	int t,ind;
	int64_t n,r,l,i;
	scanf("%d",&t);
	r = 0;
	l = 0;
	for (ind = 0; ind < t; ++ind)
	{
		scanf("%lld",&n);
		l = -(n-1);
		r = n;
		printf("%lld %lld\n",l,r);
	}	
	// char ch;
	// while ((ch = getchar())!='q');
	return 0;
}