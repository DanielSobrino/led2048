#ifndef __ACEL_CONTROL_H__
#define __ACEL_CONTROL_H__

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

enum Direction { UP, DOWN, LEFT, RIGHT, NONE };

extern const char* Directions[];

extern volatile float accel_x, accel_y;

extern volatile bool movimiento_detectado;
extern volatile bool movimiento_procesado;
extern volatile Direction process_direction;

extern sensors_event_t event;
extern Adafruit_LIS3DH lis;
// extern portMUX_TYPE mux;

void setupAccel();

#endif