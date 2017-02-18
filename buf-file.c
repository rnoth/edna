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
	return strtochar(buf->name);
}

int
bufsetname(Buffer *buf, char *name)
{
	assert(buf != NULL);

	if (!copychars(buf->name, name)) return FAIL;
	chompstr(buf->name);

	return SUCC;
}

int
bufopen(Buffer *buf, char *mode)
{
	int ret;
	char *fn;

	assert(buf != NULL);
	assert(buf->name != NULL);

	ret = FAIL;

	if (buf->file) fclose(buf->file);
	buf->file = NULL;

	fn = strtochar(buf->name);

	if (!fn || !*fn) goto finally;

	if (buf->file)
		if (fclose(buf->file) == EOF) die("fclose");

	buf->file = fopen(fn, mode);

	if (buf->file == NULL) perror("fopen");
	else ret = SUCC;

finally:
	free(fn);

	return ret;
}
