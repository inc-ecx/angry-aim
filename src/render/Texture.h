#ifndef TEXTURE_H
#define TEXTURE_H

#include "inc/inc_glfw.h"
#include <memory>

class Texture {
public:
  int width;
  int height;
  GLuint id;

  Texture() = default;

  ~Texture();

  Texture(const Texture &other) = delete;

  Texture(Texture &&other) = delete;

  Texture &operator=(const Texture &other) = delete;

  Texture &operator=(Texture &&other) = delete;

  static std::unique_ptr<Texture> load(const std::string &path);
};

#endif //TEXTURE_H
