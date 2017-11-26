#ifndef RELAY_CONTROLLER_H
#define RELAY_CONTROLLER_H

#include "Particle.h"

enum DeviceId {
  UV_LIGHT = 0x01,
  REGULAR_LIGHT = 0x02,
  IR_LIGHT = 0x04,
  HEATED_ROCK = 0x08,
  HEATED_CABLE = 0x10,
  NC = 0x20,
  FOGGER = 0x40,
  VENTILATORS = 0x80
};

struct RelayController {
  RelayController();
  void turnOn(uint8_t);
  void turnOff(uint8_t);
  void allOn();
  void allOff();
  uint8_t getDevicesOn();

  private:
  uint8_t devicesOn;
  void updateShiftRegister();
};

#endif
