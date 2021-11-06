#include <iostream>

using namespace std;

/*
8
4 -3 5 -2 -1 2 6 -2
*/

int main()
{
	int n, sum = 0, s;
	cin >> n;
	int * a = new int[n];
	for (int i = 0; i < n; ++i)
	{
		cin >> a[i];
	}
	for (int i = 0; i < n; ++i)
	{
		s = 0;
		for (int j = i; j < n; ++j)
		{
			s += a[j];
			if (s > sum) 
				sum = s;
		}
	}
	cout << sum << endl;

	char chch;
	while ((chch = getchar())!='q');

	delete [] a;
	return 0;
}
