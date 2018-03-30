#ifndef LCD_CONTROLLER_H
#define LCD_CONTROLLER_H

#include "LiquidCrystal_I2C_Spark.h"

struct LcdController {
  LcdController(uint16_t);
  void init();
  void updateScreen();
  
  private:
  Timer updater;
};

#endif
