#include "WorldUtil.h"

bool WorldUtil::hitSphere(
  const glm::vec3 &rayOrigin,
  float pitch,
  float yaw,
  const glm::vec3 &spherePos,
  float radius,
  float &tHit
) {
  glm::vec3 dir = lookVec(pitch,yaw);

  glm::vec3 L = spherePos - rayOrigin;
  float tca = glm::dot(L, dir);
  if (tca < 0) return false; // Sphere is behind the ray

  float d2 = glm::dot(L, L) - tca * tca;
  float r2 = radius * radius;
  if (d2 > r2) return false; // Misses the sphere

  float thc = sqrt(r2 - d2);
  tHit = tca - thc; // nearest intersection along the ray
  return true;
}

glm::vec3 WorldUtil::lookVec(float pitch, float yaw) {
  glm::vec3 dir;
  dir.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw + 180));
  dir.y = sin(glm::radians(pitch));
  dir.z = cos(glm::radians(pitch)) * cos(glm::radians(yaw + 180));
  dir = glm::normalize(dir);
  return dir;
}
