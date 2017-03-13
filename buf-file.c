#include "edna.h"

void
bufclean(Buffer *buf)
{
	assert(buf != NULL);
	buf->dirty = 0;
}

char *
bufgetname(Buffer *buf)
{
	assert(buf != NULL);
	return arr(buf->name);
}

int
bufsetname(Buffer *buf, char *name)
{
	assert(buf != NULL);

	vec_truncate(buf->name, 0);
	return vec_concat(buf->name, name, strlen(name));
}

int
bufopen(Buffer *buf, char *mode)
{
	char *fn;

	assert(buf != NULL);
	assert(buf->name != NULL);

	if (buf->file) fclose(buf->file);
	buf->file = NULL;
	fn = arr(buf->name);
	if (!fn || !*fn) goto finally;
	buf->file = fopen(fn, mode);
	if (!buf->file) perror("fopen");

finally:
	return 0;
}
