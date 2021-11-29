#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

#define N 15326
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);
	uint32_t tt, a, b, temp;
	cin >> tt;
	while(tt--)
	{
		cin >> a >> b;
		int sum = 0;
		while (a > 0 && b > 0)
		{
			// cout << a << ":" << b << endl;
			if (a > b)
			{
				if (a > N && b > N && (a - b) < N)
				{
					a -= N;
					b -= N;
					sum += N / 2;
				}
				if (a < 3)
					break;
				temp = (a - b) / 2;
				if (temp > b)
				{
					temp = b;
					// cout << temp << endl;
				}
				if (temp == 0)
					temp = 1;
				a -= 3 * temp;
				b -= 1 * temp;
				sum += temp;
			}
			else if (a < b)
			{
				if (a > N && b > N && (b - a) < N)
				{
					a -= N;
					b -= N;
					sum += N / 2;
				}
				if (b < 3)
					break;
				temp = (b - a) / 2;
				if (temp > a)
					temp = a;
				if (temp == 0)
					temp = 1;
				b -= 3 * temp;
				a -= 1 * temp;
				sum += temp;
			} else 
			{
				if (a == 1)
					break;
				temp = (a + b) / 4;
				b -= 2 * temp;
				a -= 2 * temp;
				sum += temp;
			}
		}
		cout << sum << endl;
	}

	// char chch;
	// while ((chch = getchar())!='q');
	return 0;
}