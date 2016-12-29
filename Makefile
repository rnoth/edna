CC ?= cc
CFLAGS ?= -g -Wall -Wextra -pedantic
LDFLAGS ?= -lc

SRC = command.c line.c main.c util.c
OBJ = ${SRC:.c=.o}
DEPS = edna.h

edna: $(OBJ)
	@echo CC $(LDFLAGS) -o edna *.o
	@$(CC) $(LDFLAGS) -o edna *.o

%.o: %.c $(DEPS)
	@echo CC $(CFLAGS) -o $@ -c $<
	@$(CC) $(CFLAGS) -o $@ -c $<

command.o: CFLAGS += -Wno-unused-parameter

main.o: config.h

clean:
	rm -f edna log *.o 
