#include "RenderUi.h"

#include "../Application.h"

void RenderUi::init() {

  // // Vertex data: x, y
  // float vertices[] = {
  //     -0.2f,  0.2f,
  //      0.2f,  0.2f,
  //      0.2f, -0.2f,
  //     -0.2f, -0.2f
  // };
  // unsigned int indices[] = {
  //     0, 1, 2,
  //     2, 3, 0
  // };
  //
  // // Create VAO
  // GLuint vao;
  // glGenVertexArrays(1, &vao);
  // glBindVertexArray(vao);
  //
  // // Create VBO
  // GLuint vbo;
  // glGenBuffers(1, &vbo);
  // glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  //
  // // Create EBO
  // GLuint ebo;
  // glGenBuffers(1, &ebo);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  //
  // // Vertex attribute: position (layout = 0)
  // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  // glEnableVertexAttribArray(0);
  //
  // // Unbind
  // glBindVertexArray(0);

}

void RenderUi::start() {
  Application &app = Application::app;
  glOrtho(0,app.getWidth(), app.getHeight(), 0, -1000, 1000);
}

void RenderUi::stop() {
  glLoadIdentity();
}

void RenderUi::rect(int x, int y, int w, int h) {
  glColor4f(1,1,1,1);
  glBegin(GL_QUADS);
  glVertex2f(x,y);
  glVertex2f(x+w,y);
  glVertex2f(x+w,y+h);
  glVertex2f(x,y+h);
  glEnd();
}

void RenderUi::resize(int width, int height) {

}
