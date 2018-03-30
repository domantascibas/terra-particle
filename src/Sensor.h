#ifndef SENSOR_H
#define SENSOR_H

#include "Particle.h"
#include "Adafruit_DHT.h"

struct Sensor {
  /* Constructor */
  Sensor(uint8_t, uint8_t, String);

  /* Public variables */

  /* Public functions */
  void init();
  bool isPresent();
  bool isNewDataAvailable();
  float getTemperature();
  float getHumidity();

  private:
  /* Private objects */
  DHT dht;
  Timer updater;

  /* Private variables */
  float temperature;
  float humidity;
  bool present;
  bool newDataAvailable;
  bool sensorBusy;
  uint8_t sensorType;
  String externalId;
  
  /* Private functions */
  void update();
};

#endif
