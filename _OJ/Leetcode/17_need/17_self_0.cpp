class Solution 
{
public:
    vector<string> letterCombinations(string digits) 
    {
        vector<string> ret;
        const string s[] = {"abc", "def", "ghi", "jkl","mno", "pqrs", "tuv", "wxyz"};
        int a[5] = {0};
        int n = digits.length();
        int ind = n - 1;
        for (;;)
        {
            string temp = "";
            for (int i = 0; i < n; ++i)
            {
                int t = digits[i] - '2';
                temp = temp + s[t][a[i]];
            }
            if (temp.length())
                ret.push_back(temp);
            while (ind >= 0 && a[ind] == s[digits[ind] - '2'].length() - 1)
                --ind;
            if (ind < 0)
                break;                   
            ++a[ind];
            if (ind != n - 1)
            {
                for (int i = ind + 1; i < n; ++i)
                    a[i] = 0;
                ind = n - 1;
            }

        }
        return ret;
    }
};