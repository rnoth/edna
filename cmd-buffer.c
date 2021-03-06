/* cmd_buffer.c -- buffer manipulation commands */
#include <string.h>
#include "edna.h"

static void listbuf_helper(Buffer *, char);

void
listbuf_helper(Buffer *buf, char prefix)
{
	bool dirty;
	char *fn, *mod="";
	size_t len, pos;

	fn = bufgetname(buf);
	len = bufgetlen(buf);
	pos = bufgetpos(buf);
	dirty = isdirty(buf);

	if (!fn) fn = "";
	if (dirty) mod = " [modified]";
	if (printf("%c \"%s\"%s, %ld / %ld\n", prefix, fn, mod, pos, len - 1) < 0)
		die("printf");
}


int
cmd_listbuf(State *st, Buffer *buf, Arg *arg, char *error)
{
	size_t i;

	for (i = 0; i < st->buffers->c; ++i)
		listbuf_helper(st->buffers->v[i], ' ');

	listbuf_helper(buf, '*');

	return 0;
}

int
cmd_openbuf(State *st, Buffer *buf, Arg *arg, char *error)
{
	Buffer *tmp;
	size_t i;

	if (!arg->param || !arg->param->c) {
		strcpy(error, "no filenames provided");
		return -1;
	}

	i = 0;
	do {
		tmp = makebuf();
		if (!tmp) return ENOMEM;
		chomp(arg->param->v[i]);
		initbuf(tmp, arg->param->v[i]);
		if (readbuf(tmp, error)) {
			strcpy(error, "error reading file");
			return -1;
		}
		bufclean(tmp);
		addbuf(st, tmp);
	} while (++i < len(arg->param));

	return 0;
}

int
cmd_switchbuf(State *st, Buffer *buf, Arg *arg, char *error)
{
	if (!arg->mode) {
		strcpy(error, "no option specified");
		return -1;

	} else if (!strcmp(arg->mode, "next")) {
		returnbuf(st, buf);
		checkoutbuf(buf, st, 0);

		return 0;

	} else if (!strcmp(arg->mode, "prev")) {
		returnbuf(st, buf);
		checkoutbuf(buf, st, (st->buffers->c != 1) ? st->buffers->c - 2 : 0 );

		return 0;
	}

	strcpy (error, "unknown option");
	return -1;
}
