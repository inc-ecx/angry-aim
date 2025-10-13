#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "assimp/scene.h"

class Model
{
public:
  explicit Model(std::string path)
  {
    loadModel(path);
  }
  // model data
  std::vector<Mesh> meshes;
  std::string directory;

  void loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif //MODEL_H
