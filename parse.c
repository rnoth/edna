/* parse.c -- functions for processing input */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "edna.h"
#include "str.h"

extern		int	parseline	(String, Arg *);

static struct reply*	lexline		(String);

struct reply {
	String str;
	size_t seg;
};

struct reply *
lexline (String line)
{
	/* `line->c + 4' is enough for even pathological inputs[0],
	 * - one null byte delimits first line address
	 * - one null byte delimits name
	 * - two null bytes terminate the string. 
	 * futhermore,
	 * - every delimiter character is collapsed into a null byte
	 * thus, bufsiz + 4 will always be enough
	 * [0]: unless it's long enough overflow
	 */
	char ch, delim; 
	size_t seg;
	String s;
	struct reply *reply;

	chomp (line);
	s = makestring (line.c + 4);
	seg = 0;

	/* ignore leading whitespace */
	for (; isspace (*line.v); ++line.v, --line.c)
		;

	/* line address */
	for (; line.c && (ch = *line.v) && (isdigit (ch) || strchr ("+-", ch)); ++line.v, --line.c)
		s.v[s.c++] = ch;
	s.v[s.c++] = 0;
	++seg;

	/* command name */
	for (; line.c && isalpha (ch = *line.v); ++line.v, --line.c)
		s.v[s.c++] = ch;
	s.v[s.c++] = 0;
	++seg;

	/* delimiter */
	delim = *line.v++;

	/* arbitrary argument vector */
	for (; (line.c && *line.v);) {
		for (; (ch = *line.v) && ch != delim; --line.c, ++line.v)
			s.v[s.c++] = ch;
		s.v[s.c++] = 0;
		++seg;
	}
	s.v[s.c++] = 0;

	/* finish */
	if (!(reply = malloc (sizeof *reply))) die ("malloc");

	reply->str = makestring (s.c);
	copystring (&reply->str, &s);
	reply->seg = seg;

	freestring (s);

	return reply;
}

int
parseline (String line, Arg *arg)
{
	String str;
	size_t i, seg, len;
	struct reply *reply;

	reply = lexline (line);

	seg = reply->seg;
	str = reply->str;

	/* line address */
	if (str.v[0] && strchr ("+-", str.v[0])) {
		arg->rel = 1;	/* FIXME: does not handle complex uses of +/- */
		if (!str.v[1])
			arg->addr = str.v[0] == '+' ? 1 : -1;
	}

	if (isdigit(str.v[0]) || isdigit(str.v[1]))	/* FIXME: so ugly */
		arg->addr = strtol (str.v, &str.v, 10);

	str.v += strlen (str.v) + 1;
	--seg;

	/* command name */
	strcpy (arg->name, str.v);

	str.v += strlen (str.v) + 1;
	--seg;

	if (seg)
		if (!(arg->vec = malloc (seg * sizeof *arg->vec))) die ("malloc");
	for (i = 0; i < seg; ++i) {
		len = strlen (str.v) + 1;
		if (!(arg->vec[i] = malloc (len * sizeof *arg->vec[i])))
			die ("malloc");
		strcpy (arg->vec[i], str.v);
		str.v += len;
	}

	freestring (reply->str);
	free (reply);

	return SUCC;
}
