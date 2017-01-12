CC ?= cc
#LD ?= ld
CFLAGS ?= -g -Wall -Wextra -Werror -pedantic -pedantic-errors
LDFLAGS ?= -lc

SRC    != find .  -maxdepth 1 -name '*.c'
OBJ	= ${SRC:.c=.o}
DEPS	= edna.h vector.h string.h
TARGET	= edna

edna: $(OBJ)
	@echo LD $(LDFLAGS) -o $(TARGET) $(OBJ)
	@$(CC) $(LDFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c $(DEPS)
	@echo CC $(CFLAGS) -o $@ -c $<
	@$(CC) $(CFLAGS) -o $@ -c $<

cmd_%.o: cmd.h
cmd_%.o: CFLAGS += -Wno-unused-parameter

address.o: addr.h set.h
addr_%.o: CFLAGS += -Wno-unused-parameter
addr_%.o: addr.h set.h

set.o: set.h

buffer.o: vector.h
cmd_buffer.o: vector.h

init.o: config.h vector.h
init.o: CFLAGS += -Wno-unused-parameter

insert.o: CFLAGS += -Wno-unused-parameter

main.o: config.h

clean:
	rm -f edna log *.o 

lint:
	mkdir -p /tmp/report
	scan-build -o /tmp make

.PHONY: clean lint
