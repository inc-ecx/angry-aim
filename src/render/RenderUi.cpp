#include "RenderUi.h"

#include <iostream>

#include "../Application.h"

void RenderUi::initRectVao() {
  float data[] = {
    // x, y, u, v
    0.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f
  };

  unsigned int indices[] = {0, 1, 2, 2, 3, 0};

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void *>(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  rectVao = vao;
}

void RenderUi::initShader() {
  auto vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 i_pos;
layout (location = 1) in vec2 i_uv;

out vec2 p_uv;
out vec2 p_pos;
out vec2 p_ndcPos;

uniform mat4 u_model;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_model * vec4(i_pos, 0, 1.0);

    p_uv = i_uv;
    p_pos = i_pos;
    p_ndcPos = gl_Position.xy / gl_Position.w;
}
)";

  auto fragmentShaderSource = R"(
#version 330 core

in vec2 p_uv;
in vec2 p_pos;
in vec2 p_ndcPos;

out vec4 o_color;

uniform vec4 u_baseColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
uniform sampler2D u_texture;
uniform int u_doTexture = 0;

uniform float u_aspectRatio = 1.0;
uniform float u_borderRadius = 0.0;
uniform float u_borderBlur = 0;

float getEdgeAlpha() {
  float alphaMod = 1;

  if (u_borderRadius > 0) {
    vec2 pos = vec2(p_pos.x, p_pos.y / u_aspectRatio);
    float l = u_borderRadius, r = 1 - u_borderRadius, t = u_borderRadius, b = 1 / u_aspectRatio - u_borderRadius;
    if (pos.x < l && pos.y < t) {
      float d = distance(vec2(pos.x, pos.y), vec2(l, t));
      alphaMod = smoothstep(1, 0, (d - u_borderRadius+ u_borderBlur * 0.5f) / u_borderBlur);
    } else if (pos.x > r && pos.y < t) {
        float d = distance(vec2(pos.x, pos.y), vec2(r, t));
        alphaMod = smoothstep(1, 0, (d - u_borderRadius + u_borderBlur * 0.5f) / u_borderBlur);
    } else if (pos.x > r && pos.y > b) {
        float d = distance(vec2(pos.x, pos.y), vec2(r, b));
        alphaMod = smoothstep(1, 0, (d - u_borderRadius + u_borderBlur * 0.5f) / u_borderBlur);
    } else if (pos.x < l && pos.y > b) {
        float d = distance(vec2(pos.x, pos.y), vec2(l, b));
        alphaMod = smoothstep(1, 0, (d - u_borderRadius + u_borderBlur * 0.5f) / u_borderBlur);
    }
  }
  return alphaMod;
}

void main()
{
  if (u_doTexture == 1) {
    o_color = texture(u_texture, p_uv) * u_baseColor;
  } else {
    o_color = u_baseColor;
    // o_color = vec4(p_pos.x, p_pos.y, 0, 1);
    o_color.a *= getEdgeAlpha();
  }

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
  glEnable(GL_ALPHA);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glUseProgram(shaderProgram);
}

void RenderUi::stop() {
  glDisable(GL_ALPHA);
  glDisable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ZERO);
  glDisable(GL_TEXTURE_2D);
  glUseProgram(0);
}

void RenderUi::color(int rgba) {
  float r = ((rgba >> 24) & 0xFF) / 255.0f;
  float g = ((rgba >> 16) & 0xFF) / 255.0f;
  float b = ((rgba >> 8) & 0xFF) / 255.0f;
  float a = (rgba & 0xFF) / 255.0f;
  glUniform4f(u_baseColor, r, g, b, a);
}

void RenderUi::rect(int x, int y, int w, int h) {
  rect(x, y, w, h, 0, 0);
}

void RenderUi::rect(int x, int y, int w, int h, float radius, float borderBlur) {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(x, y, 0.0f));
  model = glm::scale(model, glm::vec3(w, h, 1.0f));

  glUniformMatrix4fv(u_model, 1, GL_FALSE, &model[0][0]);
  glUniform1f(glGetUniformLocation(shaderProgram, "u_aspectRatio"), static_cast<float>(w) / h);
  glUniform1f(glGetUniformLocation(shaderProgram, "u_borderRadius"), radius / w);
  glUniform1f(glGetUniformLocation(shaderProgram, "u_borderBlur"), borderBlur / w);

  glBindVertexArray(rectVao);

  glDrawElements(
    GL_TRIANGLES, // draw type
    6, // number of indices
    GL_UNSIGNED_INT, // type of indices
    nullptr // offset
  );
  glBindVertexArray(0);
}

void RenderUi::texture(GLuint id) {
  if (id != 0) {
    glUniform1i(glGetUniformLocation(shaderProgram, "u_doTexture"), 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
    glUniform1i(glGetUniformLocation(shaderProgram, "u_texture"), 0);
  } else {
    glUniform1i(glGetUniformLocation(shaderProgram, "u_doTexture"), 0);
  }
}

void RenderUi::resize(int width, int height) {
  glUseProgram(shaderProgram);
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
  glUniformMatrix4fv(u_projection, 1, GL_FALSE, &projection[0][0]);
  glUseProgram(0);
}
