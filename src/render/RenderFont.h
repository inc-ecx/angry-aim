#ifndef RENDER_FONT_H
#define RENDER_FONT_H

#include <map>

#include "../inc/inc_glfw.h"

struct Character {
  unsigned int TextureID;  // ID handle of the glyph texture
  glm::ivec2   Size;       // Size of glyph
  glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
  unsigned int Advance;    // Offset to advance to next glyph
};

class RenderFont {

  struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
  };
  std::map<char, Character> charMap;
  int lineHeight = -1;

  GLuint shaderProgram = -1;
  GLuint VAO = -1, VBO = -1;

  void initCharMap(FT_Library ft);
  void initQuad();
  void initShader();

public:
  void renderText(std::string text, float x, float y, int rgba);
  int height() { return lineHeight; }
  int width(std::string text);

  void init(FT_Library ft);
  void start();
  void stop();

  void resize(int width, int height);
};

#endif //RENDER_FONT_H
