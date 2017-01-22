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
	"1234567890",	/* NUM_LITERAL */
	".$",		/* NUM_SYMBOL */
	"",		/* IDENT_LEN */
	NULL
};

const Evaluator evals[] = {
	num,
	symnum,
};

#define NO_GLOB		BIT(0)	/* don't treat contiguous sequences as on token */
#define VALUE		BIT(1)	/* symbol to be evaluated */
#define HOMOGENOUS	BIT(2)	/* use one functions for every member */

static const Rule ruleset[] = {
	VALUE | HOMOGENOUS,
	NO_GLOB | VALUE,
};

Selection
evaladdr (struct tokaddr *tok, Buffer *buf, char *error)
{
	String *s;
	size_t i, j, k;
	enum ident *t;
	Selection ret;
	/*Operator oplist[tok->height];*/
	Set sel, /*tmp,*/ values[tok->height];

	s = tok->str;
	t = tok->stack;

	memset (values, 0, tok->height * sizeof *values);
	memset (&ret, 0, sizeof ret);
	i = j = k = 0;
	for (; i < tok->height; ++i, s->v += strlen (s->v)) {
		if (ruleset[t[i]] & VALUE) {
			if (i != tok->height - 1 && ruleset[t[i+1]] & VALUE) {
				strcpy (error, "invalid sequence of values");
				goto finally;
			}

			/* this is ugly and should be fixed */
			values[j++] = (*evals[t[i]]) (s, buf, error);

		}/* else if (ruleset[t[i]] & BIN) {
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
				return NULL; / * FIXME: resource leak * /
			}

			if (!(ruleset[t[i+1]] & VALUE)) {
				strcpy (error, "invalid argument to unary operator");
				return NULL; / * FIXME: resource leak * /
			}

			tmp = (*evals[t[i+1]]) (s + strlen (s), buf, error);
			values[j++] = (*unops[strchr (symbols[UN_OP], *s) - s]) (s, tmp, buf, error);
			s += strlen (s);
			++i;
		} else if (ruleset[t[i]] & ASSIGN) {
		}*/
	}

	sel = values[0];
	/*i = 1;
	for (; i < k; ++i)
		if (!(sel = (*oplist[i]) (sel, values[i], buf, error)))
			return NULL; / * FIXME: resource leak * /
	*/

	ret = resolveset (sel, buf->len, buf, error);
finally:
	return (ret);
}

struct tokaddr *
lexaddr (String *line)
{
	size_t i = 0, k = 0, t = 0;
	enum ident tokstack[line->c];
	struct tokaddr *tok;
	char cur, prev, delim;
	String *tokline;

	tokline = makestring (line->c * 2);
	/*
	 * justification: worst case string is '+++++...', where a delimiter is
	 * required after every symbol, meaning the buffer must have length:
	 *	<input len> * 2
	 */

	prev = delim = 0;
	cur = *line->v++;
	for (; cur && i < line->c; k < IDENT_LEN ? (++k) : (k = 0))
		if (strchr (symbols[k], cur)) {
			tokstack[t++] = k;
			do {
				if (cur == '\\' && prev != '\\')
					continue;
				/*if (ruleset[k] & DELIM) {
					if (cur == delim) {
						break;
					}
					if (!delim) {
						cur = delim;
					}
				}*/
				/*if (ruleset[k] & ASSIGN) {
					if (prev == '=' && !delim) {
						delim = cur;
						continue;
					}
					if (cur == delim)
						break;
				}*/

				/* the core of the loop */
				if (cur != '\\' || prev == '\\')
					tokline->v[tokline->c++] = cur;

				prev = cur;
				cur = *line->v++;

				if (!(cur && strchr (symbols[k], cur)))
					break;
				if (ruleset[k] & NO_GLOB)
					break;
			} while (cur);

			tokline->v[tokline->c++] = 0;
			delim = 0;
			k = 0;
		} else if (k == IDENT_LEN)
			break;

	tokline->v[tokline->c++] = 0;
	tokstack[t++] = IDENT_LEN;
	--line->v;

	if (!(tok = malloc (sizeof *tok))) die ("malloc");
	if (!(tok->stack = malloc (t * sizeof *tok->stack))) die ("malloc");
	tok->str = makestring (tokline->c);

	copystring (tok->str, tokline);
	memset (tok->stack, IDENT_LEN, t);
	memcpy (tok->stack, tokstack, t * sizeof *tok->stack);
	tok->height = t;

	return tok;
}
