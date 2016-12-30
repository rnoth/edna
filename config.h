/* config.h - configuration for edna(1) */

/* settings */
#define PROMPT ":"
#define ERROR "?\n"

/* advanced examples */
//#define PROMPT "%ld:", st->lineno	/* have line numbers in prompt */
//#define ERROR "%s\n", error		/* like ed's 'H', without the '?' */

static const Command commands[] = {
       /* name,	func,	mode */
	{ "",	nop,	NULL },
	{ "",	gotol,	NULL },
//	{ "",	print,	NULL },	/* ed-like behavior */
	{ "a",	insert,	"append" },
	{ "c",	insert,	"change" },
	{ "d",	delete,	NULL },
	{ "h",	help,	NULL },
	{ "i",	insert,	NULL },
	{ "p",	print,	NULL },
	{ "q",	quit,	NULL },
	{ "w",	write,	NULL },
	{ "wq", quit,	"write" },
	{ "Q",  quit,	"force" },
};
