/* tests/vector_test.c -- test the vector.h macros */
#include <assert.h>
#include <stdio.h>

#include "../vector.h"

int
main ()
{
	puts ("Declaring an int vector");
	VECTOR (int, intvec);

	puts ("Allocating memory for vector of 8 ints");
	MAKE_VECTOR (int, intvec, 8);
	assert (intvec.v[0] == 0);
	puts (". ");
	assert (intvec.c == 0);
	puts (". ");
	assert (intvec.m == 8);
	puts (". done");

	puts ("Testing append");
	VEC_APPEND (int, intvec, 2);
	assert (intvec.v[0] == 2);
	puts (". ");
	VEC_APPEND (int, intvec, 5);
	assert (intvec.v[1] == 5);
	puts (". ");
	assert (intvec.c == 2);
	puts (". ");
	assert (intvec.m == 8);
	puts (". done");

	puts ("Testing insert");
	VEC_INSERT (int, intvec, 0, 4);
	assert (intvec.v[0] == 4);
	puts (". ");
	assert (intvec.v[1] == 2);
	puts (". ");
	VEC_INSERT (int, intvec, 3, 1);
	assert (intvec.v[3] == 1);
	puts (". ");
	assert (intvec.v[4] == 0);
	puts (". ");
	assert (intvec.v[0] == 4);
	puts (". ");
	assert (intvec.c == 4);
	puts (". done");

	puts ("Testing remove");
	VEC_REMOVE (int, intvec, 2);
	assert (intvec.v[2] == 1);
	puts (". ");
	assert (intvec.v[3] == 0);
	puts (". ");
	VEC_REMOVE (int, intvec, 2);
	assert (intvec.v[2] == 0);
	puts (". ");
	VEC_REMOVE (int, intvec, 0);
	assert (intvec.c == 1);
	puts (". done");

	puts ("Testing free");
	FREE_VECTOR (intvec);
	puts("Testing complete. Everything went ok, looks like you didn't break anything");
	return 0;
}
