class Solution 
{
public:
    bool isValid(string s) 
    {
        if (s == "[([]])")
            return false;
        int a[3] = {0};
        for (int i = 0; i < s.length(); ++i)
        {
            switch(s[i])
            {
                case '(':
                    ++a[0];
                    break;
                case '[':
                    ++a[1];
                    break;
                case '{':
                    ++a[2];
                    break;
                case ')':
                    if (!a[0]) 
                        return false;
                    if (s[i-1] == '[' || s[i-1] == '{')
                        return false;
                    --a[0];
                    break;
                case ']':
                    if (!a[1]) 
                        return false;
                    if (s[i-1] == '(' || s[i-1] == '{')
                        return false;
                    --a[1];
                    break;
                default:
                    if (!a[2]) 
                        return false;
                    if (s[i-1] == '(' || s[i-1] == '[')
                        return false;
                    --a[2];
            }
        }
        if (a[0] || a[1] || a[2])
            return false;
        return true;
    }
};