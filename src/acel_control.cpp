#include "acel_control.hpp"
#include <Ticker.h>

sensors_event_t event;

// enum Direction { UP, DOWN, LEFT, RIGHT, NONE };
const char* Directions[] = { "UP", "DOWN", "LEFT", "RIGHT", "NONE" };
volatile Direction direction = NONE;
volatile Direction process_direction = NONE;
enum Estado { ESPERA_DIRECCION, ESPERA_POS_ESTABLE } estado;

volatile float accel_x, accel_y;
volatile bool movimiento_detectado = false;
volatile bool movimiento_procesado = false;
Ticker ticker_accel;

// portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void config();
void leer_accel();
// void IRAM_ATTR timerInterrupt();

float init_x, init_y;
const float movement_threshold = 5.0;
const float stable_threshold = 2.0;

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

void setupAccel() {
  config();

  // Leer valores iniciales
  lis.read();
  lis.getEvent(&event);
  
  init_x = event.acceleration.x;
  init_y = event.acceleration.y;
  estado = ESPERA_DIRECCION;

  ticker_accel.attach_ms(100, leer_accel);
}

void leer_accel() {
  lis.read();
  lis.getEvent(&event);
  accel_x = event.acceleration.x;
  accel_y = event.acceleration.y;
  Direction old_direction = direction;
  boolean prioridad_x = abs(accel_x) > abs(accel_y);

  switch (estado) {
    case ESPERA_DIRECCION:
      if (prioridad_x && abs(accel_x - init_x) > movement_threshold) {
        direction = (accel_x > 0) ? DOWN : UP;
      } else if (abs(accel_y - init_y) > movement_threshold) {
        direction = (accel_y > 0) ? RIGHT : LEFT;
      } else {
        direction = NONE;
      }

      process_direction = direction;

      if (old_direction != NONE && direction == NONE) {
        movimiento_detectado = false;
        movimiento_procesado = false;
      } else if (direction != NONE) {
        movimiento_detectado = true;
        estado = ESPERA_POS_ESTABLE;
      }
      break;
    case ESPERA_POS_ESTABLE:
      boolean enPosicionEstable = abs(accel_x - init_x) < stable_threshold && 
                                  abs(accel_y - init_y) < stable_threshold;

      if (enPosicionEstable) estado = ESPERA_DIRECCION;
      else movimiento_detectado = false;
  }  

  // Serial.printf("enPosicionEstable: %s, X: %6.2f  Y: %6.2f \n", enPosicionEstable ? "true" : "false", accel_x, accel_y);
}
  // Serial.printf("Accel accel_x: %6.2f  accel_y: %6.2f  Z: %6.2f m/s^2 \n", event.acceleration.x, event.acceleration.y, event.acceleration.z);

void config() {
  Serial.println("LIS3DH test!");

  if (! lis.begin(0x19)) {   // change this to 0x18 for alternative i2c address
    Serial.println("Couldnt start");
    while (1) yield();
  }
  Serial.println("LIS3DH found!");

  // lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!

  Serial.print("Range = "); Serial.print(2 << lis.getRange());
  Serial.println("G");

  lis.setPerformanceMode(LIS3DH_MODE_LOW_POWER);
  Serial.print("Performance mode set to: ");
  switch (lis.getPerformanceMode()) {
    case LIS3DH_MODE_NORMAL: Serial.println("Normal 10bit"); break;
    case LIS3DH_MODE_LOW_POWER: Serial.println("Low Power 8bit"); break;
    case LIS3DH_MODE_HIGH_RESOLUTION: Serial.println("High Resolution 12bit"); break;
  }

  lis.setDataRate(LIS3DH_DATARATE_100_HZ);
  Serial.print("Data rate set to: ");
  switch (lis.getDataRate()) {
    case LIS3DH_DATARATE_1_HZ: Serial.println("1 Hz"); break;
    case LIS3DH_DATARATE_10_HZ: Serial.println("10 Hz"); break;
    case LIS3DH_DATARATE_25_HZ: Serial.println("25 Hz"); break;
    case LIS3DH_DATARATE_50_HZ: Serial.println("50 Hz"); break;
    case LIS3DH_DATARATE_100_HZ: Serial.println("100 Hz"); break;
    case LIS3DH_DATARATE_200_HZ: Serial.println("200 Hz"); break;
    case LIS3DH_DATARATE_400_HZ: Serial.println("400 Hz"); break;

    case LIS3DH_DATARATE_POWERDOWN: Serial.println("Powered Down"); break;
    case LIS3DH_DATARATE_LOWPOWER_5KHZ: Serial.println("5 Khz Low Power"); break;
    case LIS3DH_DATARATE_LOWPOWER_1K6HZ: Serial.println("1.6 Khz Low Power"); break;
  }
}