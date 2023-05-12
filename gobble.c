#include <X11/Xlib.h>
#include <X11/keysym.h>

void main() {
	Display *display;
	Window root;
	int keycode;

	display = XOpenDisplay(0);
	root = DefaultRootWindow(display);

	// Replace these with the actual keycodes for Control and Meta.
	keycode = XKeysymToKeycode(display, XK_Control_L);
	XGrabKey(display, keycode, AnyModifier, root, True, GrabModeAsync, GrabModeAsync);

	keycode = XKeysymToKeycode(display, XK_Meta_L);
	XGrabKey(display, keycode, AnyModifier, root, True, GrabModeAsync, GrabModeAsync);

	// This will need to run in a loop, waiting for events and handling them.
	XEvent event;
	while (1) {
		XNextEvent(display, &event);

		// Handle the events here. If it's a KeyPress or KeyRelease event for the
		// Control or Meta keys, you can choose to ignore it or modify it.
	}

	XCloseDisplay(display);
}

