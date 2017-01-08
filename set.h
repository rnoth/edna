/* set.h -- set operations */
typedef unsigned long long *Set;

Set
setcomplement (Set A, size_t len)
{
	Set B;
	B = malloc (sizeof *B * len);
	if (!B) die ("malloc");
	for (size_t i = 0; i < len; ++i)
		B[i] = ~A[i];
	return B;
}

Set
setdifference (Set A, Set B, size_t len)
{
	Set C;
	C = malloc (sizeof *C * len);
	if (!C) die ("malloc");
	for (size_t i = 0; i < len; ++i)
		C[i] = A[i] ^ B[i];
	return C;
}

Set
setintersect (Set A, Set B, size_t len)
{
	Set C;
	C = malloc (sizeof *C * len);
	if (!C) die ("malloc");
	for (size_t i = 0; i < len; ++i)
		C[i] = A[i] & B[i];
	return C;
}

Set
setunion (Set A, Set B, size_t len)
{
	Set C;
	C = malloc (sizeof *C * len);
	if (!C) die ("malloc");
	for (size_t i = 0; i < len; ++i)
		C[i] = A[i] | B[i];
	return C;
}

