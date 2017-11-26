#ifndef SENSOR_H
#define SENSOR_H

#include "Particle.h"
#include "Adafruit_DHT.h"

struct Sensor {
  Sensor(uint8_t, uint8_t, String);
  void init();
  bool isPresent();
  bool isNewDataAvailable();
  float getTemperature();
  float getHumidity();

  private:
  void update();
  float temperature;
  float humidity;
  bool present;
  bool newDataAvailable;
  bool sensorBusy;
  uint8_t sensorType;
  String externalId;
  DHT dht;
  Timer updater;
};

#endif
