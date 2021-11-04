#include <iostream>
#include <cmath>
#include <string>
#include <stdint.h>

using namespace std;
int main ()
{
	int t;
	cin >> t;
	for (register int ind = 0; ind < t; ++ind)
	{
		uint32_t n,s_0,s_1;
		uint64_t out;
		uint32_t a[61];
		cin >> n;
		s_0 = 0; s_1 = 0;
		for (int i = 0; i < n; ++i)
		{
			cin >> a[i];
			if (a[i] == 0) ++s_0;
			if (a[i] == 1) ++s_1;
			out = s_1;
			out *= (uint64_t)pow(2,s_0);
		}
		cout << out << endl;
	}	
	// char ch;
	// while ((ch = getchar())!='q');
	return 0;
}