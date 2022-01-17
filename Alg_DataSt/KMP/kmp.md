# KMP

## 核心思想

字符串匹配算法，针对按位匹配的 brute-force 算法, 核心优化思路如下：

* 假设主串为 s[n] 子串为 p[m], 主串匹配时按位下标为 i， 子串匹配时按位下标为 j
* __如果 s[i] != p[j] , 则在此时失配。 此时存在已知信息 `p[0: j-1] == s[i-j: i-1]`。 即失配位前的字符，两串按位相等！ 利用此信息对后续遍历进行优化。__

最终时间复杂度为 O(n+m)

## 关键: 构建 子串的 next 数组

next 数组，是一个被称为部分匹配表(Partial Match Table)的数组。

* 定义 “k-前缀” 为一个字符串的前 k 个字符, “k-后缀” 为一个字符串的后 k 个字符。k 必须小于字符串长度。 如 `abc` 的 “2-前缀” 为 `ab`, “2-后缀” 为 `bc`。
* __`next[x]` 定义为: `p[0: x]` 这一段字符串，使得 `k-前缀` 恰等于 `k-后缀` 的最大的k.__

e.g.  
`char p[14]:		a b c a b d d d a b c a b c`  
`int next[14]:		0 0 0 1 2 0 0 0 1 2 3 4 5 0`

__如果 `s[i] != p[j] && j > 0` ,此时失配,因为`p[0: j-1] == s[i-j: i-1]`, 则 i 不变, `j = next[j-1]`__
__即子串的匹配位可以跳过`next[j-1]`个字符,因为前 `next[j-1]` 个 字符与后 `next[j-1]` 个 字符按位相等，且主串失配位前的字符也与子串按位相等。 (以上述例子`p[14]` 为子串，带入大脑模拟即可理解)__

## 构建next[x]算法: 自匹配与递推

https://www.zhihu.com/question/21923021

首先 `next[0] == 0`,   
假设已知 `next[x-1]`, 令 `i = next[x-1]`，现在求 `next[x]`
```cpp
if (p[x] == p[i])
{
	next[x] = i + 1;
	i += 1;
	x += 1;
}
else if (i)
	i = next[i - 1];
else 
{
	next[x] = 0;
	x += 1;
}
```

## 遍历如下

```Cpp
int i = 0, j = 0;
while (i <= s.length())
{
    
    if (s[i] == p[j])
    {
        ++i;
        ++j;
    } 
    else if (j)
    {
        j = next[j - 1];
    } 
    else 
    {
        ++i;
    }
    // 查找到一个符合的子串
    if (j == p.length())
    {
        cout << i - j << endl;
        j = next[j - 1];
    }
}
return -1;
```