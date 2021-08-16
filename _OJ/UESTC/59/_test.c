/**
 * K_gadenflm , UESTC OJ 59.
 */

#include <stdio.h>
#include <string.h>

int main() {
	int n, data, index, flag;
	char str[201];
	char *p_str = NULL;
	scanf("%d", &n);
	for (index = 0; index < n; index++) {
		scanf("%s", str);
		str[strlen(str)] = '\0';
		p_str = str;
		data = 0; flag = 0;
		while ( (*p_str) != '\0' ) {
			if ( (*p_str) >= '0' && (*p_str) <= '9' ) {
				data = data * 10 + ((*p_str) - 48);
				flag = 1 ;
			} else {
				if (flag) {
					printf("%d ", data );
					data = 0;
					flag = 0;
				}
			}
			p_str++;
		}
		if (flag) {
			printf("%d ", data );
			data = 0;
			flag = 0;
		}
		putchar('\n');
	}
	// getchar();
	// getchar();

	return 0;


}