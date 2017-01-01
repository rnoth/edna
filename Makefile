CC ?= cc
#LD ?= ld
CFLAGS ?= -g -Wall -Wextra -pedantic
LDFLAGS ?= -lc

SRC = commands.c file.c input.c line.c main.c region.c util.c
OBJ = ${SRC:.c=.o}
DEPS = edna.h

edna: $(OBJ)
	@echo LD $(LDFLAGS) -o edna *.o
	@$(CC) $(LDFLAGS) -o edna *.o

%.o: %.c $(DEPS)
	@echo CC $(CFLAGS) -o $@ -c $<
	@$(CC) $(CFLAGS) -o $@ -c $<

commands.o: cmd.h
commands.o: CFLAGS += -Wno-unused-parameter

main.o: cmd.h config.h

clean:
	rm -f edna log *.o 

lint:
	mkdir -p /tmp/report
	scan-build -o /tmp make

.PHONY: clean lint
