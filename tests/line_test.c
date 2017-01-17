#include <assert.h>

#include "../edna.h"

int
main ()
{
	Line *foo, *bar, *baz, *ret;
	char *stringtab[] = { "foo'ing all day err day\n", NULL };
	printf ("\tMaking some test lines");
	foo = makeline ();
	bar = makeline ();
	baz = makeline ();
	assert (foo->str == NULL);
	assert (bar->len == 0);
	assert (baz->next == NULL);
	printf ("\n");

	printf ("\tTesting putline()");
	ret = putline (foo, stringtab[0], strlen (stringtab[0]));
	assert (ret == foo);
	assert (strcmp (foo->str, stringtab[0]) == 0);
	assert (foo->len == strlen(stringtab[0]));
	printf ("\n");

	return 0;
}
