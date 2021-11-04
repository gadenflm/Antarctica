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
		uint32_t a,b,c,s;
		cin >> a >> b >> c;
		s = a + 2 * b + 3 * c;
		cout << s % 2 << endl;
	}	
	// char ch;
	// while ((ch = getchar())!='q');
	return 0;
}