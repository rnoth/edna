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
readline (char **line_ptr, size_t *len_ptr, char *prompt, ...)
{
	va_list va;
	va_start (va, prompt);
	if (0 > vprintf (prompt, va))
		die ("vprintf");
	getline (line_ptr, len_ptr, stdin);
	va_end (va);
	return;
}

struct reply *
lexline (char *line, size_t linelen)
{
	/* `linelen + 4' is enough for even pathological inputs,
	 * - one null byte delimits first line address
	 * - one null byte delimits name
	 * - two null bytes terminate the string. 
	 * futhermore,
	 * - every delimiter character is collapsed into a null byte
	 * - multiple delimiters collapse into one
	 * - `line' - `name' - `addr's will alway be longer than
	 *   `line' - `name' - `addr's - `delim's
	 * thus, bufsiz + 4 will always be enough
	 */
	char ch, delim, s[linelen + 4];
	size_t i, j, k;
	struct reply *reply;

	chomp (line, linelen);
	i = j = k = 0;
	for (; isspace (ch = line[i]); ++i)
		;
	/* line address */
	for (; isdigit (ch = line[i]) || strchr ("+-,", ch); ++i, ++j)
		s[j] = ch;
	s[j] = 0;
	++j;
	++k;
	/* command name */
	for (; isalpha (ch = line[i]); ++i, ++j)
		s[j] = ch;
	s[j] = 0;
	++j;
	++k;
	/* secondary line address */
	for (; isdigit (ch = line[i]) || (ch && strchr ("+-,", ch)); ++i, ++j)
		s[j] = ch;
	s[j] = 0;
	++j;
	++k;
	/* delimiter */
	delim = line[i];
	++i;
	/* arbitrary argument vector */
	for (; (line[i]);) {
		for (; (ch = line[i]) && ch != delim; ++i, ++j)
			s[j] = ch;
		s[j] = 0;
		++j;
		++k;
		//for (; (ch = line[i]) == delim; ++i)
			//;
		// motivation: some commands like sub like to alter their
		// behavior if a certain input field is empty.
		// note that this potentitally invalids the long argument above
		// justifying linelen + 4 as a reasonable buffersize
	}
	s[j] = 0;
	/* finish */
	if (!(reply = malloc (sizeof *reply))) die ("malloc");
	if (!(reply->str = malloc (j * sizeof *reply->str))) die ("malloc");

	if (!(memcpy (reply->str, s, j))) die ("memcpy");
	reply->len = j;
	reply->seg = k;

	return reply;
}

void
parseline (char *line, size_t linelen, Arg *arg)
{
	char *str;
	size_t k;
	struct reply *reply;

	reply = lexline (line, linelen);
	if (!reply || !reply->str) die ("malloc(?)");	/* will probably never happen */

	k = reply->seg;
	str = reply->str;

	/* line address */
	if (str[0] && strchr ("+-", str[0]) ) {
		arg->rel = 1; /* FIXME: does not handle complex uses of +/- */
		if (!str[1])
			arg->addr = str[0] == '+' ? 1 : -1;
	}
	if (strchr (str, ',')) {
		; /* TODO */
	}
	if (strchr (str, '.')) {
		; /* TODO */
	}
	if (isdigit(str[0]) || isdigit(str[1])) /* FIXME: so ugly */
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

