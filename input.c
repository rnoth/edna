#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "edna.h"

int
grabline(State *st, Buffer *buf, String *s, char *err)
{
	int ret = 0;

	errno = 0;
	if (readline(s, stdin)) {
		if (feof(stdin)) strcpy(err, "quit");
		else strcpy(err, strerror(errno));

		ret = errno;
		clearerr(stdin);
	}

	return ret;

}

int
readline(String *s, FILE *f)
{
	int ch;

	vec_truncate(s, 0);
	for (;;) {
		ch = fgetc(f);
		if (ch == EOF) {
			if (feof(f)) return -1;
			else return errno;
		}
		vec_append(s, &ch);
		if (ch == '\n') return 0;
	}
}
