#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>
using namespace std;

static int temp;
static bool IsPalindrome (vector<int>& a, int n)
{
	int left = 0;
	int right = n-1;
	bool ret = true;
	while (left < right && ret)
	{
		if (a[left] == a[right])
		{
			++left;
			--right;
		}
		else
		{
			ret = false;
		}
	}
	return ret;
}

static bool IsKalindrome (vector<int>& a, int left, int right)
{
	if (left >= right)
		return true;
	while (left < right && a[left] == a[right])
	{
		++left;
		--right;
	}
	if (left >= right)
		return true;
	else if ( temp == 0 )
	{
		temp = a[left];
		if (IsKalindrome(a, left + 1, right)) 
			return true;
		else 
		{
			temp = a[right];
			return IsKalindrome(a, left, right - 1);
		}

	}
	else if ( a[left] == temp && IsKalindrome(a, left + 1, right))
	{
		return true;
	}
	else if ( a[right] == temp && IsKalindrome(a, left, right - 1))
	{
		return true;
	}
	else 
		return false;
	
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);
	uint32_t tt, n;
	cin >> tt;
	while(tt--)
	{
		// cout << "YES" << endl;
		cin >> n;
		vector<int> a(n);
		for (int i = 0; i < n; ++i)
			cin >> a[i];
		if (!IsPalindrome(a, n))
		{
			temp = 0;
			if (IsKalindrome(a, 0, n - 1))
				cout << "YES" << endl;
			else
				cout << "NO" << endl;
		}
		else 
			cout << "YES" << endl;
	}

	// char chch;
	// while ((chch = getchar())!='q');
	return 0;
}