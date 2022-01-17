class Solution 
{
public:
    double myPow(double x, int n) 
    {
        if (n == 0)
            return 1;
        if (n < 0)
        {
            if (n == -2147483648) 
                return myPow(x, n / 2);
            else
                return myPow(1 / x, -n);
        }
        if (n == 1)
            return x;
        if (n % 2 == 0)
            return myPow(x * x, n >> 1);
        else 
            return myPow(x * x, n >> 1) * x;
    }
};