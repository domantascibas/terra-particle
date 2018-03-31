#ifndef LCD_CONTROLLER_H
#define LCD_CONTROLLER_H

#include "LiquidCrystal_I2C_Spark.h"

struct LcdController {
  /* Constructor */
  LcdController();

  /* Public variables */
  
  /* Public functions */
  void init();
  
  private:
  /* Private objects */
  Timer updater;

  /* Private variables */
  
  /* Private functions */
  void updateScreen();

};

#endif
