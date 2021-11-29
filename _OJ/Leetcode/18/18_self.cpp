class Solution 
{
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) 
    {
        vector<vector<int>> ret;
        int n = nums.size();
        if (n < 4)
            return ret;
        sort(nums.begin(), nums.end());
        for (int a = 0; a < n - 3; ++a)
        {
            if (a > 0 && nums[a] == nums[a - 1])
                continue;
            for (int d = n - 1; d > a + 2; --d)
            {
                if (d < n - 1 && nums[d + 1] == nums[d])
                    continue;
                int b = a + 1, c = d - 1;
                while (b < c)
                {
                    int64_t temp = (int64_t)nums[a] + (int64_t)nums[b] + (int64_t)nums[c] + (int64_t)nums[d];
                    if (temp > target)
                    {
                        while (b < --c && nums[c] == nums[c + 1]);
                    } 
                    else if (temp < target)
                    {
                        while (++b < c && nums[b] == nums[b - 1]);
                    }
                    else 
                    {
                        ret.push_back({nums[a], nums[b], nums[c], nums[d]});
                        while (b < --c && nums[c] == nums[c + 1]);
                        while (++b < c && nums[b] == nums[b - 1]);
                    }
                }
            }
        }
        return ret;
    }
};