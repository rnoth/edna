/* parse.c -- functions for processing input */
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>

#include "edna.h"
#include "addr.h"
#include "str.h"

extern int		parseline	(String *, Buffer *, Arg *, char *);

/* TODO: static declarations for helper functions */

char *
getname (String *s, size_t *pos)
{
	char *cur, *ret;
	short esc;
	size_t i, ext;
	wchar_t wc;

	i = 0;
	cur = NULL;
	esc = 0;
	ret = malloc (s->b);
	if (!ret) die ("malloc");

	do {
		cur  = get_uchar (s->v + *pos);
		ext  = uchar_extent (*cur);
		cur[ext] = 0;

		if (*cur == '\\' && !esc) {
			esc = 1;
			continue;
		}

		mbtowc (&wc, cur, ext);

		if (iswalpha (wc) || esc) {
			memcpy (ret + i, cur, ext);
			i += ext;
		} else
			break;

		(*pos) += ext;
		free (cur);

	} while (i < s->b);

	ret[i] = 0;

	free (cur);

	return (ret);
}

char *
getarg (const String *s, const char *delim, size_t *pos)
{
	char *ret;
	size_t i;
	int ext, esc;

	ret = malloc (s->b - *pos);
	if (!ret) die ("malloc");

	i = esc = 0;
	while (*pos < s->b) {
		ext = uchar_extent (s->v[*pos]);
		if (ext == -1)	/* TODO: don't just ignore invalid utf-8 */
			continue;
		if (!esc && !strcmp (s->v + *pos, delim))
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
	ret = get_uchar (s->v + *pos);
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
	size_t i, pos;

	pos = 0;
	/* line address */
	if (!(tmp = getaddr (s, &pos, buf, error)))
		goto finally;
	arg->sel = *(Selection *)tmp;

	/* command name */
	if (!(tmp = getname (s, &pos)))
		goto finally;
	arg->name = tmp;

	/* argument vector */
	delim = setdelim (s, &pos);

	for (i = 0; (tmp = getarg (s, delim, &pos)); ++i) {
		++arg->cnt;
		arg->vec[i++] = tmp;
	}

	free (delim);
	ret = SUCC;

finally:
	return (ret);

}
