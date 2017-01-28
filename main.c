/* edna -- ed-like text editor */
#include "edna.h"

int
main (int argc, char** argv)
{
	char err[80];
	void *var;
	String *s;
	State *st;
	Buffer *buf;

	/* init stuff */
	st   = makestate ();
	buf  = makebuf (NULL);
	s    = makestring (LINESIZE);
	*err = 0;

	initst  (st);
	if (FAIL == parse_argv (st, err, argc, argv)) goto exit;
	/* end init */

	/* main execution */
	if (FAIL == checkoutbuf (buf, st, 0)) goto exit;
	for (;;) {

		if (FAIL == buf->mode->prompt (st, buf))	  goto finally;
		if (FAIL == buf->mode->input (s, err))		  goto finally;
		if (FAIL == buf->mode->parse (s, &var, buf, err)) goto finally;
		if (FAIL == buf->mode->eval (st, buf, var, err))  goto finally;
		continue;

	finally:
		if (FAIL == buf->mode->error (st, buf, err))
			break;
	}
	/* end main */

exit:
	freestate	(st);
	freestring	(s);

	return (0);
}
