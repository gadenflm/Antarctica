class Solution 
{
public:
    string convert(string s, int numRows) 
    {
        if (numRows == 1)
            return s;
        int i = 0, row = 0;
        string ret;

        int down = (numRows - row) + (numRows - row - 2);
        int up = row + row;
        ret += s[i];
        while (ret.size() < s.size())
        {
            if (down > 0 && up > 0)
            {
                i += down;
                if (i >= s.size()) 
                {
                    i = ++ row;
                    down = (numRows - row) + (numRows - row - 2);
                    up = row + row;
                    ret += s[i];
                    // cout << ret << ':' << i << ':' << row << endl;
                    if (ret.size() == s.size())
                        return (ret);
                    continue;

                }
                ret += s[i];
                // cout << ret << ':' << i << ':' << row << endl;
                if (ret.size() == s.size())
                    return (ret);

                i += up;
                if (i >= s.size()) 
                {
                    i = ++ row;
                    down = (numRows - row) + (numRows - row - 2);
                    up = row + row;
                }
                ret += s[i];
                // cout << ret << ':' << i << ':' << row << endl;
            }
            else
            {
                if (down > 0) 
                    i += down;
                else 
                    i += up;
                if (i >= s.size()) 
                {
                    i = ++ row;
                    down = (numRows - row) + (numRows - row - 2);
                    up = row + row;
                }
                ret += s[i];
                // cout << ret << ':' << i << ':' << row << endl;
            }
        }
        return (ret);
    }
};

/*
P           I           N
A       L   S       I   G
Y   A       H   R
P           I
*/