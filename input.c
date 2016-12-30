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
	char ch;
	for (size_t i = 0; (ch = buf[i]) ;++i) {
		char tmp[64];
		int j = 0;
		switch (buf[i]) {
		case ' ':
		case '\t':
		case '\n':
			break;
		case '+':
		case '-':
			arg->rel = 1;
			++i;
			goto num;
			break;
		default:
			if (isdigit(ch)) {
				arg->rel = 0;
				goto num;
			}
			if (isalpha(ch)) {
				goto cmd;
			}
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
			--i; /* push back last read char */
			break;
		cmd:
			for (j = 0; isalnum(ch = buf[i]); ++i, ++j)
				name[j] = ch;
			name[j] = 0; /* terminate */
			--i; /* push back last read char */
			break;
		}
	}
	return;
}

