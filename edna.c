/* edna.c -- ed-like text editor */
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MALLOC(X, Y) if (!(X = malloc (Y)))\
			warn();
#define PRINTF(...) if (!printf (__VA_ARGS__))\
			warn();
#define MEMCPY(X, Y, Z) if (!memcpy(X, Y, Z))\
			warn();
#define GETLINE(X, Y, Z) if (!(Y = getline(&X, &Y, Z)))\
			warn();

#define LINESIZE 80
#define BUFFSIZE 20

typedef struct Line Line;
struct Line {
	int lineno;
	size_t len;
	char* str;
	Line* next;
	Line* prev;
};

static Line	*freelines	(Line *, Line *);
static Line	*insertline	(char *, size_t, Line *);
static Line	*makeline	();
static void	linklines	(Line *, Line*);
static Line	*pushbuffer	(Line *, size_t);
static void	printline	(Line *);
static void	warn		();

Line *
freelines(Line *start, Line *stop)
{
	Line *cur, *next, *tmp;
	if (start->prev) /* link around the region to be freed */
		(start->prev)->next = stop;
	cur = start;
	next = cur->next;
	do {
		tmp = (next ? next->next : NULL);
		free (cur->str);
		free (cur);
		cur = next;
		next = tmp;
	} while (cur && cur != stop);
	return stop;
}

Line*
insertline (char* buf, size_t bufsiz, Line *position)
{
	Line *new;
	new = makeline ();
	MALLOC (new->str, sizeof new->str * LINESIZE);
	MEMCPY (new->str, buf, bufsiz + 1); /* + 1 for the terminating 0 byte */
	new->len = bufsiz;
	new->lineno = position ? position->lineno + 1 : 1;
	if (position) {
		linklines (new, position->next);
		linklines (position, new);
		pushbuffer (position->next, 1);
	}
	return new;
}

void
linklines(Line *left, Line *right)
{
	if (left)
		left->next = right;
	if (right)
		right->prev = left;

	return;
}

Line*
makeline ()
{
	Line* new;
	MALLOC (new, sizeof *new);
	//MALLOC (new->str, LINESIZE * sizeof *new->str);
	//MEMCPY (new->str, "\n\0", 2);
	new->str    = NULL;
	new->len    = 0;
	new->lineno = 0;
	new->prev   = NULL;
	new->next   = NULL;
	return new;
}

void
printline (Line *line)
{
	printf ("%s", line->str);
	return;
}

Line *
pushbuffer(Line *start, size_t offset)
{
	Line *l;
	l = start;
	for (; l && (l = l->next);)
		l->lineno += offset;
	return l;
}

void
warn ()
{
	perror("edna");
	return;
}

int
main (/*int argc, char** argv*/)
{
	char *buf;
	Line *curline, *topline, *bottomline;
	size_t bufsiz;
	MALLOC (buf, 80 * sizeof *buf);
	bufsiz = 80 * sizeof *buf;

	curline = topline = bottomline = NULL;

	for(;;){
		int i;
		char ch;
		PRINTF (": ");
		GETLINE (buf, bufsiz, stdin);
		for(i = 0; ch = buf[i], ch == ' ' || ch == '\t';  ++i)
			; /* nop */
		switch(buf[i]) {
		Line *tmp;
		case 'd':
			tmp = curline->next ? curline->next : curline->prev;
			linklines(curline->prev, curline->next);
			freelines(curline, curline->next);
			curline = tmp;
			if (tmp)
				goto update;
			break;
		case 'i':
			GETLINE (buf, bufsiz, stdin);
			curline = insertline (buf, bufsiz, curline);
			goto update;
			break;
		case 'p':
			printline (curline);
			break;
		case 'q':
			PRINTF ("quitting\n");
			freelines(topline, NULL);
			free(buf);
			exit (0);
			break; /* not reached */
		default:
			printf("?\n");
			break;
		update:
			if (!curline->prev)
				topline = curline;
			if (!curline->next)
				bottomline = curline;
			break;
		}
	}

	return 1; /* not reached */
}
