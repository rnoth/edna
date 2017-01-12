#include <stdlib.h>

#include "edna.h"
#include "addr.h"

Set
num (char *s, Buffer *buf, char *error)
{
	return setaddmemb (makeset (buf), buf->len, strtol (s, NULL, 10));
}

Set
symnum (char *s, Buffer *buf, char *error)
{
	return makeset (buf);
}

Set
neg (char *s, Set A, Buffer *buf, char *error)
{
	return makeset (buf);
}

Set
add (Set A, Set B, Buffer *buf, char *error)
{
	return makeset (buf);
}

Set
sub (Set A, Set B, Buffer *buf, char *error)
{
	return makeset (buf);
}

Set
range (Set A, Set B, Buffer *buf, char *error)
{
	return makeset (buf);
}

Set
and (Set A, Set B, Buffer *buf, char *error)
{
	return makeset (buf);
}

Set
or (Set A, Set B, Buffer *buf, char *error)
{
	return makeset (buf);
}

Set
xor (Set A, Set B, Buffer *buf, char *error)
{
	return makeset (buf);
}
