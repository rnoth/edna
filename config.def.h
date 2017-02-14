#ifndef _edna_config_
#define _edna_config_
#include "edna.h"
#include "buffer.h"
#include "cmd.h"
#include "state.h"

/* settings */
#define PROMPT		"\r\033[K:"
#define PRINT_FMT	"%s", (*arg->sel.v)->str->v
#define INS_PROMPT	" \b"
#define ERROR		"?\n"

/* advanced examples */
//#define PROMPT	"%ld:", bufgetpos(buf)			/* have line numbers in prompt */
//#define INS_PROMPT	"%ld|", bufgetpos(buf)			/* have line number in ins mode */
//#define PRINT_FMT	"%ld\t%s", bufgetpos(buf),(*arg->sel.v)->str	/* like ed's 'n' */
//#define ERROR		"%s\n", err				/* like ed's 'H', without the '?' */

extern const Mode modes[];
extern const Command commands[];

#endif
