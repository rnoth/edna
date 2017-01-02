/* util.c -- various functions */
#include <stdio.h>
#include <stdlib.h>

extern void	warn	(char *);
inline void	die	(char *);
extern void	chomp	(char *, size_t);

void
chomp(char *line, size_t linelen)
{
	char *p = line;
	for (size_t i = 0; line[i] && i < linelen; ++i)
		if (line[i] == '\n')
			p = line + i;
	*p = 0;
	return;
}

void
die (char *s)
{
	perror(s);
	exit(1);
}
	

void
warn (char *s)
{
	perror(s);
	return;
}
