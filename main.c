/* edna -- ed-like text editor */
#include "edna.h"

int
main (int argc, char** argv)
{
	char	 err[80];
	String	*s;
	State	*st;
	Buffer	*buf;

	/* init stuff */
	st   = makestate();
	buf  = makebuf();
	s    = makestring();
	*err = 0;

	initst(st);
	if (parse_argv (st, argv, err) == FAIL) goto exit;
	/* end init */

	/* main execution */
	if (checkoutbuf (buf, st, 0) == FAIL) goto exit;

	for (;;) {

		if (st->mode->prompt(st, buf, s, err)) goto finally;
		if (st->mode->input (st, buf, s, err)) goto finally;
		if (st->mode->eval  (st, buf, s, err)) goto finally;
		continue;

	finally:
		if (st->mode->error(st, buf, s, err)) break;
	}
	/* end main */

exit:
	freebuf(buf);
	freestate(st);
	freestring(s);

	return (0);
}
