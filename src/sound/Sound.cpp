#include "Sound.h"

#include "Application.h"

void Sound::play() {
  ma_sound* copy = new ma_sound();

  ma_sound_init_copy(ma_sound_get_engine(&_ma_sound), &_ma_sound, 0, nullptr, copy);
  ma_sound_start(copy);

  ma_sound_set_end_callback(
    copy,
    [](void *pUserData, ma_sound *pSound) {
      Application::app.later([pSound] {
        ma_sound_uninit(pSound);
        delete pSound;
      });
    },
    nullptr
  );
}
