#ifndef UI_EVENT_H
#define UI_EVENT_H

#include <limits>

enum class UiEventType {
  MOUSE_MOVE, MOUSE_BUTTON, KEY
};

class UiEvent {
public:
  explicit UiEvent(UiEventType type, bool down, int button)
    : type(type),
      down(down),
      button(button),
      mx(std::numeric_limits<int>::min()),
      my(std::numeric_limits<int>::min()),
      mdx(std::numeric_limits<int>::min()),
      mdy(std::numeric_limits<int>::min()) {
  }

  explicit UiEvent(int mx, int my, int mdx, int mdy)
    : type(UiEventType::MOUSE_MOVE), down(false), button(0), mx(mx), my(my), mdx(mdx), mdy(mdy) {
  }

  const UiEventType type;
  const bool down;
  const int button;
  int mx;
  int my;
  int mdx;
  int mdy;

  bool claimed = false;
};

#endif //UI_EVENT_H
