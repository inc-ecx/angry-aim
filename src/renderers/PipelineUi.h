#ifndef PIPELINE_UI_H
#define PIPELINE_UI_H
#include <memory>

#include "inc/inc_glfw.h"
#include "render/Framebuffer.h"
#include "ui/Ui.h"

class PipelineUi {
  std::shared_ptr<Ui> uiMain;
  std::shared_ptr<Ui> uiLast;
  uint64_t msBlendIn = 0;
  uint64_t msDim = 0;

  std::shared_ptr<Framebuffer> fb1;

public:
  void init();
  void render(double dt);
  void setMain(const std::shared_ptr<Ui> &ui);
  void resize(int width, int height);
};



#endif //PIPELINE_UI_H
