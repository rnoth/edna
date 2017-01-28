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

static char*	getname		(const String *, size_t *);
static char*	setdelim	(const String *, size_t *);
static char*	getarg		(const String *, const char *, size_t *);

char *
getname (const String *s, size_t *pos)
{
	char *cur, *ret;
	short esc;
	size_t off, ext;
	wchar_t wc;

	if (0 >= s->b - *pos)
		return (ERR);

	off = esc = ext = 0;
	ret = malloc (s->b - *pos);
	if (!ret) die ("malloc");

	do {
		cur = get_uchar (s->v + *pos);
		ext = uchar_extent (*cur);
		cur[ext] = 0;

		if (*cur == '\\' && !esc) {
			esc = 1;
			free (cur);
			cur = NULL;
			continue;
		}

		mbtowc (&wc, cur, ext);

		if (!iswalpha (wc) && !esc)
			break;

		memcpy (ret + off, cur, ext);
		off += ext;

		*pos += ext;
		free (cur);
		cur = NULL;

	} while (off < s->b);
	ret[off] = 0;

	if (!cur) free (cur);
	return (ret);
}

char *
setdelim (const String *s, size_t *pos)
{
	char *ret;
	int ext;

	if (s->b - *pos <= 0)
		return (NULL);

	ret = get_uchar (s->v + *pos);
	ext = uchar_extent (*ret);
	ret[ext] = 0;
	(*pos) += ext;

	return (ret);
}

char *
getarg (const String *s, const char *delim, size_t *pos)
{
	char *ret;
	size_t off;
	int ext, esc;

	if (s->b - *pos <= 0)
		return (NULL);

	ret = malloc (s->b - *pos);
	if (!ret) die ("malloc");

	off = esc = ext = 0;
	while (*pos < s->b) {
		ext = uchar_extent (s->v[*pos]);
		if (ext == -1)
			ext = 1;

		if (s->v[*pos] == '\\' && !esc) {
			esc = 1;
			continue;
		}
		if (!esc && !strcmp (s->v + *pos, delim))
			break;
		else if (s->v[*pos] == '\n')
			break;

		memcpy (ret + off, s->v + *pos, ext);

		*pos += ext;
		off += ext;
	}

	*pos += ext;
	ret[off] = 0; /* terminate */
	return (ret);
}

int
parseline (String *s, Buffer *buf, Arg *arg, char *error)
{
	int ret;
	void *tmp;
	char *delim;
	size_t pos;

	ret = FAIL;
	pos = 0;
	/* line address */
	tmp = getaddr (s, &pos, buf, error);
	if (tmp == ERR || tmp == NULL)
		goto finally;
	arg->sel = *(Selection *)tmp;

	/* command name */
	tmp = getname (s, &pos);
	if (tmp == ERR)
		goto finally;
	arg->name = tmp;

	/* delimiter */
	tmp = setdelim (s, &pos);
	if (tmp == ERR)
		goto finally;
	delim = tmp;

	/* argument vector */
	for (;;) {
		tmp = getarg (s, delim, &pos);
		if (tmp == ERR)
			goto finally;
		if (tmp == NULL)
			break;
		arg->vec = realloc (arg->vec, ++arg->cnt);
		if (!arg->vec) {
			perror ("realloc");
			goto finally;
		}
		arg->vec[arg->cnt - 1] = tmp;
	}

	free (delim);
	ret = SUCC;

finally:
	return (ret);

}
