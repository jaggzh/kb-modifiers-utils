LIBS=-lX11
# LIBS_GOBBLE=-lX11 -lxkbcommon -lxkbcommon-x11

all: kbmods-state

kbmods-state: kbmods-state.c
	gcc -o "$@" $^ $(LIBS)

debug:
	gcc -ggdb3 -o kbmods-state kbmods-state.c $(LIBS)

run_debug: debug
	gdb ./kbmods-state

vi:
	vim README.md Makefile kbmods-state.c gobble.c
