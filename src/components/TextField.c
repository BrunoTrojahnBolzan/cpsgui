#include "../APIPrivate.h"
#include <stdlib.h>
#include <string.h>

TextField createTextField(Window Window, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned long color, char *text, unsigned long textColor) {
    Component component;
    ComponentUnion componentUnion;
    TextField textField = malloc(sizeof(struct _TextField));
    textField->x = x;
    textField->y = y;
    textField->width = width;
    textField->height = height;
    textField->text = malloc(strlen(text));
    strcpy(textField->text, text);
    textField->color = color;
    textField->textColor = textColor;
    component.type = TEXT_FIELD_COMPONENT;
    componentUnion.textField = textField;
    component.componentUnion = componentUnion;
    addComponent(Window, component);
    paintWindow(Window);
    return textField;
}

void setTextFieldX(Window Window, TextField textField, unsigned int x) {
    textField->x = x;
    paintWindow(Window);
}

void setTextFieldY(Window Window, TextField textField, unsigned int y) {
    textField->y = y;
    paintWindow(Window);
}

void setTextFieldWidth(Window Window, TextField textField, unsigned int width) {
    textField->width = width;
    paintWindow(Window);
}

void setTextFieldHeight(Window Window, TextField textField, unsigned int height) {
    textField->height = height;
    paintWindow(Window);
}

void setTextFieldColor(Window Window, TextField textField, unsigned long color) {
    textField->color = color;
    paintWindow(Window);
}

void setTextFieldText(Window Window, TextField textField, char *text) {
    free(textField->text);
    textField->text = malloc(strlen(text));
    strcpy(textField->text, text);
    paintWindow(Window);
}

void setTextFieldTextColor(Window Window, TextField textField, unsigned long textColor) {
    textField->textColor = textColor;
    paintWindow(Window);
}

unsigned int getTextFieldX(TextField textField) {
    return textField->x;
}

unsigned int getTextFieldY(TextField textField) {
    return textField->y;
}

unsigned int getTextFieldWidth(TextField textField) {
    return textField->width;
}

unsigned int getTextFieldHeight(TextField textField) {
    return textField->height;
}

unsigned long getTextFieldColor(TextField textField) {
    return textField->color;
}

void getTextFieldText(TextField textField, char *destination) {
    strcpy(destination, textField->text);
}

unsigned long getTextFieldTextColor(TextField textField) {
    return textField->textColor;
}

void destroyTextField(Window Window, TextField textField) {
    Component component;
    component.type = TEXT_FIELD_COMPONENT;
    component.componentUnion.textField = textField;
    component.next = NULL;
    removeComponent(Window, component);
}