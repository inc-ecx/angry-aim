#ifndef SHADER_H
#define SHADER_H

#include "inc/inc_glfw.h"
#include <string>

class Shader {
public:
  GLuint programId = 0;
  GLuint vertId = 0;
  GLuint fragId = 0;

  Shader() = default;
  Shader(const char *vertexSource, const char *fragmentSource);

  ~Shader();

  Shader(const Shader &other) = delete;

  Shader(Shader &&other) noexcept :
    programId(other.programId),
    vertId(other.vertId),
    fragId(other.fragId) {
    other.programId = 0;
  }

  Shader & operator=(const Shader &other) = delete;

  Shader & operator=(Shader &&other) noexcept {
    if (this == &other)
      return *this;
    programId = other.programId;
    vertId = other.vertId;
    fragId = other.fragId;
    other.programId = 0;
    return *this;
  }

};

#endif //SHADER_H
