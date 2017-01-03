/* config.h - configuration for edna(1) */
#include "cmd.h"

/* settings */
#define PROMPT ":"
#define ERROR "?\n"
#define FILENAME "/tmp/edna.hup"

/* advanced examples */
//#define PROMPT "%ld:", st->lineno	/* have line numbers in prompt */
//#define ERROR "%s\n", error		/* like ed's 'H', without the '?' */

static Command commands[] = {
      /*  name,		func,		mode */
	{ "",		nop,		NULL },
	{ "",		gotol,		NULL },
	{ "a",		insert,		"append" },
	{ "c",		insert,		"change" },
	{ "d",		delete,		NULL },
	{ "e",		edit,		NULL },
	{ "E",		edit,		"force" },
	{ "f",		filename,	NULL, },
	{ "h",		help,		NULL },
	{ "i",		insert,		NULL },
	{ "next",	switchbuf,	"next" },
	{ "o",		openbuf,	NULL },
	{ "p",		print,		NULL },
	{ "prev",	switchbuf,	"prev" },
	{ "q",		quit,		NULL },
	{ "w",		write,		NULL },
	{ "wq",		quit,		"write" },
	{ "Q", 		quit,		"force" },
};
