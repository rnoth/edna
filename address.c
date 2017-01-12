/* address.c -- line address parsing methods */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "edna.h"
#include "addr.h"
#include "set.h"

const char *symbols[] = {
	"1234567890",	/* NUM_LITERAL */
	".",		/* NUM_SYMBOL */
	"/?",		/* REG_LINE */
	"`~",		/* REG_BUF */
	"!",		/* UN_OP */
	"+-*,&|^",	/* BIN_OP */
	";'\"",		/* SUBSTITUTION */
	NULL
};

const Unary unops[] = {
	neg,
};

const Evaluator evals[] = {
	num,
	symnum,
};

#define NO_GLOB (1)		/* don't treat contiguous sequences as on token */
#define DELIM (1 << 1)		/* delimited sequence */
#define VALUE (1 << 2)
#define ASSIGN (1 << 3)	
#define HOMOGENOUS (1 << 4)	/* use one functions for every member */
#define BIN (1 << 5)	
#define UN (1 << 6)	

static const Rule ruleset[] = {
	VALUE | HOMOGENOUS,
	NO_GLOB | VALUE,
	DELIM | NO_GLOB,
	DELIM | NO_GLOB,
	0,
	NO_GLOB,
	ASSIGN | NO_GLOB | VALUE,
};

Line **
evaladdr (struct tokaddr *tok, Buffer *buf, char *error)
{
	char *s;
	size_t i, j, k;
	enum ident *t;
	Operator oplist[tok->height];
	Set sel, tmp, values[tok->height];

	s = tok->str;
	t = tok->stack;

	if (!memset (values, 0, tok->height)) die ("memset");
	i = j = k = 0;
	for (; i < tok->height; ++i, s += strlen (s)) {
		if (ruleset[t[i]] & VALUE) {
			if (i != tok->height - 1 && ruleset[t[i+1]] & VALUE) {
				strcpy (error, "invalid sequence of values");
				return NULL; /* FIXME: resource leak */
			}

			values[j++] = (*evals[t[i]]) (s, buf, error);

		} else if (ruleset[t[i]] & BIN) {
			if (i == 0) {
				values[j++] = defaultarg (s, buf, LEFT, error);
			}

			if (i == tok->height - 1 || ruleset[t[i+1]] & BIN) {
				values[j++] = defaultarg (s, buf, RIGHT, error);
			}

			oplist[k++] = getbinop (s);

		} else if (ruleset[t[i]] & UN) {
			if (i == tok->height - 1) {
				strcpy (error, "dangling unary operator");
				return NULL; /* FIXME: resource leak */
			}

			if (!(ruleset[t[i+1]] & VALUE)) {
				strcpy (error, "invalid argument to unary operator");
				return NULL; /* FIXME: resource leak */
			}

			tmp = (*evals[t[i+1]]) (s + strlen (s), buf, error);
			values[j++] = (*unops[strchr (symbols[UN_OP], *s) - s]) (s, tmp, buf, error);
			s += strlen (s);
			++i;
		} else if (ruleset[t[i]] & ASSIGN) {
		}
	}

	sel = values[0];
	i = 1;
	for (; i < k; ++i)
		if (!(sel = (*oplist[i]) (sel, values[i], buf, error)))
			return NULL; /* FIXME: resource leak */

	return resolveset (sel, buf->len, buf, error);
}

struct tokaddr *
lexaddr (char *line, size_t len)
{
	size_t i = 0, j = 0, k = 0, t = 0;
	enum ident tokstack[len];
	struct tokaddr *tok;
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

	if (!(tok = malloc (sizeof *tok))) die ("malloc");
	if (!(tok->str = malloc (j * sizeof *tok->str))) die ("malloc");
	if (!(tok->stack = malloc (t * sizeof *tok->stack))) die ("malloc");

	memcpy (tok->str, tokline, j);
	memcpy (tok->stack, tokstack, t);
	tok->len = j;
	tok->height = t;

	return tok;
}

