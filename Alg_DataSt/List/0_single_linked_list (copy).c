/**
 * Single_linked_list.c by K_gadenflm
 *
 * @breif		Try to construct a single-linked list with header node.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct st_data {
	char 					key[256];
	int  					value;
} KV_DATA;


/* pNode is defined as `struct st_list_node *` */
typedef struct st_list_node
{
	struct st_data			data;
	struct st_list_node*	next;
} List_Node, * pNode;

typedef unsigned char u8;

const unsigned int NODE_SIZE = sizeof(List_Node);

/**
 * @breif			Init a List, return the pointer to head node.
 * @return			head node pointer;
 */
static inline pNode ListInit(void)
{
	pNode list_head = (pNode) malloc(NODE_SIZE);
	memset(list_head, 0, NODE_SIZE);
	list_head->next = NULL;
	return list_head;
}

/**
 * @breif			Find whether a list is empty.
 * @return			0 stands for empty, others for not.
 */
static inline u8 IsListEmpty(pNode list_header) {
	return (list_header == NULL);
}

/**
 * @breif			Show all list data with the form like "[key]--[value]"
 */
static void ShowListData(pNode list_header) {
	pNode list_node =  list_header->next;
	putchar ('\n');
	while (list_node) {
		printf("%40s -- %d\n", list_node->data.key, list_node->data.value);
		list_node = list_node->next;
	}
	putchar ('\n');
}

/**
 * @breif 			Insert a K-V data to list tail
 */
static void InsertToTail(pNode list_header, KV_DATA data) {
	pNode list_tail = list_header;
	while (list_tail->next) {
		list_tail = list_tail->next;
	}
	pNode temp_node = (pNode) malloc(NODE_SIZE);
	strcpy(temp_node->data.key, data.key);
	temp_node->data.value = data.value;
	temp_node->next = NULL;
	list_tail->next = temp_node;
}

int main()
{
	pNode history = ListInit();

	InsertToTail(history, (KV_DATA) {"Jesus Christ", 0});
	InsertToTail(history, (KV_DATA) {"Fallen of Constantinople", 1453});

	ShowListData(history);

	return 0;
}

