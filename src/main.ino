#include "Particle.h"
#include "Adafruit_DHT.h"
#include "LcdController.h"

/*

Project parts:
- Relay controller (controls relays)
- LCD controller (controls lcd display)
- Sensor controller (controls and reads sensor data)
- Terra controller (connects relays with sensors)
- Wifi controller (allows to switch on/off devices over wifi)

*/

/*
Index number gives the left shift amount (0x01 << index);
          1 2 3 4 5 6 7 8
Hour      [][][][][][][][]  int
Minute    [][][][][][][][]  int
NextState [][][][][][][][]  bool
*/

/* Consts */
// BMP180 address 0x77
static const uint8_t TIME_ZONE = 3;
static const uint16_t LCD_REFRESH_INTERVAL = 1000;

/* Object instances */
LcdController lcdController(LCD_REFRESH_INTERVAL);

/* Function definitions */


/* Function implementations */
void setup(void) {
  Time.zone(TIME_ZONE);
  lcdController.init();
}

void loop(void) {
}
