.PHONY: clear all

CCFLAGS += -pipe -O2 -g
LDFLAGS += -lm -lpng

INCLUDE_HEADERS := src/grooph.h src/GRDraw.h src/GRDefs.h src/GRTypes.h src/GREssence.h

all: bin/grooph.so

bin/GRizmos.o: src/GRizmos.c src/GRizmos.h
	cc $(CCFLAGS) -c src/GRizmos.c -o bin/GRizmos.o

bin/GREssence.o: src/GREssence.c src/GREssence.h src/GRTypes.h src/GRDefs.h
	cc $(CCFLAGS) -c src/GREssence.c -o bin/GREssence.o

bin/GRDraw.o: src/GRDraw.c src/GRDraw.h src/GRTypes.h src/GRDefs.h
	cc $(CCFLAGS) -c src/GRDraw.c -o bin/GRDraw.o

bin/grooph.so: bin/GREssence.o bin/GRDraw.o bin/GRizmos.o
	cc $(LDFLAGS) -shared bin/GREssence.o bin/GRDraw.o bin/GRizmos.o -o bin/grooph.so

include: $(INCLUDE_HEADERS)
	cp src/grooph.h include
	cp src/GRDraw.h include
	cp src/GRDefs.h include
	cp src/GRTypes.h include
	cp src/GREssence.h include

clear:
	rm -f bin/GREssence.o bin/GRizomos.o bin/GRDraw.o bin/grooph.so
