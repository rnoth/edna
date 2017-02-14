#ifndef _edna_
#define _edna_

#define TYPE(t) typedef struct t { void *v; } t

TYPE(Buffer);

typedef struct Arg	Arg;
typedef struct Command	Command;
typedef struct Mode	Mode;
typedef struct Node	Node;
typedef struct Line	Line;
typedef struct State	State;
typedef struct Selection Selection;

#endif
