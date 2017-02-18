/* parse.c -- functions for processing input */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#include "edna.h"

static char*	getname		(String const *, size_t *);
static char*	setdelim	(String const *, size_t *);
static char*	getarg		(String const *, size_t *, char *);

bool
alphacheck(char const *cur, void *_)
{
	wchar_t wc;
	mbtowc(&wc, cur, 4);
	return iswalpha(wc);
}

bool
delimcheck(char const *cur, void *_delim)
{
	char *delim;
	delim = _delim;
	return !!strcmp(cur, delim);
}

char *
getseq(const String *s, size_t *pos,
		bool (*check)(char const *, void *),
		void *context)
{
	char cur[5], tmp[s->b - *pos], *ret;
	short esc, ext;
	size_t off;

	if (eol(s, *pos)) return NULL;

	off = esc = ext = 0;
	while (!eol(s, *pos)) {
		ext = get_uchar(cur, s->v + *pos);

		if (*cur == '\\' && !esc) {
			esc = 2;
			continue;
		}

		if (esc || !check(cur, context))
			break;

		memcpy(tmp + off, cur, ext);
		off += ext;

		*pos += ext;
		if (esc) --esc;

	}

	tmp[off] = 0;
	ret = malloc(off + 1);
	if (!ret) die("malloc");
	memcpy(ret, tmp, off + 1);

	return ret;
}

char *
getname(const String *s, size_t *pos)
{

	return getseq(s, pos, alphacheck, NULL);
}

char *
getarg(const String *s, size_t *pos, char *delim)
{
	return getseq(s, pos, delimcheck, delim);
}

char *
setdelim(const String *s, size_t *pos)
{
	char *ret;

	if (eol(s, *pos)) return NULL;

	ret = malloc(5);
	(*pos) += get_uchar(ret, s->v + *pos);

	return ret;
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
	tmp = getaddr(s, &pos, buf, error);
	if (tmp) {
		make_vector(arg->sel);
		vec_copy(arg->sel, *(Selection *)tmp);
		free_vector(*(Selection *)tmp);
		free (tmp);
	}

	/* command name */
	tmp = getname(s, &pos);
	if (!tmp)
		goto finally;
	arg->name = tmp;

	/* delimiter */
	tmp = setdelim(s, &pos);
	if (!tmp)
		goto finally;
	delim = tmp;

	/* argument vector */
	make_vector(arg->param);
	do {
		tmp = getarg(s, &pos, delim);
		if (!tmp)
			break;
		vec_append(arg->param, tmp);
	} while (tmp);


	free(delim);

finally:
	ret = SUCC;
	return ret;

}
