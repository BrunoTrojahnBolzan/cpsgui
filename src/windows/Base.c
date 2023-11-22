#include <Windows.h>

#define STATIC_TEXT_COMPONENT 0
#define RECTANGLE_COMPONENT 1
#define LINE_COMPONENT 2
#define BUTTON_COMPONENT 3
#define TEXT_FIELD_COMPONENT 4

#define CLICK_EVENT 0
#define KEY_PRESS_EVENT 1

typedef struct _InternalWindow {
    HWND hwnd;
    int *buffer;
    int x, y;
    unsigned int width, height;
} *InternalWindow;

typedef struct _InternalEvent {
    int type;
    long long data;
} InternalEvent;

LRESULT WINAPI WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
  if(message == WM_DESTROY) {
    PostQuitMessage(wParam);
    ExitProcess(wParam);
    return 0;
  }
  if(message == WM_CLOSE) {
    SendMessageA(hwnd, WM_DESTROY, 0, 0);
    return 0;
  }
  return DefWindowProcA(hwnd, message, wParam, lParam);
}

InternalWindow createInternalWindow(char *title, unsigned int x, unsigned int y,
                                    unsigned int width, unsigned int height,
                                    unsigned long color,
                                    unsigned long *buffer) {
  InternalWindow internalWindow = malloc(sizeof(struct _InternalWindow));
  WNDCLASSA class = {0};
  class.hCursor = LoadCursor(NULL, IDC_ARROW);
  class.lpszClassName = "window";
  class.lpfnWndProc = &WinProc;
  RegisterClass(&class);
  HWND hwnd = CreateWindowA("window", title, WS_VISIBLE | WS_OVERLAPPEDWINDOW, x, y, width, height, NULL, NULL, NULL, NULL);
  internalWindow->x = x;
  internalWindow->y = y;
  internalWindow->width = width;
  internalWindow->height = height;
  internalWindow->hwnd = hwnd;
  internalWindow->buffer = (int*) buffer;
  return internalWindow;
}

void destroyInternalWindow(InternalWindow internalWindow) {
  DestroyWindow(internalWindow->hwnd);
  free(internalWindow);
}

void setInternalWindowTitle(InternalWindow internalWindow, char *title) {
  SetWindowTextA(internalWindow->hwnd, title);
}

void paintInternalWindow(InternalWindow internalWindow) {
  HDC windowDC = GetDC(internalWindow->hwnd);
  BITMAPINFO bmi;
  memset(&bmi, 0, sizeof(bmi));
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = internalWindow->width;
  bmi.bmiHeader.biHeight = internalWindow->height;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
  int *buffer = malloc(4 * internalWindow->height * internalWindow->width);
  for(int i = 0, k = internalWindow->height - 1; i < internalWindow->height; i++, k--) {
    for(int j = 0; j < internalWindow->width; j++) {
      buffer[k * internalWindow->width + j] = internalWindow->buffer[i * internalWindow->width + j];
    }
  }
  for(int i = 0; i < internalWindow->height; i++) {
    for(int j = 0; j < internalWindow->width; j++) {
      internalWindow->buffer[i * internalWindow->width + j] = buffer[i * internalWindow->width + j];
    }
  }
  free(buffer);
  SetDIBitsToDevice(windowDC, 0, 0, internalWindow->width, internalWindow->height, 0, 0, 0, internalWindow->height, internalWindow->buffer, &bmi, DIB_RGB_COLORS);
  ReleaseDC(internalWindow->hwnd, windowDC);
}

InternalEvent getInternalEvent(InternalWindow internalWindow) {
  InternalEvent internalEvent = {0};
  MSG message;
  while(GetMessageA(&message, 0, 0, 0)) {
    TranslateMessage(&message);
    switch(message.message) {
      case WM_CHAR:
        internalEvent.type = KEY_PRESS_EVENT;
        internalEvent.data = message.wParam;
        break;
      case WM_LBUTTONDOWN:
        internalEvent.type = CLICK_EVENT;
        internalEvent.data = (((long long) message.lParam) & 0xFFFF)<<32 | ((long long) message.lParam)>>16;
        break;
      default:
        DefWindowProcA(message.hwnd, message.message, message.wParam, message.lParam);
        continue;
    }
    return internalEvent;
  }
}