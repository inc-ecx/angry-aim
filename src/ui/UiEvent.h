#ifndef UI_EVENT_H
#define UI_EVENT_H

#include <limits>

enum class UiEventType {
  MOUSE_MOVE, // mx,my,mdx,mdy
  MOUSE_BUTTON, // down,button
  SCROLL, // sx,sy
  KEY, // down,button
  CHAR // codepoint
};

class UiEvent {
public:
  explicit UiEvent(UiEventType type, bool down, int button) :
    type(type),
    down(down),
    button(button),
    codepoint(std::numeric_limits<int>::min()),
    mx(std::numeric_limits<int>::min()),
    my(std::numeric_limits<int>::min()),
    mdx(std::numeric_limits<int>::min()),
    mdy(std::numeric_limits<int>::min()),
    sx(std::numeric_limits<double>::signaling_NaN()),
    sy(std::numeric_limits<double>::signaling_NaN()) {}

  explicit UiEvent(int codepoint) :
    type(UiEventType::CHAR),
    down(false),
    button(std::numeric_limits<int>::min()),
    codepoint(codepoint),
    mx(std::numeric_limits<int>::min()),
    my(std::numeric_limits<int>::min()),
    mdx(std::numeric_limits<int>::min()),
    mdy(std::numeric_limits<int>::min()),
    sx(std::numeric_limits<double>::signaling_NaN()),
    sy(std::numeric_limits<double>::signaling_NaN()),
    claimed(false) {}

  explicit UiEvent(int mx, int my, int mdx, int mdy) :
    type(UiEventType::MOUSE_MOVE),
    down(false),
    button(std::numeric_limits<int>::min()),
    codepoint(std::numeric_limits<int>::min()),
    mx(mx),
    my(my),
    mdx(mdx),
    mdy(mdy),
    sx(std::numeric_limits<double>::signaling_NaN()),
    sy(std::numeric_limits<double>::signaling_NaN()) {}

  explicit UiEvent(double sx, double sy) :
    type(UiEventType::SCROLL),
    down(false),
    button(std::numeric_limits<int>::min()),
    codepoint(std::numeric_limits<int>::min()),
    mx(std::numeric_limits<int>::min()),
    my(std::numeric_limits<int>::min()),
    mdx(std::numeric_limits<int>::min()),
    mdy(std::numeric_limits<int>::min()),
    sx(sx),
    sy(sy) {}

  const UiEventType type;
  const bool down;
  const int button;
  const int codepoint;
  const int mx;
  const int my;
  const int mdx;
  const int mdy;
  const double sx;
  const double sy;

  bool claimed = false;
};

#endif //UI_EVENT_H
