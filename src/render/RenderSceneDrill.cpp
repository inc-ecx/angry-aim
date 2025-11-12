#include "RenderSceneDrill.h"

#include "Application.h"
#include "Log.h"
#include "state/State.h"

void RenderSceneDrill::initRectVao() {}

void RenderSceneDrill::initShader() {
  auto vertSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

  auto fragSource = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D texture_diffuse1;
uniform vec4 u_baseColor = vec4(1,1,1,1);
uniform int u_doTexture = 0;

void main()
{
  if (u_doTexture == 1)
  {
    FragColor = u_baseColor * texture(texture_diffuse1, vec2(TexCoords.x, 1 - TexCoords.y));
  }
  else
  {
    FragColor = u_baseColor;
  }
}
)";
  shader = Shader(vertSource, fragSource);
}

void RenderSceneDrill::init() {
  initRectVao();
  initShader();
}

void RenderSceneDrill::start() {
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_ALPHA);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glUseProgram(shader.programId);
}

void RenderSceneDrill::stop() {
  glDepthMask(GL_TRUE);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_ALPHA);
  glDisable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ZERO);
  glUseProgram(0);
}

void RenderSceneDrill::depthMask(bool depthMask) {
  glDepthMask(depthMask ? GL_TRUE : GL_FALSE);
}

void RenderSceneDrill::texture(bool value) {
  glUniform1i(glGetUniformLocation(shader.programId, "u_doTexture"), value ? 1 : 0);
}

void RenderSceneDrill::color(int rgba) {
  float r = ((rgba >> 24) & 0xFF) / 255.0f;
  float g = ((rgba >> 16) & 0xFF) / 255.0f;
  float b = ((rgba >> 8) & 0xFF) / 255.0f;
  float a = (rgba & 0xFF) / 255.0f;
  glUniform4f(glGetUniformLocation(shader.programId, "u_baseColor"), r, g, b, a);
}

void RenderSceneDrill::mesh(const Mesh &mesh) {
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  for (unsigned int i = 0; i < mesh.textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
    // retrieve texture number (the N in diffuse_textureN)
    std::string number;
    std::string name = mesh.textures[i].type;
    if (name == "texture_diffuse")
      number = std::to_string(diffuseNr++);
    else if (name == "texture_specular")
      number = std::to_string(specularNr++);

    glUniform1i(glGetUniformLocation(shader.programId, ("material." + name + number).c_str()), i);
    glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
  }
  glActiveTexture(GL_TEXTURE0);

  // draw mesh
  glBindVertexArray(mesh.VAO);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indices.size()), GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
}

void RenderSceneDrill::draw(const Model &model) {
  for (unsigned int i = 0; i < model.meshes.size(); i++)
    mesh(*model.meshes[i]);
}

// pxOffset is offset to center of screen, up=-y, right=x
// note: should only be called when renderer is active. angles are deg.
void RenderSceneDrill::updateView(glm::vec3 pos, float pitch, float yaw, glm::vec2 pxOffset) {
  // auto &app = Application::app;
  // glm::mat4 view = glm::identity<glm::mat4>();
  // apply pxOffset
  // pxOffset = {50.0f,50.0f};
  // glm::vec4 viewport(0.0f, 0.0f, app.getWidth(), app.getHeight());
  // glm::vec3 unProjected = glm::unProject(
  //   // note: unProject expects window coordinates with the origin at the bottom left
  //   glm::vec3(app.getWidth() / 2.0f + pxOffset.x, app.getHeight() / 2.0f - pxOffset.y, 0.01f),
  //   glm::identity<glm::mat4>(),
  //   projection,
  //   viewport
  // );
  // double unProjectedPitch = atan2(unProjected.y, -unProjected.z);
  // double unProjectedYaw = atan2(unProjected.x, -unProjected.z);
  // Log::info(std::format("unProjected {:.7f}, {:.7f}, {:.7f}", unProjected.x, unProjected.y, unProjected.z));
  // Log::info(std::format("unProjectedPitch {}", static_cast<int>(glm::degrees(unProjectedPitch))));
  // Log::info(std::format("unProjectedYaw {}", static_cast<int>(glm::degrees(unProjectedYaw))));
  // view = glm::rotate(view, glm::radians(-pitch) - static_cast<float>(unProjectedPitch), glm::vec3(1, 0, 0));
  // view = glm::rotate(view, glm::radians(-yaw) + static_cast<float>(unProjectedYaw), glm::vec3(0, 1, 0));
  // view = glm::translate(view, -glm::vec3(unProjected.x, unProjected.y, 0));

  glm::mat4 view = glm::identity<glm::mat4>();
  view = glm::rotate(view, glm::radians(-pitch), glm::vec3(1, 0, 0));
  view = glm::rotate(view, glm::radians(-yaw), glm::vec3(0, 1, 0));
  view = glm::translate(view, -pos);
  glUniformMatrix4fv(glGetUniformLocation(shader.programId, "view"), 1, GL_FALSE, &view[0][0]);
}

void RenderSceneDrill::updateModel(glm::vec3 pos, float scale) {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, pos);
  model = glm::scale(model, glm::vec3(scale));
  glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, &model[0][0]);
}

void RenderSceneDrill::resize(int width, int height) {
  updateProjection();
}

void RenderSceneDrill::updateProjection() {
  glUseProgram(shader.programId);

  auto &settings = State::state.settings;
  auto &app = Application::app;
  int width = app.getWidth();
  int height = app.getHeight();
  projection = glm::perspective(
    glm::radians(static_cast<float>(settings.fov)),
    static_cast<float>(width) / static_cast<float>(height),
    0.01f, 1000.0f
  );

  glUniformMatrix4fv(glGetUniformLocation(shader.programId, "projection"), 1, GL_FALSE, &projection[0][0]);

  glUseProgram(0);
}
