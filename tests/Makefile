all: test1/t test2/t 

test1/t: test1/test.c bin/grooph.so
	cc test1/test.c $(CURDIR)/bin/grooph.so -o test1/t

test2/t: test2/test.c bin/grooph.so
	cc test2/test.c $(CURDIR)/bin/grooph.so -o test2/t
