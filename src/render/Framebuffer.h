#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "inc/inc_glfw.h"

class Framebuffer {
public:
  GLuint id = 0;
  GLuint textureId = 0;
  GLuint depthBufferId = 0;
  GLint prevId = -1;

  Framebuffer();
  ~Framebuffer();

  Framebuffer(const Framebuffer &other) = delete;

  Framebuffer(Framebuffer &&other) noexcept :
    id(other.id),
    textureId(other.textureId),
    depthBufferId(other.depthBufferId),
    prevId(other.prevId) {}

  Framebuffer & operator=(const Framebuffer &other) = delete;

  Framebuffer & operator=(Framebuffer &&other) noexcept {
    if (this == &other)
      return *this;
    id = other.id;
    textureId = other.textureId;
    depthBufferId = other.depthBufferId;
    prevId = other.prevId;
    return *this;
  }

  void bind();
  void unbind();
};

#endif //FRAMEBUFFER_H
