#include <stdio.h>
#define MAX_VAL 1000000
#define MAX_N 1000
int main() {
	int n, i, j, sum;
	int a[MAX_N];
	_Bool no_prime_num[MAX_VAL] = {0, 1};
	i = 2;
	while (i <= (MAX_VAL / 2)) {
		for (j = 2; j * i <= MAX_VAL; j++) {
			no_prime_num[j * i] = 1;
		}
		i++;
	}
	sum = 0;
	scanf("%d", &n);
	for (i = 0; i < n; i++) {
		scanf("%d", &a[i]);
		sum += no_prime_num[a[i]];
	}
	printf ("%d %d\n", n - sum, n);
	// getchar();
	// getchar();
}
