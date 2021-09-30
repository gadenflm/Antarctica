#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int t,i,j;
	char str[51];
	char* ps;
	scanf("%d",&t);

	for (int ii = 0; ii < t ; ++ii) 
	{
		scanf("%s",str);
		ps = str;
		i=0;
		j=0;
		do 
		{
			if ((*ps) == 'B') {
				i++;
			} else {
				j++;
			}
			ps++;
		} while ( (*ps) != '\0');
		// printf("%d : %d\n",i,j);
		if (j == i) 
			printf("YES\n");
		else
			printf("NO\n");
	}

	// char ch2;
	// while ((ch2= getchar()) != 'q');

	return 0;
}