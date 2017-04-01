#ifndef _edna_expr_
#define _edna_expr_

struct Expr {
	String *str;
	Vector(Expr) chld;
};

char *expr_type(Expr *);
#endif
