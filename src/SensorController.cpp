#include "SensorController.h"

/*
  Sensor 1: far side from control box, on the shelf
  Sensor 2: far side from control box, in the rocks
  Sensor 3: on the wall next to the control box
  Sensor 4: inside the carcass above the mid-light
 */

// Sensor s1(TEMP1, DHT22, "1UD86RVLUEi85Tv04hmAOhgg06IS");
// Sensor s2(TEMP1, DHT22, "74Ra78275x4Im3v88Vy3038U5gq1");
// Sensor s3(TEMP1, DHT11, "hD3oT6kuJoVINCSzLL34V2z3v0T6");
// Sensor s4(TEMP1, DHT11, "");

SensorController::SensorController() {
  sensorCount = 0;
  isFirst = true;
  avgHumidity = 0.00;
  avgTemperature = 0.00;
}

void SensorController::init() {
  // s1.init();
  // s2.init();
  // s3.init();
  // s4.init();
}

float SensorController::update() {
  // float s1t, s2t, s3t, sum;
  
  // sum = 0;
  // s1t = s1.getTemperature();
  // s2t = s2.getTemperature();
  // s3t = s3.getTemperature();
  return 0.0;
}

float SensorController::getTemperature() {
  // if(s1.isPresent()) {
  //   avgTemperature = s1.getTemperature();
  //   sensorCount = 1;
  // }
  return avgTemperature;
}

uint8_t SensorController::getSensorCount() {
  return sensorCount;
}