#include <stdio.h>

#define MAX_N 6

const static int direc[4][2] = {{0, 1}, {1, 0}, {0, -1}, { -1, 0}};

int main ()
{
	int a[MAX_N][MAX_N] = {0};
	int i = 0, j = 0, num = 1, dir = 0;
	while (num <= MAX_N * MAX_N)
	{
		a[i][j] = num;
		i += direc[dir][0];
		j += direc[dir][1];
		if (i == MAX_N  || j == MAX_N || i < 0 || j < 0 || a[i][j] != 0) {
			i -= direc[dir][0];
			j -= direc[dir][1];
			dir ++;
			if (dir == 4) dir = 0;
			i += direc[dir][0];
			j += direc[dir][1];
		}
		num++;
	}

	for (i = 0; i < MAX_N ; i++) {
		for (j = 0 ; j < MAX_N ; j++)
		{
			printf ("%d\t", a[i][j]);
		}
		putchar('\n');
	}
}