/**
 * Weighted_Quick_Union.c by K_gadenflm
 *
 * @Time Complexity     O(m lg(n)), where m is the union counts.
 * @breif               This Alg. is better than Quick Union.
 *                      This Alg. is improved frome 'Quick Union' by add weight.
 *                      the weight[...] is to record the number of nodes linked to a specified node.
 *
 *                      Key: When it is to link two nodes, by comparing their root weight to ensure
 *                       that the smaller tree is link to the bigger one. So it can ensure that the
 *                       tree's depth is always minimum refer to log2(n).
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

// O(lg(n)), This add-weight improvements prove that this tree's max depth is log2(n);
uint32_t find_root (uint32_t node) {
    while (link_arr[node] != node) {
        node = link_arr[node];
    }
    return node;
}

// O(lg(n))
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

// O(lg(n))
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