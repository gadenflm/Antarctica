#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>
using namespace std;

typedef struct DATA
{
	int value;
	int ind;
}data;

bool cmp(data a, data b)
{
	return a.value > b.value;
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);
	uint32_t tt, n;
	cin >> tt;
	while(tt--)
	{
		cin >> n;
		std::vector<data> a(n);
		// std::vector<int> ind(n);
		for (int i = 0; i < n; ++i)
		{
			cin >> a[i].value;
			a[i].ind = i + 1;
		}
		sort(a.begin(), a.end(), cmp);
		// SortFunc(a, ind , n);
		// for (int i = 0; i < n; ++i)
		// {
		// 	cout << i << ":" << a[i].value << ":" << a[i].ind << endl;
		// }
		std::vector<int> x(n + 1);
		x[0] = (n + 1) / 2;
		int ret = 0;
		int flag = 1, temp = 1;
		for (int i = 0; i < n; ++i)
		{
			x[a[i].ind] = x[0] + temp * flag;
			ret += temp * a[i].value * 2;
			if (flag == 1)
				flag = -1;
			else 
			{
				flag = 1;
				++temp;
			}
		}
		cout << ret << endl;
		for (int i = 0 ; i < n + 1; ++i)
		{
			cout << x[i] << ' ';
		}
		cout << endl;
	}

	char chch;
	while ((chch = getchar())!='q');
	return 0;
}