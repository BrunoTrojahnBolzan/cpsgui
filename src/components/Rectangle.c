#include "../APIPrivate.h"
#include <stdlib.h>

Rectangle createRectangle(Window Window, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned long color) {
    Component component;
    ComponentUnion componentUnion;
    Rectangle Rectangle = malloc(sizeof(struct _Rectangle));
    Rectangle->x = x;
    Rectangle->y = y;
    Rectangle->width = width;
    Rectangle->height = height;
    Rectangle->color = color;
    component.type = RECTANGLE_COMPONENT;
    componentUnion.Rectangle = Rectangle;
    component.componentUnion = componentUnion;
    addComponent(Window, component);
    paintWindow(Window);
    return Rectangle;
}

void setRectangleX(Window Window, Rectangle Rectangle, unsigned int x) {
    Rectangle->x = x;
    paintWindow(Window);
}

void setRectangleY(Window Window, Rectangle Rectangle, unsigned int y) {
    Rectangle->y = y;
    paintWindow(Window);
}

void setRectangleWidth(Window Window, Rectangle Rectangle, unsigned int width) {
    Rectangle->width = width;
    paintWindow(Window);
}

void setRectangleHeight(Window Window, Rectangle Rectangle, unsigned int height) {
    Rectangle->height = height;
    paintWindow(Window);
}

void setRectangleColor(Window Window, Rectangle Rectangle, unsigned long color) {
    Rectangle->color = color;
    paintWindow(Window);
}

unsigned int getRectangleX(Rectangle Rectangle) {
    return Rectangle->x;
}

unsigned int getRectangleY(Rectangle Rectangle) {
    return Rectangle->y;
}

unsigned int getRectangleWidth(Rectangle Rectangle) {
    return Rectangle->width;
}

unsigned int getRectangleHeight(Rectangle Rectangle) {
    return Rectangle->height;
}

unsigned long getRectangleColor(Rectangle Rectangle) {
    return Rectangle->color;
}

void destroyRectangle(Window Window, Rectangle Rectangle) {
    Component component;
    component.type = RECTANGLE_COMPONENT;
    component.componentUnion.Rectangle = Rectangle;
    component.next = NULL;
    removeComponent(Window, component);
}