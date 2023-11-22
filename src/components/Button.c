#include "../APIPrivate.h"
#include <stdlib.h>
#include <string.h>

Button createButton(Window Window, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned long color, char *text, unsigned long textColor, unsigned long id) {
    Component component;
    ComponentUnion componentUnion;
    Button button = malloc(sizeof(struct _Button));
    button->x = x;
    button->y = y;
    button->width = width;
    button->height = height;
    button->text = malloc(strlen(text));
    strcpy(button->text, text);
    button->color = color;
    button->textColor = textColor;
    button->id = id;
    component.type = BUTTON_COMPONENT;
    componentUnion.button = button;
    component.componentUnion = componentUnion;
    addComponent(Window, component);
    paintWindow(Window);
    return button;
}

void setButtonX(Window Window, Button button, unsigned int x) {
    button->x = x;
    paintWindow(Window);
}

void setButtonY(Window Window, Button button, unsigned int y) {
    button->y = y;
    paintWindow(Window);
}

void setButtonWidth(Window Window, Button button, unsigned int width) {
    button->width = width;
    paintWindow(Window);
}

void setButtonHeight(Window Window, Button button, unsigned int height) {
    button->height = height;
    paintWindow(Window);
}

void setButtonColor(Window Window, Button button, unsigned long color) {
    button->color = color;
    paintWindow(Window);
}

void setButtonText(Window Window, Button button, char *text) {
    free(button->text);
    button->text = malloc(strlen(text));
    strcpy(button->text, text);
    paintWindow(Window);
}

void setButtonTextColor(Window Window, Button button, unsigned long textColor) {
    button->textColor = textColor;
    paintWindow(Window);
}

unsigned int getButtonX(Button button) {
    return button->x;
}

unsigned int getButtonY(Button button) {
    return button->y;
}

unsigned int getButtonWidth(Button button) {
    return button->width;
}

unsigned int getButtonHeight(Button button) {
    return button->height;
}

unsigned long getButtonColor(Button button) {
    return button->color;
}

void getButtonText(Button button, char *destination) {
    strcpy(destination, button->text);
}

unsigned long getButtonTextColor(Button button) {
    return button->textColor;
}

void destroyButton(Window Window, Button button) {
    Component component;
    component.type = BUTTON_COMPONENT;
    component.componentUnion.button = button;
    component.next = NULL;
    removeComponent(Window, component);
}