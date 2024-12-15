#include "led_control.hpp"

#define DATA_PIN    GPIO_NUM_12
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    16
#define BRIGHTNESS  15

CRGB leds[NUM_LEDS];

const CRGB colorPalette[] = {
  CRGB::White,  // 2
  CRGB(60,60,60),    // 4
  CRGB::Red,       // 8
  CRGB(125,55,0),  // 16
  CRGB::Yellow,   // 32
  CRGB::Green,    // 64
  CRGB(0,155,55), // 128
  CRGB(0,85,125),   // 256
  CRGB::Blue,   // 512
  CRGB(65,0,190),    // 1024
  CRGB::Magenta   // 2048
};

void init_leds() {
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip);
    // .setDither(BRIGHTNESS < 255);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void update_leds(uint16_t *tablero) {
  for (size_t i = 0; i < NUM_LEDS; ++i) {
    uint16_t value = tablero[i];
    leds[i] = CRGB::Black;
    if (value == 0) continue;
    int pos = -1;
    while (value > 1) {
        value >>= 1; // Desplaza un bit a la derecha
        pos++;
    }
    leds[i] = colorPalette[pos];
  }
  FastLED.show();
}

void test_colors() {
  for (int i = 0; i < 11; i++) {
    leds[i] = colorPalette[i];
  }

  FastLED.show();
}
