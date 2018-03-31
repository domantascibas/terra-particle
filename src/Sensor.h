#ifndef SENSOR_H
#define SENSOR_H

#include "Adafruit_DHT.h"

struct Sensor {
  /* Constructor */
  Sensor(uint8_t, uint8_t);

  /* Public variables */

  /* Public functions */
  void init();
  bool isPresent();
  float getTemperature();
  float getHumidity();

  private:
  /* Private objects */
  DHT dht;

  /* Private variables */
  bool present;
  uint8_t sensorType;
  
  /* Private functions */
};

#endif
