#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

static void *getnext(void *);
static void link(void *, void const *);
static void *walk(void *, size_t);

#if 0
void *
cadr(void *li)
{
	List(char) *tmp;

	tmp = getnext(li);
	if (!tmp) return NULL;

	return car(tmp);
}

void *
cddr(void *li)
{
	List(char) *tmp;

	tmp = getnext(li);
	if (!tmp) return NULL;

	return getnext(tmp);
}
#endif

void
free_list(void *list)
{
	void *next, *prev;

	next = list;
	while (next) {
		prev = next;
		next = getnext(prev);
		free(prev);
	}
}

void *
list_cons(void *data, void *_list)
{
	char *ret;
	size_t off;
	List(char) list;

	assert(data);
	assert(_list);

	memcpy(&list, _list, sizeof list);

	ret = calloc(1, sizeof list);
	if (!ret) return NULL;

	off = offsetof(List(char), z);
	memcpy(ret + off, &list.z, sizeof list.z);
	off = offsetof(List(char), next);
	if (list.w) memset(ret + off, 0, sizeof list.next);
	else memcpy(ret + off, &_list, sizeof _list);
	off = offsetof(List(char), val);
	memcpy(ret + off, data, list.z);

	if (list.w) {
		memcpy(_list, ret, sizeof list);
		free(ret);
		ret = _list;
		list.w = 0;
	}

	return ret;
}

void *
getnext(void *list)
{
	size_t off;
	void *ret;

	off = offsetof(List(char), next);
	memcpy(&ret, (char *)list + off, sizeof (void *));

	return ret;
}

void
link(void *prev, void const *next)
{
	size_t off;

	off = offsetof(List(char), next);
	memcpy((char *)prev + off, &next, sizeof next);
}

void *
walk(void *list, size_t ext)
{
	size_t rem;
	void *cur, *prev;

	rem = ext;
	cur = list;
	while (--rem) {
		prev = cur;
		cur = getnext(cur);
		if (!cur) return prev;
	}

	return cur;
}

void
list_append(void *dest, void *src)
{
	List(char) li;

	memcpy(&li, dest, sizeof li);
	if (li.w) {
		memcpy(dest, src, sizeof li);
		free(src);
	} else link(walk(dest, (size_t)-1), src);
}
