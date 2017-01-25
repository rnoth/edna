# Makefile for edna v0.2
CC ?= cc
CFLAGS  ?= -std=c89 -fPIC -W -Wall -Wextra -Wpedantic -Werror
CPPFLAGS?= #-D_POSIX_C_SOURCE=199309L
LDFLAGS ?= -lc
SOFLAGS ?= -lc -fPIC -shared -Wl,-rpath=$(shell pwd)
ARFLAGS ?= rcs
DEBUG   ?= yes

SRC  != find . -maxdepth 1 -name '*.c'
OBJ  := ${SRC:.c=.o}
TEST != find ./tests -name '*.c' | sed s/.c$$//
LIB  := str.a vector.a set.a
TARG := edna
VERS := 0.1

edna: deps.mk $(OBJ)

# includes
-include deps.mk

# conditionals
ifdef DEBUG
	CFLAGS += -g3 -O0
endif

# depencies
str.a: string.o str_utf8.o
vector.a: vector.o
set.a: set.o

# recipes
edna: deps.mk $(OBJ) $(LIB)
	@echo LD $@
	@$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LIB)

libedna.so: deps.mk $(OBJ) $(LIB)
	@echo CCLD -o $@
	@$(CC) $(SOFLAGS) -o $@ $(filter-out main.o, $(OBJ)) $(LIB)

test: $(TEST)
	@echo -e "Testing complete. \e[32mEverything went ok\e[0m, looks like you didn't break anything"

deps.mk: $(SRC)
	@echo "Making deps.mk"
	@$(CC) -M $(SRC) > deps.mk

%.o: %.c
	@echo CC $<
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

%.a:
	@echo AR $@
	@ar $(ARFLAGS) $@ $+ > /dev/null
	@printf "\e[A\e[2K" # ar has no --silent option

tests/%_test: CFLAGS += -Wl,-rpath=$(shell pwd)
tests/%_test: tests/%_test.c %.o libedna.so
	@echo -e "Compiling:\e[35m" $< "\e[0m"
	@echo CC $<
	@$(CC) $(CFLAGS) -o $@ $< libedna.so
	@echo -e "Running:\e[36m" $@ "\e[0m"
	@$@
	@echo -e "Test successful:\e[36m" $@ "\e[0m"
	@echo

clean:
	rm -f edna *.o *.a core vgcore.* tests/*_test deps.mk

lint:
	mkdir -p /tmp/report
	scan-build -o /tmp make

# defending against false positives
addr_%.o: CFLAGS += -Wno-unused-parameter
cmd_%.o:  CFLAGS += -Wno-unused-parameter
insert.o: CFLAGS += -Wno-unused-parameter
init.o: CFLAGS += -Wno-unused-parameter

.PHONY: clean lint test
