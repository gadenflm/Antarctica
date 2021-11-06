/**
 * Author: K_gadenflm 2021-11-6
 */
#include <iostream>

using namespace std;

/*
8
4 -3 5 -2 -1 2 6 -2
*/
int MaxSum(const int* a, int left , int right)
{
	if (left == right)
		if (a[left] > 0)
			return a[left];
		else
			return 0;	
	
	int max_LeftPart, max_RightPart, max_LeftToBorder = 0, max_BorderToRight = 0, max; 
	int border = (left + right) >> 1;  // 整除 2 得到中间值.

	max_LeftPart = MaxSum (a, left, border); // 递归实现分治
	max_RightPart = MaxSum (a, border+1, right); 

	// 合并子问题解 O(N)
	// 处理解包含左右两侧的情况 O(N)
	int s = 0;
	for (int i = border; i >= left; --i)
	{
		s += a[i];
		if (s > max_LeftToBorder) 
			max_LeftToBorder = s;
	}

	s = 0;
	for (int i = border + 1; i <= right; ++i)
	{
		s += a[i];
		if (s > max_BorderToRight) 
			max_BorderToRight = s;
	}

	// 合并
	max = 0;
	if (max_LeftPart > max) 
		max = max_LeftPart;
	if (max_RightPart > max)
		max = max_RightPart;
	if (max_BorderToRight + max_LeftToBorder > max)
		max = max_BorderToRight + max_LeftToBorder;
	
	return max;
}

int MaxSubSeqSum(const int* a, int n)
{
	return MaxSum(a, 0, n-1);
}

int main()
{
	int n, ret;
	cin >> n;
	int * a = new int[n];
	for (int i = 0; i < n; ++i)
	{
		cin >> a[i];
	}
	
	ret = ( MaxSubSeqSum(a, n) > 0)? MaxSubSeqSum(a, n) : 0;

	cout << ret << endl;

	char chch;
	while ((chch = getchar())!='q');

	delete [] a;
	return 0;
}
