/**
 * K_gadenflm , UESTC OJ 2361
 * Alg. Union-Find
 * Name: Ogiso Setsuna's Sweety Home
 */

//一共n个罐子为n个节点,钥匙在哪个罐子及相连,构建并查集树形结构. 问题统计:共有多少个连通分支。

#include <stdio.h>
#define MAX_N 1000001

static int a[MAX_N] = {0};
static int b[MAX_N] = {0};

int find_root (int node) {
	while (a[node] != node)
	{
		a[node] = a[a[node]];
		node = a[node];
	}
	return node;
}

void union_node (int node_1 , int node_2 ) {
	int root_1 = find_root (node_1);
	int root_2 = find_root (node_2);
	if (root_1 == root_2) return ;
	if (b[root_1] < b[root_2]) {
		a[root_1] = root_2;
		b[root_2] += b[root_1];
	} else {
		a[root_2] = root_1;
		b[root_1] += b[root_2];
	}
	return ;
}

int main ()
{
	int n, m;
	scanf("%d", &n);

	for (int i = 1; i < n + 1; i++ ) {
		a[i] = i;
		b[i] = 1;
	}

	for (int i = 1; i < n + 1 ; i++) {
		scanf("%d", &m );
		union_node(i, m);
	}
	int sum = 0;
	for (int i = 1; i < n + 1; i++) {
		sum += (find_root(i) == i ? 1 : 0 );
	}
	printf ("%d\n", sum);

	// while ( (getchar() != 'q') ) ;

	return 0;
}