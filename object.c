void *
call(Object *obj, char *name)
{
	void *(*func)(Object *);

	func = NULL;
	for (i = 0; i < obj->meth.c; ++i) {
		if (!strcmp(obj->meth.v[i]->name, name))
			func = obj->meth[i]->func;
	}

	if (!func) return NULL;

	return func(obj);
}
