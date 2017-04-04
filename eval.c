#include "edna.h"

int
docmd(State *st, Buffer *buf, Arg *arg, char *errmsg)
{
	Command *cmd = 0;

	cmd = findcmd(st, arg->name);
	if (!cmd) return ENOENT;

	arg->mode = cmd->mode;

	return cmd->func(st, buf, arg, errmsg);
}
