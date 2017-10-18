#include <DallasTemperature.h>

#define ONE_WIRE_BUS 6
#define BUZZER 5
#define DEBUG false

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tsaddress;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  Serial.begin(9600);
  sensors.begin();
  debug("" + sensors.getDeviceCount());
  debugln(" Geraete am Bus.");
  sensors.getAddress(tsaddress,0);
}

void loop() {
  float temp = getTemperature();
  
  Serial.println("act temp: "+floatToString(temp));
  
  
  
  if (temp > 27.0){
    Serial.println("Temperature too HIGH!");
    setAlarm();
  }
  
  else if (temp < 20.0){
    debugln("Temperature too LOW!");
    setAlarm();
  }
  else {
    debugln("no Alarm");
    resetAlarm();
  }
  
  
  delay(1000);
}


// set Alarm
void setAlarm(){
  digitalWrite(LED_BUILTIN, true);
  digitalWrite(BUZZER, true);
}

void resetAlarm(){
  digitalWrite(LED_BUILTIN, false);
  digitalWrite(BUZZER, false);
}



// Temperature

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



