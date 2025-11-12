#include "Texture.h"

#include "Log.h"
#include "util/file_util.h"

Texture::~Texture() {
  if (id != 0) glDeleteTextures(1, &id);
}

std::unique_ptr<Texture> Texture::load(const std::string &path) {
  int width, height, nrChannels;
  unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

  GLuint texture = 0;
  if (data == nullptr) {
    Log::error(std::format("Could not load texture file \"{}\"", fs::absolute(path).string()));
  } else {
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.2f);

    if (nrChannels == 3) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else if (nrChannels == 4) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } else {
      Log::error(std::format("Unsupported chanel count \"{}\": {}", fs::absolute(path).string(), nrChannels));
    }
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
  }

  auto res = std::make_unique<Texture>();
  res->width = width;
  res->height = height;
  res->id = texture;
  return res;
}
