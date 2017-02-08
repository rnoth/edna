/* config.h - configuration for edna(1) */
#include "edna.h"
#include "cmd.h"
#include "mode.h"

/* settings */
#define PROMPT		"\r\033[K:"
#define PRINT_FMT	"%s", (*arg->sel.v)->str->v
#define INS_PROMPT	" \b"
#define ERROR		"?\n"
#define FILENAME	"/tmp/edna.hup"

/* advanced examples */
#if 0
#  define PROMPT	"%ld:", st->lineno			/* have line numbers in prompt */
#  define INS_PROMPT	"%ld|", buf->lineno			/* have line number in ins mode */
#  define PRINT_FMT	"%ld\t%s", buf->lineno,(*arg->sel.v)->str	/* like ed's 'n' */
#  define ERROR		"%s\n", err				/* like ed's 'H', without the '?' */
#endif

extern int cmdprompt (State *, Buffer *, String *, char *);
extern int cmderror  (State *, Buffer *, String *, char *);
extern int insprompt (State *, Buffer *, String *, char *);
extern int print     (State *, Buffer *, Arg *, char *);

static const Mode modes[] = {
       /* name,	       init,   prompter,    reader      evaluator,   handler, */
        { "command",   NULL,   cmdprompt,   grabline,   cmdeval,     cmderror, },
        { "insert",    NULL,   insprompt,   grabline,   insline,     inserror, },
};

static const Command commands[] = {
       /* name,		func,		mode		defaddr */
	{ "",		cmd_print,	NULL,		".+1", },
	{ "a",		cmd_insert,	"append",	".", },
	{ "c",		cmd_insert,	"change",	".", },
	{ "d",		cmd_delete,	NULL,		".", },
	{ "e",		cmd_edit,	NULL,		NULL, },
	{ "E",		cmd_edit,	"force",	NULL, },
	{ "f",		cmd_filename,	NULL,		NULL, },
	{ "h",		cmd_help,	NULL,		NULL, },
	{ "i",		cmd_insert,	"insert",	".", },
	{ "next",	cmd_switchbuf,	"next",		NULL, },
	{ "o",		cmd_openbuf,	NULL,		NULL, },
	{ "p",		cmd_print,	NULL,		".", },
	{ "prev",	cmd_switchbuf,	"prev",		NULL, },
	{ "q",		cmd_quit,	NULL,		NULL, },
//	{ "s",		subst,		NULL,		".", },
	{ "w",		cmd_write,	NULL,		NULL, },
	{ "wq",		cmd_quit,	"write",	".", },
	{ "Q", 		cmd_quit,	"force",	".", },
};

/* functions */
int
cmdprompt (State *st, Buffer *buf, String *s, char *err)
{
	if (printf (PROMPT) < 0) die ("printf");
	if (fflush (stdout) == EOF) warn ("fflush");
	return SUCC;
}

int
cmderror (State *st, Buffer *buf, String *s, char *err)
{
	if (!strcmp (err, "quit"))
		return FAIL;
	if (printf (ERROR) < 0) die ("printf");
	if (fflush (stdout) == EOF) warn ("fflush");
	return SUCC;
}
int
insprompt (State *st, Buffer *buf, String *s, char *err)
{
	if (printf (INS_PROMPT) < 0) die ("printf");
	if (fflush (stdout) == EOF) warn ("fflush");
	return SUCC;
}

int
cmd_print (State *st, Buffer *buf, Arg *arg, char *error)
{
	if (buf->len == 0) {
		strcpy (error, "empty buffer");
		return (FAIL);
	}

	if (printf (PRINT_FMT) < 0) die ("printf");
	buf->cur = *arg->sel.v;
	buf->pos = getlineno (buf->cur);
	return (SUCC);
}
