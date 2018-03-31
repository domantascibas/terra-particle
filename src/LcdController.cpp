#include "LcdController.h"

/* Consts */
static const uint8_t LCD_ADDRESS = 0x3F;
static const uint8_t LCD_WIDTH = 16;
static const uint8_t LCD_HEIGHT = 2;
static const uint16_t LCD_REFRESH_INTERVAL = 1000;

/* Object instances */
LiquidCrystal_I2C *lcd;

/* Public functions */
/* Constructor */
LcdController::LcdController() : updater(LCD_REFRESH_INTERVAL, &LcdController::updateScreen, *this) {
  lcd = new LiquidCrystal_I2C(LCD_ADDRESS, LCD_WIDTH, LCD_HEIGHT);
}

/* Initializer */
void LcdController::init() {
  lcd->init();
  lcd->clear();
  lcd->backlight();
  updater.start();
}

/* Private functions */
void LcdController::updateScreen() {
  __disable_irq();
  lcd->setCursor(0,0);
  lcd->print(Time.format(Time.now(), "%H:%M:%S"));
  __enable_irq();


}
