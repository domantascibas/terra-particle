#include "Particle.h"
#include "Adafruit_DHT.h"
#include "LcdController.h"

/*

Project parts:
- Relay controller (controls relays)
- LCD controller (controls lcd display)
- Sensor controller (controls and reads sensor data)
- Terra controller (connects relays with sensors)
- Wifi controller (allows to switch on/off devices over wifi)

*/

#define LED D7
//#define LAMP D4
//#define IRLAMP D5

// Three DHT22 sensors connected to pins A4-6
// One DHT11 sensor connected to pin A7
#define TEMP1 A4
#define TEMP2 A5
#define TEMP3 A6
#define TEMP4 A7

// Sensor 1: far side from control box, on the shelf
// Sensor 2: far side from control box, in the rocks
// Sensor 3: on the wall next to the control box
// Sensor 4: inside the carcass above the mid-light
DHT Sensor1(TEMP1, DHT22);
DHT Sensor2(TEMP2, DHT22);
DHT Sensor3(TEMP3, DHT11);
DHT Sensor4(TEMP4, DHT11);
// BMP baro();

LcdController lcdController;
Timer timer(1000, &timerIsr);

int counter = 0;

// Automatic timer for device switching
int autoTimerOn[8][3];
int autoTimerOff[8][3];
/*
          1 2 3 4 5 6 7 8
Hour      [][][][][][][][]  int
Minute    [][][][][][][][]  int
NextState [][][][][][][][]  1/0

Index number gives the left shift amount (0x01 << index);
*/

// get init value for devices from server
int devices = 0x00;

int currHour = 0;
int currMinute = 0;
int currSecond = 0;
int lastMinute = 0;

// Update flags
int updateLcdFlag = 0;
int updateRelaysFlag = 0;
int updateSensorFlag = 0;
int updateBaroFlag = 0;
int sendDataFlag = 0;
int startSendingDataFlag = 0;
int getRelaysFromServerFlag = 0;
int checkAutoTimerFlag = 0;
int sendStatesToServerFlag = 0;

int sensorError = 0;

// Sensor readings
float s1t = 0;
float s2t = 0;
float s3t = 0;
float s4t = 0;

float s1h = 0;
float s2h = 0;
float s3h = 0;
float s4h = 0;

float baroT = 0;
float baroP = 0;

// Sensor IDs
const char *SENSOR1_ID = "1UD86RVLUEi85Tv04hmAOhgg06IS";
const char *SENSOR2_ID = "74Ra78275x4Im3v88Vy3038U5gq1";
const char *SENSOR3_ID = "hD3oT6kuJoVINCSzLL34V2z3v0T6";
const char *SENSOR4_ID = "";
const char *BARO_ID = "";

float currHumid = 0;
float currTemp = 0;

int currSensor = 0;

// Server comms variables
char buffer[256];
String received = "";

const char *CHECK_EVENT_NAME = "terario-devices";
const char *PUBLISH_EVENT_NAME = "terario-sensors";
const char *PUBLISH_DEVICES_STATE = "terario-state";
const char *PUBLISH_DEVICE_SINGLE_STATE = "terario-single-state";
//void getDataHandler(const char *topic, const char *data);

void myHandler(const char *topic, const char *data) {
  devices = atoi(data);
  updateRelaysFlag = 1;
}

void timerIsr() {
  // Runs every second
  currSecond = Time.second();
  if (currSecond == 0) {
    currMinute = Time.minute();

    if (currMinute == 0) {
      currHour = Time.hour();
}
  }
  lcdController.updateScreen();
}

void updateSensor(int sensor) {
  updateSensorFlag = 0;
  switch(sensor) {
    case 1:
      s1t = Sensor1.getTempCelcius();
      s1h = Sensor1.getHumidity();

      currTemp = s1t;
      currHumid = s1h;
      currSensor = 1;
      // send sensor vals to server
    break;

    case 2:
      s2t = Sensor2.getTempCelcius();
      s2h = Sensor2.getHumidity();

      currTemp = s2t;
      currHumid = s2h;
      currSensor = 2;
      // send sensor vals to server
    break;

    case 3:
      s3t = Sensor3.getTempCelcius();
      s3h = Sensor3.getHumidity();

      currTemp = s3t;
      currHumid = s3h;
      currSensor = 3;
      // send sensor vals to server
    break;

    case 4:
      s4t = Sensor4.getTempCelcius();
      s4h = Sensor4.getHumidity();

      currTemp = s4t;
      currHumid = s4h;
      currSensor = 4;
      // send sensor vals to server
    break;

    default:
    break;
  }
  if(isnan(currHumid) || isnan(currTemp)) {
    sensorError = 1;
  } else {
    sensorError = 0;
  }
}

int getRelaysFromServer(String command) {
  devices = atoi(command);
  updateRelaysFlag = 1;
}

int getNextStatesFromServer() {

}

int sendStatesToServer(const char *deviceId, int state) {
  sendStatesToServerFlag = 0;
  /*snprintf(buffer, sizeof(buffer), "", deviceId, state);*/
  snprintf(buffer, sizeof(buffer),"{\"devicesState\":%d}",devices);
  Particle.publish(PUBLISH_DEVICES_STATE, buffer, PRIVATE);
}

void sendDataToServer(int sensor) {
  sendDataFlag = 0;
  switch(sensor){
    case 1:
      snprintf(buffer, sizeof(buffer),"{\"data1\":%.2f,\"data2\":%.2f,\"sensor_id\":\"%s\"}", s1t, s1h, SENSOR1_ID);
      Particle.publish(PUBLISH_EVENT_NAME, buffer, PRIVATE);
    break;

    case 2:
      snprintf(buffer, sizeof(buffer),"{\"data1\":%.2f,\"data2\":%.2f,\"sensor_id\":\"%s\"}", s2t, s2h, SENSOR2_ID);
      Particle.publish(PUBLISH_EVENT_NAME, buffer, PRIVATE);
    break;

    case 3:
      snprintf(buffer, sizeof(buffer),"{\"data1\":%.2f,\"data2\":%.2f,\"sensor_id\":\"%s\"}", s3t, s3h, SENSOR3_ID);
      Particle.publish(PUBLISH_EVENT_NAME, buffer, PRIVATE);
    break;

    default:
    break;
  }
}

void updateShiftRegister(int data) {
  digitalWrite(D5, LOW);
  shiftOut(D2, D4, MSBFIRST, data);
  digitalWrite(D5, HIGH);
}

void checkAutoTimer() {
  checkAutoTimerFlag = 0;
  int pushUpdates = 0;
  int i;
  // make sure that it only toggles the 'i'-th device on currHour and currMinute
  for(i=0; i<8; i++) {
    if((autoTimerOn[i][0] == currHour) && (autoTimerOn[i][1] == currMinute)) {
      int turnOn = autoTimerOn[i][2];
      if(turnOn) {
        devices = devices | (0x1 << i);
      } else {
        devices = devices & ~(0x1 << i);
      }
      pushUpdates = 1;
    }

    if((autoTimerOff[i][0] == currHour) && (autoTimerOff[i][1] == currMinute)) {
      int turnOn = autoTimerOff[i][2];
      if(turnOn) {
        devices = devices | (0x1 << i);
      } else {
        devices = devices & ~(0x1 << i);
      }
      pushUpdates = 1;
    }
  }
  // updateDevicesState on the server
  // download new toggle device times from server
  if(pushUpdates) {
    pushUpdates = 0;
    updateRelaysFlag = 1;
    /*sendStatesToServerFlag = 1;*/
  } else {
    pushUpdates = 0;
    updateRelaysFlag = 0;
    /*sendStatesToServerFlag = 0;*/
  }
}

void startUp(void) {
  int i;
  for(i=0; i<8; i++) {
    if(autoTimerOn[i][0] < autoTimerOff[i][0]) {
      if((autoTimerOn[i][0] < currHour) && (currHour < autoTimerOff[i][0])) {
        devices = devices | (0x1 << i);
      } else {
        devices = devices & ~(0x1 << i);
      }
    } else if(autoTimerOn[i][0] > autoTimerOff[i][0]) {
      if((autoTimerOn[i][0] < currHour) || (currHour < autoTimerOff[i][0])) {
        devices = devices | (0x1 << i);
      } else {
        devices = devices & ~(0x1 << i);
      }
    } else {
      int turnOn = autoTimerOn[i][2];
      if(turnOn) {
        devices = devices | (0x1 << i);
      } else {
        devices = devices & ~(0x1 << i);
      }
    }
  }
  updateRelaysFlag = 1;
}

void setup(void) {
  /*Serial.begin(9600);*/

  // BMP180 address 0x77

  Time.zone(2);
//  SPI.begin();
//  SPI.setBitOrder(MSBFIRST);
//  SPI.setClockSpeed(15000000);

  // MOSI   D2 - p14
  // SCK    D4 - p11
  // SS     D5 - p12
  pinMode(D2, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  // Added 10uF caps on Vcc for 595 and ULN2803. Could use 100nF instead
  // Additionally added 1uF cap on latch pin (p12/D4) to filter AC noise

  currHour = Time.hour();
  currMinute = Time.minute();
  currSecond = Time.second();
  lastMinute = currMinute;

  //UV ON
  autoTimerOn[0][0] = 10;
  autoTimerOn[0][1] = 30;
  autoTimerOn[0][2] = true;
  //Daylight ON
  autoTimerOn[1][0] = 8;
  autoTimerOn[1][1] = 10;
  autoTimerOn[1][2] = true;
  //IR ON
  autoTimerOn[2][0] = 0;
  autoTimerOn[2][1] = 0;
  autoTimerOn[2][2] = true;
  //Rock ON
  autoTimerOn[3][0] = 17;
  autoTimerOn[3][1] = 30;
  autoTimerOn[3][2] = true;
  //Cable ON
  autoTimerOn[4][0] = 0;
  autoTimerOn[4][1] = 0;
  autoTimerOn[4][2] = false;
  //-- ON
  autoTimerOn[5][0] = 0;
  autoTimerOn[5][1] = 0;
  autoTimerOn[5][2] = false;
  //Fog ON
  autoTimerOn[6][0] = 0;
  autoTimerOn[6][1] = 0;
  autoTimerOn[6][2] = false;
  //Vents ON
  autoTimerOn[7][0] = 19;
  autoTimerOn[7][1] = 00;
  autoTimerOn[7][2] = false;

  //UV OFF
  autoTimerOff[0][0] = 17;
  autoTimerOff[0][1] = 30;
  autoTimerOff[0][2] = false;
  //Daylight OFF
  autoTimerOff[1][0] = 19;
  autoTimerOff[1][1] = 10;
  autoTimerOff[1][2] = false;
  //IR OFF
  autoTimerOff[2][0] = 0;
  autoTimerOff[2][1] = 0;
  autoTimerOff[2][2] = true;
  //Rock OFF
  autoTimerOff[3][0] = 10;
  autoTimerOff[3][1] = 30;
  autoTimerOff[3][2] = false;
  //Cable OFF
  autoTimerOff[4][0] = 0;
  autoTimerOff[4][1] = 0;
  autoTimerOff[4][2] = false;
  //-- OFF
  autoTimerOff[5][0] = 0;
  autoTimerOff[5][1] = 0;
  autoTimerOff[5][2] = false;
  //Fog OFF
  autoTimerOff[6][0] = 0;
  autoTimerOff[6][1] = 0;
  autoTimerOff[6][2] = false;
  //Vents OFF
  autoTimerOff[7][0] = 19;
  autoTimerOff[7][1] = 01;
  autoTimerOff[7][2] = false;

  startUp();

  Sensor1.begin();
  Sensor2.begin();
  Sensor3.begin();
  Sensor4.begin();
  //bmp.begin();
//  pinMode(LED, OUTPUT);
//  pinMode(LAMP, OUTPUT);
//  pinMode(IRLAMP, OUTPUT);

  Particle.function("relayUpdate", getRelaysFromServer);

  /*Particle.subscribe("hook-response/terario-devices", myHandler, MY_DEVICES);*/
  /*Particle.publish("terario-devices", "", PRIVATE);*/

  timer.start();
}

void loop(void) {
  if(updateRelaysFlag == 1) {
    updateRelays();
  }

  if(updateSensorFlag != 0) {
    updateSensor(updateSensorFlag);
  }

  if(sendDataFlag != 0) {
    sendDataToServer(sendDataFlag);
  }

  if(checkAutoTimerFlag == 1) {
    checkAutoTimer();
  }

  /*if(sendStatesToServerFlag == 1) {
    sendStatesToServer();
  }*/
}
