#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>

#include "edna.h"

extern void	readline	(char **, size_t *, char *, ...);
extern void	parseline	(char *, char *, Arg *);

void
readline (char **buf_ptr, size_t *bufsiz_ptr, char *prompt, ...)
{
	va_list v;
	va_start(v, prompt);
	if (0 > vprintf (prompt, v))
		die("vprintf");
	getline (buf_ptr, bufsiz_ptr, stdin);
	return;
}

void
parseline (char *buf, char *name, Arg *arg)
{
	char ch, delim;
	for (size_t i = 0; (ch = buf[i]) ;++i) {
		char tmp[64];
		int j = 0;
		switch (buf[i]) {
		case '+':
		case '-':
			arg->rel = 1;
			++i;
			goto num;
			break;
		default:
			if (isdigit(ch) && !arg->addr) {
				arg->rel = 0;
				goto num;
			}
			if (isalpha(ch) && !name[0]) {
				goto cmd;
			}
			goto str;
			break;
		num:
			if (arg->rel) {
				tmp[j] = ch; /* + or - */
				++j;
				if (!isdigit(buf[i + 1])) {
					tmp[j++] = '1';
				}	/* default to 1 on bare + and - */
			}
			for (; isdigit(ch = buf[i]); ++i, ++j)
				tmp[j] = ch;
			tmp[j] = 0; /* terminate */
			arg->addr = strtol (tmp, NULL, 10);
			--i;
			break;
		cmd:
			for (j = 0; isalpha(ch = buf[i]); ++i, ++j)
				name[j] = ch;
			name[j] = 0; /* terminate */
			if (isdigit(buf[i]))
				goto arg;
			--i;
			break;
		arg:
			for (j = 0; !isblank (ch = buf[i]); ++i, ++j)
				arg->str[j] = buf[i];
			arg->str[j] = 0;
			break;
		str:
			delim = buf[i];
			++i;
			for (j = 0; (ch = buf[i]) && ch != delim; ++i, ++j)
				arg->str[j] = ch;
			arg->str[j] = 0;
			chomp (arg->str, j); /* only used here because only str
					      * will read to the end of the buf
					      */
			--i;
		}
	}
	return;
}

