class Solution 
{
public:
    int removeElement(vector<int>& nums, int val) 
    {
        #define a nums
        int n = a.size();
        if (n == 0)
            return 0;
        int len = 0;
        for (int i = 0; i < n; ++i)
            if (a[i] != val)
                a[len++] = a[i];
        return len;
    }
};