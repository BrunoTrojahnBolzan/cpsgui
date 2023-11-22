#include "../APIPrivate.h"
#include <stdlib.h>

Line createLine(Window Window, int x0, int y0, int x1, int y1, unsigned long color) {
    Component component;
    ComponentUnion componentUnion;
    Line line = malloc(sizeof(struct _Line));
    line->x0 = x0;
    line->y0 = y0;
    line->x1 = x1;
    line->y1 = y1;
    line->color = color;
    component.type = LINE_COMPONENT;
    componentUnion.line = line;
    component.componentUnion = componentUnion;
    addComponent(Window, component);
    paintWindow(Window);
    return line;
}

void setLineX0(Window Window, Line line, int x0) {
    line->x0 = x0;
    paintWindow(Window);
}

void setLineY0(Window Window, Line line, int y0) {
    line->y0 = y0;
    paintWindow(Window);
}

void setLineX1(Window Window, Line line, int x1) {
    line->x1 = x1;
    paintWindow(Window);
}

void setLineY1(Window Window, Line line, int y1) {
    line->y1 = y1;
    paintWindow(Window);
}

void setLineColor(Window Window, Line line, unsigned long color) {
    line->color = color;
    paintWindow(Window);
}

int getLineX0(Line line) {
    return line->x0;
}

int getLineY0(Line line) {
    return line->y0;
}

int getLineX1(Line line) {
    return line->x1;
}

int getLineY1(Line line) {
    return line->y1;
}

unsigned long getLineColor(Line line) {
    return line->color;
}

void destroyLine(Window Window, Line line) {
    Component component;
    component.type = LINE_COMPONENT;
    component.componentUnion.line = line;
    component.next = NULL;
    removeComponent(Window, component);
}