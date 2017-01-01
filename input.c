/* input.c -- functions for getting and processing input */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "edna.h"

extern void	readline	(char **, size_t *, char *, ...);
extern void	parseline	(char *, size_t, Arg *);

static struct reply*	lexline		(char *, size_t);

struct reply {
	char *str;
	size_t len;
	size_t seg;
};

void
readline (char **buf_ptr, size_t *bufsiz_ptr, char *prompt, ...)
{
	va_list va;
	va_start (va, prompt);
	if (0 > vprintf (prompt, va))
		die ("vprintf");
	getline (buf_ptr, bufsiz_ptr, stdin);
	va_end (va);
	return;
}

struct reply *
lexline (char *buf, size_t bufsiz)
{
	/* `bufsiz + 4' is enough for even pathological inputs,
	 * - one null byte delimits first line address
	 * - one null byte delimits name
	 * - two null bytes terminate the string. 
	 * futhermore,
	 * - every delimiter character is collapsed into a null byte
	 * - multiple delimiters collapse into one
	 * - `buf' - `name' - `addr's will alway be longer than
	 *   `buf' - `name' - `addr's - `delim's
	 * thus, bufsiz + 4 will always be enough
	 */
	char ch, delim, s[bufsiz + 4];
	size_t i, j, k;
	struct reply *reply;

	chomp (buf, bufsiz);
	i = j = k = 0;
	for (; isspace (ch = buf[i]); ++i)
		;
	/* line address */
	for (; isdigit (ch = buf[i]) || strchr ("+-,", ch); ++i, ++j)
		s[j] = ch;
	s[j] = 0;
	++j;
	++k;
	/* command name */
	for (; isalpha (ch = buf[i]); ++i, ++j)
		s[j] = ch;
	s[j] = 0;
	++j;
	++k;
	/* secondary line address */
	for (; isdigit (ch = buf[i]) || (ch && strchr ("+-,", ch)); ++i, ++j)
		s[j] = ch;
	s[j] = 0;
	++j;
	++k;
	/* delimiter */
	delim = buf[i];
	++i;
	/* arbitrary argument vector */
	for (; (buf[i]);) {
		for (; (ch = buf[i]) && ch != delim; ++i, ++j)
			s[j] = ch;
		s[j] = 0;
		++j;
		++k;
		for (; (ch = buf[i]) == delim; ++i)
			;
	}
	s[j] = 0;
	/* finish */
	if (!(reply = malloc (sizeof *reply)))
		die ("malloc");
	if (!(reply->str = malloc (j * sizeof *reply->str)))
		die ("malloc");

	if (!(memcpy (reply->str, s, j)))
		die ("memcpy");
	reply->len = j;
	reply->seg = k;
	return reply;
}

void
parseline (char *buf, size_t bufsiz, Arg *arg)
{
	char *str;
	size_t k;
	struct reply *reply;
	reply = lexline (buf, bufsiz);
	if (!reply || !reply->str)
		die ("malloc(?)");	/* will probably never happen */
	k = reply->seg;
	str = reply->str;
	/* line address */
	if (str[0] && strchr ("+-", str[0]) )
		arg->rel = 0; /* FIXME: doesn't parse complex use of +/-
			       * e.g. 1+2, .-4, +++, etc.
			       */
	if (strchr (str, ',')) {
		; /* TODO */
	}
	if (strchr (str, '.')) {
		; /* TODO */
	}
	if (str[0])
		arg->addr = strtol (str, &str, 10);
	//if (reply->str[0])
		//arg->addr2 = strtol (reply->str, NULL, 10);
	str += strlen (str) + 1;
	--k;
	/* command name */
	strcpy (arg->name, str);
	str += strlen (str) + 1;
	--k;
	/* secondary line address */
	//strcpy (arg->addr2, str);
	str += strlen (str) + 1;
	--k;
	arg->cnt = k;
	if (k)
		if (!(arg->vec = malloc (k * sizeof *arg->vec)))
			die ("malloc");
	for (size_t len, j = 0; j < k; ++j) {
		len = strlen (str) + 1;
		if (!(arg->vec[j] = malloc (len * sizeof *arg->vec[j])))
			die ("malloc");
		strcpy (arg->vec[j], str);
		str += len;
	}

	free (reply->str);
	free (reply);

	return;
}

