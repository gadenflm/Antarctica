#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>
using namespace std;

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);
	uint32_t tt, n, l, r, k;
	cin >> tt;
	while(tt--)
	{
		cin >> n >> l >> r >> k;
		std::vector<int> a(n);
		for (int i = 0; i < n; ++i)
		{
			cin >> a[i];
		}
		sort(a.begin(), a.end());
		int ret = 0;
		for (int i = 0; i < n; ++i)
		{
			if (a[i] >= l && a[i] <= r && a[i] <= k)
			{
				k -= a[i];
				++ret;
				// cout << a[i] << endl;
			}
		}
		cout << ret << endl;
		// cout << "****" << endl;
	}

	// char chch;
	// while ((chch = getchar())!='q');
	return 0;
}