#include <Arduino.h>
#include <EEPROM.h>

#include <duinocom.h>

#include "Common.h"
#include "LightPRSensor.h"

//#define SERIAL_MODE_CALIBRATED 1
//#define SERIAL_MODE_RAW 2
//#define SERIAL_MODE_CSV 3
//#define SERIAL_MODE_QUERYSTRING 4

#define VERSION "1-0-0-0"
#define BOARD_TYPE "uno"

//int serialMode = SERIAL_MODE_CSV;

void setup()
{
  Serial.begin(9600);

  Serial.println("Starting light (photoresistor) monitor");

  serialPrintDeviceInfo();
  
  setupLightPRSensor();

  serialOutputIntervalInSeconds = lightPRSensorReadingIntervalInSeconds;
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

/* Commands */
void checkCommand()
{
  if (isDebugMode)
  {
    Serial.println("Checking incoming serial commands");
  }

  if (checkMsgReady())
  {
    char* msg = getMsg();
        
    char letter = msg[0];

    int length = strlen(msg);

    Serial.print("Received message: ");
    Serial.println(msg);

    switch (letter)
    {
      case '#':
        serialPrintDeviceInfo();
        break;
      case 'D':
        setDarkCalibrationValue(msg);
        break;
      case 'B':
        setBrightCalibrationValue(msg);
        break;
      case 'I':
        setLightPRSensorReadingInterval(msg);
        break;
      case 'X':
        restoreDefaultSettings();
        break;
      case 'R':
        reverseSoilMoistureCalibrationValues();
        break;
      case 'Z':
        Serial.println("Toggling isDebugMode");
        isDebugMode = !isDebugMode;
        break;
    }

    forceSerialOutput();
  }
  delay(1);
}

void serialPrintDeviceInfo()
{
  Serial.println("");
  Serial.println("-- Start Device Info");
  Serial.println("Family: GrowSense");
  Serial.println("Group: monitor");
  Serial.println("Project: LightPRSensorCalibratedSerial");
  Serial.print("Board: ");
  Serial.println(BOARD_TYPE);
  Serial.print("Version: ");
  Serial.println(VERSION);
  Serial.println("ScriptCode: light-pr-monitor");
  Serial.println("-- End Device Info");
  Serial.println("");
}

/* Settings */
void restoreDefaultSettings()
{
  Serial.println("Restoring default settings");

  restoreDefaultLightPRSensorSettings();
}

/* Serial Output */
void serialPrintData()
{
  bool isTimeToPrintData = lastSerialOutputTime + secondsToMilliseconds(serialOutputIntervalInSeconds) < millis()
      || lastSerialOutputTime == 0;

  bool isReadyToPrintData = isTimeToPrintData && lightPRSensorReadingHasBeenTaken;

  if (isReadyToPrintData)
  {
    if (isDebugMode)
      Serial.println("Ready to serial print data");
  
    long numberOfSecondsOnline = millis()/1000;

 //   if (serialMode == SERIAL_MODE_CSV)
 //   {
      Serial.print("D;"); // This prefix indicates that the line contains data.
      //Serial.print("T:");
      //Serial.print(numberOfSecondsOnline);
      //Serial.print(";");
      Serial.print("R:");
      Serial.print(soilMoistureLevelRaw);
      Serial.print(";");
      Serial.print("C:");
      Serial.print(soilMoistureLevelCalibrated);
      Serial.print(";");
      Serial.print("I:");
      Serial.print(lightPRSensorReadingIntervalInSeconds);
      Serial.print(";");
      Serial.print("D:");
      Serial.print(darkCalibrationValue);
      Serial.print(";");
      Serial.print("B:");
      Serial.print(brightCalibrationValue);
      Serial.print(";");
      Serial.print("V:");
      Serial.print(VERSION);
      Serial.print(";;");
      Serial.println();
 /*   }
    else if (serialMode == SERIAL_MODE_QUERYSTRING)
    {
      //Serial.print("time=");
      //Serial.print(numberOfSecondsOnline);
      //Serial.print("&");
      Serial.print("raw=");
      Serial.print(soilMoistureLevelRaw);
      Serial.print("&");
      Serial.print("calibrated=");
      Serial.print(soilMoistureLevelCalibrated);
      Serial.print("&");
      Serial.print("readInterval=");
      Serial.print(lightPRSensorReadingIntervalInSeconds); // Convert to seconds
      Serial.print("&");
      Serial.print("dark=");
      Serial.print(darkCalibrationValue);
      Serial.print("&");
      Serial.print("bright=");
      Serial.print(brightCalibrationValue);
      Serial.println();
    }
    else if (serialMode == SERIAL_MODE_CALIBRATED)
    {
      Serial.println(soilMoistureLevelCalibrated);
    }
    else if (serialMode == SERIAL_MODE_RAW)
    {
      Serial.println(soilMoistureLevelRaw);
    }*/

    lastSerialOutputTime = millis();
  }
  else
  {
    /*if (isDebugMode)
    {    
      Serial.println("Not ready to serial print data");

      Serial.print("  Is time to serial print data: ");
      Serial.println(isTimeToPrintData);
      if (!isTimeToPrintData)
      {
        Serial.print("    Time remaining before printing data: ");
        Serial.print(millisecondsToSecondsWithDecimal(lastSerialOutputTime + secondsToMilliseconds(serialOutputIntervalInSeconds) - millis()));
        Serial.println(" seconds");
      }
      Serial.print("  Light sensor reading has been taken: ");
      Serial.println(lightPRSensorReadingHasBeenTaken);
      Serial.print("  Is ready to print data: ");
      Serial.println(isReadyToPrintData);

    }*/
  }
}
