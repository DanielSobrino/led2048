#ifndef __SOUND_CONTROL_HPP
#define __SOUND_CONTROL_HPP
#include <Arduino.h>
#include "MusicDefinitions.h"
#include "XT_DAC_Audio.h"

#define AUDIO_ON 33
#define AUDIO_OUT 25

extern void init_sound();
extern void audio_on();
extern void audio_off();
extern void play_note_for_score(int16_t note);
extern void test_sound();

extern XT_DAC_Audio_Class DacAudio;

#endif