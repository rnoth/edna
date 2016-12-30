#include <stdio.h>
#include <stdlib.h>

extern void	warn	();
inline void	die	();
extern void	chomp	(char * buf, size_t biufsiz);

void
chomp(char *buf, size_t bufsiz)
{
	for (size_t i = 0; buf[i] && i < bufsiz; ++i)
		if (buf[i] == '\n')
			buf[i] = 0;	/* also breaks the loop */
}

void
die (char *s)
{
	perror(s);
	exit(1);
}
	

void
warn ()
{
	perror("edna");
	return;
}
