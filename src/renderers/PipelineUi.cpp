#include "PipelineUi.h"

#include "inc/inc_glfw.h"

#include "Application.h"
#include "Log.h"

void PipelineUi::init() {
  fb1 = std::make_unique<Framebuffer>();
}

void PipelineUi::render(double dt) {
  auto &app = Application::app;
  auto &renderUi = app.renderUi;

  int blendInMs = 200;
  int dimMs = 200;

  int uiMainAlpha = 0xff;
  int uiLastAlpha = 0xff;
  int bgAlpha = 0x60;

  double blendInProgress = static_cast<double>(msCurrent() - msBlendIn) / blendInMs;
  if (blendInProgress < 1) uiMainAlpha = static_cast<int>(uiMainAlpha * blendInProgress);
  if (blendInProgress < 1) uiLastAlpha = static_cast<int>(uiLastAlpha * (1 - blendInProgress));

  double dimProgress = static_cast<double>(msCurrent() - msDim) / dimMs;
  if (dimProgress < 1) bgAlpha = static_cast<int>(bgAlpha * dimProgress);

  if (blendInProgress < 1 && uiLast != nullptr) {
    fb1->bind();
    glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    uiLast->render(dt, {.toBuffer = true});
    fb1->unbind();
    renderUi.start();
    renderUi.color(0xffffff00 | uiLastAlpha);
    renderUi.texture(fb1->textureId);
    renderUi.rect(0, 0, app.getWidth(), app.getHeight());
    renderUi.texture(0);
    renderUi.stop();
  }

  if (uiMain != nullptr) {
    if (app.getScene() != nullptr) {
      renderUi.start();
      renderUi.color(0x00000000 | bgAlpha);
      renderUi.rect(0, 0, app.getWidth(), app.getHeight());
      renderUi.stop();
    }

    fb1->bind();
    glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    uiMain->render(dt, {.toBuffer = true});
    fb1->unbind();
    renderUi.start();
    renderUi.color(0xffffff00 | uiMainAlpha);
    renderUi.texture(fb1->textureId);
    renderUi.rect(0, 0, app.getWidth(), app.getHeight());
    renderUi.texture(0);
    renderUi.stop();
  }
}

void PipelineUi::setMain(const std::shared_ptr<Ui> &ui) {
  uiLast = uiMain;
  uiMain = ui;
  msBlendIn = msCurrent();

  auto &app = Application::app;
  if (uiLast == nullptr && app.getScene() != nullptr) {
    msDim = msCurrent();
  }
}

void PipelineUi::resize(int width, int height) {
  if (fb1 != nullptr) fb1 = std::make_unique<Framebuffer>();
}
