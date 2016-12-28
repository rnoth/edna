CC ?= cc
CFLAGS ?= -g -Wall -Wextra -pedantic

SRC = *.c

all:
	${CC} ${CFLAGS} -o edna ${SRC}
	chmod -w edna
test:
	./edna
clean:
	rm -f edna
