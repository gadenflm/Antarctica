/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution 
{
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) 
    {
        #define MAX 10001
        ListNode *ret_h = nullptr, *ret = nullptr;
        int n = lists.size(), min_i, min_val;
        bool flag = true;
        for(;;)
        {
            flag =false;
            min_val = MAX;
            min_i = -1;
            for (int i = 0; i < n; ++i)
            {
                if (lists[i] == nullptr) 
                    continue;
                flag =true;
                if (lists[i]->val < min_val)
                {
                    min_val = lists[i]->val;
                    min_i = i;
                }
            }
            if (!flag) return ret_h;
            if (ret)
            {
                ListNode* temp = new ListNode(min_val);
                ret->next = temp;
                ret = temp;
                lists[min_i] = lists[min_i]->next;
            }
            else 
            {
                ret = new ListNode(min_val);
                ret_h = ret;
                lists[min_i] = lists[min_i]->next;
            }
        }
        return ret_h;
    }
};