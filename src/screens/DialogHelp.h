#ifndef DIALOG_HELP_H
#define DIALOG_HELP_H

#include "ui/components.h"

class DialogHelp : public Ui {
  std::shared_ptr<Ui> prev;

public:
  explicit DialogHelp(const std::shared_ptr<Ui> &prev);

  void handle(UiEvent &event) override;

};

#endif //DIALOG_HELP_H
