#include <iostream>
#include <stdint.h>
using namespace std;

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);
	uint32_t tt, n, left, ret;
	cin >> tt;
	while(tt--)
	{
		cin >> n;
		left = 0; ret = 0;
		for (register int i = 0; i < n; ++i)
		{
			char ch;
			cin >> ch;
			if (ch == '(')
			{
				++left;
				++ret;
			}
			else 
			{
				if (left) 
				{
					--left; 
					--ret;
				}
				else
				{
					++ret;
				}	
			}
			// printf("%c, left:%u, ret:%u\n",ch,left,ret);
		}
		cout << ret << endl;
	}

	// char chch;
	// while ((chch = getchar())!='q');
	return 0;
}