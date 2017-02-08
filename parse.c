/* parse.c -- functions for processing input */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#include "edna.h"
#include "addr.h"
#include "cmd.h"
#include "line.h"
#include "str.h"
#include "util.h"

static char*	getname		(const String *, size_t *);
static char*	setdelim	(const String *, size_t *);
static char*	getarg		(const String *, const char *, size_t *);

char *
getname (const String *s, size_t *pos)
{
	char cur[5], tmp[s->b - *pos], *ret;
	short esc, ext;
	size_t off;
	wchar_t wc;

	if (*pos >= s->b - 1)
		return (NULL);

	off = esc = ext = 0;

	while (*pos < s->b) {
		ext = get_uchar (cur, s->v + *pos);

		if (ext <= 0) {
			if (esc)
				ext = -ext;
			else
				break;
		}

		if (*cur == '\\' && !esc) {
			esc = 2;
			continue;
		}

		mbtowc (&wc, cur, ext);

		if (!iswalpha (wc) && !esc)
			break;

		memcpy (tmp + off, cur, ext);
		off += ext;

		*pos += ext;
		if (esc) --esc;

	}

	tmp[off] = 0;
	ret = malloc (off + 1);
	if (!ret) die ("malloc");
	memcpy (ret, tmp, off + 1);

	return (ret);
}

char *
setdelim (const String *s, size_t *pos)
{
	void *tmp;
	char *ret;
	short ext;

	if (*pos >= s->b - 1)
		return (NULL);

	ret = malloc (5);
	ext = get_uchar (ret, s->v + *pos);
	if (ext <= 0)
		ext = -ext;
	(*pos) += ext;

	tmp = realloc (ret, ext);
	if (!tmp) {
		die ("realloc");
	} else
		ret = tmp;

	if (!ext) return (NULL);

	return (ret);
}

char *
getarg (const String *s, const char *delim, size_t *pos)
{
	char *ret;
	size_t off;
	int ext, esc;

	if (*pos >= s->b)
		return (NULL);

	ret = malloc (s->b - *pos + 1);
	if (!ret) die ("malloc");

	off = esc = ext = 0;
	while (*pos < s->b - 1) {
		ext = uchar_extent (s->v[*pos]);
		if (ext == -1)
			ext = 1;

		if (s->v[*pos] == '\\' && !esc) {
			esc = 2;
			continue;
		}
		if (!esc && !strncmp (s->v + *pos, delim, strlen (delim)))
			break;
		else if (s->v[*pos] == '\n')
			break;

		memcpy (ret + off, s->v + *pos, ext);

		*pos += ext;
		off += ext;
		if (esc) --esc;
	}

	if (off == 0) {
		free (ret);
		return (NULL);
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
	if (tmp) {
		make_vector (arg->sel);
		vec_copy (arg->sel, *(Selection *)tmp);
		free_vector (*(Selection *)tmp);
		free (tmp);
	}

	/* command name */
	tmp = getname (s, &pos);
	arg->name = tmp;

	/* delimiter */
	tmp = setdelim (s, &pos);
	delim = tmp;

	/* argument vector */
	tmp = getarg (s, delim, &pos);
	if (tmp)
		make_vector (arg->param);
	while (tmp) {
		vec_append (arg->param, tmp);
		tmp = getarg (s, delim, &pos);
	}

	ret = SUCC;

	free (delim);

	return (ret);

}
