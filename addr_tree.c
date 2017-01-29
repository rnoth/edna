/* tree.c -- routines for manipulating syntax nodes */
#include <stdlib.h>

#include "addr.h"
#include "edna.h" /* FIXME: only needed for LINESIZE, which is deprecated */
#include "str.h"
#include "util.h"

int
addnode (Node *mother, Node *child)
{
	int ret = FAIL;
	if (!mother->left) {
		mother->left = child;
		child->up = mother;
		ret = SUCC;
	} else if (!mother->right) {
		mother->right = child;
		child->up = mother;
		ret = SUCC;
	}
	return ret;
}

Node *
getroot (Node *cur)
{
	while (cur && cur->up)
		cur = cur->up;
	return cur;
}

void
freenode (Node *cur)
{
	freestring (cur->str);
	free (cur);
}

void
freetree (Node *cur)
{
	if (cur) {
		freetree (cur->left);
		freetree (cur->right);
		freestring (cur->str);
		free (cur);
	}
	return;
}
	

Node *
makenode (void)
{
	Node *ret;
	ret = calloc (1, sizeof *ret);
	if (!ret)
		die ("calloc");
	ret->str = makestring (LINESIZE);
	return (ret);
}
