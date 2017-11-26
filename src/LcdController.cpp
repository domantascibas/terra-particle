#include "LcdController.h"
#include "RelayController.h"

LiquidCrystal_I2C *lcd;
extern RelayController relayController;

LcdController::LcdController() {
  // LCD address 0x3F
  lcd = new LiquidCrystal_I2C(0x3F, 16, 2);
  lcd->init();
  lcd->clear();
  lcd->backlight();
}

void LcdController::updateScreen() {
  lcd->setCursor(0, 0);
  lcd->print(Time.format(Time.now(), "%H:%M:%S"));
  lcd->setCursor(0,1);
  lcd->print(relayController.getDevicesOn());
}