#ifndef SOUND_H
#define SOUND_H

#include <memory>
#include "lib/miniaudio.h"

class Sound {
public:
  ma_sound _ma_sound;

  Sound() :
    _ma_sound() {}

  ~Sound() {
    ma_sound_uninit(&_ma_sound);
  }

  void play();

  Sound(const Sound &other) = delete;

  Sound(Sound &&other) noexcept :
    _ma_sound(std::move(other._ma_sound)) {}

  Sound &operator=(const Sound &other) = delete;

  Sound &operator=(Sound &&other) noexcept {
    if (this == &other)
      return *this;
    _ma_sound = std::move(other._ma_sound);
    return *this;
  }
};

#endif //SOUND_H
