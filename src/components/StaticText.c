#include "../APIPrivate.h"
#include <stdlib.h>
#include <string.h>

StaticText createStaticText(Window Window, unsigned int x, unsigned int y,
                            unsigned int width, unsigned int height,
                            unsigned long color, char *text) {
  char *allocatedText = malloc(strlen(text));
  strcpy(allocatedText, text);
  Component component;
  ComponentUnion componentUnion;
  StaticText staticText = malloc(sizeof(struct _StaticText));
  staticText->text = allocatedText;
  staticText->height = height;
  staticText->width = width;
  staticText->x = x;
  staticText->y = y;
  staticText->color = color;
  component.type = STATIC_TEXT_COMPONENT;
  componentUnion.staticText = staticText;
  component.componentUnion = componentUnion;
  addComponent(Window, component);
  paintWindow(Window);
  return staticText;
}

void setStaticText(Window Window, StaticText staticText, char *title) {
    free(staticText->text);
    staticText->text = malloc(strlen(title));
    strcpy(staticText->text, title);
    paintWindow(Window);
}

void getStaticText(StaticText staticText, char *destination) {
    strcpy(destination, staticText->text);
}

void setStaticTextX(Window Window, StaticText staticText, unsigned int x) {
    staticText->x = x;
    paintWindow(Window);
}

void setStaticTextY(Window Window, StaticText staticText, unsigned int y) {
    staticText->y = y;
    paintWindow(Window);
}

void setStaticTextWidth(Window Window, StaticText staticText, unsigned int width) {
    staticText->width = width;
    paintWindow(Window);
}

void setStaticTextHeight(Window Window, StaticText staticText, unsigned int height) {
    staticText->height = height;
    paintWindow(Window);
}

void setStaticTextColor(Window Window, StaticText staticText, unsigned long color) {
    staticText->color = color;
    paintWindow(Window);
}

unsigned int getStaticTextX(StaticText staticText) {
    return staticText->x;
}

unsigned int getStaticTextY(StaticText staticText) {
    return staticText->y;
}

unsigned int getStaticTextWidth(StaticText staticText) {
    return staticText->width;
}

unsigned int getStaticTextHeight(StaticText staticText) {
    return staticText->height;
}

unsigned long getStaticTextColor(StaticText staticText) {
    return staticText->color;
}

void destroyStaticText(Window Window, StaticText staticText) {
    Component component;
    component.type = STATIC_TEXT_COMPONENT;
    component.componentUnion.staticText = staticText;
    component.next = NULL;
    removeComponent(Window, component);
}