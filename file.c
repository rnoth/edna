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

extern int readbuf	(Buffer *, char *);
extern int writebuf	(Buffer *, char *);

int
readbuf(Buffer *buf, char *err)
{
	int ret = FAIL;
	String *s;
	Line *new;

	errno = 0;
	if (bufopen(buf, "r") == FAIL) {
		if (errno)
			strncpy(err, strerror(errno), 80);
		else
			strcpy(err, "invalid filename");
		return FAIL;
	}

	s = makestring();

	errno = 0;
	while (feof(buf->file) == 0) {
		if (readline(s, buf->file) == FAIL) {
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
	clearerr (buf->file);

finally:
	freestring (s);
	buf->dirty = 0;
	return (ret);
}

int
writebuf (Buffer *buf, char *error)
{
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
		
	/* FIXME */
	for (tmp = getnext(buf->top); tmp; tmp = getnext(tmp))
		fputs (tmp->str->v, buf->file);

	return SUCC;
}
