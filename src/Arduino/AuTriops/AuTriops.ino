#include <DallasTemperature.h>

#define ONE_WIRE_BUS 6
#define BUZZER 5
#define QUIT_BUTTON 2
#define HIGH_TEMP 27.0
#define LOW_TEMP 20.0
#define DEBUG false

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tsaddress;

float temp;
bool globalQuit;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(QUIT_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(QUIT_BUTTON), setQuit, FALLING);
  Serial.begin(115200);
  sensors.begin();
  debug("" + sensors.getDeviceCount());
  debugln(" Geraete am Bus.");
  sensors.getAddress(tsaddress,0);
}

void loop() {
  temp = getTemperature();
  
  Serial.println("act temp: "+floatToString(temp));
  
  alarmController();
  
  ackMonitor();

  //temperatureMonitor();
  //Serial.println("temp mon");

  delay(1000);
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
    Serial.println("Temperature out of Range! " + floatToString(temp));
  }
  else{
    Serial.print("Temperature OK! ");
    Serial.println(floatToString(temp));
  }
}

bool isTempOk(){
  if (temp <= HIGH_TEMP && temp >= LOW_TEMP){
    debugln("Temp OK");
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
  return str;
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



