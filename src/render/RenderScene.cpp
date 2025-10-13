#include "RenderScene.h"

void RenderScene::initRectVao() {
}

void RenderScene::initShader() {
  auto vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat4 u_model;
uniform mat4 u_view;
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

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
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

  u_model = glGetUniformLocation(shaderProgram, "u_model");
  u_view = glGetUniformLocation(shaderProgram, "u_view");
  u_projection = glGetUniformLocation(shaderProgram, "u_projection");
}

void RenderScene::init() {
  initRectVao();
  initShader();
}

void RenderScene::start() {
  glEnable(GL_ALPHA);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glUseProgram(shaderProgram);
}

void RenderScene::stop() {
  glDisable(GL_ALPHA);
  glDisable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ZERO);
  glUseProgram(0);
}

void RenderScene::mesh(const Mesh &mesh) {
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  for(unsigned int i = 0; i < mesh.textures.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
    // retrieve texture number (the N in diffuse_textureN)
    std::string number;
    std::string name = mesh.textures[i].type;
    if(name == "texture_diffuse")
      number = std::to_string(diffuseNr++);
    else if(name == "texture_specular")
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

void RenderScene::model(const Model &model) {
  for(unsigned int i = 0; i < model.meshes.size(); i++)
        mesh(model.meshes[i]);
}

void RenderScene::resize(int width, int height) {
  glUseProgram(shaderProgram);
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
  glUniformMatrix4fv(u_projection, 1, GL_FALSE, &projection[0][0]);
  glUseProgram(0);
}
