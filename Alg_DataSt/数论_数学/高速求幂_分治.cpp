/**
 * @Author: K_gadenflm 2021-11-6
 * @brief: 高速求幂，思想:分治, 降维.
 * 		   O(log(n))
 */
#include <iostream>

using namespace std;

long long int SelfPow (long long int n, int m)
{
	if (m == 0)
		return 1;
	if (m == 1)
		return n;
	if (m % 2 == 0)
		return SelfPow(n * n, m / 2);
	else 
		return SelfPow(n * n, m >> 1) * n;

}

int main()
{
	int n, m;
	cin >> n >> m;
	long long int ret = SelfPow(n, m);
	cout << ret << endl;

	char chch;
	while ((chch = getchar())!='q');

	return 0;
}
