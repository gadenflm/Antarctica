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
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) 
    {
        ListNode* ret = nullptr;
        ListNode* ret_h = nullptr;
        ListNode* p1 = list1;
        ListNode* p2 = list2;
        while (p1 != nullptr || p2 != nullptr)    
        {
            // cout << p1->val << ':' << p2->val << endl;
            if (p1 == nullptr)
            {
                while (p2)
                {
                    ListNode* temp = new ListNode(p2->val);
                    if (ret)
                    {
                        ret->next = temp;
                        ret = temp;
                    }
                    else
                    {
                        ret = temp;
                        ret_h = ret;
                    }
                    p2 = p2->next;
                }
            }
            else if (p2 == nullptr)
            {
                while (p1)
                {
                    ListNode* temp = new ListNode(p1->val);
                    if (ret)
                    {
                        ret->next = temp;
                        ret = temp;
                    }
                    else
                    {
                        ret = temp;
                        ret_h = ret;
                    }
                    p1 = p1->next;
                }
            } 
            else if (p2->val > p1->val)
            {
                ListNode* temp = new ListNode(p1->val);
                if (ret)
                {
                    ret->next = temp;
                    ret = temp;
                }
                else
                {
                    ret = temp;
                    ret_h = ret;
                }
                p1 = p1->next;
            } 
            else 
            {
                ListNode* temp = new ListNode(p2->val);
                if (ret)
                {
                    ret->next = temp;
                    ret = temp;
                }
                else
                {
                    ret = temp;
                    ret_h = ret;
                }
                p2 = p2->next;
            }
        }
        return ret_h;
    }
};