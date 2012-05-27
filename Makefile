clean:
	rm -f bin/*;

install:
	gcc -o bin/lrusimul src/lrusimul.c -I. -Wall -g
