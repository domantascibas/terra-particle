#ifndef SENSOR_CONTROLLER_H
#define SENSOR_CONTROLLER_H

#include "Particle.h"

struct SensorController {
  /* Constructor */
  SensorController();

  /* Public variables */

  /* Public functions */
  void init();
  float getTemperature();
  float getHumidity();
  bool isNewDataAvailable();
  uint8_t getSensorCount();

  private:
  /* Private objects */
  Timer updater;

  /* Private variables */
  uint8_t sensorCount;
  bool isFirst;
  bool newDataAvailable;
  float avgTemperature;
  float avgHumidity;

  /* Private functions */
  void update();

};

#endif
