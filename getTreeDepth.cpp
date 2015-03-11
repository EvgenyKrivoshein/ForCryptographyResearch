#include <assert.h>
#include <iostream>

using namespace std;

typedef struct tTreeNode
{
	int data;
	struct tTreeNode * left;
	struct tTreeNode * right;
}TreeNode;

size_t GetTreeDepth(TreeNode * node, size_t lvl, size_t max)
{
	if (!node)
		return 0;

	size_t t = 0;
	if (node->left)
		t = GetTreeDepth(node->left, lvl + 1, max);
	
	if (t > max)
		max = t;

	if (node->right)
		t = GetTreeDepth(node->right, lvl + 1, max);
	
	if (t > max)
		max = t;
	
	return (lvl > max) ? lvl : max;
}

int main(void)
{
	TreeNode n0;
	TreeNode n1;
	TreeNode n2;
	TreeNode n3;
	TreeNode n4;
	TreeNode n5;
	TreeNode n6;
	TreeNode n7;
	TreeNode n8;

	n0.left = NULL;//&n1;
	n0.right = NULL;// &n2;
	assert(GetTreeDepth(&n0, 0, 0) == 0);

	n0.left = &n1;
	n0.right =&n2;
	n1.left = NULL;//&n3;
	n1.right = NULL;//&n4;
	n2.left = NULL;
	n2.right = NULL;
	assert(GetTreeDepth(&n0, 0, 0) == 1);

	n0.left = &n1;
	n0.right = &n2;
	n1.left = &n3;
	n1.right = &n4;
	n2.left = NULL;
	n2.right = &n5;
	n3.left = NULL;
	n3.right = NULL;
	n4.left = &n6;
	n4.right = NULL;
	n5.left = NULL;
	n5.right = &n7;
	n6.left = NULL;
	n6.right = &n8;
	n7.left = NULL;
	n7.right = NULL;
	n8.left = NULL;
	n8.right = NULL;
	assert(GetTreeDepth(&n0, 0, 0) == 4);
	
}