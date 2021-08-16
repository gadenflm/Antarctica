/**
 * K_gadenflm , 0 -> prime_num (2 3 5 7 11 ...)
 */

#include <stdio.h>

#define MAX_VAL 1000000007

_Bool no_prime_num[MAX_VAL] = {0, 1};
void prime_set();
int main() {

}

void prime_set() {
	unsigned long i, j;
	i = 2;
	while (i <= (MAX_VAL / 2)) {
		if (!no_prime_num[i]) {
			for (j = 2; j * i <= MAX_VAL; j++) {
				no_prime_num[j * i] = 1;
			}
		}
		i++;
	}
	return;
}
