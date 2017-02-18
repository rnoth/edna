#ifndef _edna_config_
#define _edna_config_
#include "edna.h"
#include "buffer.h"
#include "cmd.h"
#include "state.h"

/* settings */

/* advanced examples */
#define PROMPT		"%ld:", bufgetpos(buf)			/* have line numbers in prompt */
#define INS_PROMPT	"%ld ", bufgetpos(buf)			/* have line number in ins mode */
#define PRINT_FMT	"%ld|%s", bufgetpos(buf),(*arg->sel.v)->str->v	/* like ed's 'n' */
#define ERROR		"%s\n", err				/* like ed's 'H', without the '?' */

extern const Mode modes[];
extern const Command commands[];

#endif
