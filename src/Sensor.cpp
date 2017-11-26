#include "Sensor.h"

Sensor::Sensor(uint8_t pin, uint8_t type, String id) : dht(pin, type) {
  sensorType = type;
  extrnalId = id;
}

float Sensor::getTemperature() {
  return dht.getTempCelcius();
}

float Sensor::getHumidity() {
  return dht.getHumidity();
}