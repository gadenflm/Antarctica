class Solution 
{
public:
    int threeSumClosest(vector<int>& nums, int target) 
    {
        #define a nums
        int n = a.size();
        int ret = a[0] + a[1] + a[2], min = abs(target - ret);
        if (min == 0 || n == 3) 
            return ret;
        sort(a.begin(),a.end());
        for (int left = 0; left < n-2; ++left)
        {
            if (left > 0 && a[left] == a[left-1])
                continue;
            int mid = left + 1, right = n - 1;
            int temp = target - a[left];
            while (mid < right)
            {
                if (abs(target - a[left] - a[mid] - a[right]) < min)
                {
                    min = abs(target - a[left] - a[mid] - a[right]);
                    ret = a[left] + a[mid] + a[right];
                }
                if (a[mid] + a[right] > temp)
                    --right;
                else if (a[mid] + a[right] < temp)
                    ++mid;
                else
                {
                    ret = a[left] + a[mid] + a[right];
                    goto RET;
                }
            }
        }
        RET:
        return ret;
    }
};