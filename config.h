/* config.h - configuration for edna(1) */
#include "edna.h"
#include "cmd.h"

/* settings */
#define PROMPT		"\r\033[K:"
#define PRINT_FMT	"%s", (*arg->sel.v)->str
#define INS_PROMPT	" \b"
#define ERROR		"?\n"
#define FILENAME	"/tmp/edna.hup"

/* advanced examples */
#if 0
#  define PROMPT	"%ld:", st->lineno			/* have line numbers in prompt */
#  define INS_PROMPT	"%ld|", buf->lineno			/* have line number in ins mode */
#  define PRINT_FMT	"%ld\t%s", buf->lineno,(*arg->sel.v)->str	/* like ed's 'n' */
#  define ERROR		"%s\n", error				/* like ed's 'H', without the '?' */
#endif

extern int cmdprompt (State *, Buffer *);
extern int cmderror  (State *, Buffer *, char *);
extern int insprompt (State *, Buffer *);
extern int print     (State *, Buffer *, Arg *, char *);

static const Mode modes[] = {
       /* name,	       prompter,    reader     evaluator,   handler, */
	{ "command",   cmdprompt,   grabline,   evalcmd,     cmderror, },
	{ "insert",    insprompt,   grabline,   insline,     inserror, },
};

static const Command commands[] = {
       /* name,		func,		mode		defaddr */
	{ "",		gotol,		NULL,		".", },
	{ "a",		insert,		"append",	".", },
	{ "c",		insert,		"change",	".", },
	{ "d",		delete,		NULL,		".", },
	{ "e",		edit,		NULL,		".", },
	{ "E",		edit,		"force",	".", },
	{ "f",		filename,	NULL,		".", },
	{ "h",		help,		NULL,		".", },
	{ "i",		insert,		NULL,		".", },
	{ "next",	switchbuf,	"next",		".", },
	{ "o",		openbuf,	NULL,		".", },
	{ "p",		print,		NULL,		".", },
	{ "prev",	switchbuf,	"prev",		".", },
	{ "q",		quit,		NULL,		".", },
	{ "s",		subst,		NULL,		".", },
	{ "w",		cmd_write,	NULL,		".", },
	{ "wq",		quit,		"write",	".", },
	{ "Q", 		quit,		"force",	".", },
};

/* functions */
int
cmdprompt (State *st, Buffer *buf)
{
	if (printf (PROMPT) < 0) die ("printf");
	if (fflush (stdout) == EOF) warn ("fflush");
	return SUCC;
}

int
cmderror (State *st, Buffer *buf, char *error)
{
	if (!strcmp (error, "quit"))
		return FAIL;
	if (printf (ERROR) < 0) die ("printf");
	if (fflush (stdout) == EOF) warn ("fflush");
	return SUCC;
}
int
insprompt (State *st, Buffer *buf)
{
	if (printf (INS_PROMPT) < 0) die ("printf");
	if (fflush (stdout) == EOF) warn ("fflush");
	return SUCC;
}

int
print (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (!buf->curline->str) {
		strcpy (error, "empty buffer");
		return FAIL;
	}

	if (printf (PRINT_FMT) < 0) die ("printf");
	return SUCC;
}
