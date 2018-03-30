#include "LcdController.h"

LiquidCrystal_I2C *lcd;

LcdController::LcdController(uint16_t refreshRate) : updater(refreshRate, &LcdController::updateScreen, *this) {
  // LCD address 0x3F
  lcd = new LiquidCrystal_I2C(0x3F, 16, 2);
}

void LcdController::init() {
  lcd->init();
  lcd->clear();
  lcd->backlight();
  updater.start();
}

void LcdController::updateScreen() {
  lcd->setCursor(0, 0);
  lcd->print(Time.format(Time.now(), "%H:%M:%S"));
}
