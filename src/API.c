#include "APIPrivate.h"
#include "Font.h"
#include "Base.h"
#include <stdlib.h>
#include <string.h>

void DrawText(char *text, unsigned long x, unsigned long y, unsigned long width,
              unsigned long height, unsigned long bufferWidth,
              unsigned long bufferHeight, unsigned long color, int *buffer);
void DrawRectangle(unsigned long x, unsigned long y, unsigned long width,
                   unsigned long height, unsigned long bufferWidth,
                   unsigned long bufferHeight, unsigned long color,
                   int *buffer);
void DrawLine(int x0, int y0, int x1, int y1, unsigned long bufferWidth,
              unsigned long bufferHeight, unsigned long color, int *buffer);
void DrawButton(unsigned long x, unsigned long y, unsigned long width,
                unsigned long height, unsigned long bufferWidth,
                unsigned long bufferHeight, unsigned long color, char *text,
                unsigned long textColor, unsigned long id, int *buffer);
void DrawTextField(unsigned long x, unsigned long y, unsigned long width,
                   unsigned long height, unsigned long bufferWidth,
                   unsigned long bufferHeight, unsigned long color, char *text,
                   unsigned long textColor, int *buffer);
void paintWindow(Window Window);
void addComponent(Window Window, Component component);
void removeComponent(Window Window, Component component);

Event getEvent(Window Window) {
  InternalEvent internalEvent = getInternalEvent(Window->internalWindow);
  Event event;
  event.Window = Window;
  event.type = internalEvent.type;
  event.data = internalEvent.data;
  switch (event.type) {
  case CLICK_EVENT: {
    Component *components = Window->components;
    int x = internalEvent.data >> 32;
    int y = internalEvent.data & 0xFFFFFFFF;
    char gotFocus = 0;
    if (components) {
      do {
        if (components->type == BUTTON_COMPONENT) {
          Button button = components->componentUnion.button;
          if (x >= button->x && x <= button->width + button->x &&
              y >= button->y && y <= button->y + button->height) {
            event.data = button->id;
            break;
          }
        }
        if (components->type == TEXT_FIELD_COMPONENT) {
          TextField textField = components->componentUnion.textField;
          if (x >= textField->x && x <= textField->width + textField->x &&
              y >= textField->y && y <= textField->y + textField->height) {
            Window->focus = components;
            gotFocus = 1;
            break;
          }
        }
        components = components->next;
      } while (components);
      if (!gotFocus) {
        Window->focus = NULL;
      }
    }
  } break;
  case KEY_PRESS_EVENT: {
    if (Window->focus) {
      TextField textField = Window->focus->componentUnion.textField;
      if (event.data != 8) {
        size_t size = strlen(textField->text);
        textField->text = realloc(textField->text, size + 1);
        textField->text[size] = (char)event.data;
        textField->text[size + 1] = '\0';
      } else {
        size_t size = strlen(textField->text);
        if (size) {
          textField->text[size - 1] = '\0';
          textField->text = realloc(textField->text, size);
        }
      }
    }
  } break;
  default:
    break;
  }
  paintWindow(Window);
  return event;
}

Window createWindow(char *title, unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height,
                       unsigned long color) {
  int *buffer = malloc(width * height * 4);
  InternalWindow internalWindow = createInternalWindow(
      title, x, y, width, height, color, (unsigned long *)buffer);
  for (int i = 0; i < width * height; i++) {
    buffer[i] = color;
  }
  Window Window = malloc(sizeof(struct _Window));
  Window->internalWindow = internalWindow;
  Window->buffer = buffer;
  Window->color = color;
  Window->x = x;
  Window->y = y;
  Window->width = width;
  Window->height = height;
  Window->components = NULL;
  Window->focus = NULL;
  return Window;
}

void destroyWindow(Window Window) {
  Component *components = Window->components;
  do {
    removeComponent(Window, *components);
    components = Window->components;
  } while(Window->components);
  destroyInternalWindow(Window->internalWindow);
  free(Window->buffer);
  free(Window);
}

void setWindowTitle(Window Window, char *title) {
  setInternalWindowTitle(Window->internalWindow, title);
}

void DrawText(char *text, unsigned long x, unsigned long y, unsigned long width,
              unsigned long height, unsigned long bufferWidth,
              unsigned long bufferHeight, unsigned long color, int *buffer) {
  int currentCharacter = 0;
  int curX = x;
  int curY = y;
  for (int i = 0; i < strlen(text); i++) {
    for (int j = 0; j < 16; j++) {
      for (int k = text[currentCharacter] * 10;
           k < text[currentCharacter] * 10 + 10; k++) {
        if (fontData[j * 2560 + k] == 1 && curY <= y + height &&
            curX <= x + width) {
          buffer[curX + bufferWidth * curY] = color;
        }
        curX++;
      }
      curX -= 10;
      curY++;
    }
    curX += 10;
    curY = y;
    currentCharacter++;
  }
}

void paintWindow(Window Window) {
  for (int i = 0; i < Window->width * Window->height; i++) {
    Window->buffer[i] = Window->color;
  }
  Component *component = Window->components;
  if (component) {
    do {
      switch (component->type) {
      case STATIC_TEXT_COMPONENT: {
        StaticText st = component->componentUnion.staticText;
        DrawText(st->text, st->x, st->y, st->width, st->height,
                 Window->width, Window->height, st->color,
                 Window->buffer);
      } break;
      case RECTANGLE_COMPONENT: {
        Rectangle rect = component->componentUnion.Rectangle;
        DrawRectangle(rect->x, rect->y, rect->width, rect->height,
                      Window->width, Window->height, rect->color,
                      Window->buffer);
      } break;
      case LINE_COMPONENT: {
        Line line = component->componentUnion.line;
        DrawLine(line->x0, line->y0, line->x1, line->y1, Window->width,
                 Window->height, line->color, Window->buffer);
      } break;
      case BUTTON_COMPONENT: {
        Button button = component->componentUnion.button;
        DrawButton(button->x, button->y, button->width, button->height,
                   Window->width, Window->height, button->color,
                   button->text, button->textColor, button->id,
                   Window->buffer);
      } break;
      case TEXT_FIELD_COMPONENT: {
        TextField textField = component->componentUnion.textField;
        DrawTextField(textField->x, textField->y, textField->width,
                      textField->height, Window->width, Window->height,
                      textField->color, textField->text, textField->textColor,
                      Window->buffer);
      } break;
      default:
        break;
      }
      component = component->next;
    } while (component);
  }
  paintInternalWindow(Window->internalWindow);
}

void addComponent(Window Window, Component component) {
  Component *allocatedComponent = malloc(sizeof(Component));
  allocatedComponent->componentUnion = component.componentUnion;
  allocatedComponent->type = component.type;
  allocatedComponent->next = NULL;
  if (!Window->components) {
    Window->components = allocatedComponent;
  } else {
    Component *components = Window->components;
    while (components->next) {
      components = components->next;
    }
    components->next = allocatedComponent;
  }
}

void removeComponent(Window Window, Component component) {
  Component *components = Window->components;
  if (components) {
    Component *previous = NULL;
    do {
      switch (components->type) {
        case STATIC_TEXT_COMPONENT:
          if(components->componentUnion.staticText == component.componentUnion.staticText){
            free(components->componentUnion.staticText->text);
            if(previous) {
              previous->next = components->next;
            }
            free(component.componentUnion.staticText);
            if(Window->components == components) {
              if(Window->components->next == NULL){
                Window->components = NULL;
              } else {
                Window->components = Window->components->next;
              }
            }
            free(components);
            components = NULL;
          }
          break;
        case RECTANGLE_COMPONENT:
          if(components->componentUnion.Rectangle == component.componentUnion.Rectangle){
            if(previous) {
              previous->next = components->next;
            }
            free(component.componentUnion.Rectangle);
            if(Window->components == components) {
              if(Window->components->next == NULL){
                Window->components = NULL;
              } else {
                Window->components = Window->components->next;
              }
            }
            free(components);
            components = NULL;
          }
          break;
        case LINE_COMPONENT:
          if(components->componentUnion.line == component.componentUnion.line){
            if(previous) {
              previous->next = components->next;
            }
            free(component.componentUnion.line);
            if(Window->components == components) {
              if(Window->components->next == NULL){
                Window->components = NULL;
              } else {
                Window->components = Window->components->next;
              }
            }
            free(components);
            components = NULL;
          }
          break;
        case BUTTON_COMPONENT:
          if(components->componentUnion.button == component.componentUnion.button){
            free(components->componentUnion.button->text);
            if(previous) {
              previous->next = components->next;
            }
            free(component.componentUnion.button);
            if(Window->components == components) {
              if(Window->components->next == NULL){
                Window->components = NULL;
              } else {
                Window->components = Window->components->next;
              }
            }
            free(components);
            components = NULL;
          }
          break;
        case TEXT_FIELD_COMPONENT:
          if(components->componentUnion.textField == component.componentUnion.textField){
            if(Window->focus && Window->focus->componentUnion.textField == component.componentUnion.textField) {
              Window->focus = NULL;
            }
            free(components->componentUnion.textField->text);
            if(previous) {
              previous->next = components->next;

            }
            free(component.componentUnion.textField);
            if(Window->components == components) {
              if(Window->components->next == NULL){
                Window->components = NULL;
              } else {
                Window->components = Window->components->next;
              }
            }
            free(components);
            components = NULL;
          }
          break;
        default:
          break;
      }
      previous = components;
      if(components) {
        components = components->next;
      }
    } while (components);
    paintWindow(Window);
  }
}

void DrawRectangle(unsigned long x, unsigned long y, unsigned long width,
                   unsigned long height, unsigned long bufferWidth,
                   unsigned long bufferHeight, unsigned long color,
                   int *buffer) {
  for (int i = 0; i < height && i + y < bufferHeight; i++) {
    for (int j = 0; j < width && j + x < bufferWidth; j++) {
      buffer[(i + y) * bufferWidth + (j + x)] = color;
    }
  }
}

void DrawLine(int x0, int y0, int x1, int y1, unsigned long bufferWidth,
              unsigned long bufferHeight, unsigned long color, int *buffer) {
  int rise = y1 - y0;
  int run = x1 - x0;
  if (run == 0) {
    if (y1 < y0) {
      int aux = y0;
      y0 = y1;
      y1 = y0;
    }
    for (int y = 0; y <= y1; y++) {
      buffer[y * bufferWidth + x1] = color;
    }
  } else {
    float m = (float)rise / run;
    int adjust = m >= 0 ? 1 : -1;
    int offset = 0;
    if (m <= 1 && m >= -1) {
      int delta = abs(rise) * 2;
      int threshold = abs(run);
      int thresholdInc = abs(run) * 2;
      int y = y0;
      if (x1 < x0) {
        int aux = x0;
        x0 = x1;
        x1 = aux;
        y = y1;
      }
      for (int x = x0; x <= x1; x++) {
        if (x < bufferWidth && y < bufferHeight)
          buffer[y * bufferWidth + x] = color;
        offset += delta;
        if (offset >= threshold) {
          y += adjust;
          threshold += thresholdInc;
        }
      }
    } else {
      int delta = abs(run) * 2;
      int threshold = abs(rise);
      int thresholdInc = abs(rise) * 2;
      int x = x0;
      if (y1 < y0) {
        int aux = y0;
        y0 = y1;
        y1 = aux;
        x = x1;
      }
      for (int y = y0; y <= y1; y++) {
        if (x < bufferWidth && y < bufferHeight)
          buffer[y * bufferWidth + x] = color;
        offset += delta;
        if (offset >= threshold) {
          x += adjust;
          threshold += thresholdInc;
        }
      }
    }
  }
}

void DrawButton(unsigned long x, unsigned long y, unsigned long width,
                unsigned long height, unsigned long bufferWidth,
                unsigned long bufferHeight, unsigned long color, char *text,
                unsigned long textColor, unsigned long id, int *buffer) {
  DrawRectangle(x, y, width, height, bufferWidth, bufferHeight, color, buffer);
  DrawText(text, x + width / 2 - strlen(text) * 10 / 2, y + height / 2 - 8,
           width, height, bufferWidth, bufferHeight, textColor, buffer);
}

void DrawTextField(unsigned long x, unsigned long y, unsigned long width,
                   unsigned long height, unsigned long bufferWidth,
                   unsigned long bufferHeight, unsigned long color, char *text,
                   unsigned long textColor, int *buffer) {
  DrawRectangle(x, y, width, height, bufferWidth, bufferHeight, color, buffer);
  DrawText(text, x + 2, y + height / 2 - 8, width, height, bufferWidth,
           bufferHeight, textColor, buffer);
}