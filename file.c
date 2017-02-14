/* file.c -- file manipulation functions */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edna.h"
#include "buffer.h"
#include "line.h"
#include "str.h"
#include "util.h"

extern int readbuf	(Buffer, char *);
extern int writebuf	(Buffer, char *);

/* FIXME: could these functions be redesigned to not depend on buffer's internals? */

int
readbuf(Buffer buf, char *err)
{
	int ret = FAIL;
	String *s;
	Line *new;
	FILE *f;

	errno = 0;
	if (bufopen(buf, "r") == FAIL) {
		if (errno)
			strncpy(err, strerror(errno), 80);
		else
			strcpy(err, "invalid filename");
		return FAIL;
	}

	s = makestring();
	f = getfile(buf);

	errno = 0;
	while (feof(f) == 0) {
		if (readline(s, f) == FAIL) {
			if (errno) {
				strncpy (err, strerror (errno), 20);
				goto finally;
			} else
				continue;
		}

		new = makeline ();
		if (changeline (new, s) == FAIL) {
			strcpy (err, "readbuf(): changeline() failed. memory errors?");
			freelines (new, new->next);
			goto finally;
		} else if (addline (buf, new) == FAIL) {
			strcpy (err, "readbuf(): changeline() failed. buffer inconsistency?");
			freelines (new, new->next);
			goto finally;
		}
	}
	clearerr (f);

finally:
	freestring (s);
	return (ret);
}

int
writebuf (Buffer buf, char *error)
{
	FILE *f;
	Line *tmp;

	errno = 0;
	if (bufopen(buf, "w+") == FAIL) {
		if (errno) {
			strcpy (error, "fopen: ");
			strcpy (error + strlen (error), strerror (errno));
		} else {
			strcpy (error, "invalid filename");
		}
		return FAIL;
	}
		
	f = getfile(buf);
	for (tmp = bufprobe(buf, 1); tmp; tmp = getnext(tmp))
		fputs (tmp->str->v, f);

	return SUCC;
}
