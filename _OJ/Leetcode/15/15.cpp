class Solution 
{
private:

public:
// -4 -1 -1 0 1 2 
    vector<vector<int>> threeSum(vector<int>& nums) 
    {
        #define a nums
        vector<vector<int>> ret;
        if (a.size() < 3) 
            return ret;
        sort(a.begin(),a.end());
        const int n = a.size();
        for (int left = 0; left < n; ++left)
        {
            if (left > 0 && a[left-1] == a[left])
                continue;
            if (a[left] > 0)
                break;
            int temp = -a[left], mid = left + 1, right = n - 1;
            while (mid < right)
            {
                if (a[mid] + a[right] < temp)
                    ++mid;
                else if (a[mid] + a[right] > temp)
                    --right;
                else
                {
                    ret.push_back({a[left], a[mid], a[right]});
                    while (mid++ < right && a[mid] == a[mid-1]);
                    while (mid < right-- && a[right] == a[right+1]);
                }
            }
        }
        return ret;
    }
};