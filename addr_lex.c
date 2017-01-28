/* addr_lex.c -- routines for lexing line addresses */
#include <ctype.h>

#include "addr.h"
#include "str.h"

/*
 * FIXME: lots of duplicated code in here
 */

Node *
tryarith (String *s, size_t *pos)
{
	Node *ret;
	size_t i, j, len;

	len = 0;

	for (i = j = 0; symbols[OP_ARITH][i]; i += len + 1, ++j) {
		len = strlen (symbols[OP_ARITH] + i);
		if (!strncmp (s->v + *pos, symbols[OP_ARITH] + i, len))
			break;
	}

	if (!symbols[OP_ARITH] + i)
		return (NULL);

	ret = makenode ();

	ret->op = arithops[j];
	strncpy (ret->str->v, symbols[OP_ARITH] + i, len);
	ret->str->b = len + 1;

	*pos += len;

	return (ret);
}

Node *
trynum (String *s, size_t *pos)
{
	Node *ret;

	ret = makenode ();

	for (; isdigit (s->v[*pos]); ++(*pos))
		appendchar (ret->str, s->v[*pos]);

	if (!ret->str->c) { /* no numbers */
		freenode (ret);
		ret = NULL;
	} else
		ret->ev = addr_num;

	return (ret);

}

Node *
trysym (String *s, size_t *pos)
{
	Node *ret;
	size_t i, j, len;

	for (i = j = 0; symbols[NUM_SYMBOL] + i; i += len + 1, ++j) {

		len = strlen (symbols[NUM_SYMBOL] + i);

		if (!strncmp (s->v, symbols[NUM_SYMBOL] + i, len))
			break;
	}

	if (!symbols[NUM_SYMBOL] + i)
		return (NULL);

	ret = makenode ();

	ret->ev = numsyms[j];
	/* TODO: put this in its own function */
	strncpy (ret->str->v, symbols[NUM_SYMBOL] + i, len);
	ret->str->b = len + 1;
	/* TODO: set ret->str->c (needs a ustrlen() function */

	*pos += len;

	return (ret);
}
