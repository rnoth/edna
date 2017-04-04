#include "edna.h"
#include "config.h"

Command const commands[] = {
       /* name,		func,		mode		defaddr */
	{ "",		cmd_print,	NULL,		".+1", },
	{ "a",		cmd_insert,	"append",	".", },
	{ "b",		cmd_listbuf,	NULL,		NULL },
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
	{ "wq",		cmd_quit,	"write",	NULL, },
	{ "Q", 		cmd_quit,	"force",	NULL, },
};

Mode const modes[] = {
       /* name,	       init,     prompter,    reader      evaluator,   handler, */
        { "command",   NULL,     cmdprompt,   grabline,   cmdeval,     cmderror, },
        { "insert",    NULL,     insprompt,   grabline,   insline,     inserror, },
	{ "quit",      quit,     NULL,        NULL,       NULL,        NULL, }
};

#define cmd(name, argv) { name, 'a', false, argv, docmd, }
Symbol const symbols_[] = {
	/* name,      class, comment?, argv,  eval */
	 { "<space>", 'w',    true,     "",   NULL, },
	 /*  name, argv */
	 cmd("q",  "\0\0\0"),
};

size_t
sizeof_commands(void)
{
	return sizeof commands / sizeof *commands;
}

size_t
sizeof_modes(void)
{
	return sizeof modes / sizeof *modes;
}

size_t
sizeof_symbols(void)
{
	return sizeof symbols_ / sizeof *symbols_;
}
