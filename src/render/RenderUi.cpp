#include "RenderUi.h"

#include <iostream>

#include "../Application.h"

void RenderUi::initRectVao() {
  // Vertex data: x, y
  float vertices[] = {
    -0.2f, 0.2f,
    0.2f, 0.2f,
    0.2f, -0.2f,
    -0.2f, -0.2f
  };
  unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
  };

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(
    0, // attribute index (matches your vertex shader)
    2, // number of components (x, y)
    GL_FLOAT, // data type
    GL_FALSE, // should OpenGL normalize values?
    2 * sizeof(float), // stride: total size of one vertex
    nullptr // offset: where this attribute starts
  );
  glEnableVertexAttribArray(0);

  // Unbind
  glBindVertexArray(0);

  rectVao = vao;
}

void RenderUi::initShader() {
  auto vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0, 1.0);
}
)";
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  GLint success;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cout << "SHADER COMPILATION FAILED:\n" << infoLog << std::endl;
  }

  auto fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

uniform vec4 u_baseColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);

void main()
{
    FragColor = u_baseColor;
}
)";
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    std::cout << "SHADER COMPILATION FAILED:\n" << infoLog << std::endl;
  }

  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    std::cout << "SHADER COMPILATION FAILED:\n" << infoLog << std::endl;
  }

  u_baseColor = glGetUniformLocation(shaderProgram, "u_baseColor");
}

void RenderUi::init() {
  initRectVao();
  initShader();
}

void RenderUi::start() {
  Application &app = Application::app;
  glEnable(GL_ALPHA);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RenderUi::stop() {
  glDisable(GL_ALPHA);
  glDisable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ZERO);
}

void RenderUi::rect(int x, int y, int w, int h) {
  glUseProgram(shaderProgram);
  glUniform4f(u_baseColor, 1.0f, 0.0, 0.0f, 1.0f);

  glBindVertexArray(rectVao);

  glDrawElements(
    GL_TRIANGLES, // draw type
    6, // number of indices
    GL_UNSIGNED_INT, // type of indices
    nullptr // offset
  );
  glBindVertexArray(0);
}

void RenderUi::resize(int width, int height) {
}
