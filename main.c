/* edna -- ed-like text editor */
#include "edna.h"
#include "buffer.h"
#include "mode.h"
#include "str.h"

int
main (int argc, char** argv)
{
	char	 err[80];
	String	*s;
	State	*st;
	Buffer	*buf;

	/* init stuff */
	st   = makestate ();
	buf  = makebuf ();
	s    = makestring (LINESIZE);
	*err = 0;

	initst  (st);
	if (FAIL == parse_argv (st, err, argc, argv)) goto exit;
	/* end init */

	/* main execution */
	if (FAIL == checkoutbuf (buf, st, 0)) goto exit;

	for (;;) {

		if (FAIL == buf->mode->prompt (st, buf, s, err)) goto finally;
		if (FAIL == buf->mode->input  (st, buf, s, err)) goto finally;
		if (FAIL == buf->mode->eval   (st, buf, s, err)) goto finally;
		continue;

	finally:
		if (FAIL == buf->mode->error (st, buf, s, err))
			break;
	}
	/* end main */

exit:
	freebuf		(buf);
	freestate	(st);
	freestring	(s);

	return (0);
}
