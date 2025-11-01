#include "Shader.h"

#include <format>

#include "Log.h"

Shader::Shader(const char *vertexSource, const char *fragmentSource) {
  vertId = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertId, 1, &vertexSource, nullptr);
  glCompileShader(vertId);

  GLint success;
  int len;

  glGetShaderiv(vertId, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderiv(vertId, GL_INFO_LOG_LENGTH, &len);
    std::string infoLog(len, '\0');
    glGetShaderInfoLog(fragId, len, nullptr, infoLog.data());
    Log::error(std::format("VERTEX SHADER COMPILATION FAILED:\n {}", infoLog));
  }
  fragId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragId, 1, &fragmentSource, nullptr);
  glCompileShader(fragId);

  glGetShaderiv(fragId, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderiv(fragId, GL_INFO_LOG_LENGTH, &len);
    std::string infoLog(len, '\0');
    glGetShaderInfoLog(fragId, len, nullptr, infoLog.data());
    Log::error(std::format("FRAGMENT SHADER COMPILATION FAILED:\n {}", infoLog));
  }

  programId = glCreateProgram();

  glAttachShader(programId, vertId);
  glAttachShader(programId, fragId);
  glLinkProgram(programId);

  glGetProgramiv(programId, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &len);
    std::string infoLog(len, '\0');
    glGetProgramInfoLog(programId, len, nullptr, infoLog.data());
    Log::error(std::format("PROGRAM LINK FAILED:\n {}", infoLog));
  }
}

Shader::~Shader() {
  if (programId != 0) {
    glDetachShader(programId, vertId);
    glDetachShader(programId, fragId);
    glDeleteShader(vertId);
    glDeleteShader(fragId);
    glDeleteProgram(programId);
  }
}
