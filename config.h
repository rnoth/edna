/* config.h - configuration for edna(1) */

//#define LINESIZE 80
size_t LINESIZE = 80; /* variable instead of macro to allow it to be extern'd properly */
#define BUFFSIZE 20 /* currently unused */

const struct Command commands[] = {
	{ "",	nop },
	{ "a",	append },
	{ "d",	delete },
	{ "h",	help },
	{ "p",	print },
	{ "q",	quit },
};
