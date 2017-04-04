#ifndef _edna_symbol_
#define _edna_symbol_

#define type(p) ((p)->type)
#define isnull(t) (!(t))

enum Type {
	NIL = '\0',
	ALPHA = 'a',
	DELIM = 'd',
	IDENT = 'i',
	NUMBR = 'n',
	SPACE = 'w',
	EXPR  = 'x',
	IGNOR = ',',

	CONT = '*',
};

typedef enum Type	Type;

typedef int (*Eval)(State *, Buffer *, Arg *, char *);

struct Expr {
	String		*str;
	Type		 type;
	Type const	*typev;
	Eval		 eval;
	Vector(Expr *)	*chld;
};

struct Symbol {
	char   *name;
	Type    type;
	bool    comment;
	char   *argv;
	Eval    eval;
};

/* eval.c */
int docmd(State *, Buffer *, Arg *, char *);

/* expr.c */
Expr *expr_ctor(void);
int parseline(Expr *, State *, String *);
int evalexpr(String *, Expr *, State *, Buffer *, Arg *, char *);

/* lex.c */
size_t lex(Symbol *, String *);
size_t lex_alpha(Symbol *, String *);
size_t lex_ident(Symbol *, String *);
size_t lex_ignor(Symbol *, String *);
size_t lex_delim(Symbol *, String *);
size_t lex_numbr(Symbol *, String *);
size_t lex_space(Symbol *, String *);

/* type.c */
int typecmp(Type const [static 1], Type const);
size_t typenext(Type const [static 1]);

#endif
