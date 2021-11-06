/**
 * Author: K_gadenflm 2021-11-6
 */
#include <iostream>

using namespace std;

/*
8
4 -3 5 -2 -1 2 6 -2
*/

int main()
{
	int n, s = 0, ret = 0, a;
	cin >> n;
	for (int i = 0; i < n; ++i)
	{
		cin >> a;
		s += a;
		if (s > ret)
			ret = s;
		else if (s < 0)
			s = 0;
	}
	cout << ret << endl;

	char chch;
	while ((chch = getchar())!='q');

	return 0;
}
