#include "RelayController.h"

// MOSI   D2 - p14
// SCK    D4 - p11
// SS     D5 - p12
#define TERRA_MOSI  D2
#define TERRA_SCK   D4
#define TERRA_SS    D5

RelayController::RelayController() {
  pinMode(TERRA_MOSI, OUTPUT);
  pinMode(TERRA_SCK, OUTPUT);
  pinMode(TERRA_SS, OUTPUT);

  // Added 10uF caps on Vcc for 595 and ULN2803. Could use 100nF instead
  // Additionally added 1uF cap on latch pin (p12/D4) to filter AC noise
}

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