/* edna -- ed-like text editor */
#include "edna.h"

int
main (int argc, char** argv)
{
	String s, err;
	State *st;
	Buffer *buf;
	Arg *arg;

	/* init stuff */
	st  = makestate ();
	buf = makebuf (NULL);
	arg = makearg ();
	s   = makestring (LINESIZE);
	err = makestring (LINESIZE);

	initst  (st);
	parse_argv (st, err, argc, argv);
	/* end init */

	arg->addr = buf->lineno;
	checkoutbuf (buf, st, MIN (0, st->buffers.c - 1));

	/* main execution */
	for (;;) {

		if (!(*buf->mode->prompt) (st, buf, arg))
			goto finally;
		if (!readline (&s, stdin, err.v))
			goto finally;
		if (!(*buf->mode->parse) (s, arg))
			goto finally;
		if (!(*buf->mode->eval) (st, buf, arg, err.v))
			goto finally;
		continue;

	finally:
		if (!(*buf->mode->error) (st, buf, arg, err.v))
			break;
	}
	/* end main */

	freestate	(st);
	freearg		(arg);
	freestring	(s);
	freestring	(err);

	return 0;
}
