#include <time.h>
#include <Arduino.h>
#include <vector>
#include "acel_control.hpp"
#include "led_control.hpp"
using namespace std;

#define side 4

void displayGrid();
void spawnNumber();
bool move(Direction dir);
void startGame();
bool checkGameOver();

uint16_t tablero[side*side]; //current board
uint16_t last_tablero[side*side]; //last board
uint8_t num_empty_cells = 0;
uint8_t empty_cells[side*side]; //empty cells indexes
bool anyEmpties = true;
bool hasLost;
bool hasWon;
uint16_t victoryNumber = 64;

void setup(void) {
  Serial.begin(115200);
  setCpuFrequencyMhz(80);
  setupAccel();
  test_colors();
  startGame();
}

void loop() {

  accel_x = event.acceleration.x;
  accel_y = event.acceleration.y;

  procesar_movimiento = (process_direction != NONE && !procesar_movimiento && movimiento_detectado);

  if (hasLost) {
    Serial.printf("You lost!\n");
    delay(1000);
    startGame();
  } else if (hasWon) {
    Serial.printf("You won!\n");
    delay(1000);
    startGame();
  } else if (procesar_movimiento) {
    bool hasMoved;
    Serial.printf("Dir: %s\n", Directions[process_direction]);

    if (procesar_movimiento) {
      hasMoved = move(process_direction);
      if (anyEmpties == false) {
        hasLost = checkGameOver();
        if (hasLost) {
          Serial.printf("Game Over!\n");
        }
      }
      if (hasMoved) {
        spawnNumber();
      }
      process_direction = NONE;
    }
    displayGrid();
  }

  delayMicroseconds(100);
}

void displayGrid() {
  for (int i = 0; i < side; i++) {
    for (int j = 0; j < side; j++) {
      Serial.printf("%5d ", tablero[i*side+j]);
    }
    Serial.println();
  }
  Serial.printf("(r) restart\n\n");
}

void spawnNumber() {
  // first scan empty cells
  num_empty_cells = 0;
  fill(empty_cells, empty_cells+side*side, 0);
  for (int i = 0; i < side*side; i++) {
    if (tablero[i] == 0) {
      empty_cells[num_empty_cells] = i;
      num_empty_cells++;
    }
  }

  if (num_empty_cells == 0) {
    Serial.printf("ERROR: No se ha podido spawnear!!!\n");
  } else {
    int random_cell = rand() % num_empty_cells;
    tablero[empty_cells[random_cell]] = 2;
  }

}

void fill (uint8_t* first, uint8_t* last, uint8_t val) {
  while (first != last) {
    *first = val;
    ++first;
  }
}

bool move(Direction dir) {
  bool hasMoved = false;
  copy(begin(tablero), end(tablero), begin(last_tablero));

  uint8_t ini_cell;
  int8_t offset;
  uint8_t next_line_index;

  switch (dir) {
    case DOWN:
      ini_cell = side * (side - 1);
      offset = -side;
      next_line_index = 1;
      break;
    case UP:
      ini_cell = 0;
      offset = side;
      next_line_index = 1;
      break;
    case RIGHT:
      ini_cell = side-1;
      offset = -1;
      next_line_index = side;
      break;
    case LEFT:
      ini_cell = 0;
      offset = +1;
      next_line_index = side;
      break;
  }

  // Particionamiento
  vector<vector<uint16_t>> tablero_temp;
  for (uint8_t i = 0; i < side; i++) {
    vector<uint16_t> linea;
    for (uint8_t j = 0; j < side; j++) {
      uint8_t cell_index = ini_cell + i * next_line_index + j*offset;
      uint16_t cell_value = tablero[cell_index];
      // Serial.printf("%2d: %2d, ", cell_index, cell_value);

      if (cell_value != 0) {
        linea.push_back(cell_value);
      }
    }
    tablero_temp.push_back(linea);
    // cout << endl;
  }

  // Movimiento
  for (uint8_t j = 0; j < tablero_temp.size(); j++) {
    uint16_t val1=0;
    vector<uint16_t> linea_temp;
    for (uint8_t i = 0; i < tablero_temp[j].size(); i++) {
      vector<uint16_t> linea = tablero_temp[j];
      uint16_t val = linea[i];
      if (linea.size() > 1) {
        if (val1 == 0) {
          val1 = val;
        } else {
          if (val1 == val) {
            linea_temp.push_back(val1*2);
            if (val1*2 == victoryNumber) {
              hasWon = true;
              Serial.printf("Has ganado!!!\n");
            }
            val1 = 0;
          } else {
              linea_temp.push_back(val1);
              val1 = val;
          }
        }
      } else {
        linea_temp.push_back(val);
        val1 = 0;
      }
    }

    if (val1 != 0) {
        linea_temp.push_back(val1);
    }
  
    tablero_temp[j] = linea_temp;
  }

  //Reconstruccion
  anyEmpties = false;
  for (uint8_t i = 0; i < side; i++) {
    for (uint8_t j = 0; j < side; j++) {
      uint8_t cell_index = ini_cell + i * next_line_index + j*offset;
      // Serial.printf("%2d ", cell_index);
      if (j < tablero_temp[i].size()) {
        tablero[cell_index] = tablero_temp[i][j];
      } else {
        tablero[cell_index] = 0;
        anyEmpties = true;
      }
    }
  }

  // comprobar si se ha movido
  for (uint8_t i = 0; i < side*side; i++) {
    if (tablero[i] != last_tablero[i]) {
      hasMoved = true;
      break;
    }
  }
  return hasMoved;
}

void startGame() {
  hasLost = false;
  hasWon = false;
  srand((unsigned)time(NULL));
  fill(tablero, tablero+side*side, 0);
  spawnNumber();
  spawnNumber();
  displayGrid();
}

bool checkGameOver() {
  for (int i = 0; i < side; i++) {
    // rows
    for (int j = 0; j < side-1; j++) {
      if (tablero[i*side+j] == tablero[i*side+j+1]) {
        return false;
      }
    }
    // columns
    for (int j = 0; j < side-1; j++) {
      if (tablero[j*side+i] == tablero[(j+1)*side+i]) {
        return false;
      }
    }
    return true;
  }
  return true; // para evitar warning
}