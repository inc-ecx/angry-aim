#include "Color.h"

#include "Application.h"

//
// Color Preview Class
//
void ColorPreview::render(double dt, const UiRenderParams &params) {
  auto &app = Application::app;
  auto &render = app.renderUi;

  int inset = 3;

  render.start();

  if (params.toBuffer) glBlendFunc(GL_ONE, GL_ZERO);

  render.color(0x000000ff);
  render.rect(x, y, width, height, 3, 1.0);

  if (params.toBuffer) glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  render.color(rgba);
  render.rect(x+inset, y + inset, width - inset * 2, height - inset * 2, 3, 1.0);

  render.stop();
}

void ColorPreview::setColor(int rgba) { this->rgba = rgba; }

int ColorPreview::getColor() { return rgba; }

//
// Color Class
//

// @formatter:off
Color::Color(Token) {
  add(Row::make({
    Cell::abs(preview = std::make_shared<ColorPreview>(), 25),
    Cell::abs(10),
    Cell::rel(field = Field::make(
      "",
      std::bind(&Color::actionTextChanged, this),
      std::bind(&Color::actionTextSubmit, this),
      [](char c) {
        return c >= '0' && c <= '9' || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F';
      }
    )),
  }));
  field->set("80ffffff");
}
// @formatter:on

void Color::onChanged() {
  if (listenerChanged) listenerChanged(preview->getColor());
}

int Color::getRgba() {
  return preview->getColor();
}

void Color::setRgba(int rgba) {
  preview->setColor(rgba);
  field->setSilently(std::format("{:0>8x}", static_cast<uint32_t>(rgba)));
}

void Color::actionTextChanged() {
  std::string text = field->text();

  std::stringstream ss;
  ss << std::hex << text;
  uint32_t rgba;
  if (ss >> rgba) {
    preview->setColor(static_cast<int>(rgba));
    onChanged();
  }
}

void Color::actionTextSubmit() {}
