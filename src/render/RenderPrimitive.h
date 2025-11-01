#ifndef RENDER_PRIMITIVE_H
#define RENDER_PRIMITIVE_H

#include "inc/inc_glfw.h"
#include "render/Shader.h"

class RenderPrimitive {
  GLuint pointVao = 0;

  Shader shader;

public:
  void init();

  void start();

  void stop();

  void resize(int width, int height);

  void updateView(glm::vec3 pos, float pitch, float yaw, glm::vec2 pxOffset);

  void updateProjection();

  void updateModel(glm::vec3 pos, float scale);

  void color(int rgba);

  void drawPoint();
};

#endif //RENDER_PRIMITIVE_H
