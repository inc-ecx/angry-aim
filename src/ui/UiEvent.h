#ifndef UI_EVENT_H
#define UI_EVENT_H

enum class UiEventType {
  MOUSE_CLICK, KEY
};

class UiEvent {
public:
  explicit UiEvent(UiEventType type)
    : type(type) {
  }

private:
  UiEventType type;
};

#endif //UI_EVENT_H
