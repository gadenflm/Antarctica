class Solution 
{
public:
    int myAtoi(string s) 
    {
        int i = 0, ret = 0;
        int64_t temp = 0;
        char* ps = &s[0];
        // delete space
        while (*ps == ' ') ++ps;

        // judge
        bool IsNagetive = false;
        if ( *ps == '-' )
        {
            ps++;
            IsNagetive = true;
        } else if (*ps == '+') ++ps;

        while( *ps >='0' && *ps <= '9' && *ps!='\0')
        {
            i = *ps - '0';
            temp = (IsNagetive) ? temp * 10 - i : temp * 10 + i;

            if (temp > 2147483647) return 2147483647;
            if (temp < -2147483648) return -2147483648;
            ++ps;
        }
        ret = (int)temp;
        return ret;
    }
};