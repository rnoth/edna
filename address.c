/* address.c -- line address parsing methods */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "edna.h"
#include "cmd.h"
#include "addr.h"
#include "set.h"
#include "str.h"

const char *symbols[] = {
	"1234567890\0",	/* NUM_LITERAL */
	".\0"	"$\0",	/* NUM_SYMBOL */
	"+\0" /*"-\0"*/,/* OP_ARITH */
	NULL		/* IDENT_LEN */
};

const Lexer lexers[] = {
	trynum,
	trysym,
	tryarith,
	NULL
};

const Operator arithops[] = {
	addr_plus,
	NULL
};

const Evaluator nums[] = {
	addr_num,
	NULL
};

const Evaluator numsyms[] = {
	addr_dot,
	addr_dollar,
	NULL
};

const Rule ruleset[] = {
	VALUE | NUMBER,
	VALUE | LINE,
	OPERATOR,
	0
};

void *
getaddr (String *s, size_t *pos, Buffer *buf, char *err)
{
	Set sel;
	Selection *ret;
	Node *tree;

	tree = parseaddr (s, pos, err);
	sel  = evaltree (tree, buf, err);
	ret  = resolveset (sel, buf->len, buf, err);
	freetree (tree);
	return (ret);
}

Set
evaltree (Node *cur, Buffer *buf, char *err)
{
	if (!cur)
		return NULL;

	if (ruleset[cur->tok] & VALUE) {
		return ((cur->ev) (cur, buf, err));

	} else if (ruleset[cur->tok] & OPERATOR) {
		return ((cur->op) (cur->left, cur->right, buf, err));
	} else
		strcpy (err, "unknown token (this is not your fault)");
		return NULL;
}

Node *
next (String *s, size_t *pos)
{
	size_t i, j;
	Node *tmp, *ret = NULL;

	if (*pos >= s->b)
		return NULL;

	i =j = 0;
	for (; i < IDENT_LEN; ++i) {
		tmp = (lexers[i]) (s, pos); /* try and find a match */
		if (!tmp)
			continue;
		ret = tmp;
		ret->tok = i;
		break;
	}

	return (ret);
}

Node *
parseaddr (String *s, size_t *pos, char *err)
{
	Node *cur, *new;

	cur = NULL;
	while ((new = next(s, pos))) {
		if (!cur) {
			cur = new;
			continue;
		}
		if (ruleset[new->tok] & VALUE) {

			if (addnode (cur, new) == FAIL) {
				sprintf (err, "invalid sequence of values %lu bytes in", *pos);
				goto fail;
			}

			cur = new;
			continue;

		} else if (ruleset[new->tok] & OPERATOR) {

			if (cur->tok >= new->tok) { /* precedence */
				if (addnode (cur, new)) {
					cur = new;
					continue;
				} else if (extendbranch_r (cur, new)) {
					cur = new;
					continue;
				} else {
					strcpy (err, "parsing error (this is not your fault)");
					goto fail;
				}
			} else { /* cur->tok < new->tok */
				if (addnode (new, cur)) {
					cur = new;
					continue;
				} else {
					strcpy (err, "parsing error (this is not your fault)");
					goto fail;
				}
			}
		}
	}

	return getroot (cur);

fail:
	cur = getroot (cur);
	freetree (cur);
	return NULL;
}
