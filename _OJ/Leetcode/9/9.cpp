class Solution 
{
public:
    bool isPalindrome(int x) 
    {
        if (x < 0) return false;
        int64_t temp = x, xx = 0; 
        while (temp) 
        {
            xx = xx * 10 + temp % 10;
            temp /= 10;
        }
        if (xx == (int64_t)x) 
            return true;
        else
            return false;
    }
};