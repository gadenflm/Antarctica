#include <iostream>
#include <cmath>
#include <cstring>
#include <stdint.h>


using namespace std;

bool IsPalindrome(char* s)
{
	int l = strlen (s);
	char* p1 = s; char* p2 = &s[l-1];
	while ( (p1!=p2) && ((p1-1)!=p2) )
	{
		// cout << *p1 << ':' << *p2 <<endl;
		if ((*p1) != (*p2))
		{
			return false;
		}
		++p1; --p2;
	}

	return true;
}

int DeleteSymbol(char* s, const char c, char* s2)
{
	int ret = 0;
	memset(s2,0,strlen(s));
	int j = 0;
	for (int i = 0; i < strlen(s); ++i)
	{
		if (s[i] != c)
		{
			s2[j++] = s[i];
		} 
		else 
		{
			ret++;
		}
	}
	s2[j++] = '\0';
	return ret;
}

int main ()
{
	int tt;
	cin >> tt;
	for (register int ind = 0; ind < tt; ++ind)
	{
		uint32_t n;
		char s[100001];
		char s2[100001];
		cin >> n;
		scanf("%s",s);
		if (IsPalindrome(s))
		{
			cout << "0" << endl;
			continue;
		}
		int m = -1; int min = strlen(s) + 1;
		for (char c = 'a'; c <= 'z'; ++c)
		{
			int del_num = DeleteSymbol(s,c,s2);
			if (del_num != 0)
			{
				if (IsPalindrome(s2))
				{
					m = del_num;	
					/*
					1
					8
					abbbbbca
					khhhhk
					*/

					char *p1 = &s[0];
					int mid = ((int)(strlen(s2)+1)/2)-1;
					// cout << "mid:" << mid << endl;
					for (int i = 0; i <= mid; ++i)
					{
						while ( ((*p1)==c) || (*p1)!=s2[i] ) 
						{
							++p1;
						}
						++p1;
						// cout << *p1 << ":" << c << endl;
					}
					--p1;
					char *p2 = nullptr;
					if (strlen(s2) % 2 == 0 ) 
						p2 = p1 + 1;
					else 
						p2 = p1;
					char *ps = &s[0];
					while (p1 != ps)
					{
						if (((*p1)==c) && ((*p2)==c))
							m-=2;
						--p1;++p2;
					}
					//
					if (m < min)
					{
						min = m;
					}
				}
			}
		}
		if (m != -1)
			cout << min << endl;
		else 
			cout << m << endl;
	}	
	// char chch;
	// while ((chch = getchar())!='q');
	return 0;
}