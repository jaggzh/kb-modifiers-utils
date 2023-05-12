#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

int main() {
	Display *display;
	XEvent ev;

	display = XOpenDisplay(NULL);
	if (!display) {
		fprintf(stderr, "Unable to open display.\n");
		return 1;
	}

	XQueryKeymap(display, ev.xkeymap.key_vector);

	KeyCode controlKey = XKeysymToKeycode(display, XK_Control_L);
	KeyCode shiftKey = XKeysymToKeycode(display, XK_Shift_L);
	KeyCode altKey = XKeysymToKeycode(display, XK_Alt_L);
	KeyCode superKey = XKeysymToKeycode(display, XK_Super_L);

	char isControlPressed = ev.xkeymap.key_vector[controlKey >> 3] & (1 << (controlKey & 7));
	char isShiftPressed = ev.xkeymap.key_vector[shiftKey >> 3] & (1 << (shiftKey & 7));
	char isAltPressed = ev.xkeymap.key_vector[altKey >> 3] & (1 << (altKey & 7));
	char isSuperPressed = ev.xkeymap.key_vector[superKey >> 3] & (1 << (superKey & 7));

	printf("control: %s\n", isControlPressed ? "Pressed" : "Released");
	printf("shift: %s\n", isShiftPressed ? "Pressed" : "Released");
	printf("alt: %s\n", isAltPressed ? "Pressed" : "Released");
	printf("super: %s\n", isSuperPressed ? "Pressed" : "Released");

	XCloseDisplay(display);

	return 0;
}

