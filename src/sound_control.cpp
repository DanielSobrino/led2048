#include "sound_control.hpp"
#include "SoundData.h"

XT_DAC_Audio_Class DacAudio(PIN_DAC1, 0);

XT_Wav_Class* CurrentSound(nullptr);

void audio_on() {
  digitalWrite(AUDIO_OUT, LOW);
  delay(50);
}

void audio_off() {
  digitalWrite(AUDIO_OUT, HIGH);
}

void init_sound() {
  pinMode(AUDIO_ON, OUTPUT);
  audio_on();
}

void play_start() {
  delete CurrentSound;
  CurrentSound = new XT_Wav_Class(game_start);
  CurrentSound->SampleRate = 8000;
  DacAudio.Play(CurrentSound);
}

void play_win() {
  delete CurrentSound;
  CurrentSound = new XT_Wav_Class(game_win);
  CurrentSound->SampleRate = 8000;
  DacAudio.Play(CurrentSound);
}

void play_loss() {
  delete CurrentSound;
  CurrentSound = new XT_Wav_Class(game_over);
  CurrentSound->SampleRate = 8000;
  DacAudio.Play(CurrentSound);
}

void newNote(XT_Wav_Class note) {
  delete CurrentSound;
  CurrentSound = new XT_Wav_Class(note);
  CurrentSound->SampleRate = 11025;
}

void play_note_for_score(int16_t score) {

  // Serial.printf("Playing score: %d\n", score);
  switch (score) {
    case 4:newNote(nota_C);break;
    case 8:newNote(nota_CS);break;
    case 16:newNote(nota_D);break;
    case 32:newNote(nota_DS);break;
    case 64:newNote(nota_E);break;
    case 128:newNote(nota_F);break;
    case 256:newNote(nota_FS);break;
    case 512:newNote(nota_G);break;
    case 1024:newNote(nota_GS);break;
    case 2048:newNote(nota_A);break;
    default:CurrentSound = nullptr;return;
  }
  DacAudio.Play(CurrentSound);
}

void test_sound() {
  for (uint16_t i = 4; i <= 2048; i *= 2) {
    while(CurrentSound->Playing) {
      DacAudio.FillBuffer();
    }
    Serial.printf("Playing note for %d\n", i);
    play_note_for_score(i);
    delay(500);
  }
}