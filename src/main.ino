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

LcdController lcdController(1000);

/*
          1 2 3 4 5 6 7 8
Hour      [][][][][][][][]  int
Minute    [][][][][][][][]  int
NextState [][][][][][][][]  1/0

Index number gives the left shift amount (0x01 << index);
*/

void setup(void) {
  Time.zone(3);
  lcdController.init();
  // BMP180 address 0x77

}

void loop(void) {
}
