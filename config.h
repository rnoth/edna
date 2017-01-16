/* config.h - configuration for edna(1) */
#include "edna.h"
#include "cmd.h"

/* settings */
#define PROMPT		"\r\033[K:"
#define INS_PROMPT	" \b"
#define ERROR		"?\n"
#define FILENAME	"/tmp/edna.hup"

/* advanced examples */
//#define PROMPT "%ld:", st->lineno	/* have line numbers in prompt */
//#define INS_PROMPT "%ld|", buf->lineno	/* have line number in ins mode */
//#define ERROR "%s\n", error		/* like ed's 'H', without the '?' */

extern int cmdprompt (State *, Buffer *, Arg *);
extern int insprompt (State *, Buffer *, Arg *);
extern int cmderror  (State *, Buffer *, Arg *, char *);

static const Mode modes[] = {
       /* name,	       prompter,    parser,     evaluator,   handler, */
	{ "command",   cmdprompt,   parseline,  evalcmd,     cmderror, },
	{ "insert",    insprompt,   insparse,   insline,     inserror, },
};

static const Command commands[] = {
       /* name,		func,		mode */
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
	{ "s",		subst,		NULL },
	{ "w",		write,		NULL },
	{ "wq",		quit,		"write" },
	{ "Q", 		quit,		"force" },
};

/* functions */
int
cmdprompt (State *st, Buffer *buf, Arg *arg)
{
	if (printf (PROMPT) < 0) die ("printf");
	if (fflush (stdout) == EOF) warn ("fflush");
	return SUCC;
}

int
insprompt (State *st, Buffer *buf, Arg *arg)
{
	if (printf (INS_PROMPT) < 0) die ("printf");
	if (fflush (stdout) == EOF) warn ("fflush");
	return SUCC;
}

int
cmderror (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (!strcmp (error, "quit"))
		return FAIL;
	if (printf (ERROR) < 0) die ("printf");
	if (fflush (stdout) == EOF) warn ("fflush");
	return SUCC;
}
