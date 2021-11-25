#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>
using namespace std;
#define N 200001

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);
	uint32_t tt, n;
	cin >> tt;
	while(tt--)
	{
		cin >> n;
		int a[N][2] = {0};
		int b[N][2] = {0};
		for (int i = 0; i < n; ++i)
		{
			cin >> a[i][0] >> a[i][1];
		}
		

	}

	char chch;
	while ((chch = getchar())!='q');
	return 0;
}