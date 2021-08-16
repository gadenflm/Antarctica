#include <stdio.h>

int main(int argc, char const *argv[])
{
	/* code */
	int w;
	scanf ("%d",&w);
	if (( w % 2 ) == 0 )
		if ( w  == 2 )
			printf("%s\n","NO" );
		else
		 	printf("YES\n");	
	else 
		printf("NO\n");

	return 0;
}