/**
 * Quick Find.c by K_gadenflm
 *
 * @Time Complexity		O(mn), where m is the connection counts
 * @breif				To find whether 2 nodes are linked (exist a path between them)!
 *           			The name 'Quick Find' is given that this Alg. is quick in finding but not union.
 */

#include <stdio.h>
#include <stdint.h>
//n
#define NODES_NUM 10

static uint32_t link_arr[NODES_NUM] = {0};

// O(n)
void ar_init(void)
{
	for (int i = 0; i < NODES_NUM ; i++) {
		/* `link_arr[i] = i` indicates that i node is only connected to itself */
		link_arr[i] = i;
	}
	return ;
}

// O(n)
void connect_two_nodes(const uint32_t node_1, const uint32_t node_2)
{
	uint32_t tmp_node = link_arr[node_2];
	/* all node linked to node_2, make it link to node_1 */
	for (int i = 0; i < NODES_NUM; i++) {
		if (link_arr[i] == tmp_node) link_arr[i] = link_arr[node_1];
	}
	return ;
}

// O(1)
uint8_t is_linked (const uint32_t node_1, const uint32_t node_2)
{
	return (link_arr[node_1] == link_arr[node_2]);
}

int main ()
{
	ar_init();
	int32_t node_1, node_2;
	while (1)
	{
		printf ("Please input two connected nodes index in range [0,%u].\
		 		Nagative number will stop input.\n", NODES_NUM - 1);

		scanf("%d %d", &node_1, &node_2);
		if (node_1 < 0 || node_2 < 0) break;

		connect_two_nodes(node_1, node_2);
	}

	while (1)
	{
		printf ("Please input two nodes index in range [0,%u]. \
				Nagative number will stop input.\n", NODES_NUM - 1);

		scanf("%d %d", &node_1, &node_2);
		if (node_1 < 0 || node_2 < 0) break;

		if (is_linked(node_1, node_2)) {
			printf("node[%d] and node[%d] is linked\n", node_1, node_2);
		} else {
			printf("node[%d] and node[%d] is not linked\n", node_1, node_2);
		}
	}

	return 0;
}