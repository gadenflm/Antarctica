class Solution 
{
public:
    int removeDuplicates(vector<int>& nums) 
    {
        #define a nums
        int n = nums.size();
        if (n == 0)
            return 0;
        int len = 0, temp = a[0];
        for (int i = 1; i < n; ++i)
        {
            if (a[i] != temp)
            {
                len ++;
                a[len] = a[i];
                temp = a[i];
            }
        }
        return len + 1;
    }
};