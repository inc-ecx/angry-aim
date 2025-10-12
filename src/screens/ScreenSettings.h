#ifndef SCREEN_SETTINGS_H
#define SCREEN_SETTINGS_H

#include "../ui/components.h"

class ScreenSettings : public Ui {
public:
  explicit ScreenSettings(const std::shared_ptr<Ui> &prev);
private:
  std::shared_ptr<Ui> prev;
  void actionBack();
};

#endif //SCREEN_SETTINGS_H
