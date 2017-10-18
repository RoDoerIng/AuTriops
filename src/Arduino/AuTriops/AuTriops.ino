#include <DallasTemperature.h>

#define ONE_WIRE_BUS 6
#define BUZZER 5
#define QUIT_BUTTON 2
#define HEATING 1
#define HIGH_TEMP 27.0
#define LOW_TEMP 20.0
#define HEAT_LOW_TEMP 23
#define HEAT_HIGH_TEMP 25
#define DEBUG false
#define INFO true

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tsaddress;

float globalTemp;
String globalTempString;
bool globalQuit;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(QUIT_BUTTON, INPUT_PULLUP);
  pinMode(HEATING, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(QUIT_BUTTON), acknowledge, FALLING);
  Serial.begin(9600);
  sensors.begin();
  debug("" + sensors.getDeviceCount());
  debugln(" Geraete am Bus.");
  sensors.getAddress(tsaddress,0);
}

void loop() {
  globalTemp = getTemperature();
  globalTempString = floatToString(globalTemp);
  
  //Serial.println("act temp: " + globalTempString);//+floatToString(globalTemp));
  
  alarmController();
  
  ackMonitor();

  temperatureMonitor();
  //Serial.println("temp mon");

  heatingController();

  delay(1000);
}


// Heating

void heatingController(){
  if (globalTemp > HEAT_HIGH_TEMP){
    heatingOff();
    infoln("heating off");
  }
  if (globalTemp < HEAT_LOW_TEMP){
    heatingOn();
    infoln("heating on");
  }

/*
  if (isHeatingOn()){
    infoln("heating on");
  }
  else{
    infoln("heating off");
  }
  */
}

void heatingOn(){
  digitalWrite(HEATING, true);
}

void heatingOff(){
  digitalWrite(HEATING, false);
}

bool isHeatingOn(){
  return digitalRead(HEATING);
}

void alarmController(){
  if (!isTempOk() && !isQuit()){
    setAlarm();
  }
  else {
    debugln("no Alarm");
    resetAlarm();
  }
}

// set Alarm
void setAlarm(){
  digitalWrite(LED_BUILTIN, true);
  tone(BUZZER, 3);
}

void resetAlarm(){
  digitalWrite(LED_BUILTIN, false);
  noTone(BUZZER);
}

// Button

bool isAckButtonPressed(){
  return !digitalRead(QUIT_BUTTON);
}

// Acknowledge

void ackMonitor(){
  if (isAckButtonPressed()){
    setQuit();
  }

  if (isTempOk()){
    resetQuit();
  }

  if (isQuit()){
    debugln("quittiert!");
  }
}

void acknowledge(){
  setQuit();
  resetAlarm();
}

void setQuit(){
  globalQuit = true;
}

void resetQuit(){
  globalQuit = false;
}

bool isQuit(){
  return globalQuit;
}

// Temperature

void temperatureMonitor(){
  if (!isTempOk()){
    infoln("Temperature out of Range! " + globalTempString);
  }
  else{
    info("Temperature OK! ");
    infoln(globalTempString);
  }
}

bool isTempOk(){
  if (globalTemp <= HIGH_TEMP && globalTemp >= LOW_TEMP){
    return true;
  }
  else{
    return false;
  }
}

float getTemperature(){
  float temp;
  if(sensors.isConnected(tsaddress)) {
    debug("Requesting temperature...");
    sensors.requestTemperaturesByAddress(tsaddress);
    debugln("DONE");
    debug("Temperature for the device is: ");
    temp = sensors.getTempC(tsaddress);
    debugln(floatToString(temp));
  } 
  else {
    debugln("Nicht angeschlossen.");
  }
  return temp;
}


// Conversion

String floatToString(float num) {
  char str[5];
  dtostrf(num,5, 2, str);
  //Serial.println(str);
  String out((char*)str);
  return out;
}


// Printing


// Debugging

void debug(String message){
  if (DEBUG){
    Serial.print(message);
  }
}

void debugln(String message){
  if (DEBUG){
    Serial.println(message);
  }
}


// Info
void info(String message){
  if (INFO){
    Serial.print(message);
  }
}


void infoln(String message){
  if (INFO){
    Serial.println(message);
  }
}

