#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>

typedef struct {
    KeySym keysym;
    int is_pressed;
} ModifierKeyStatus;

ModifierKeyStatus keys[MODIFIER_KEYS_NUM] = {
	{XK_Control_L, 0},
	{XK_Control_R, 0},
	{XK_Alt_L, 0},
	{XK_Alt_R, 0},
	{XK_Shift_L, 0},
	{XK_Shift_R, 0},
	{XK_Super_L, 0},
	{XK_Super_R, 0}
};
const int MODIFIER_KEYS_NUM = sizeof(keys) / sizeof(ModifierKeyStatus);

void grab_keys(Display* display, Window root, ModifierKeyStatus* keys) {
    for (int i = 0; i < MODIFIER_KEYS_NUM; i++) {
        int keycode = XKeysymToKeycode(display, keys[i].keysym);
        XGrabKey(display, keycode, AnyModifier, root, True, GrabModeAsync, GrabModeAsync);
    }
}

void monitor_keys(Display* display, XkbDescPtr kbdesc) {
    XEvent event;

    Window root = DefaultRootWindow(display);
    grab_keys(display, root, keys);

    // Ungrab keys
    for (int i = 0; i < MODIFIER_KEYS_NUM; i++) {
        int keycode = XKeysymToKeycode(display, keys[i].keysym);
        XUngrabKey(display, keycode, AnyModifier, root);
    }

    // Pause briefly
    usleep(10000); // 10 ms

    // Re-grab keys
    grab_keys(display, root, keys);

    // Check the current state of the modifier keys
    Window root_return, child_return;
    int root_x_return, root_y_return, win_x_return, win_y_return;

	XkbStateRec state;
	XkbGetState(display, XkbUseCoreKbd, &state);

	for (int i = 0; i < MODIFIER_KEYS_NUM; i++) {
		if ((keys[i].keysym == XK_Control_L || keys[i].keysym == XK_Control_R) && (state.mods & ControlMask))
			keys[i].is_pressed = 1;
		else if ((keys[i].keysym == XK_Alt_L || keys[i].keysym == XK_Alt_R) && (state.mods & Mod1Mask))
			keys[i].is_pressed = 1;
		else if ((keys[i].keysym == XK_Shift_L || keys[i].keysym == XK_Shift_R) && (state.mods & ShiftMask))
			keys[i].is_pressed = 1;
		else if ((keys[i].keysym == XK_Super_L || keys[i].keysym == XK_Super_R) && (state.mods & Mod4Mask))
			keys[i].is_pressed = 1;
	}

    // Print current state of modifier keys
    int any_pressed=0;
    for (int i = 0; i < MODIFIER_KEYS_NUM; i++) {
        if (keys[i].is_pressed) {
            printf("%s is currently pressed\n", XKeysymToString(keys[i].keysym));
            any_pressed=1;
        }
    }
    if (!any_pressed) return;

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

            for (int i = 0; i < MODIFIER_KEYS_NUM; i++) {
                if (keys[i].keysym == keysym) {
                    if (event.type == KeyPress) {
						keys[i].is_pressed = 1;
                    	printf("%s event occurred (PRESSED)\n", XKeysymToString(keysym));
					} else {
                    	printf("%s event occurred (Released)\n", XKeysymToString(keysym));
					}
                }
            }

            int all_released = 1;
            for (int i = 0; i < MODIFIER_KEYS_NUM; i++) {
                if (keys[i].is_pressed) {
                    printf("%s is still pressed\n", XKeysymToString(keys[i].keysym));
                    all_released = 0;
                    break;
                }
            }

            if (all_released) {
                break;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "acsw")) != -1) {
        switch (opt) {
        case 'a':
            printf("Monitoring Alt keys\n");
            break;
        case 'c':
            printf("Monitoring Control keys\n");
            break;
        case 's':
            printf("Monitoring Shift keys\n");
            break;
        case 'w':
            printf("Monitoring Win/Super keys\n");
            break;
        default:
            fprintf(stderr, "Usage: %s [-acsw]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    Display *display;
    display = XOpenDisplay(0);
    if (display == NULL) {
        fprintf(stderr, "Unable to open display\n");
        exit(EXIT_FAILURE);
    }

    XkbDescPtr kbdesc = XkbGetKeyboard(display, XkbAllComponentsMask, XkbUseCoreKbd);
    if (kbdesc == NULL) {
        fprintf(stderr, "Failed to get keyboard description.\n");
        exit(EXIT_FAILURE);
    }

    monitor_keys(display, kbdesc);

    XkbFreeKeyboard(kbdesc, XkbAllComponentsMask, True);
    XCloseDisplay(display);
    return 0;
}
