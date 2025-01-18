#include "sound_control.hpp"
#include "SoundData.h"

XT_DAC_Audio_Class DacAudio(PIN_DAC1, 0);

XT_Wav_Class NoteC(nota_C);
XT_Wav_Class NoteCS(nota_CS);
XT_Wav_Class NoteD(nota_D);
XT_Wav_Class NoteDS(nota_DS);
XT_Wav_Class NoteE(nota_E);
XT_Wav_Class NoteF(nota_F);
XT_Wav_Class NoteFS(nota_FS);
XT_Wav_Class NoteG(nota_G);
XT_Wav_Class NoteGS(nota_GS);
XT_Wav_Class NoteA(nota_A);
XT_Wav_Class NoteAS(nota_AS);
XT_Wav_Class NoteB(nota_B);

XT_Wav_Class* selectedNote = &NoteC;

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

void play_note_for_score(int16_t score) {

  Serial.printf("Playing score: %d\n", score);
  switch (score) {
    case 4:selectedNote = &NoteC;break;
    case 8:selectedNote = &NoteCS;break;
    case 16:selectedNote = &NoteD;break;
    case 32:selectedNote = &NoteDS;break;
    case 64:selectedNote = &NoteE;break;
    case 128:selectedNote = &NoteF;break;
    case 256:selectedNote = &NoteFS;break;
    case 512:selectedNote = &NoteG;break;
    case 1024:selectedNote = &NoteGS;break;
    case 2048:selectedNote = &NoteA;break;
    default:selectedNote = nullptr;return;
  }
  DacAudio.Play(selectedNote);
}



void test_sound() {
  for (uint16_t i = 4; i <= 2048; i *= 2) {
    while(selectedNote->Playing) {
      DacAudio.FillBuffer();
    }
    Serial.printf("Playing note for %d\n", i);
    play_note_for_score(i);
    delay(500);
  }
}