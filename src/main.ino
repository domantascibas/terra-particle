#include "Particle.h"
#include "Adafruit_DHT.h"
#include "RelayController.h"
#include "LcdController.h"
#include "SensorController.h"

/*

Project parts:
- Relay controller (controls relays)
- LCD controller (controls lcd display)
- Sensor controller (controls and reads sensor data)
- Terra controller (connects relays with sensors)
- Wifi controller (allows to switch on/off devices over wifi)

*/

enum DeviceId {
  UV_LIGHT = 0x01,
  REGULAR_LIGHT = 0x02,
  IR_LIGHT = 0x04,
  HEATED_ROCK = 0x08,
  HEATED_CABLE = 0x10,
  NC = 0x20,
  FOGGER = 0x40,
  VENTILATORS = 0x80
};

// BMP baro();
RelayController relayController;
SensorController sensorController;
LcdController lcdController;
Timer timer(1000, &timerIsr);
// Timer sensorUpdater(5000, &sensorIsr);

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
    checkAutoTimerFlag = 1;

    if (currMinute == 0) {
      currHour = Time.hour();
    }
  }
  updateSensorFlag = 1;
  lcdController.updateScreen(0.00, 0);
}

void sensorIsr() {
  currTemp = sensorController.update();
}

int getRelaysFromServer(String command) {
  devices = atoi(command);
  updateRelaysFlag = 1;
  return 0;
}

int getNextStatesFromServer() {
  return 0;
}

int sendStatesToServer(const char *deviceId, int state) {
  sendStatesToServerFlag = 0;
  /*snprintf(buffer, sizeof(buffer), "", deviceId, state);*/
  snprintf(buffer, sizeof(buffer),"{\"devicesState\":%d}",devices);
  Particle.publish(PUBLISH_DEVICES_STATE, buffer, PRIVATE);
  return 0;
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

void checkAutoTimer() {
  checkAutoTimerFlag = 0;
  int pushUpdates = 0;
  int i;
  // make sure that it only toggles the 'i'-th device on currHour and currMinute
  for(i=0; i<8; i++) {
    if((autoTimerOn[i][0] == currHour) && (autoTimerOn[i][1] == currMinute)) {
      int turnOn = autoTimerOn[i][2];
      if(turnOn) {
        relayController.turnOn(i);
      } else {
        relayController.turnOff(i);
      }
      pushUpdates = 1;
    }

    if((autoTimerOff[i][0] == currHour) && (autoTimerOff[i][1] == currMinute)) {
      int turnOn = autoTimerOff[i][2];
      if(turnOn) {
        relayController.turnOn(i);
      } else {
        relayController.turnOff(i);
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
    // TODO: fix startup code. current checks aren't accurate enough
    if(autoTimerOn[i][0] < autoTimerOff[i][0]) {
      if((autoTimerOn[i][0] < currHour) && (currHour < autoTimerOff[i][0])) {
        relayController.turnOn(i);
      } else {
        relayController.turnOff(i);
      }
    } else if(autoTimerOn[i][0] > autoTimerOff[i][0]) {
      if((autoTimerOn[i][0] < currHour) || (currHour < autoTimerOff[i][0])) {
        relayController.turnOn(i);
      } else {
        relayController.turnOff(i);
      }
    } else {
      int turnOn = autoTimerOn[i][2];
      if(turnOn) {
        relayController.turnOn(i);
      } else {
        relayController.turnOff(i);
      }
    }
  }
  updateRelaysFlag = 1;
}

void setup(void) {
  lcdController.init();
  // BMP180 address 0x77

  Time.zone(3);

  currHour = Time.hour();
  currMinute = Time.minute();
  currSecond = Time.second();
  lastMinute = currMinute;

  //UV ON
  autoTimerOn[0][0] = 9;
  autoTimerOn[0][1] = 51;
  autoTimerOn[0][2] = true;
  //Daylight ON
  autoTimerOn[1][0] = 9;
  autoTimerOn[1][1] = 10;
  autoTimerOn[1][2] = true;
  //IR ON
  autoTimerOn[2][0] = 0;
  autoTimerOn[2][1] = 0;
  autoTimerOn[2][2] = true;
  //Rock ON
  autoTimerOn[3][0] = 16;
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
  autoTimerOff[1][0] = 17;
  autoTimerOff[1][1] = 30;
  autoTimerOff[1][2] = false;
  //IR OFF
  autoTimerOff[2][0] = 0;
  autoTimerOff[2][1] = 0;
  autoTimerOff[2][2] = true;
  //Rock OFF
  autoTimerOff[3][0] = 10;
  autoTimerOff[3][1] = 10;
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

  // // Sensor1.begin();
  // s1.start();
  // s2.start();
  // s3.start();
  // s4.start();
  //bmp.begin();

  Particle.function("relayUpdate", getRelaysFromServer);

  /*Particle.subscribe("hook-response/terario-devices", myHandler, MY_DEVICES);*/
  /*Particle.publish("terario-devices", "", PRIVATE);*/

  timer.start();
  // sensorUpdater.start();
}

void loop(void) {
  // if(updateSensorFlag != 0) {
  //   updateSensor(updateSensorFlag);
  // }
  updateSensorFlag = 0;

  if(sendDataFlag != 0) {
    sendDataToServer(sendDataFlag);
  }

  if(checkAutoTimerFlag == 1) {
    checkAutoTimer();
    Particle.publish("terra/devicesOn", String(relayController.getDevicesOn()));
  }
}
