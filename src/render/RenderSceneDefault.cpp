#include "RenderSceneDefault.h"

void RenderSceneDefault::initRectVao() {
}

void RenderSceneDefault::initShader() {
  auto vertexShaderSource = R"(
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

in vec2 TexCoords;
uniform sampler2D texture_diffuse1;
uniform vec4 u_baseColor = vec4(1,1,1,1);
uniform int u_doTexture = 0;

void main()
{
  if (u_doTexture == 1)
  {
    FragColor = texture(texture_diffuse1, vec2(TexCoords.x,1 - TexCoords.y));
  }
  else
  {
    FragColor = u_baseColor;
  }
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
}

void RenderSceneDefault::init() {
  initRectVao();
  initShader();
}

void RenderSceneDefault::start() {
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_ALPHA);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glUseProgram(shaderProgram);
}

void RenderSceneDefault::stop() {
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_ALPHA);
  glDisable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ZERO);
  glUseProgram(0);
}

void RenderSceneDefault::texture(bool value) {
  glUniform1i(glGetUniformLocation(shaderProgram, "u_doTexture"), value ? 1 : 0);
}

void RenderSceneDefault::color(int rgba) {
  float r = ((rgba >> 24) & 0xFF) / 255.0f;
  float g = ((rgba >> 16) & 0xFF) / 255.0f;
  float b = ((rgba >> 8) & 0xFF) / 255.0f;
  float a = (rgba & 0xFF) / 255.0f;
  glUniform4f(glGetUniformLocation(shaderProgram, "u_baseColor"), r, g, b, a);
}

void RenderSceneDefault::mesh(const Mesh &mesh) {
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

    glUniform1i(glGetUniformLocation(shaderProgram, ("material." + name + number).c_str()), i);
    glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
  }
  glActiveTexture(GL_TEXTURE0);

  // draw mesh
  glBindVertexArray(mesh.VAO);
  glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void RenderSceneDefault::draw(const Model &model) {
  for (unsigned int i = 0; i < model.meshes.size(); i++)
    mesh(model.meshes[i]);
}

// note: should only be called when renderer is active. angles are deg.
void RenderSceneDefault::updateView(glm::vec3 pos, float pitch, float yaw) {
  glm::mat4 view = glm::identity<glm::mat4>();
  view = glm::rotate(view, glm::radians(pitch), glm::vec3(1, 0, 0));
  view = glm::rotate(view, glm::radians(yaw), glm::vec3(0, 1, 0));
  view = glm::translate(view, -pos);
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
}

void RenderSceneDefault::updateModel(glm::vec3 pos, float scale) {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, pos);
  model = glm::scale(model, glm::vec3(scale));
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
}

void RenderSceneDefault::resize(int width, int height) {
  glUseProgram(shaderProgram);

  glm::mat4 projection = glm::perspective(glm::radians(90.0f), static_cast<float>(width) / static_cast<float>(height),
                                          0.1f, 100.0f);
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

  glUseProgram(0);
}
