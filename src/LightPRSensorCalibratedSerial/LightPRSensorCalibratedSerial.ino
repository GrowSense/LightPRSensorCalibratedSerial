#include <Arduino.h>
#include <EEPROM.h>

#include <duinocom2.h>

#include "Common.h"
#include "LightPRSensor.h"
#include "SerialOutput.h"
#include "Commands.h"
#include "DeviceName.h"

void setup()
{
  Serial.begin(9600);

  Serial.println("Starting light (photoresistor) monitor");
  
  loadDeviceNameFromEEPROM();

  serialPrintDeviceInfo();
  
  setupLightPRSensor();

  serialOutputIntervalInSeconds = lightPRSensorReadingIntervalInSeconds;
  
  Serial.println("Online");
}

void loop()
{
  loopNumber++;

  serialPrintLoopHeader();

  checkCommand();

  takeLightPRSensorReading();

  serialPrintData();

  serialPrintLoopFooter();

  delay(1);
}
