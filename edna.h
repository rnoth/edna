#ifndef _edna_
#define _edna_
#include <assert.h>
#include <errno.h>
#include <stdbool.h>

#include "set.h"
#include "str.h"
#include "vec.h"
#include "util.h"

typedef struct Arg	Arg;
typedef struct Buffer	Buffer;
typedef struct Command	Command;
typedef struct Mode	Mode;
typedef struct Node	Node;
typedef struct Line	Line;
typedef struct State	State;
typedef Vector(Line *)	Selection;

#include "addr.h"
#include "buf.h"
#include "cmd.h"
#include "line.h"
#include "state.h"

int readline(String *, FILE *);

#endif
