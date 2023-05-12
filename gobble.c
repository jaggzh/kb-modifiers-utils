
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>

int main() {
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

    XSelectInput(display, root, KeyPressMask | KeyReleaseMask);

    XEvent event;

    // Get the keyboard description
    XkbDescPtr kbdesc = XkbGetKeyboard(display, XkbAllComponentsMask, XkbUseCoreKbd);
    if (kbdesc == NULL) {
        fprintf(stderr, "Failed to get keyboard description.\n");
        return 1;
    }

    while (1) {
        XNextEvent(display, &event);
        printf("Event!\n");

        if (event.type == KeyPress || event.type == KeyRelease) {
            KeyCode key = event.xkey.keycode;
            unsigned int modifier;
            KeySym keysym;

            XkbStateRec state;
            XkbGetState(display, XkbUseCoreKbd, &state);

            XkbTranslateKeyCode(kbdesc, key, state.compat_state, &modifier, &keysym);

            if (keysym == XK_Control_L)
                printf("Control_L event occurred\n");
            else if (keysym == XK_Meta_L)
                printf("Meta_L event occurred\n");
        }
    }

    XkbFreeKeyboard(kbdesc, XkbAllComponentsMask, True);
    XCloseDisplay(display);
    return 0;
}
