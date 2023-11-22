#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdlib.h>

#define STATIC_TEXT_COMPONENT 0
#define RECTANGLE_COMPONENT 1
#define LINE_COMPONENT 2
#define BUTTON_COMPONENT 3
#define TEXT_FIELD_COMPONENT 4

#define CLICK_EVENT 0
#define KEY_PRESS_EVENT 1

typedef struct _InternalWindow {
    Display *display;
    int screen;
    Window window;
    XImage *image;
    XIM im;
    XIC ic;
    unsigned int width;
    unsigned int height;
} *InternalWindow;

typedef struct _InternalEvent {
    int type;
    long long data;
} InternalEvent;

InternalWindow createInternalWindow(char *title, unsigned int x, unsigned int y,
                                    unsigned int width, unsigned int height,
                                    unsigned long color,
                                    unsigned long *buffer) {
  XSetLocaleModifiers("@im=none");
  Display *display = XOpenDisplay(NULL);
  int screen = XDefaultScreen(display);
  XSetWindowAttributes attributes = {.background_pixel = color};
  Window window =
      XCreateWindow(display, XRootWindow(display, screen), x, y, width, height,
                    0, XDefaultDepth(display, screen), InputOutput,
                    XDefaultVisual(display, screen), CWBackPixel, &attributes);
  XStoreName(display, window, title);
  XSelectInput(display, window, KeyPressMask | ButtonPressMask | ExposureMask);
  XMapWindow(display, window);

  XImage *image = XCreateImage(display, XDefaultVisual(display, screen),
                               XDefaultDepth(display, screen), ZPixmap, 0,
                               (char *)buffer, width, height, 32, width * 4);

  InternalWindow internalWindow = malloc(sizeof(struct _InternalWindow));
  internalWindow->display = display;
  internalWindow->window = window;
  internalWindow->screen = screen;
  internalWindow->image = image;
  internalWindow->width = width;
  internalWindow->height = height;
  internalWindow->im = XOpenIM(display, NULL, NULL, NULL);
  internalWindow->ic = XCreateIC(internalWindow->im, XNInputStyle,
                                XIMPreeditNothing | XIMStatusNothing,
                                XNClientWindow, window, NULL);
  XSetICFocus(internalWindow->ic);
  return internalWindow;
}

void destroyInternalWindow(InternalWindow internalWindow) {
  XUnmapWindow(internalWindow->display, internalWindow->window);
  XDestroyWindow(internalWindow->display, internalWindow->window);
  XDestroyIC(internalWindow->ic);
  XCloseIM(internalWindow->im);
  XCloseDisplay(internalWindow->display);
  free(internalWindow);
}

void setInternalWindowTitle(InternalWindow internalWindow, char *title) {
  XStoreName(internalWindow->display, internalWindow->window, title);
}

void paintInternalWindow(InternalWindow internalWindow) {
  XPutImage(internalWindow->display, internalWindow->window,
            XDefaultGC(internalWindow->display, internalWindow->screen),
            internalWindow->image, 0, 0, 0, 0, internalWindow->width,
            internalWindow->height);
}

InternalEvent getInternalEvent(InternalWindow internalWindow) {
  XEvent event;
  XNextEvent(internalWindow->display, &event);
  InternalEvent internalEvent = {0};
  internalEvent.type = -1;
  if (XFilterEvent(&event, internalWindow->window))
    return internalEvent;
  switch (event.type) {
  case ButtonPress:
    switch (event.xbutton.button) {
    case Button1:
      internalEvent.type = CLICK_EVENT;
      internalEvent.data =
          ((long long)event.xbutton.x) << 32 | ((long long)event.xbutton.y);
      break;
    }
    break;
  case KeyPress: {
    KeySym keysym = 0;
    char buffer[20] = {0};
    Status status = 0;
    XmbLookupString(internalWindow->ic, (XKeyPressedEvent *)&event, buffer, 20,
                    &keysym, &status);
    internalEvent.type = KEY_PRESS_EVENT;
    internalEvent.data = buffer[0];
    if (keysym == 65288) {
      internalEvent.data = 8;
    }
  } break;
  default:
    break;
  }
  return internalEvent;
}