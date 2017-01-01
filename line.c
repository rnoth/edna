/* line.c -- low-level line manipulation functions */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "edna.h"

extern Line*	changeline	(Line *, char *, size_t);
extern Line*	makeline	();
extern Line*	walk		(Line *, int, char *error);

Line *
changeline (Line *l, char *buf, size_t bufsiz)
{
	if (!l->str)
		if (!(l->str = malloc (sizeof *l->str * bufsiz)))
			die("malloc");
	if (l->len && l->len < bufsiz)
		if (!(l->str = realloc (l->str, sizeof *l->str * bufsiz)))
			die("realloc");
	if (!memcpy (l->str, buf, bufsiz + 1)) /* + 1 for the terminating 0 byte */
		die("memcpy");
	l->len = bufsiz;
	return l;
}

Line*
makeline ()
{
	Line* new;
	if (!(new = calloc (1, sizeof *new)))
		die("calloc");
	return new;
}

Line*
putline (Line *cur, char* buf, size_t bufsiz, int option)
{
	Line *new;

	/* only call makeline() when buffer isn't already empty */
	new = cur->str ? makeline() : cur;
	if (!changeline (new, buf, bufsiz)) {
		free (new);
		return NULL;
	}
	switch (option) {
	case 1:	/* append */
		linklines (new, cur->next);
		if (new != cur)	/* don't make a infinite loop */
			linklines (cur, new);
		break;
	case 0:	/* change */
		linklines (cur->prev, new);
		linklines (new, cur->next);
		if (new != cur) /* no use-after-free, please */
			freelines (cur, cur->next);
		break;
	case -1: /* insert */
		linklines (cur->prev, new);
		if (new != cur)
			linklines (new, cur);
		break;
	}
	return new;
}

Line *
walk (Line *cur, int offset, char *error)
{
	Line *l = cur;
	if (0 < offset) {
		while ((l = l->next))
			if (!--offset)
				return l;
		strcpy (error, "end of file");
		return NULL;
	}
	if (0 > offset) {
		while ((l = l->prev))
			if (!++offset)
				return l;
		strcpy (error, "start of file");
		return NULL;
	}
	return cur;		
}
