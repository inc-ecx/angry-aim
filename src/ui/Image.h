#ifndef IMAGE_H
#define IMAGE_H

#include "Ui.h"

class Image : public Ui {
public:
  int texture;

  void render(double dt, const UiRenderParams &params) override;

  static std::unique_ptr<Image> make(int texture);
};

#endif //IMAGE_H
