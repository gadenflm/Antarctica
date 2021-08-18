
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
typedef struct hash_table
{
	int key;
	int value;
	UT_hash_handle hh;
} HASH;

int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
	int* ret = (int*) malloc (sizeof(int) * 2);
	*returnSize = 2;

	HASH* hash_list = NULL;

	for (int i = 0; i < numsSize ; i++) {
		int key = target - nums[i];
		HASH* hash_node = NULL;
		HASH_FIND_INT(hash_list, &key, hash_node);
		if (hash_node) {
			ret[0] = hash_node->value;
			ret[1] = i;
			return ret;
		}
		hash_node = (HASH*) malloc (sizeof (HASH));
		hash_node-> key = nums[i];
		hash_node-> value = i;
		HASH_ADD_INT(hash_list, key, hash_node);
	}

	return ret;
}
