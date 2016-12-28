/* edna.c -- ed-like text editor */
/* TODO:
	- bumplines() hasn't been testing with lines > 1
*/
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
	//char dirty;
	int lineno;
	size_t len;
	char* str;
	Line* next;
	Line* prev;
};

static Line*	bumplines	(Line *, int, Line **);
static Line*	freelines	(Line *, Line *);
static Line*	insertline	(char *, size_t, Line *);
static Line*	makeline	();
static void	printline	(Line *);
static void	warn		();

Line*
bumplines (Line *start, int offset, Line **lines)
{
	Line *line, *end;
	line = *lines;

	line->prev = start;
	end = (start ? start->next : NULL);
	if (start)
		start->next = line;
	
	line->next = (offset > 1 ? line + 1 : NULL);
	line->lineno = (start ? start->lineno + 1 : 1);
	for (int i = 1; i < offset; ++i, ++line) {
		line->prev = line - 1;
		if (i < offset - 1)
			line->next = line + 1;
		line->lineno = (line - 1)->lineno + 1;
	}
	if (offset > 1) {
		line->prev = line - 1;
		line->lineno = (line - 1)->lineno + 1;
	}
	if (end)
		end->lineno += offset;
	line->next = end;
	return end ? end : line;
		
}

Line *
freelines(Line *start, Line *stop)
{
	Line *cur, *next, *tmp;
	if (start->prev) /* link around the region to be freed */
		(start->prev)->next = stop;
	cur = start;
	next = cur->next;
	do {
		PRINTF("freeing line: \n\t")
		printline(cur);
		tmp = (next ? next->next : NULL);
		free (cur->str);
		free (cur);
		cur = next;
		next = tmp;
	} while (cur && cur != stop);
	return stop;
}

Line*
insertline(char* buf, size_t bufsiz, Line *position)
{
	Line *new;
	new = makeline();
	MALLOC(new->str, sizeof new->str * LINESIZE);
	MEMCPY(new->str, buf, bufsiz + 1); /* + 1 for the terminating 0 byte */
	new->len = bufsiz;
	return bumplines(position, 1, &new);
}

Line*
linklines(Line *left, Line *right)
{
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

void
warn ()
{
	perror("edna");
	return;
}

int
main (/*int argc, char** argv*/)
{
	char ch, *buf;
	Line *curline, *topline, *bottomline;
	size_t bufsiz;
	MALLOC (buf, 80 * sizeof *buf);
	bufsiz = 80 * sizeof *buf;

	curline = topline = bottomline = NULL;

	for(;;){
		int i;
		PRINTF (": ");
		GETLINE (buf, bufsiz, stdin);
		for(i = 0; ch = buf[i], ch == ' ' || ch == '\t';  ++i)
			; /* nop */
		switch(buf[i]) {
		case 'q':
			PRINTF ("quitting\n");
			freelines(topline, NULL);
			free(buf);
			exit (0);
			break; // not reached
		case 'i':
			GETLINE (buf, bufsiz, stdin);
			curline = insertline (buf, bufsiz, curline);
			if (!curline->prev)
				topline = curline;
			if (!curline->next)
				bottomline = curline;
			break;
		case 'p':
			printline (curline);
			break;
		default:
			printf("?\n");
			break;
		}
	}

	return 1; /* not reached */
}
