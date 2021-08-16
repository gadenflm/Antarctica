#include <stdio.h>
int main() {
	int ind, l, m, i, sum, a, b;
	_Bool has_no_tree[10001] = {0};
	scanf("%d %d", &l, &m);
	for (ind = 0; ind < m; ind++) {
		scanf ("%d %d", &a, &b);
		for (i = a; i <= b; i++) {
			has_no_tree[i] = 1;
		}
	}
	sum = 0;
	for (i = 0; i < 10001; i++) {
		sum += has_no_tree[i];
	}
	printf("%d\n", l - sum + 1 );
	// getchar();
	// getchar();

}