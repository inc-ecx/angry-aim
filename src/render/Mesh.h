#ifndef MESH_H
#define MESH_H

#include "../inc/inc_glfw.h"
#include "../inc/inc_glm.h"

#include <string>
#include <vector>

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

struct Texture {
  unsigned int id;
  std::string type;
  std::string path;
};

class Mesh {
public:
  // mesh data
  std::vector<Vertex>       vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture>      textures;

  Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<Texture> &textures);

  //  render data
  unsigned int VAO, VBO, EBO;

  void setupMesh();
};

#endif //MESH_H
