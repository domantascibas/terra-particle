#include "Sensor.h"

Sensor::Sensor(uint8_t pin, uint8_t type) : dht(pin, type) {
  sensorType = type;
  present = false;
}

void Sensor::init() {
  dht.begin();
}

// void Sensor::update() {
//   __disable_irq();
//   temperature = dht.getTempCelcius();
//   humidity = dht.getHumidity();
//   if(!isnan(temperature) && !isnan(humidity)) present = true;
//   else present = false;
//   newDataAvailable = true;
//   __enable_irq();
// }

float Sensor::getTemperature() {
  return dht.getTempCelcius();
}

float Sensor::getHumidity() {
  return dht.getHumidity();
}

bool Sensor::isPresent() {
  return present;
}

// bool Sensor::isNewDataAvailable() {
//   return newDataAvailable;
// }