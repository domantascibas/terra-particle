#ifndef LCD_CONTROLLER_H
#define LCD_CONTROLLER_H

#include "LiquidCrystal_I2C_Spark.h"

struct LcdController {
  LcdController();
  void init();
  void updateScreen(float, uint8_t);
};

#endif
