#include "Sensor.h"

Sensor::Sensor(uint8_t pin, uint8_t type, String id) : dht(pin, type), updater(5000, &Sensor::update, *this) {
  sensorType = type;
  externalId = id;
  present = false;
  newDataAvailable = false;
  temperature = 0.00;
  humidity = 0.00;
}

void Sensor::init() {
  dht.begin();
  updater.start();
}

void Sensor::update() {
  __disable_irq();
  temperature = dht.getTempCelcius();
  humidity = dht.getHumidity();
  if(!isnan(temperature) && !isnan(humidity)) present = true;
  else present = false;
  newDataAvailable = true;
  __enable_irq();
}

float Sensor::getTemperature() {
  __disable_irq();
  if(newDataAvailable) newDataAvailable = false;
  __enable_irq();
  return temperature;
}

float Sensor::getHumidity() {
  __disable_irq();
  if(newDataAvailable) newDataAvailable = false;
  __enable_irq();
  return humidity;
}

bool Sensor::isPresent() {
  return present;
}

bool Sensor::isNewDataAvailable() {
  return newDataAvailable;
}