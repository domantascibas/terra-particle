#include "LcdController.h"

/* Consts */
static const uint8_t LCD_ADDRESS = 0x3F;
static const uint8_t LCD_WIDTH = 16;
static const uint8_t LCD_HEIGHT = 2;

/* Object instances */
LiquidCrystal_I2C *lcd;

/* Constructor */
LcdController::LcdController(uint16_t refreshRate) : updater(refreshRate, &LcdController::updateScreen, *this) {
  lcd = new LiquidCrystal_I2C(LCD_ADDRESS, LCD_WIDTH, LCD_HEIGHT);
}

/* Initializer */
void LcdController::init() {
  lcd->init();
  lcd->clear();
  lcd->backlight();
  updater.start();
}

/* Object functions */
void LcdController::updateScreen() {
  lcd->setCursor(0, 0);
  lcd->print(Time.format(Time.now(), "%H:%M:%S"));
}
