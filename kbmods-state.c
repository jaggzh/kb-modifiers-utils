#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <unistd.h>
#include <stdlib.h>

void usage() {
	printf("Usage: kbmods-state [-wv]\n"
	       "  -w  wait for no modifiers to be held\n"
	       "  -v  with -w, still outputs state\n");
}

int main(int argc, char *argv[]) {
	Display *display;
	XEvent ev;

	int opt;
	int opt_wait=0;
	int opt_verbose=0;
	while ((opt = getopt(argc, argv, "wvh")) != -1) {
		switch (opt) {
		case 'w': opt_wait=1; break;
		case 'v': opt_verbose=1; break;
		case 'h': usage(); exit(0);
		default: usage(); exit(EXIT_FAILURE);
		}
	}

	display = XOpenDisplay(NULL);
	if (!display) {
		fprintf(stderr, "Unable to open display.\n");
		return 1;
	}

	while (1) {
		XQueryKeymap(display, ev.xkeymap.key_vector);

		KeyCode leftControlKey = XKeysymToKeycode(display, XK_Control_L);
		KeyCode rightControlKey = XKeysymToKeycode(display, XK_Control_R);
		KeyCode leftShiftKey = XKeysymToKeycode(display, XK_Shift_L);
		KeyCode rightShiftKey = XKeysymToKeycode(display, XK_Shift_R);
		KeyCode leftAltKey = XKeysymToKeycode(display, XK_Alt_L);
		KeyCode rightAltKey = XKeysymToKeycode(display, XK_Alt_R);
		KeyCode leftSuperKey = XKeysymToKeycode(display, XK_Super_L);
		KeyCode rightSuperKey = XKeysymToKeycode(display, XK_Super_R);

		char isLeftControlPressed = ev.xkeymap.key_vector[leftControlKey >> 3] & (1 << (leftControlKey & 7));
		char isRightControlPressed = ev.xkeymap.key_vector[rightControlKey >> 3] & (1 << (rightControlKey & 7));
		char isControlPressed = isLeftControlPressed || isRightControlPressed;

		char isLeftShiftPressed = ev.xkeymap.key_vector[leftShiftKey >> 3] & (1 << (leftShiftKey & 7));
		char isRightShiftPressed = ev.xkeymap.key_vector[rightShiftKey >> 3] & (1 << (rightShiftKey & 7));
		char isShiftPressed = isLeftShiftPressed || isRightShiftPressed;

		char isLeftAltPressed = ev.xkeymap.key_vector[leftAltKey >> 3] & (1 << (leftAltKey & 7));
		char isRightAltPressed = ev.xkeymap.key_vector[rightAltKey >> 3] & (1 << (rightAltKey & 7));
		char isAltPressed = isLeftAltPressed || isRightAltPressed;

		char isLeftSuperPressed = ev.xkeymap.key_vector[leftSuperKey >> 3] & (1 << (leftSuperKey & 7));
		char isRightSuperPressed = ev.xkeymap.key_vector[rightSuperKey >> 3] & (1 << (rightSuperKey & 7));
		char isSuperPressed = isLeftSuperPressed || isRightSuperPressed;

		if (!opt_wait || opt_verbose) {
			printf("control: %d\n", isControlPressed ? 1 : 0);
			printf("shift: %d\n", isShiftPressed ? 1 : 0);
			printf("alt: %d\n", isAltPressed ? 1 : 0);
			printf("super: %d\n", isSuperPressed ? 1 : 0);

			printf("control_l: %d\n", isLeftControlPressed ? 1 : 0);
			printf("control_r: %d\n", isRightControlPressed ? 1 : 0);
			printf("shift_l: %d\n", isLeftShiftPressed ? 1 : 0);
			printf("shift_r: %d\n", isRightShiftPressed ? 1 : 0);
			printf("alt_l: %d\n", isLeftAltPressed ? 1 : 0);
			printf("alt_r: %d\n", isRightAltPressed ? 1 : 0);
			printf("super_l: %d\n", isLeftSuperPressed ? 1 : 0);
			printf("super_r: %d\n", isRightSuperPressed ? 1 : 0);
		}

		if (!opt_wait) break;
		if (! (isControlPressed || isShiftPressed ||
		       isAltPressed || isSuperPressed)) {
		    break;
		}
		usleep(10000); // 10 ms
	}

	XCloseDisplay(display);

	return 0;
}
