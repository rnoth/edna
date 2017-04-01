#include <stdio.h>
#include <string.h>
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

	if (!buf->name) return NULL;
	return arr(buf->name);
}

int
bufsetname(Buffer *buf, char *name)
{
	assert(buf != NULL);

	if (!buf->name) {
		buf->name = str_alloc();
		if (!buf->name) return ENOMEM;
	}
	vec_truncate(buf->name, 0);
	return vec_concat(buf->name, name, strlen(name));
}

int
bufopen(Buffer *buf, char *mode)
{
	char *fn;

	assert(buf != NULL);
	if (buf->file) {
		if (fflush(buf->file) == -1) return errno;
		if (fclose(buf->file) == -1) return errno;
	}
	if (buf->name == NULL) return 0;

	if (buf->file) fclose(buf->file);
	buf->file = NULL;
	fn = arr(buf->name);
	if (!fn || !*fn) goto finally;
	buf->file = fopen(fn, mode);
	if (!buf->file) perror("fopen");

finally:
	return 0;
}
