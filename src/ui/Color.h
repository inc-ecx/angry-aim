#ifndef COLOR_H
#define COLOR_H

#include "Field.h"
#include "Row.h"
#include "Column.h"

//
//
//

class ColorPreview : public Ui {
  int rgba = 0;

public:
  void render(double dt, const UiRenderParams &params) override;

  void setColor(int rgba);
  int getColor();
};

//
//
//

class Color : public Ui {
  struct Token {};

  std::function<void(int rgba)> listenerChanged;

  void onChanged();

  void actionTextChanged();

  void actionTextSubmit();

public:
  std::shared_ptr<ColorPreview> preview;
  std::shared_ptr<Field> field;

  explicit Color(Token);

  int getRgba();

  void setRgba(int rgba);

  static std::unique_ptr<Color> make(int rgba, const std::function<void(int rgba)> &listener) {
    auto res = std::make_unique<Color>(Token());
    res->setRgba(rgba);
    res->listenerChanged = listener;
    return res;
  }
};

#endif //COLOR_H
