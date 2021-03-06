/* parse.c -- functions for processing input */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#include "edna.h"

static char *	getname		(String const *, size_t *);
static char *	setdelim	(String const *, size_t *);
static char *	getarg		(String const *, size_t *, char *);

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
	char cur[5], tmp[len(s) - *pos], *ret;
	short esc, ext;
	size_t off;

	if (eol(s, *pos)) return NULL;

	off = esc = ext = 0;
	while (!eol(s, *pos)) {
		ext = get_uchar(cur, s->v + *pos);

		if (ext == -1) ext = 1;
		else if (ext == 0) {
			*pos += 1;
			break;
		}

		if (*cur == '\\' && !esc) {
			esc = 2;
			continue;
		}

		if (!esc && !check(cur, context)) break;

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
	char *tmp = 0;
	char *delim  = 0;
	size_t pos = 0;

	arg->sel = getaddr(s, &pos, buf, error);

	arg->name = getname(s, &pos);
	if (!arg->name) goto finally;

	delim = setdelim(s, &pos);
	if (!delim) goto finally;

	make_vector(arg->param);
	if (!arg->param) goto nomem;
	do {
		tmp = getarg(s, &pos, delim);
		if (!tmp) break;
		vec_append(arg->param, &tmp);
	} while (tmp);


	free(delim);

finally:
	return 0;

nomem:
	free(delim);
	free(arg->name);
	vec_free(arg->sel);
	return ENOMEM;
}
