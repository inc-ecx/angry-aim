#ifndef SCREEN_TEST_H
#define SCREEN_TEST_H

#include "../ui/components.h"

class ScreenTest : public Ui {
public:
  explicit ScreenTest(const std::shared_ptr<Ui> &prev);

  void handle(UiEvent &event) override;

private:
  std::shared_ptr<Ui> prev;
  void actionBack();
};

#endif //SCREEN_TEST_H
