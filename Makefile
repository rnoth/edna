# Makefile for edna v0.2
CC ?= cc
LD ?= ld
CFLAGS  ?= -std=c99 -fPIC -W -Wall -Wextra -pedantic -Werror -pedantic-errors
LDFLAGS ?= -lc
SOFLAGS ?= -lc -fPIC -shared -Wl,-rpath=$(shell pwd)
ARFLAGS ?= rcs
DEBUG   ?= yes

SRC  != find . -name '*.c'
TEST != find ./tests -name '*.c' | sed s/.c$$// 
OBJ  := ${SRC:.c=.o}
LIB  := str.a
DEPS := edna.h vector.h str.h
TARG := edna
VERS := 0.2

edna: deps.mk $(OBJ)

# includes
-include deps.mk
include string.mk
include vector.mk

# conditional
ifdef DEBUG
	CFLAGS += -g3 -O0
endif

# recipes
edna: deps.mk $(OBJ) $(LIB)
	@echo LD $(LDFLAGS) -o $@ $(OBJ) $(LIB)
	@$(CC)   $(LDFLAGS) -o $@ $(OBJ) $(LIB)

libedna.so: deps.mk $(OBJ) $(LIB)
	@echo LD $(SOFLAGS) -o $@ $(filter-out main.o, $(OBJ)) $(LIB)
	@$(CC) $(SOFLAGS) -o $@ $(filter-out main.o, $(OBJ)) $(LIB)

test: $(TEST)
	@echo
	@echo -e "Testing complete. \e[32mEverything went ok\e[0m, looks like you didn't break anything"

deps.mk: $(SRC)
	@echo "Making deps.mk"
	@$(CC) -MM $(SRC) > deps.mk

%.o: %.c
	@echo CC $(CFLAGS) -c -o $@ $<
	@$(CC)   $(CFLAGS) -c -o $@ $<

tests/%_test: CFLAGS += -Wl,-rpath=$(shell pwd)
tests/%_test: tests/%_test.c %.o libedna.so
	@echo -e "Compiling:\e[35m" $< "\e[0m"
	@echo CC $(CFLAGS) -o $@ $< libedna.so
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
init.o:   CFLAGS += -Wno-unused-parameter
insert.o: CFLAGS += -Wno-unused-parameter

.PHONY: clean lint
