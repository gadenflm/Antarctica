#include <iostream>
#include <cmath>
#include <cstring>
#include <stdint.h>

using namespace std;
static char key[51],word[51];
/*
1
abcdefghijklmnopqrstuvwxyz
hello
 */
int TypeCost(int i)
{
	char prev = word[i];
	char next = word[i+1];
	// cout << (int)key[next-'a'] << " : " << (int)key[prev-'a'] << endl;
	// cout << next << " : " << prev << endl;
	return abs(key[next-'a']-key[prev-'a']);
}

int main()
{
	int tt,i,sum;
	cin >> tt;
	char temp [51];
	for (register int ind = 0; ind < tt; ++ind)
	{
		memset(key,0,51);
		scanf("%s",temp);
		for (i = 0; i < strlen (temp); ++i)
		{
			key[temp[i] - 'a'] = i;
		}
		scanf("%s",word);
		sum = 0;
		for (i = 0; i < strlen(word)-1; ++i)
		{
			sum += TypeCost(i);
		}		
		cout << sum << endl;
	}
	// char ch;
	// while((ch = getchar())!='q');
}