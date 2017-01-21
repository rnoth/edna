/* edna -- ed-like text editor */
#include "edna.h"

int
main (int argc, char** argv)
{
	char err[80];
	String *s;
	State *st;
	Buffer *buf;

	/* init stuff */
	st   = makestate ();
	buf  = makebuf (NULL);
	s    = makestring (LINESIZE);

	initst  (st);
	parse_argv (st, err, argc, argv);
	/* end init */

	checkoutbuf (buf, st, 0);
	*err = 0;

	/* main execution */
	for (;;) {

		if (!(*buf->mode->prompt) (st, buf))
			goto finally;
		if (!readline (s, stdin, err))
			goto finally;
		if (!(*buf->mode->eval) (st, buf, s, err))
			goto finally;
		continue;

	finally:
		if (!(*buf->mode->error) (st, buf, err))
			break;
	}
	/* end main */

	freestate	(st);
	freestring	(s);

	return 0;
}
