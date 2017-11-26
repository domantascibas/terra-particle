#include "LcdController.h"

LiquidCrystal_I2C *lcd;

LcdController::LcdController() {
  // LCD address 0x3F
  lcd = new LiquidCrystal_I2C(0x3F, 16, 2);
  lcd->init();
  lcd->clear();
  lcd->backlight();

  currHour = 0;
  currMinute = 0;
  currSecond = 0;
}

void LcdController::timerIsr() {
  // Runs every second
  currSecond = Time.second();
  if(currSecond == 0) {
    currMinute = Time.minute();

    if (currMinute == 0) {
      currHour = Time.hour();
    }
  }
  updateScreen();
}

void LcdController::updateScreen() {
  lcd->setCursor(0, 0);
  lcd->print(Time.format(Time.now(), "%H:%M:%S"));
}