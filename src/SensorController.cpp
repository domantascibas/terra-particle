#include "SensorController.h"
#include "Adafruit_DHT.h"

/*
  Sensor 1: far side from control box, on the shelf
  Sensor 2: far side from control box, in the rocks
  Sensor 3: on the wall next to the control box
  Sensor 4: inside the carcass above the mid-light
 */

// Three DHT22 sensors connected to pins A4-6
// One DHT11 sensor connected to pin A7

/* Consts */
static const uint8_t SHELF_SENSOR_ADDRESS = A4;
static const uint8_t FLOOR_SENSOR_ADDRESS = A5;
static const uint8_t BRANCH_SENSOR_ADDRESS = A6;
static const uint8_t CEILING_SENSOR_ADDRESS = A7;
static const uint8_t NUMBER_OF_SENSORS = 1;
static const uint16_t SENSOR_REFRESH_INTERVAL = 60000;

/* Global variables */
float temperatureArray[NUMBER_OF_SENSORS];
float humidityArray[NUMBER_OF_SENSORS];

/* Object instances */
DHT shelfSensor(SHELF_SENSOR_ADDRESS, DHT22);
// Sensor floorSensor(FLOOR_SENSOR_ADDRESS, DHT22);
// Sensor branchSensor(BRANCH_SENSOR_ADDRESS, DHT22);
// Sensor ceilingSensor(CEILING_SENSOR_ADDRESS, DHT11);

/* Public functions */
/* Constructor */
SensorController::SensorController() : updater(SENSOR_REFRESH_INTERVAL, &SensorController::update, *this) {
  sensorCount = NUMBER_OF_SENSORS;
  isFirst = true;
  newDataAvailable = false;
  avgHumidity = 0.00;
  avgTemperature = 0.00;
}

/* Initializer */
void SensorController::init() {
  shelfSensor.begin();
  updater.start();
}

float SensorController::getTemperature() {
  return avgTemperature;
}

float SensorController::getHumidity() {
  return avgHumidity;
}

bool SensorController::isNewDataAvailable() {
  bool result = newDataAvailable;
  if(newDataAvailable) {
    newDataAvailable = false;
  }
  return result;
}

uint8_t SensorController::getSensorCount() {
  return sensorCount;
}

/* Private functions */
void SensorController::update() {
  avgTemperature = shelfSensor.getTempCelcius();
  avgHumidity = shelfSensor.getHumidity();
  newDataAvailable = true;
}
