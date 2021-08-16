#include <stdio.h>

#define MAX_N 10

int main ()
{
	int a[MAX_N][MAX_N] = {1, 0};
	for (int i = 1; i < MAX_N; i++)
		for (int j = 0; j < i + 1; j++)
		{
			if (j == 0 || j == i) a[i][j] = 1; else a[i][j] = a[i - 1][j - 1] + a[i - 1][j];
		}

	for (int i = 0; i < MAX_N ; i++ ) {
		for (int j = 0 ; j < i + 1; j++) {printf("%d ", a[i][j]);}
		putchar('\n');
	}
}