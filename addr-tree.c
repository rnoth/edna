#include <stdlib.h>
#include "edna.h"

int
addnode(Node *mother, Node *child)
{
	int ret = -1;
	if (!mother->left) {
		mother->left = child;
		child->up = mother;
		ret = 0;
	} else if (!mother->right) {
		mother->right = child;
		child->up = mother;
		ret = 0;
	}
	return ret;
}

Node *
getroot(Node *cur)
{
	while (cur && cur->up) cur = cur->up;
	return cur;
}

void
freenode(Node *cur)
{
	str_free(cur->str);
	free(cur);
}

void
freetree(Node *cur)
{
	if (cur) {
		freetree(cur->left);
		freetree(cur->right);
		str_free(cur->str);
		free(cur);
	}
	return;
}
	

Node *
makenode(void)
{
	Node *ret;
	ret = calloc(1, sizeof *ret);
	if (!ret)
		die("calloc");
	ret->str = str_alloc();
	return ret;
}
