#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "assimp/scene.h"

class Model
{
public:
  explicit Model(const std::string &path)
  {
    loadModel(path);
  }

  ~Model();

  // model data
  std::vector<std::shared_ptr<Mesh>> meshes;
  std::string directory;
  std::vector<Texture> textures_loaded;

  void loadModel(const std::string &path);
  void processNode(aiNode *node, const aiScene *scene);
  std::shared_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName);
};

#endif //MODEL_H
