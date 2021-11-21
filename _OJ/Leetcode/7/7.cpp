class Solution {
public:
    int reverse(int x) {
        int ret = 0, i;
        while (x)
        {
            i = x % 10;
            if (ret > 214748364 || ret < -214748364)
                return 0;
            ret = ret * 10 + i;
            x = x / 10;
        }
        return ret;
    }
};