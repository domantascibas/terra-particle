#include "LcdController.h"
#include "RelayController.h"
#include "Sensor.h"

#define TEMP1 A4
#define TEMP2 A5
#define TEMP3 A6
#define TEMP4 A7

LiquidCrystal_I2C *lcd;
extern RelayController relayController;
Sensor s1(TEMP1, DHT22, "1UD86RVLUEi85Tv04hmAOhgg06IS");

LcdController::LcdController() {
  // LCD address 0x3F
  lcd = new LiquidCrystal_I2C(0x3F, 16, 2);
  lcd->init();
  lcd->clear();
  lcd->backlight();
  s1.init();
}

void LcdController::updateScreen(float val, uint8_t count) {
  static float temp, humid;
  static bool avail;
  __disable_irq();
  if (s1.isNewDataAvailable()) {
    avail = true;
    temp = s1.getTemperature();
    humid = s1.getHumidity();
  } else {
    avail = false;
  }
  __enable_irq();
  
  lcd->setCursor(0, 0);
  lcd->print(Time.format(Time.now(), "%H:%M:%S"));
  lcd->setCursor(15,0);
  lcd->print(avail);
  lcd->setCursor(0,1);
  lcd->print(relayController.getDevicesOn());
  lcd->setCursor(5,1);
  lcd->print(temp);
  lcd->setCursor(11,1);
  lcd->print(humid);
}