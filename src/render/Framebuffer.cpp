#include "Framebuffer.h"
#include "Application.h"
#include "Log.h"

Framebuffer::Framebuffer() {
  auto &app = Application::app;
  int width = app.getWidth();
  int height = app.getHeight();

  // create framebuffer
  glGenFramebuffers(1, &id);
  glBindFramebuffer(GL_FRAMEBUFFER, id);

  // attach color texture
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

  // add depth buffer
  glGenRenderbuffers(1, &depthBufferId);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBufferId);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferId);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    throw std::runtime_error("Failed to create framebuffer");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer() {
  if (prevId != -1) unbind();
  glDeleteTextures(1, &textureId);
  glDeleteRenderbuffers(1, &depthBufferId);
  glDeleteFramebuffers(1, &id);
}

void Framebuffer::bind() {
  glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &prevId);
  glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void Framebuffer::unbind() {
  if (prevId == -1) Log::warn("Tried to unbind unbound framebuffer");
  glBindFramebuffer(GL_FRAMEBUFFER, prevId);
  prevId = -1;
}
