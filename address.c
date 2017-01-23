/* address.c -- line address parsing methods */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "edna.h"
#include "cmd.h"
#include "addr.h"
#include "set.h"
#include "str.h"

Selection
getaddr (String *s, size_t *pos, buffer *buf, char *err)
{
	Selection sel;
	Node *tree;

	tree = parseaddr (s, pos, error);
	if (!tree)
		return NULL;

	sel = evaltree (tree, buf, error);
	return resolveset (sel, buf->len, buf, err);
}

Set
evaltree (Node *cur, Buffer *buf, char *err)
{
	Operator op;
	Evaluator ev;
	Set sl, sr;

	if (!cur)
		return NULL;

	if (ruleset[cur->tok] & VALUE) {
		ev = geteval (cur);
		return ((ev) (buf, err));
	} else if (ruleset[cur->tok] & OPERATOR) {
		op = getop (cur);

		sl = evaltree (cur->left, buf, err);
		sr = evaltree (cur->right, buf, err);

		return ((op) (sl, sr, buf, err);
	} else
		strcpy (err, "unknown token (this is not your fault)");
		return NULL;
}

Node *
next (String *s, *size_t pos)
{
	size_t i, j, len;
	Node *tmp, *ret = NULL;

	if (pos >= s->b)
		return NULL;

	i =j = 0;
	for (; i < IDENT_LEN; ++i) {
		tmp = (lexers[i]) (s, pos); /* try and find a match */
		if (!tmp)
			continue;
		ret = tmp;
		break;
	}

	return (ret);
}

Node *
parseaddr (String *s, size_t *pos, char *err)
{
	Node *cur, *new, *tmp;
	Selection ret = NULL;

	while (new = next(s, pos)) {
		if (!cur) {
			cur = new;
			continue;
		}
		if (ruleset[tmp->tok] & VALUE) {
			if (addnode (new) == FAIL) {
				snprintf (err, 80,
					"invalid sequence of values %ld bytes in",
					pos)
				goto error;
			}

			continue;

		} else if (ruleset[new->tok] & OPERATOR) {
			if (cur->tok >= new->tok) { /* precedence */
				if (addnode (cur, new))
					continue;
				else if (extendbranch_r (cur, new))
					continue;
				else {
					strcpy (err, "parsing error (this is not your fault)");
					goto error;
				}
			} else { /* cur->tok < new->tok */
				if (addnode (new, cur)) {
					cur = new;
					continue;
				} else {
					strcpy (err, "parsing error (this is not your fault)");
					goto error;
				}
			}
		}
	}

	return getroot (cur);

error:
	freetree (cur);
	return NULL;
}
