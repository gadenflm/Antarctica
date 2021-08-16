/**
 * K_gadenflm , UESTC OJ 60.
 */

#include <stdio.h>
#include <string.h>
int main() {
	int n, count, index, flag;
	char ch;
	char str[201];
	char *p_str = NULL;
	scanf("%d", &n); getchar();
	for (index = 0; index < n; index++) {
		count = 0; flag = 0;
		while ( (ch = getchar()) != '\n' ) {
			if ( ch != ' ' ) {
				flag = 1;
			} else {
				if (flag) {
					count++;
					flag = 0;
				}
			}
		}
		if (flag) {
			count++;
			flag = 0;
		}
		printf("%d\n", count);
	}
	// while (getchar() != 'q') {};
	return 0;
}