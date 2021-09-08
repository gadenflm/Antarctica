/**
 * Union_Find.c by K_gadenflm
 *
 * @Time Complexity     O(m lg*(n)), where m is the union counts.
 * 						lg*(n) is a function that lg*(2^66536) == 5. So we can approximate it to O(1)
 *
 * @breif 				This Alg. is improved from 'Weighted Quick Union' by link compression.
 *                      Key: In the find root process, Link the son node to grandparents but not parents.
 */
#include <stdio.h>
#include <stdint.h>

// n
#define NODES_NUM 10

static uint32_t link_arr[NODES_NUM] = {0};

// number of nodes linked to specified node
static uint32_t node_weight[NODES_NUM] = {0};

// O(n)
void ar_init(void)
{
	for (int i = 0; i < NODES_NUM ; i++) {
		/* `link_arr[i] = i` indicates that i node is only connected to itself */
		link_arr[i] = i;
		/* each nodes only link to itself. so weight is 1*/
		node_weight[i] = 1;
	}
	return ;
}

// O(lg*(n)), when we find, we can compress the link by the way.
uint32_t find_root (uint32_t node) {
	while (link_arr[node] != node) {

		// link the node to father's father.
		link_arr[node] = link_arr[link_arr[node]];
		node = link_arr[node];
	}
	return node;
}

// O(lg*(n))
void union_two_nodes(const uint32_t node_1, const uint32_t node_2)
{
	uint32_t root_1 = find_root(node_1);
	uint32_t root_2 = find_root(node_2);

	// has been linked.
	if (root_1 == root_2) return;

	// Comparing root weights to ensure smaller tree is under bigger tree.
	if (node_weight[root_2] <= node_weight[root_1])
	{
		link_arr[root_2] = root_1;
		node_weight[root_1] += node_weight[root_2];
	} else {
		link_arr[root_1] = root_2;
		node_weight[root_2] += node_weight[root_1];
	}

	return ;
}

// O(lg*(n))
uint8_t is_linked (const uint32_t node_1, const uint32_t node_2)
{
	return (find_root(node_1) == find_root(node_2));
}

int main()
{
	ar_init();
	int32_t node_1, node_2;
	while (1)
	{
		printf ("Please input two connected nodes index in range [0,%u].\
                Nagative number will stop input.\n", NODES_NUM - 1);

		scanf("%d %d", &node_1, &node_2);
		if (node_1 < 0 || node_2 < 0) break;
		union_two_nodes(node_1, node_2);
		putchar ('\n');
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
		putchar ('\n');
	}
	return 0;
}