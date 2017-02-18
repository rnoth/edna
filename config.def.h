#ifndef _edna_config_
#define _edna_config_
#include "edna.h"

/* settings */
#define PROMPT		"\r\033[K:"
#define PRINT_FMT	"%s", (*arg->sel.v)->str->v
#define INS_PROMPT	" \b"
#define ERROR		"?\n"

extern const Mode modes[];
extern const Command commands[];

#endif
