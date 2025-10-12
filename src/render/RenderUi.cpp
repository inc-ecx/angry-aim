#include "RenderUi.h"

#include <iostream>

#include "../Application.h"

void RenderUi::initRectVao() {
  // Vertex data: x, y
  float vertices[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
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

uniform mat4 u_model;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_model * vec4(aPos.x, aPos.y, 0, 1.0);
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
  u_model = glGetUniformLocation(shaderProgram, "u_model");
  u_projection = glGetUniformLocation(shaderProgram, "u_projection");
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
  glUseProgram(shaderProgram);
}

void RenderUi::stop() {
  glDisable(GL_ALPHA);
  glDisable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ZERO);
  glUseProgram(0);
}

void RenderUi::color(int rgba) {
  float r = ((rgba >> 24) & 0xFF) / 255.0f;
  float g = ((rgba >> 16) & 0xFF) / 255.0f;
  float b = ((rgba >> 8)  & 0xFF) / 255.0f;
  float a = (rgba & 0xFF) / 255.0f;
  glUniform4f(u_baseColor, r, g, b, a);
}

void RenderUi::rect(int x, int y, int w, int h) {
  Application &app = Application::app;

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(x + 0.5f, y + 0.5f, 0.0f));
  model = glm::scale(model, glm::vec3(w, h, 1.0f));

  glUniformMatrix4fv(u_model, 1, GL_FALSE, &model[0][0]);

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
  glUseProgram(shaderProgram);
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
  glUniformMatrix4fv(u_projection, 1, GL_FALSE, &projection[0][0]);
  glUseProgram(0);
}
