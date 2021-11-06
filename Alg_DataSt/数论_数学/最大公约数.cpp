/**
 * @Author: K_gadenflm 2021-11-6
 * @brief: 欧几里得法求最大公约数, 计算 余数序列，直到出现0.
 			O(log(n));
 */
#include <iostream>

using namespace std;

// 辗转相除法
int Gcd (int n, int m)
{
	int r;
	while ( m > 0)
	{
		r = n % m;
		n = m;
		m = r;
	}
	return n;
}

int main()
{
	int n, m, ret;
	cin >> n >> m;
	ret = (n >= m) ? Gcd(n, m) : Gcd(m, n);
	cout << ret << endl;

	char chch;
	while ((chch = getchar())!='q');

	return 0;
}
