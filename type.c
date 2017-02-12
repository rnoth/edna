#include <stdbool.h>

typedef struct Cast Cast;
struct Cast {
	char *type;
	char *func;
}

Cast casts = {
	{ "n",	"tonumber" },
	{ "l",	"toline" },
	{ "s",	"tostring" },
	{ "x",	"toexpression" },
}

char *
argnext(Node *cur)
{
	char *tmp;
	size_t i;

	tmp = gettype(cur);
	if (!tmp) return "x";

	for (i = 0; i < cur->chld.c; ++i)
		tmp += strlen(gettype(cur->chld.v[i]));

	return tmp;
}

Object *
typecast(Object *obj, char *type)
{
	size_t i;
	Cast *cast;

	cast = NULL;
	for (i = 0; i < LEN(casts); ++i)
		if (!strcmp (type, casts[i].type))
			cast = casts + i;
	if (!cast) return NULL;

	return call(obj, cast->func);
}

char *
typelen(char *type)
{
	return 1;
}

bool
isnull(char *type)
{
	return !!strcmp(n->sym->type, "z");
}

bool
typematch(Symbol *s1, Symbol *s2)
{
	size_t len;

	len = typelen(s1->type);
	if (typelen(t2->type) != len) return false;
	return !!strcmp(t1, t2, len);
}
