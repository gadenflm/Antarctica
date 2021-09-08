/**
 * Quick Union.c by K_gadenflm
 *
 * @Time Complexity     O(mn'), where m is union counts, lg(n) <= n' <= n
 * @breif               This Alg. is better than Quick Find.
 *                      The name 'Quick Union' is given that this Alg. is quick in union but not finding
 */
#include <stdio.h>
#include <stdint.h>

// n
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

// O(n'), lg(n) <= n' <= n.where lg(n) is the min tree depth, n is max.
uint32_t find_root (uint32_t node) {
    while (link_arr[node] != node) {
        node = link_arr[node];
    }
    return node;
}

// O(n')
void union_two_nodes(const uint32_t node_1, const uint32_t node_2)
{
    link_arr[find_root(node_2)] = link_arr[find_root(node_1)];
}

// O(n')
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