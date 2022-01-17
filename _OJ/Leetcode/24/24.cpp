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
    ListNode* swapPairs(ListNode* head) 
    {
        ListNode* hp = new ListNode(0, head);
        ListNode* p = hp;
        while (p->next)
        {
            if (!p->next->next)
            {
                p = p->next;
            }
            else 
            {
                ListNode* p1 = p->next;   // 1
                p->next = p->next->next;  // h->2
                p1->next = p->next->next; // 1->(2->next)
                p->next->next = p1;
                p = p1;
            }
        }
        return hp->next;
    }
};