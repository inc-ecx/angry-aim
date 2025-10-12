#ifndef RENDER_UI_H
#define RENDER_UI_H

#include "../inc/inc_glfw.h"

class RenderUi {
public:
  void init();

  void start();

  void stop();

  void rect(int x, int y, int w, int h);

  void resize(int width, int height);
};

#endif //RENDER_UI_H
