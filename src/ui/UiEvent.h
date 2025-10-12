#ifndef UI_EVENT_H
#define UI_EVENT_H

enum class UiEventType {
  MOUSE_BUTTON, KEY
};

class UiEvent {
public:
  explicit UiEvent(UiEventType type, bool down, int button)
    : type(type), down(down), button(button) {
  }

  const UiEventType type;
  const bool down;
  const int button;

  bool claimed = false;
};

#endif //UI_EVENT_H
