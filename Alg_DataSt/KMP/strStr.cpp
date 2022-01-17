class Solution 
{
public:
    void ComNext(string p, vector<int>& next)
    {
        next[0] = 0;
        int x = 1;
        int i = 0; // i = next[x - 1];
        while (x <= p.length())
        {
            if (p[i] == p[x])
                next [x++] = ++i;
            else if (i)
                i = next[i - 1];
            else
                next [x++] = 0;
        }
    }

    // 此题为匹配到则返回，即只查 父串中 子串第一次出现的位置。
    int strStr(string s, string p) 
    {
        if (p.length() == 0)
            return 0;

        vector<int> next(p.length() + 1);
        ComNext(p, next);

        // for (int i = 0; i < p.length(); ++i)
        //     cout << next[i];

        int i = 0, j = 0;
        while (i <= s.length())
        {
            
            if (s[i] == p[j])
            {
                ++i;
                ++j;
            } 
            else if (j)
            {
                j = next[j - 1];
            } 
            else 
            {
                ++i;
            }
            if (j == p.length())
                return i - j;
        }
        return -1;
    }
};