.PHONY: bin clean all include test

CCFLAGS += -pipe -O2 -g -fPIC
LDFLAGS += -lm -lpng

BOLD ?= `[ -z $${TERM} ] && echo "" || tput bold`
UNDERLINE ?= `[ -z $${TERM} ] && echo "" || tput smul`
ANSI_RESET ?= `[ -z $${TERM} ] && echo "" || tput sgr0`
CARGO_COLOR ?= `[ -z $${NO_COLOR+x} ] && echo "" || echo "--color=never "`
RED ?= `[ -z $${NO_COLOR+x} ] && ([ -z $${TERM} ] && echo "" || tput setaf 1) || echo ""`

OBJECTF := bin/grooph_essence.o bin/grooph_draw.o bin/grizmos.o

ALL := bin lib lib/libgrooph.so

all: $(ALL)

help:
	@echo " 1) You need libpng and gcc/clang installed on your machine\n"\
		"2) If png library with headers is not in usual path specify it:\n"\
		"\t* add to CCFLAGS in Makefile -I(your_header_path)\n"\
		"\t* add to LDFLAGS in Makefile -L(your_library_path)\n"\
		"3) run "$(UNDERLINE)"make"$(ANSI_RESET)

bin:
	mkdir -p bin

lib:
	mkdir -p lib

bin/grizmos.o: src/tools/grizmos.c src/tools/grizmos.h
	cc src/tools/grizmos.c $(CCFLAGS) -c -o $@

bin/grooph_essence.o: src/grooph_essence.c src/grooph_essence.h src/grooph_core.h
	cc src/grooph_essence.c $(CCFLAGS) -c -o $@

bin/grooph_draw.o: src/grooph_draw.c src/grooph_draw.h src/grooph_core.h
	cc src/grooph_draw.c $(CCFLAGS) -c -o $@

lib/libgrooph.so: $(OBJECTF)
	cc $(OBJECTF) $(LDFLAGS) -shared -o $@

include:
	cp src/grooph.h include
	cp src/grooph_draw.h include
	cp src/grooph_core.h include
	cp src/grooph_essence.h include

tests/t1: all tests/test1.c
	cc tests/test1.c -L$(CURDIR)/lib -lgrooph -o $@
	
tests/t2: all tests/test2.c
	cc tests/test2.c -L$(CURDIR)/lib -lgrooph -o $@

tests: tests/t1 tests/t2


clean:
	rm -f $(OBJECTF) bin/grooph.so tests/t1 tests/t2 lib/libgrooph.so test1.png test2.png
