# Makefile for edna v0.2
CC ?= cc
LD ?= ld
DBFLAGS ?= -g3 -O0 -W -Wall -Wextra -pedantic -pedantic-errors -Werror
CFLAGS  ?= -W -Wall -Wextra -pedantic -Werror -pedantic-errors
LDFLAGS ?= -lc
DEBUG    = yes

SRC  != find .  -maxdepth 1 -name '*.c'
TEST != find ./tests -name '*.c'
TEST := ${TEST:.c=}
OBJ  := ${SRC:.c=.o}
DEPS := edna.h vector.h str.h
TARG := edna
VERS := 0.2

edna:

# includes
-include deps.mk

# conditional
ifdef DEBUG
	CFLAGS := $(DBFLAGS)
endif

# recipes
edna: deps.mk $(OBJ)
	@echo LD $(LDFLAGS) -o $@ $(OBJ)
	@$(CC)   $(LDFLAGS) -o $@ $(OBJ)

test: $(TEST)
	@echo "Testing complete. Everything went ok, looks like you didn't break anything"

deps.mk: $(SRC)
	@echo Making deps.mk
	@$(CC) -MM $(SRC) > deps.mk

%.o: %.c $(DEPS)
	@echo CC $(CFLAGS) -c -o $@ $<
	@$(CC)   $(CFLAGS) -c -o $@ $<

tests/%_test: tests/%_test.c
	@echo Compiling $<
	@$(CC) $(DBFLAGS) -o $@ $<
	@echo Running $@
	@$@
	@echo Test successful: $@

clean:
	rm -f edna log core *.o vgcore.* tests/*_test deps.mk

lint:
	mkdir -p /tmp/report
	scan-build -o /tmp make

# defending against false positives
addr_%.o: CFLAGS += -Wno-unused-parameter
cmd_%.o:  CFLAGS += -Wno-unused-parameter
init.o:   CFLAGS += -Wno-unused-parameter
insert.o: CFLAGS += -Wno-unused-parameter

.PHONY: clean lint test
