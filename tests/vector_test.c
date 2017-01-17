/* tests/vector_test.c -- test the vector.h macros */
#include <assert.h>
#include <stdio.h>

#include "../vector.h"

int
main ()
{
	printf ("\tDeclaring an int vector...");
	VECTOR (int, intvec);
	printf ("done.\n");

	printf ("\tAllocating memory for vector of 8 ints...");
	MAKE_VECTOR (int, intvec, 8);
	assert (intvec.v[0] == 0);
	assert (intvec.c == 0);
	assert (intvec.m == 8);
	printf ("done.\n");

	printf ("\tTesting append...");
	VEC_APPEND (int, intvec, 2);
	assert (intvec.v[0] == 2);
	VEC_APPEND (int, intvec, 5);
	assert (intvec.v[1] == 5);
	assert (intvec.c == 2);
	assert (intvec.m == 8);
	printf ("done.\n");

	printf ("\tTesting insert...");
	VEC_INSERT (int, intvec, 0, 4);
	assert (intvec.v[0] == 4);
	assert (intvec.v[1] == 2);
	VEC_INSERT (int, intvec, 3, 1);
	assert (intvec.v[3] == 1);
	assert (intvec.v[4] == 0);
	assert (intvec.v[0] == 4);
	assert (intvec.c == 4);
	printf ("done.\n");

	printf ("\tTesting remove...");
	VEC_REMOVE (int, intvec, 2);
	assert (intvec.v[2] == 1);
	assert (intvec.v[3] == 0);
	VEC_REMOVE (int, intvec, 2);
	assert (intvec.v[2] == 0);
	VEC_REMOVE (int, intvec, 0);
	assert (intvec.c == 1);
	printf ("done.\n");

	printf ("\tTesting free...");
	FREE_VECTOR (intvec);
	printf ("done.\n");
	return 0;
}
