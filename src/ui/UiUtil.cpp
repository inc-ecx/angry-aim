#include "UiUtil.h"

uint32_t UiUtil::lerpColor(uint32_t x, uint32_t y, float t) {
  uint8_t x0 = (x >> 24) & 0xFF;
  uint8_t x1 = (x >> 16) & 0xFF;
  uint8_t x2 = (x >> 8) & 0xFF;
  uint8_t x3 = x & 0xFF;

  uint8_t y0 = (y >> 24) & 0xFF;
  uint8_t y1 = (y >> 16) & 0xFF;
  uint8_t y2 = (y >> 8) & 0xFF;
  uint8_t y3 = y & 0xFF;

  uint8_t z0 = x0 + (y0 - x0) * t;
  uint8_t z1 = x1 + (y1 - x1) * t;
  uint8_t z2 = x2 + (y2 - x2) * t;
  uint8_t z3 = x3 + (y3 - x3) * t;

  return (z0 << 24) | (z1 << 16) | (z2 << 8) | z3;
}
