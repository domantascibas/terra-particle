#ifndef SENSOR_CONTROLLER_H
#define SENSOR_CONTROLLER_H

#include "Sensor.h"

// Three DHT22 sensors connected to pins A4-6
// One DHT11 sensor connected to pin A7
#define TEMP1 A4
#define TEMP2 A5
#define TEMP3 A6
#define TEMP4 A7

struct SensorController {
  SensorController();
  void init();
  float update();
  uint8_t getSensorCount();
  float getTemperature();

  private:
  uint8_t sensorCount;
  bool isFirst;
  float avgTemperature;
  float avgHumidity;
};

#endif
