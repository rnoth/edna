/* edna -- ed-like text editor */
#include "edna.h"

char *strerror(int);

int
main (int argc, char** argv)
{
	int	 err;
	char	 errmsg[80];
	String	*str;
	State	*st;
	Buffer	*buf;

	/* init stuff */
	st   = makestate();
	buf  = makebuf();
	str  = str_alloc();
	*errmsg = 0;

	if (!(st && buf && str)) {
		err = ENOMEM;
		goto exit;
	}

	err = initst(st);
	if (err) goto exit;

	err = parse_argv(st, argv, errmsg);
	if (err) goto exit;
	/* end init */

	/* main execution */
	err = checkoutbuf(buf, st, 0);
	if (err) goto exit;

	for (;;) {

		/* TODO: error handling */
		if (st->mode->prompt(st, buf, str, errmsg)) goto finally;
		if (st->mode->input(st, buf, str, errmsg)) goto finally;
		if (st->mode->eval(st, buf, str, errmsg)) goto finally;
		continue;

	finally:
		if (st->mode->error(st, buf, str, errmsg)) break;
	}
	/* end main */

exit:
	if (err) fprintf(stderr, "edna: %s\n", strerror(err));
		
	freebuf(buf);
	freestate(st);
	str_free(str);

	return 0;
}
