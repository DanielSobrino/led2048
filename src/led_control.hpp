#ifndef __LED_CONTROL_H__
#define __LED_CONTROL_H__

#include <FastLED.h>

extern void init_leds();
extern void test_colors();
extern void update_leds(uint16_t *tablero);
extern void win_animation();
extern void lose_animation(uint16_t *tablero);
extern void start_animation();

#endif