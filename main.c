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
	st   = makestate();
	buf  = makebuf();
	s    = makestring(LINESIZE);
	*err = 0;

	initst(st);
	if (parse_argv (st, err, argc, argv) == FAIL) goto exit;
	/* end init */

	/* main execution */
	if (checkoutbuf (buf, st, 0) == FAIL) goto exit;

	for (;;) {

		if (buf->mode->prompt(st, buf, s, err) == FAIL) goto finally;
		if (buf->mode->input (st, buf, s, err) == FAIL) goto finally;
		if (buf->mode->eval  (st, buf, s, err) == FAIL) goto finally;
		continue;

	finally:
		if (buf->mode->error(st, buf, s, err) == FAIL) break;
	}
	/* end main */

exit:
	freebuf(buf);
	freestate(st);
	freestring(s);

	return (0);
}
