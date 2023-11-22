#define STATIC_TEXT_COMPONENT 0
#define RECTANGLE_COMPONENT 1
#define LINE_COMPONENT 2
#define BUTTON_COMPONENT 3
#define TEXT_FIELD_COMPONENT 4

#define CLICK_EVENT 0
#define KEY_PRESS_EVENT 1

typedef struct _InternalEvent {
    int type;
    long long data;
} InternalEvent;

#pragma once

#define CLICK_EVENT 0
#define KEY_PRESS_EVENT 1

#define rgb(r, g, b) (((unsigned char) r)<<16 | ((unsigned char) g)<<8 | ((unsigned char) b))

typedef struct _StaticText {
    unsigned int x, y;
    unsigned int width, height;
    unsigned long color;
    char *text;
} *StaticText;

typedef struct _Rectangle {
    unsigned int x, y;
    unsigned int width, height;
    unsigned long color;
} *Rectangle;

typedef struct _Line {
    int x0, y0, x1, y1;
    unsigned long color;
} *Line;

typedef struct _Button {
    unsigned int x, y;
    unsigned int width, height;
    unsigned long color, textColor;
    unsigned long id;
    char *text;
} *Button;

typedef struct _TextField {
    unsigned int x, y;
    unsigned int width, height;
    unsigned long color, textColor;
    char *text;
} *TextField;

typedef union _ComponentUnion {
    StaticText staticText;
    Rectangle Rectangle;
    Line line;
    Button button;
    TextField textField;
} ComponentUnion;

typedef struct _Component {
    struct _Component *next;
    int type;
    ComponentUnion componentUnion;
} Component;

typedef struct _Window {
    void *internalWindow;
    Component *components;
    int *buffer;
    unsigned long color;
    unsigned int x, y;
    unsigned int width, height;
    Component *focus;
} *Window;

typedef struct _Event {
    Window Window;
    int type;
    long long data;
} Event;

Window createWindow(char *title, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned long color);
void setWindowTitle(Window Window, char *title);
void destroyWindow(Window Window);

Event getEvent(Window Window);
StaticText createStaticText(Window Window, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned long color, char *text);
void setStaticText(Window Window, StaticText staticText, char *title);
void getStaticText(StaticText staticText, char *destination);
void setStaticTextX(Window Window, StaticText staticText, unsigned int x);
void setStaticTextY(Window Window, StaticText staticText, unsigned int y);
void setStaticTextWidth(Window Window, StaticText staticText, unsigned int width);
void setStaticTextHeight(Window Window, StaticText staticText, unsigned int height);
void setStaticTextColor(Window Window, StaticText staticText, unsigned long color);
unsigned int getStaticTextX(StaticText staticText);
unsigned int getStaticTextY(StaticText staticText);
unsigned int getStaticTextWidth(StaticText staticText);
unsigned int getStaticTextHeight(StaticText staticText);
unsigned long getStaticTextColor(StaticText staticText);
void destroyStaticText(Window Window, StaticText staticText);

Rectangle createRectangle(Window Window, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned long color);
void setRectangleX(Window Window, Rectangle Rectangle, unsigned int x);
void setRectangleY(Window Window, Rectangle Rectangle, unsigned int y);
void setRectangleWidth(Window Window, Rectangle Rectangle, unsigned int width);
void setRectangleHeight(Window Window, Rectangle Rectangle, unsigned int height);
void setRectangleColor(Window Window, Rectangle Rectangle, unsigned long color);
unsigned int getRectangleX(Rectangle Rectangle);
unsigned int getRectangleY(Rectangle Rectangle);
unsigned int getRectangleWidth(Rectangle Rectangle);
unsigned int getRectangleHeight(Rectangle Rectangle);
unsigned long getRectangleColor(Rectangle Rectangle);
void destroyRectangle(Window Window, Rectangle Rectangle);

Line createLine(Window Window, int x0, int y0, int x1, int y1, unsigned long color);
void setLineX0(Window Window, Line line, int x0);
void setLineY0(Window Window, Line line, int y0);
void setLineX1(Window Window, Line line, int x1);
void setLineY1(Window Window, Line line, int y1);
void setLineColor(Window Window, Line line, unsigned long color);
int getLineX0(Line line);
int getLineY0(Line line);
int getLineX1(Line line);
int getLineY1(Line line);
unsigned long getLineColor(Line line);
void destroyLine(Window Window, Line line);

Button createButton(Window Window, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned long color, char *text, unsigned long textColor, unsigned long id);
void setButtonX(Window Window, Button button, unsigned int x);
void setButtonY(Window Window, Button button, unsigned int y);
void setButtonWidth(Window Window, Button button, unsigned int width);
void setButtonHeight(Window Window, Button button, unsigned int height);
void setButtonColor(Window Window, Button button, unsigned long color);
void setButtonText(Window Window, Button button, char *text);
void setButtonTextColor(Window Window, Button button, unsigned long textColor);
unsigned int getButtonX(Button button);
unsigned int getButtonY(Button button);
unsigned int getButtonWidth(Button button);
unsigned int getButtonHeight(Button button);
unsigned long getButtonColor(Button button);
void getButtonText(Button button, char *destination);
unsigned long getButtonTextColor(Button button);
void destroyButton(Window Window, Button button);

TextField createTextField(Window Window, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned long color, char *text, unsigned long textColor);
void setTextFieldX(Window Window, TextField textField, unsigned int x);
void setTextFieldY(Window Window, TextField textField, unsigned int y);
void setTextFieldWidth(Window Window, TextField textField, unsigned int width);
void setTextFieldHeight(Window Window, TextField textField, unsigned int height);
void setTextFieldColor(Window Window, TextField textField, unsigned long color);
void setTextFieldText(Window Window, TextField textField, char *text);
void setTextFieldTextColor(Window Window, TextField textField, unsigned long textColor);
unsigned int getTextFieldX(TextField textField);
unsigned int getTextFieldY(TextField textField);
unsigned int getTextFieldWidth(TextField textField);
unsigned int getTextFieldHeight(TextField textField);
unsigned long getTextFieldColor(TextField textField);
void getTextFieldText(TextField textField, char *destination);
unsigned long getTextFieldTextColor(TextField textField);
void destroyTextField(Window Window, TextField textField);