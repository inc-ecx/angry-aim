#ifndef FIELD_H
#define FIELD_H

#include <functional>

#include "Ui.h"

class Field : public Ui {
  int max = 1024;

  uint64_t msInteracted = 0;
  std::vector<char> textData;
  int cursor = -1;
  int select = -1;

  std::function<void()> listenerChanged;
  std::function<void()> listenerSubmit;

  // state

  bool focused() { return cursor >= 0; }

  bool selected() { return focused() && select >= 0 && select != cursor; }

  // internal events
  void onSubmit();

  void onTextChanged();

  void handleClick(UiEvent &event);

  void handleKeyDown(UiEvent &event);

  void handleChar(UiEvent &event);

  // util

  bool deleteSelected();

  bool exitSelection();

  int getNextCursor(int direction);

  bool allowChar(char c);

public:
  static std::shared_ptr<Field> make(
    std::string value,
    const std::function<void()> &listenerChanged,
    const std::function<void()> &listenerSubmit
  ) {
    auto r = std::make_shared<Field>();
    r->textData = std::vector(value.begin(), value.end());
    r->listenerChanged = listenerChanged;
    r->listenerSubmit = listenerSubmit;
    return r;
  }

  void handle(UiEvent &event) override;

  void render(double dt, const UiRenderParams& params) override;

  void distract();

  //
  // accessors
  //

  void set(const std::string &value);

  void setSilently(const std::string &value);

  std::string text() { return std::string(textData.begin(), textData.end()); }
};

#endif //FIELD_H
