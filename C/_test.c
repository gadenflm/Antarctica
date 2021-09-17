#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main()
{
	time_t cur_time = time(NULL);
	printf("%s\n", ctime(&cur_time));

	while (getchar() != 'q');
	return 0;
}