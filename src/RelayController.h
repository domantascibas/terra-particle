#ifndef RELAY_CONTROLLER_H
#define RELAY_CONTROLLER_H

#include "Particle.h"

struct RelayController {
  RelayController();
  void init();
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
