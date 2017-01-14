/* util.c -- various functions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str.h"

extern void	chomp	(String);

void
chomp (String s)
{
	size_t i;
	char *p = s.v;
	for (i = 0; s.v[i] && i < s.c; ++i)
		if (s.v[i] == '\n')
			p = s.v + i;
	*p = 0;
	return;
}
