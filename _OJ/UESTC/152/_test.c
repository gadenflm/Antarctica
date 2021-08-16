#include <stdio.h>

int main() {
	int n, i, c, s;
	float sum1, sum2;
	scanf ("%d", &n);
	sum1 = 0; sum2 = 0;
	for (i = 0; i < n; i++) {
		scanf ("%d %d", &c, &s);
		if (s < 60 ) s = 0;
		if (s < 70 && s >= 60) s = 1;
		if (s < 80 && s >= 70) s = 2;
		if (s < 90 && s >= 80) s = 3;
		if (s <= 100 && s >= 90) s = 4;
		sum1 += c * s;
		sum2 += c;
	}
	printf ("%.2f",  (sum1 / sum2));
	// getchar();
	// getchar();
}