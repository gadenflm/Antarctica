#include <iostream>
#include <cmath>
#include <cstring>
#include <stdint.h>

using namespace std;
/*
	1 - 2 - 3 + 4 + 5 - 6 - 7 + 8 + 9 - 10 - 11 
	ou -> ji -> ji -> ou -> ou -> ji -> ji 
	ji -> ou -> ou -> ji -> ji -> ou -> ou
*/

int64_t CalSum(int64_t n)
{
	int64_t ret = (n / 4) * 4 + 1;
	if (n % 4 == 0)
		return 0;
	if (n % 4 == 2)
		return -1;
	if (n % 4 == 3)
	{
		return -(ret + 3);
	}
	return ret;
}

static void OddGrasshopper()
{
	int64_t x,n,sum;
	cin >> x >> n;
	if (x % 2 == 0) 
	{
		sum = -CalSum(n) + x;
	}
	else 
	{
		sum = CalSum(n) + x;
	}
	// cout << x << ":" << CalSum(n) << endl;
	cout << sum << endl;
	return;
}

int main ()
{
	int tt;
	cin >> tt;
	for (register int ind = 0; ind < tt ; ++ind)
	{
		OddGrasshopper();
	}
 	// char ch;
 	// while((ch = getchar())!='q');
 	return 0;
}