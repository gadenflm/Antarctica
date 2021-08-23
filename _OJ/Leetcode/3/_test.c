#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint8_t hash[256] = {0};

int lengthOfLongestSubstring(char * s) {
	int head = 0, tail = 0, max = 0, count = 0;
	uint8_t flag = 1;
	while (head < strlen(s)) {
		while (flag) {
			hash[s[tail]] = 1;
			tail++;
			if (tail == strlen (s)) {
				tail--;
				flag = 0;
			}
			if (hash[s[tail]] == 1) {
				flag = 0;
			}
			count ++;
		}
		if (count > max ) max = count;
		while (flag == 0) {
			printf ("%d %d\n",  head, tail);
			if (s[tail] == s[head]) {
				flag = 1;
			}
			hash[s[head]] = 0;
			head ++;
			count --;
			if (head == strlen (s)) {
				flag = 1;
			}
		}
		// printf ("%d %d \n", head, tail);
	}
	return max;
}

int main() {
	char* s = "";
	printf("%d\n", lengthOfLongestSubstring(s));
	char ch;
	while ( (ch = getchar() != 'q') ) ;
}