#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>
#include <list>
#include <cmath>
using namespace std;


void print_list(list<int>& ret)
{
	for (list<int>::iterator it = ret.begin(); it != ret.end(); ++it)
		cout << *it << ' ';
	cout << endl;
}

static bool func(vector<int>& a, int left, int right, int key, list<int>& ret)
{
	if (a[left] > key && a[right] > key)
	{
		return false;
	}
	if (left == right)
	{	
		if (key == ret.front())
			ret.push_back(a[left]);
		else 
			ret.push_front(a[right]);
		return true;
	}
	else if (a[left] < a[right])
	{	
		ret.push_front(a[left]);
		while (a[left] < key && left < right && a[left] < a[left + 1])
		{ 
			++left;
			ret.push_front(a[left]);
		}
		if (left == right)
		{
			return true;
		}
		return func (a, left + 1, right, a[left], ret);
	}
	else 
	{
		ret.push_back(a[right]);
		while (a[right] < key && left < right && a[right] < a[right - 1])
		{ 
			--right;		
			ret.push_back(a[right]);
		}
		if (left == right)
		{
			return true;
		}
		return func(a, left, right - 1, a[right], ret);	
	}
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
		vector<int> a(n);
		list<int> ret(n);
		for (int i = 0; i < n; ++i)
			cin >> a[i];
		if (n == 1)
		{	
			cout << a[0] << endl;
		}
		else
		{
			ret.clear();
			ret.push_back(a[0]);
			if (func(a, 1, n - 1, a[0], ret))
			{
				print_list(ret);
				continue;
			}
			ret.clear();
			ret.push_back(a[n - 1]);
			if  (func(a, 0, n - 2, a[n - 1], ret))
			{
				print_list(ret);
				continue;
			}
			cout << "-1" << endl;
		}
	}

	// char chch;
	// while ((chch = getchar())!='q');
	return 0;
}