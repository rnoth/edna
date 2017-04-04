#ifndef _edna_
#define _edna_
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>

#include "set.h"
#include "str.h"
#include "vec.h"
#include "util.h"

typedef struct Arg	Arg;
typedef struct Buffer	Buffer;
typedef struct Command	Command;
typedef struct Expr	Expr;
typedef struct Mode	Mode;
typedef struct Line	Line;
typedef struct State	State;
typedef struct Symbol	Symbol;
typedef Vector(Line *)	Selection;

#include "buf.h"
#include "cmd.h"
#include "line.h"
#include "parse.h"
#include "state.h"

/* input.c */
int readline(String *, FILE *);
int grabline(State *, Buffer *, String *, char *);

#endif
