CC ?= cc
#LD ?= ld
CFLAGS ?= -g -Wall -Wextra -Werror -pedantic
LDFLAGS ?= -lc

SRC != find . -name '*.c'
OBJ = ${SRC:.c=.o}
DEPS = edna.h

edna: ${OBJ}
	@echo LD $(LDFLAGS) -o edna *.o
	@$(CC) $(LDFLAGS) -o edna *.o

%.o: %.c $(DEPS)
	@echo CC $(CFLAGS) -o $@ -c $<
	@$(CC) $(CFLAGS) -o $@ -c $<

cmd_%.o: cmd.h
cmd_%.o: CFLAGS += -Wno-unused-parameter

init.o: config.h

main.o: config.h

clean:
	rm -f edna log *.o 

lint:
	mkdir -p /tmp/report
	scan-build -o /tmp make

.PHONY: clean lint
