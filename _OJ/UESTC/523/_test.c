/**
 * K_gadenflm , UESTC OJ 523.
 */

#include <stdio.h>

int main() {
	int i, j, ind, n, t, m;
	scanf ("%d %d", &n, &t);
	for (ind = 0; ind < t; ind++) {
		scanf("%d", &m);
		i = n + 1 - (m % n);
		for (j = 0; j < n; j++) {
			i = (i <= n ? i : 1);
			printf("%d ", i++ );
		}
		putchar('\n');
	}
	// while ((getchar()) != 'q') {};
	return 0;
}