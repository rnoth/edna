#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "edna.h"

int
grabline(State *st, Buffer *buf, String *s, char *errmsg)
{
	int err = 0;

	err = readline(s, stdin);
	if (err == -1) {
		strcpy(errmsg, "eof");
		printf("^D\n");
		fflush(stdout);

	} else if (err > 0)  {
		err = errno;
		strcpy(errmsg, strerror(errno));
		clearerr(stdin);
	}

	return err;

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
