/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
 
 typedef struct ListNode LIST;

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2){
    LIST* list_head = (LIST*) malloc (sizeof (LIST));
    LIST * list = list_head;
    int a,b,c=0;
    do
    {
        if (l1) a=l1->val; else a=0;
        if (l2) b=l2->val; else b=0; 
        list->val = a+b+c;
        list->next = NULL;
        if (list->val >= 10) c = 1; else c=0;  
        list->val  =list->val  % 10;
        
        if (l1) l1=l1->next;
        if (l2) l2=l2->next;
        if (l1 != NULL || l2!=NULL ) {
            list->next = (LIST*) malloc (sizeof (LIST));
            list = list->next;
        }
    }while (l1 != NULL || l2!=NULL ) ;
    if (c == 1) {
        list->next = (LIST*) malloc (sizeof (LIST));
        list = list->next;
        list->val = c;
        list->next = NULL;
    }
    return list_head;
}