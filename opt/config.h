#ifndef _edna_config_
#define _edna_config_
#include "edna.h"

/* settings */

/* advanced examples */
#define PROMPT		"%ld:", bufgetpos(buf)			/* have line numbers in prompt */
#define INS_PROMPT	"%ld ", bufgetpos(buf) + 1		/* have line number in ins mode */
#define PRINT_FMT	"%ld|%s", bufgetpos(buf),arr(arr(arg->sel)[0]->str)	/* like ed's 'n' */
#define ERROR		"%s\n", err				/* like ed's 'H', without the '?' */

extern const Mode modes[];
extern const Command commands[];

size_t sizeof_modes(void);
size_t sizeof_commands(void);

#endif
