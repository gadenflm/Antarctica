class Solution 
{
public:
    int maxArea(vector<int>& height) 
    {
        int n = height.size(), max = 0; 
        for (int h = 0; h <= 10000; ++h)
        {
            int left = 0, right = n - 1;
            while (height[left] < h && left < right) ++left;
            if (left == right) break;
            while (height[right] < h && left < right) --right;
            if (left == right) break;
            int temp = (right - left) * h;
            if (max < temp) 
                max = temp;
        }
        return max;
    }
};