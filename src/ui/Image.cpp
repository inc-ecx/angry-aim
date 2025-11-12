#include "Image.h"

#include "Application.h"
#include "inc/inc_glfw.h"

void Image::render(double dt, const UiRenderParams &params) {
  auto &app = Application::app;
  auto &render = app.renderUi;

  render.start();
  render.color(0xffffffff);
  render.texture(texture);
  if (params.toBuffer)
    glBlendFunc(GL_ONE, GL_ZERO);
  render.rect(x, y, width, height);
  if (params.toBuffer)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  render.texture(0);
  render.stop();
}

std::unique_ptr<Image> Image::make(int texture) {
  auto res = std::make_unique<Image>();
  res->texture = texture;
  return res;
}
