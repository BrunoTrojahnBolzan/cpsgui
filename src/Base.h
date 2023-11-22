#define STATIC_TEXT_COMPONENT 0
#define RECTANGLE_COMPONENT 1
#define LINE_COMPONENT 2
#define BUTTON_COMPONENT 3
#define TEXT_FIELD_COMPONENT 4

#define CLICK_EVENT 0
#define KEY_PRESS_EVENT 1

typedef void *InternalWindow;

typedef struct _InternalEvent {
    int type;
    long long data;
} InternalEvent;

InternalWindow createInternalWindow(char *title, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned long color, unsigned long *buffer);
void destroyInternalWindow(InternalWindow internalWindow);
void setInternalWindowTitle(InternalWindow internalWindow, char *title);
void paintInternalWindow(InternalWindow internalWindow);
InternalEvent getInternalEvent(InternalWindow internalWindow);
void destroyInternalWindow(InternalWindow internalWindow);