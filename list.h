#ifndef _list_
#define _list_
#include <stdbool.h>

#define List(Type)		\
	struct {		\
		bool w;		\
		void *next;	\
		size_t z;	\
		Type val;	\
	}

#define make_list(LI) do {		\
	List(char) *_li;		\
	_li = calloc(1, sizeof *LI);	\
	_li->z = sizeof (LI)->val;	\
	_li->w = true;			\
	LI = (void *)_li;		\
} while (0)

/* e.g. mapl(list, free(each)) */
#define mapl(LI, expr) do {		\
	void *each;			\
	List(void *) *_li;		\
	_li = (void *)LI;		\
	do {				\
		each = car(_li);	\
		expr;			\
		_li = cdr(_li);		\
	} while (_li);			\
} while (0)

#define car(li) ((li)->val)
#define cdr(li) ((li)->next)

void *list_cons(void *, void *);
void free_list(void *);
void list_append(void *, void *);
#endif
