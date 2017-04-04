#include "edna.h"

size_t
lex(Symbol *sym, String *str)
{
	switch (sym->type) {
	case ALPHA: return lex_alpha(sym, str);
	case IDENT: return lex_ident(sym, str);
	case IGNOR: return lex_ignor(sym, str);
	case SPACE: return lex_space(sym, str);
	case DELIM: return lex_delim(sym, str);
	case NUMBR: return lex_numbr(sym, str);
	default: assert(false /* this will never happen */);
	}
}

size_t
lex_alpha(Symbol *sym, String *str)
{
	size_t ret;
	eat(ret, arr(str), iswalpha(wc));
	return ret;
}

size_t
lex_ident(Symbol *sym, String *str)
{
	size_t ret;
	eat(ret, arr(str), !iswspace(wc) && !iswpunct(wc));
	return ret;
}

size_t
lex_ignor(Symbol *sym, String *str)
{
	return !!len(str);
}

size_t
lex_delim(Symbol *sym, String *str)
{
	char delim[4 + 1] = {0};
	char *matq = 0;
	size_t ret = 0;
	size_t len = 0;

	len = mblen(arr(str), len(str));
	memcpy(delim, arr(str), len);

again:
	matq = memchr(arr(str) + ret, *delim, len(str) - ret);
	if (!matq) return ret;

	if (arr(str)[-1] == '\\') {
		ret += matq - arr(str) + ret;
		goto again;
	}

	return ret;
}

size_t
lex_numbr(Symbol *sym, String *str)
{
	size_t ret;
	eat(ret, arr(str), iswdigit(wc));
	return ret;
}

size_t
lex_space(Symbol *sym, String *str)
{
	size_t ret;
	eat(ret, arr(str), iswspace(wc));
	return ret;
}
