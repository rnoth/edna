#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "edna.h"

const char *symbols[] = {
	[NUM_LITERAL] = "1234567890\0",
	[NUM_SYMBOL] =	".\0"	"$\0",
	[OP_ARITH] = "+\0",
	NULL
};

const Lexer lexers[] = {
	[NUM_LITERAL] =	trynum,
	[NUM_SYMBOL] =	trysym,
	[OP_ARITH] =	tryarith,
	NULL
};

const Rule ruleset[] = {
	[NUM_LITERAL] =	VALUE | NUMBER,
	[NUM_SYMBOL] =	VALUE | LINE,
	[OP_ARITH] =	OPERATOR,
	0
};

void *
getaddr(String *s, size_t *pos, Buffer *buf, char *err)
{
	Set *sel;
	Selection *ret;
	Node *tree;

	tree = parseaddr(s, pos, err);
	sel  = evaltree(tree, buf, err);
	ret  = resolveset(sel, buf, err);
	freetree(tree);
	freeset(sel);
	return ret;
}

Set *
evaltree(Node *cur, Buffer *buf, char *err)
{
	if (!cur) return NULL;

	if (ruleset[cur->tok] & VALUE) {
		return ((cur->ev) (cur, buf, err));

	} else if (ruleset[cur->tok] & OPERATOR) {
		return ((cur->op) (cur->left, cur->right, buf, err));

	} else
		strcpy (err, "evaltree(): unknown token");
		return NULL;
}

Node *
next(String *s, size_t *pos)
{
	size_t i, j;
	Node *tmp, *ret = NULL;

	if (eol(s, *pos)) return NULL;

	i =j = 0;
	for (; i < IDENT_LEN; ++i) {
		tmp = lexers[i](s, pos); /* try and find a match */
		if (!tmp) continue;
		ret = tmp;
		break;
	}

	return ret;
}

Node *
parseaddr(String *s, size_t *pos, char *err)
{
	Node *cur, *new;

	cur = NULL;
	while ((new = next(s, pos))) {
		if (!cur) {
			cur = new;
			continue;
		}
		if (ruleset[new->tok] & VALUE) {

			if (addnode(cur, new)) {
				sprintf(err, "invalid sequence of values %lu bytes in", *pos);
				goto fail;
			}

			continue;

		} else if (ruleset[new->tok] & OPERATOR) {
			if (ruleset[cur->tok] & VALUE) {
				addnode(new, cur);
				cur = new;
				continue;
			}
			for (;;) {
				if (cur->tok >= new->tok) {
					if (addnode(cur, new)) {
						cur = new;
						break;
					}
				} else {
					if (addnode(new, cur)) {
						cur = new;
						break;
					}
				}

				if (cur->up == NULL) {
					addnode(new, cur);
					cur = new;
					break;
				}
				cur = cur->up;
			}
		}
	}

	return getroot(cur);

fail:
	cur = getroot(cur);
	freetree(cur);
	return NULL;
}
