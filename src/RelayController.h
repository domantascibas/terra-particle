#ifndef RELAY_CONTROLLER_H
#define RELAY_CONTROLLER_H

#include "Particle.h"

struct RelayController {
  /* Constructor */
  RelayController();

  /* Public variables */

  /* Public functions */
  void init();
  void turnOn(uint8_t);
  void turnOff(uint8_t);
  void allOn();
  void allOff();
  uint8_t getDevicesOn();

  private:
  /* Private objects */

  /* Private variables */
  uint8_t devicesOn;

  /* Private functions */
  void updateShiftRegister();
};

#endif
