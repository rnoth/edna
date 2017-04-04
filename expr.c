#include "edna.h"

struct token {
	size_t len;
	Symbol *sym;
};

static Expr * expr_tok  (String *, struct token *);
static int    expr_next (Expr **, Type const *, State *, String *);
static void   toknext   (struct token *, Type const *, State *, String *);

Type const init_type[] = { EXPR, CONT, NIL };

Expr *
expr_alloc(void)
{
	Expr *ret;

	ret = calloc(1, sizeof *ret);

	make_vector(ret->chld);
	if (!ret->chld) goto nomem;

	ret->str = edna_str_alloc();
	if (!ret->str) goto nomem;

	return ret;

nomem:
	edna_str_free(ret->str);
	vec_free(ret->chld);
	free(ret);
	
	return NULL;
}

Expr *
expr_ctor(void)
{
	Expr *ret;

	ret = expr_alloc();
	if (!ret) return NULL;

	ret->typev = init_type;
	ret->eval = cmd_eval;

	return ret;
}

void
expr_free(Expr *ex)
{
	Expr **chld;

	edna_str_free(ex->str);
	mapv (chld, ex->chld) expr_free(*chld);
	vec_free(ex->chld);
	free(ex);
}

Expr *
expr_tok(String *s, struct token *tok)
{
	Expr *ret;

	ret = expr_alloc();

	vec_concat(ret->str, arr(s), tok->len);

	ret->type = tok->sym->type;
	ret->typev = (void*)tok->sym->argv;
	ret->eval = tok->sym->eval;

	return ret;
}

int
expr_next(Expr **out, Type const *typev, State *st, String *str)
{
	struct token tok = {0,0};

	*out = NULL;

	toknext(&tok, typev, st, str);
	
	if (!tok.sym) return EILSEQ;

	if (tok.sym->comment) goto exit;

	*out = expr_tok(str, &tok);
	if (!*out) return ENOMEM;

exit:
	vec_shift(str, tok.len);

	return 0;
}

void
toknext(struct token *tok, Type const *typev, State *st, String *str)
{
	size_t max = 0;
	size_t tmp = 0;
	Symbol *res = 0;
	Symbol *sym = 0;

	mapv (sym, st->syms) {
		if (typecmp(typev, type(sym)))
			continue;

		tmp = lex(sym, str);

		if (tmp > max) {
			max = tmp;
			res = sym;
		}
	}

	tok->sym = res;
	tok->len = max;
}

int
parseline(Expr *cur, State *st, String *str)
{
	int err = 0;
	Expr *next = 0;
	String s = {0};
	Vector(Expr *) *ex = 0;

	make_vector(ex);
	if (!ex) return ENOMEM;

	memcpy(&s, str, sizeof s);

	while (len(&s)) {
		err = expr_next(&next, cur->typev, st, &s);
		if (err) goto finally;
		if (!next) continue;

		cur->typev += typenext(cur->typev);

		err = vec_append(cur->chld, &next);
		if (err) goto finally;

		if (!isnull(next->typev[0])) {
			err = vec_append(ex, &cur);
			if (err) goto finally;

			cur = next;
		}

		if (isnull(cur->typev[0])) {
			if (len(ex)) {
				cur = arr(ex)[len(ex)-1];
				vec_delete(ex, len(ex)-1);
			} else break;
		}
	}

finally:
	vec_free(ex);
	return err;
}

int
evalexpr(String *out, Expr *ex, State *st, Buffer *buf, Arg *arg, char *errmsg)
{
	int err;
	Expr **sub;
	String *tmp;

	mapv (sub, ex->chld) {
		tmp = edna_str_alloc();
		if (!tmp) {
			err = ENOMEM;
			goto finally;
		}

		err = evalexpr(tmp, *sub, st, buf, arg, errmsg);
		if (err) goto finally;

		vec_append(arg->param, tmp);

		expr_free(*sub);
	}
	arg->name = arr(ex->str);
	err = ex->eval(st, buf, arg, errmsg);
finally:
	return err;
}
