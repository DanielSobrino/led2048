#ifndef __LED_CONTROL_H__
#define __LED_CONTROL_H__

#include <FastLED.h>

extern void init_leds();
extern void test_colors();
extern void update_leds(uint16_t *tablero);

#endif