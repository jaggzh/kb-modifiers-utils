LIBS=-lX11
LIBS_GOBBLE=-lX11 -lxkbcommon -lxkbcommon-x11

all: gobble kbmods-state

kbmods-state: kbmods-state.c
	gcc -o "$@" $^ $(LIBS)

gobble: gobble.c
	gcc -o gobble gobble.c $(LIBS_GOBBLE)

debug:
	gcc -ggdb3 -o gobble gobble.c $(LIBS_GOBBLE)

run_debug: debug
	gdb ./gobble

vi:
	vim README.md Makefile gobble.c kbmods-state.c


# vim: noet
