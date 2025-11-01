#include "RenderPrimitive.h"

#include "Application.h"
#include "state/State.h"

void RenderPrimitive::init() {
  auto vertSource = R"(
#version 330 core
layout (location = 0) in vec3 a_pos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
  gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
  gl_PointSize = 5.0;
}
)";

  auto fragSource = R"(
#version 330 core

out vec4 o_color;

uniform vec2 u_viewportSize;
uniform vec4 u_color = vec4(1,0,0,1); // rgba
uniform float u_edgeSoftness = 0.08; // e.g. 0.08

void main()
{
  //o_color = vec4(1, 1, 0, 1);

  // gl_PointCoord is in [0,1] across the point sprite
  vec2 p = gl_PointCoord * 2.0 - 1.0;
  float r = length(p);
  // radius is 0.5; create smooth alpha near the edge
  float alpha = 1.0 - smoothstep(0.5 - u_edgeSoftness, 0.5, r);
  if (alpha <= 0.001) discard; // outside the circle
  o_color = vec4(u_color.rgb, u_color.a * alpha);
}
)";
  shader = Shader(vertSource, fragSource);

  float data[] = {
    // x, y, z
    0.0f, 0.0f, 0.0f,
  };

  unsigned int indices[] = {0};

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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  pointVao = vao;
}

void RenderPrimitive::start() {
  glEnable(GL_CULL_FACE);
  // glEnable(GL_DEPTH_TEST);
  glEnable(GL_ALPHA);
  glEnable(GL_BLEND);
  glEnable(GL_PROGRAM_POINT_SIZE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glUseProgram(shader.programId);
}

void RenderPrimitive::stop() {
  glDisable(GL_CULL_FACE);
  // glDisable(GL_DEPTH_TEST);
  glDisable(GL_ALPHA);
  glDisable(GL_BLEND);
  glDisable(GL_PROGRAM_POINT_SIZE);
  glBlendFunc(GL_ONE, GL_ZERO);
  glUseProgram(0);
}

void RenderPrimitive::resize(int width, int height) {
  updateProjection();
}

void RenderPrimitive::drawPoint() {
  glBindVertexArray(pointVao);

  glDrawElements(
    GL_POINTS, // draw type
    6, // number of indices
    GL_UNSIGNED_INT, // type of indices
    nullptr // offset
  );
  glBindVertexArray(0);
}

void RenderPrimitive::updateModel(glm::vec3 pos, float scale) {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, pos);
  model = glm::scale(model, glm::vec3(scale));
  glUniformMatrix4fv(glGetUniformLocation(shader.programId, "u_model"), 1, GL_FALSE, &model[0][0]);
}

void RenderPrimitive::updateView(glm::vec3 pos, float pitch, float yaw, glm::vec2 pxOffset) {
  glm::mat4 view = glm::identity<glm::mat4>();
  view = glm::rotate(view, glm::radians(-pitch), glm::vec3(1, 0, 0));
  view = glm::rotate(view, glm::radians(-yaw), glm::vec3(0, 1, 0));
  view = glm::translate(view, -pos);
  glUniformMatrix4fv(glGetUniformLocation(shader.programId, "u_view"), 1, GL_FALSE, &view[0][0]);
}

void RenderPrimitive::updateProjection() {
  glUseProgram(shader.programId);

  auto &settings = State::state.settings;
  auto &app = Application::app;
  int width = app.getWidth();
  int height = app.getHeight();
  auto projection = glm::perspective(
    glm::radians(static_cast<float>(settings.fov)),
    static_cast<float>(width) / static_cast<float>(height),
    0.01f, 1000.0f
  );

  glUniformMatrix4fv(glGetUniformLocation(shader.programId, "u_projection"), 1, GL_FALSE, &projection[0][0]);
  glUniform2f(glGetUniformLocation(shader.programId, "u_viewportSize"), static_cast<float>(width), static_cast<float>(height));

  glUseProgram(0);
}

void RenderPrimitive::color(int rgba) {
  float r = ((rgba >> 24) & 0xFF) / 255.0f;
  float g = ((rgba >> 16) & 0xFF) / 255.0f;
  float b = ((rgba >> 8) & 0xFF) / 255.0f;
  float a = (rgba & 0xFF) / 255.0f;
  glUniform4f(glGetUniformLocation(shader.programId, "u_color"), r, g, b, a);
}