#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "edna.h"
#include "cmd.h"

/* TODO: assumes way too much about the internals of String */
#if 0 
/*
 * retiring this after switching from arg->vec to arg->param.
 * I just don't feel like maintaining this at the moment
 */
int
subst (State *st, Buffer *buf, Arg *arg, char *error)
{
	regex_t *re;
	regmatch_t *m;
	char *tail;
	size_t i, j;

	if (!(re = malloc (sizeof *re))) die ("malloc");

	if (regcomp (re, arg->vec[0], REG_NEWLINE)) {
		strcpy (error, "error compiling regex");
		free (re);
		return FAIL;
	}

	if (!(m = malloc (sizeof *m))) die ("malloc");

	if (regexec (re, buf->curline->str->v, 1, m, 0) == REG_NOMATCH) {
		strcpy (error, "no matches");
		goto finally;
	}

	i = strlen (buf->curline->str->v + m->rm_eo);
	if (m->rm_so + i + strlen (arg->vec[1])
	    > buf->curline->len) {
		strcpy (error, "too large a replacement (FIXME)");
		goto finally;
	}

	if (!(tail = malloc (i))) die ("malloc");
	memcpy (tail, buf->curline->str->v + m->rm_eo, i + 1);
	memcpy (buf->curline->str->v + m->rm_so, arg->vec[1], strlen (arg->vec[1]) + 1);
	j = strlen (buf->curline->str->v);
	memcpy (buf->curline->str->v + j, tail, i + 1);

	regfree (re);
	free (m);
	free (tail);

	return print (st, buf, arg, error);

finally:
	regfree (re);
	free (m);
	return SUCC;
}
#endif
