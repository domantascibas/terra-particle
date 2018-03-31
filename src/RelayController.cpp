#include "RelayController.h"
/* 
Added 10uF caps on Vcc for 595 and ULN2803. Could use 100nF instead
Additionally added 1uF cap on latch pin (p12/D4) to filter AC noise
 */

/* 
  #   Device          |Voltage  |
 ---------------------|---------|
  0 - UV Lamp         |220      |
  1 - Light           |220      |
  2 - Infra-Red Lamp  |220      |
  3 - Heater Rock     |220      |
  4 - Heated Cable    |220      |
  5 - NC              |220      |
  6 - Fogger          |220      |
  7 - Ventilators     |12       |
 */

/* Consts */
static const uint8_t TERRA_MOSI = D2;     // MOSI   D2 - p14
static const uint8_t TERRA_SCK = D4;      // SCK    D4 - p11
static const uint8_t TERRA_SS = D5;       // SS     D5 - p12

/* Global variables */

/* Constructor */
RelayController::RelayController() {
  pinMode(TERRA_MOSI, OUTPUT);
  pinMode(TERRA_SCK, OUTPUT);
  pinMode(TERRA_SS, OUTPUT);
}

/* Initializer */
void RelayController::init() {

}

/* Object functions */
void RelayController::updateShiftRegister() {
  digitalWrite(TERRA_SS, LOW);
  shiftOut(TERRA_MOSI, TERRA_SCK, MSBFIRST, devicesOn);
  digitalWrite(TERRA_SS, HIGH);
}

void RelayController::turnOn(uint8_t id) {
  devicesOn = devicesOn | (0x01 << id);
  updateShiftRegister();
}

void RelayController::turnOff(uint8_t id) {
  devicesOn = devicesOn & ~(0x01 << id);
  updateShiftRegister();
}

void RelayController::allOn() {
  devicesOn = 0xFF;
  updateShiftRegister();
}

void RelayController::allOff() {
  devicesOn = 0x00;
  updateShiftRegister();
}

uint8_t RelayController::getDevicesOn() {
  return devicesOn;
}