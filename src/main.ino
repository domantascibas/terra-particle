#include "Particle.h"
#include "LcdController.h"
#include "SensorController.h"

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

/* Object instances */
LcdController lcdControl;
SensorController sensorControl;

/* Variables */
float temperature;
float humidity;

/* Function definitions */


/* Function implementations */
void setup(void) {
  Time.zone(TIME_ZONE);
  lcdControl.init();
  sensorControl.init();
}

void loop(void) {
  if(sensorControl.isNewDataAvailable()) {
    temperature = sensorControl.getTemperature();
    humidity = sensorControl.getHumidity();
    Particle.publish("terra/temperature", String(temperature));
    Particle.publish("terra/humidity", String(humidity));
  }

  delay(1000);
}
