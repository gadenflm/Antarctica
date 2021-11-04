#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

static uint8_t a[32];
static int len;

static int max = (int)(pow(10,9)) + 7;

void dec2bin(int d)
{
	int i = 0;
 	while (d > 0) {
        a[i] = d % 2;
        ++i;
        d = d / 2;
    }
    len = i - 1;
    return ;
}

int main() 
{
	int t,ind,n,k,i;
	int64_t sum, m;
	scanf("%d",&t);
	for (ind = 0; ind < t; ++ind)
	{
		scanf("%d %d",&n,&k);
		dec2bin(k);
		// for (i = len; i >=0 ; --i)
		// {
		// 	printf("%d",a[i]);
		// }
		// putchar('\n');
		sum = 0; 
		for (i = 0; i <= len; ++i)
		{
			m = 0;
			if (a[i] == 1)
			{
				m = 1;
				for (register int j = 0; j < i; ++j)
				{
					m *= n;
					if (m > (max)) 
						m = m % max;
				}
			}
			sum += m;
			if (sum > (max)) 
				sum = sum % max;
		}
		printf("%lld\n",sum);
	}	
	// char ch;
	// while ((ch = getchar())!='q');
	return 0;
}