/* addr_node.c -- routines for manipulating syntax nodes */

int
addnode (Node *mother, Node *child)
{
	int ret = FAIL;
	if (!mother->left) {
		mother->left = child;
		child->up = mother;
		ret = SUCC;
	else if (!moth->right) {
		mother->right = child;
		child->up = mother;
		ret = SUCC;
	}
	return ret;
}

int
extendbranch_r (Node *mother, Node *child)
{
	if (!addnode (child, mother->right))
		return (FAIL);

	mother->right = child;

	return (ret);
}
	

Node *
makenode (void)
{
	Node *ret;
	ret = calloc (1, sizeof *node);
	if (!ret)
		die ("calloc");
	return (ret);
}
