/* address.c -- line address parsing methods */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "edna.h"
#include "addr.h"

typedef void (*op)(Set *, char *, State *);

struct reply {
	char *str;
	size_t len;
	enum ident *stack;
	size_t height;
};

static char *symbols[] = {
	"1234567890",	/* NUM_LITERAL */
	".",		/* NUM_SYMBOL */
	"/?",		/* REG_LINE */
	"`~",		/* REG_BUF */
	"!",		/* UN_OP */
	"+-*,&|^",	/* BIN_OP */
	";'\"",		/* SUBSTITUTION */
	NULL
};

/*
op line_ops[] = { forwl, backl, NULL };
op buf_ops[] = { forwb, backb, NULL };
op un_ops[] = { neg, NULL };
op bin_ops[] = { add, sub, rep, range, NULL };
op sub_ops[] = { expmac, expmrk, expreg, NULL };
*/
op ops[][] =  {
	{ NULL },
	{ NULL },
	{ forwl, backl, NULL },
	{ forwb, backb, NULL },
	{ neg, NULL },
	{ add, sub, rep, range, and, or, xor, NULL },
	{ expmac, expmrk, expreg, NULL },
};

#define NO_GLOB (1)		/* don't treat contiguous sequences as on token */
#define DELIM (1 << 1)		/* delimited sequence */
#define LITERAL (1 << 2)	/* does nothing */
#define ASSIGN (1 << 3)	
#define HOMOGENOUS (1 << 4)	/* use one functions for every member */

static Rule ruleset[] = {
	LITERAL | HOMOGENOUS,
	NO_GLOB | LITERAL,
	DELIM | NO_GLOB,
	DELIM | NO_GLOB,
	0,
	NO_GLOB,
	ASSIGN | NO_GLOB,
};

struct reply *
lexaddr (char *line, size_t len)
{
	size_t i = 0, j = 0, k = 0, t = 0;
	enum ident tokstack[len];
	struct reply *reply;
	char cur, prev, delim, tokline[len * 2];
	/* justification: worst case string is '+++++...', where a delimiter is
	 * required after every symbol, meaning the buffer must have length:
	 *	<input len> * 2
	 */

	prev = delim = 0;
	cur = line[0];
	for (; cur && i < len; k < IDENT_LEN - 1 ? (++k) : (k = 0))
		if (strchr (symbols[k], cur)) {
			tokstack[t++] = k;
			do {
				if (cur == '\\' && prev != '\\')
					continue;
				if (ruleset[k] & DELIM) {
					if (cur == delim) {
						break;
					}
					if (!delim) {
						cur = delim;
					}
				}
				if (ruleset[k] & ASSIGN) {
					if (prev == '=' && !delim) {
						delim = cur;
						continue;
					}
					if (cur == delim)
						break;
				}

				/* the core of the loop */
				if (cur != '\\' || prev == '\\')
					tokline[j++] = cur;

				prev = cur;
				cur = line[++i];

				if (!(cur && strchr (symbols[k], cur)))
					break;
				if (ruleset[k] & NO_GLOB)
					break;
			} while (cur);
			tokline[j++] = 0;
			delim = 0;
		}
	tokline[j++] = 0;
	tokstack[t++] = 0;

	if (!(reply = malloc (sizeof *reply))) die ("malloc");
	if (!(reply->str = malloc (i * sizeof *reply->str))) die ("malloc");
	if (!(reply->stack = malloc (t * sizeof *reply->stack))) die ("malloc");

	memcpy (reply->str, tokline, j);
	memcpy (reply->stack, tokstack, t);
	reply->len = j;
	reply->height = t;

	return reply;
}

Line **
evaladdr (char *line, size_t len, State *st, Arg *arg)
{
	char *s;
	size_t i, acc, len, *t;
	Set *sel[st->buflen];
	struct reply;

	reply = lexaddr (line, len);
	s = reply->str;
	t = reply->stack;

	acc = 0;
	for (size_t off; i < reply->hieght; ++i) {
		len = strlen (s);
		off = strchr (symbols[*t], *s);
		acc = (*ops[*t][off]) (sel, acc, s, s + len, st);
		s += len;
	}

	free (reply->str);
	free (reply->stack);
	free (reply);
}
