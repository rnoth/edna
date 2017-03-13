# Makefile for edna v0.2
CC	?= cc
CFLAGS  ?= -std=c99 -fPIC -W -Wall -Wextra -Werror -pedantic -Wno-unused-parameter
LDFLAGS ?= -lc
SOFLAGS ?= -lc -fPIC -shared -Wl,-rpath=$(shell pwd)
DEBUG   ?= yes

SRC  != find . -maxdepth 1 -name '*.c'
OBJ  := ${SRC:.c=.o}
TARG := edna
VERS := 0.1

edna: deps.mk config.h $(OBJ)

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
edna:
	@echo LD -o $@
	@$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LIB)

libedna.so: deps.mk $(OBJ) $(LIB)
	@echo LD -o $@
	@$(CC) $(SOFLAGS) -o $@ $(filter-out main.o, $(OBJ)) $(LIB)

deps.mk: $(SRC)
	@echo "Making deps.mk"
	@$(CC) -M $(SRC) > deps.mk

config.h:
	@echo "Making config.h"
	@cp config.def.h config.h

%.o: %.c
	@echo CC $<
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f edna *.o *.a core vgcore.* tests/*_test deps.mk

lint:
	mkdir -p /tmp/report
	scan-build -o /tmp make

.PHONY: clean lint test
