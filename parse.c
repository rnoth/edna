/* parse.c -- functions for processing input */
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>

#include "edna.h"
#include "addr.h"
#include "str.h"

extern int		parseline	(String *, Buffer *, Arg *, char *);

/* TODO: static declarations for helper functions */

Line **
getaddr (String *s, size_t pos, Buffer *buf, char *error)
{
	return evaladdr (s, pos, buf, error);
}

char *
getname (String *s, size_t *pos)
{
	char cur[5], prev[5], *ret;
	Bool esc;
	size_t i, ext;
	wchat_t wc;

	i = 0;
	cur = 0;
	ret = malloc (s->b);
	if (!ret) die ("malloc");

	do {
		free (prev);
		prev = cur;
		cur  = get_uchar (s->v[*pos]);
		ext  = uchar_extent (*cur);
		cur[ext] = 0;
		(*pos) += ext;

		if (*cur == '\\' && !esc) {
			esc = True;
			continue
		}

		mbtowc (&wc, cur, ext));

		if (iswalpha (wc) || esc) {
			memcpy (ret + i, cur, ext);
			i += ext;
		} else
			break;

	} while (i < s.b);

	ret[i] = 0;

	free (cur);

	return (ret);
}

char *
getdelim (const String *s, const char *delim, size_t *pos)
{
	char *ret;
	size_t i;
	size_t len = strlen (delim);
	int ext;

	ret = malloc (s->b - *pos);
	if (!ret) die ("malloc");

	i = 0;
	while (*pos < s.b) {
		ext = uchar_extent (s->v + *pos);
		if (ext == -1)	/* TODO: don't just ignore invalid utf-8 */
			continue;
		if (!esc && strncmp (s->v, delim->v, MIN (ext, len))
			break;
		memcpy (ret + i, s->v + *pos, uchar_extent (s->v[*pos]));
		*pos += ext;
		i += ext;
	}

	if (!i) {
		free (ret);
		ret = NULL;
	} else
		ret[i] = 0; /* terminate */

	return (ret);
}

char *
setdelim (const String *s, size_t *pos)
{
	char *ret;
	int ext;

	ret = malloc (4);
	if (!ret) die ("malloc");
	ret = get_uchar (s->v);
	ext = uchar_extent (*ret);
	ret[ext] = 0;
	(*pos) += ext;

	return (ret);
}

int
parseline (String *s, Buffer *buf, Arg *arg, char *error)
{
	int ret = FAIL;
	void *tmp;
	char *delim;
	size_t pos;

	/* line address */
	if (!(tmp = getaddr (s, &pos, buf, error)
		goto finally;
	arg->sel = tmp;
	if (!arg->sel.v)
		goto finally;

	/* command name */
	if (!(tmp = getname (s, &pos)))
		goto finally;
	arg->name = tmp;
		
	/* argument vector */
	delim = setdelim (s, &pos);

	while (tmp = getdelim (s, delim, &pos)) {
		++arg->cnt;
		arg->vec[i++] = tmp;
	}

	free (delim);

finally:
	return (ret);
}
