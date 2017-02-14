#ifndef _edna_config_
#define _edna_config_
#include "edna.h"
#include "cmd.h"
#include "state.h"

/* settings */
#define PROMPT		"\r\033[K:"
#define PRINT_FMT	"%s", (*arg->sel.v)->str->v
#define INS_PROMPT	" \b"
#define ERROR		"?\n"

/* advanced examples */
#if 0
# define PROMPT	"%ld:", st->lineno			/* have line numbers in prompt */
# define INS_PROMPT	"%ld|", buf->lineno			/* have line number in ins mode */
# define PRINT_FMT	"%ld\t%s", buf->lineno,(*arg->sel.v)->str	/* like ed's 'n' */
# define ERROR		"%s\n", err				/* like ed's 'H', without the '?' */
#endif

extern const Mode modes[];
extern const Command commands[];

#endif
