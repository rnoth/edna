#include <stdio.h>
#include <stdlib.h>

extern void	warn	(char *);
inline void	die	(char *);
extern void	chomp	(char * buf, size_t biufsiz);

void
chomp(char *buf, size_t bufsiz)
{
	char *p = buf;
	for (size_t i = 0; buf[i] && i < bufsiz; ++i)
		if (buf[i] == '\n')
			p = buf + i;
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
