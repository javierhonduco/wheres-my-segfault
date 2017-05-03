all:
	@cc -c segfault.c -fpic -Wall -pedantic
	@gcc -shared -o segfault.so segfault.o
	@gcc experiment.c -o experiment -g
