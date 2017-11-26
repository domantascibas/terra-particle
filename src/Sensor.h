#ifndef SENSOR_H
#define SENSOR_H

#include "Particle.h"
#include "Adafruit_DHT.h"

struct Sensor {
  Sensor(uint8_t, uint8_t, String);
  void start();
  float getTemperature();
  float getHumidity();

  private:
  uint8_t sensorType;
  String externalId;
  DHT dht;
};

#endif
