#ifndef _edna_object_
#define _edna_object_

struct Method {
	char *name;
	void *func;
};

struct Object {
	char *type;
	VECTOR(Method, *meth);
	void *val;
};

extern void	free_object	(Object *);
#endif
