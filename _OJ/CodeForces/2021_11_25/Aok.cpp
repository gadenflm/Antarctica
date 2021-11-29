#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>
using namespace std;

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);
	uint32_t tt, n;
	cin >> tt;
	while(tt--)
	{
		cin >> n;
		if (n % 2 == 0)
			cout << "0" << endl;
		else
		{
			n /= 10;
			bool f = false;
			int temp = n % 10;
			bool ff = false;
			while (n > 0)
			{
				if (temp % 2 == 0)
					f = true;
				if (n < 10 && temp % 2 == 0)
				{
					ff = true;
				}				
				n /= 10;
				temp = n % 10;
			}
			if (!f)
				cout << "-1" << endl;
			else if (ff)
				cout << "1" << endl;
			else 
				cout << "2" << endl;
		}
	}

	// char chch;
	// while ((chch = getchar())!='q');
	return 0;
}