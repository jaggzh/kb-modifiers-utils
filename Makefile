LIBS=-lX11

all: gobble kbmods-state

kbmods-state: kbmods-state.c
	gcc -o "$@" $^ $(LIBS)

gobble: gobble.c
	gcc -o gobble gobble.c $(LIBS)

debug:
	gcc -ggdb3 -o gobble gobble.c $(LIBS)

run_debug: debug
	gdb ./gobble

vi:
	vim README.md Makefile gobble.c kbmods-state.c


# vim: noet
