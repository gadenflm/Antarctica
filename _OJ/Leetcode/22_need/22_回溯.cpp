class Solution 
{
public:
    void AddFunc(vector<string>& ret, string s, int left, int right, int n)
    {
        if (left == n && right == n)
            ret.push_back(s);
        if (left > right)
            AddFunc(ret, s + ')', left, right + 1, n);
        if (left < n)
            AddFunc(ret, s + '(', left + 1, right, n);
    }

    vector<string> generateParenthesis(int n) 
    {
        vector<string> ret;
        AddFunc(ret, "(", 1, 0, n);
        return ret;
    }
};