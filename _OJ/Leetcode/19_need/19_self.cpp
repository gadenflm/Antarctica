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
    ListNode* removeNthFromEnd(ListNode* head, int n) 
    {
        vector<ListNode*> a;
        ListNode* p = head;
        while (p != nullptr)
        {
            a.push_back(p);
            p = p->next;
        }
        int sz = a.size();
        if (n == sz) 
        {
            head = a[0]->next;
            return head;
        }
        else if (1 == n)
        {
            a[sz - 2]->next = nullptr;
            return head;
        }
        else
        {
            // cout << a[sz - n - 1]->val << endl;
            a[sz - n - 1]->next = a[sz - n + 1];
            // a[sz - n] -> next = nullptr;
            return head;
        }
    }
};