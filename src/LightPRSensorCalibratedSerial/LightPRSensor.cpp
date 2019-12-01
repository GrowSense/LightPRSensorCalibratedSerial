#include <Arduino.h>
#include <EEPROM.h>

#include <duinocom2.h>

#include "Common.h"
#include "EEPROMHelper.h"
#include "LightPRSensor.h"

#define lightPRSensorPin A0
#define lightPRSensorPowerPin 12

bool lightPRSensorIsOn = true;
unsigned long lastSensorOnTime = 0;
int delayAfterTurningLightPRSensorOn = 3 * 1000;

bool lightPRSensorReadingHasBeenTaken = false;
long lightPRSensorReadingIntervalInSeconds = 5;
unsigned long lastLightPRSensorReadingTime = 0; // Milliseconds

int lightLevelCalibrated = 0;
int lightLevelRaw = 0;

bool reverseLightPRSensor = false;
//int darkCalibrationValue = ANALOG_MAX;
int darkCalibrationValue = (reverseLightPRSensor ? 0 : ANALOG_MAX);
//int brightCalibrationValue = 0;
int brightCalibrationValue = (reverseLightPRSensor ? ANALOG_MAX : 0);

#define lightPRSensorIsCalibratedFlagAddress 1
#define darkCalibrationValueAddress 2
#define brightCalibrationValueAddress 6

#define lightPRSensorReadIntervalIsSetFlagAddress 10
#define lightPRSensorReadingIntervalAddress 13

/* Setup */
void setupLightPRSensor()
{
  setupCalibrationValues();

  setupLightPRSensorReadingInterval();

  pinMode(lightPRSensorPowerPin, OUTPUT);

  // If the interval is less than specified delay then turn the sensor on and leave it on (otherwise it will be turned on each time it's needed)
  if (secondsToMilliseconds(lightPRSensorReadingIntervalInSeconds) <= delayAfterTurningLightPRSensorOn)
  {
    turnLightPRSensorOn();
  }
}

/* Sensor On/Off */
void turnLightPRSensorOn()
{
  if (isDebugMode)
    Serial.println("Turning sensor on");

  digitalWrite(lightPRSensorPowerPin, HIGH);

  lastSensorOnTime = millis();

  lightPRSensorIsOn = true;
}

void turnLightPRSensorOff()
{
  if (isDebugMode)
    Serial.println("Turning sensor off");

  digitalWrite(lightPRSensorPowerPin, LOW);

  lightPRSensorIsOn = false;
}

/* Sensor Readings */
void takeLightPRSensorReading()
{
  bool sensorReadingIsDue = millis() - lastLightPRSensorReadingTime >= secondsToMilliseconds(lightPRSensorReadingIntervalInSeconds)
    || lastLightPRSensorReadingTime == 0;

  if (sensorReadingIsDue)
  {
    if (isDebugMode)
      Serial.println("Sensor reading is due");

  	bool sensorGetsTurnedOff = secondsToMilliseconds(lightPRSensorReadingIntervalInSeconds) > delayAfterTurningLightPRSensorOn;
  
  	bool sensorIsOffAndNeedsToBeTurnedOn = !lightPRSensorIsOn && sensorGetsTurnedOff;
  
  	bool postSensorOnDelayHasPast = millis() - lastSensorOnTime >= delayAfterTurningLightPRSensorOn;
  
  	bool lightPRSensorIsOnAndReady = lightPRSensorIsOn && (postSensorOnDelayHasPast || !sensorGetsTurnedOff);

    bool lightPRSensorIsOnButSettling = lightPRSensorIsOn && !postSensorOnDelayHasPast && sensorGetsTurnedOff;

    /*if (isDebugMode)
    {
        Serial.print("  Sensor is on: ");
        Serial.println(lightPRSensorIsOn);
        
        Serial.print("  Last sensor on time: ");
        Serial.print(millisecondsToSecondsWithDecimal(millis() - lastSensorOnTime));
        Serial.println(" seconds ago");
        
        Serial.print("  Sensor gets turned off: ");
        Serial.println(sensorGetsTurnedOff);
        
        Serial.print("  Sensor is off and needs to be turned on: ");
        Serial.println(sensorIsOffAndNeedsToBeTurnedOn);
        
        Serial.print("  Post sensor on delay has past: ");
        Serial.println(postSensorOnDelayHasPast);
        
        Serial.print("  Sensor is off and needs to be turned on: ");
        Serial.println(sensorIsOffAndNeedsToBeTurnedOn);
        
        Serial.print("  Sensor is on and ready: ");
        Serial.println(lightPRSensorIsOnAndReady);
        
        Serial.print("  Sensor is on but settling: ");
        Serial.println(lightPRSensorIsOnButSettling);
        
        if (lightPRSensorIsOnButSettling)
        {
          Serial.print("    Time remaining to settle: ");
          long timeRemainingToSettle = lastSensorOnTime + delayAfterTurningLightPRSensorOn - millis();
          Serial.print(millisecondsToSecondsWithDecimal(timeRemainingToSettle));
          Serial.println(" seconds");
        }
    }*/

    if (sensorIsOffAndNeedsToBeTurnedOn)
    {
      turnLightPRSensorOn();
    }
    else if (lightPRSensorIsOnButSettling)
    {
      // Skip this loop. Wait for the sensor to settle in before taking a reading.
      if (isDebugMode)
        Serial.println("Soil moisture sensor is settling after being turned on");
    }
    else if (lightPRSensorIsOnAndReady)
    {
      if (isDebugMode)
        Serial.println("Preparing to take reading");

      lastLightPRSensorReadingTime = millis();
      
      // Remove the delay (after turning soil moisture sensor on) from the last reading time to get more accurate timing
      if (sensorGetsTurnedOff)
        lastLightPRSensorReadingTime = lastLightPRSensorReadingTime - delayAfterTurningLightPRSensorOn;

      lightLevelRaw = getAverageLightPRSensorReading();

      lightLevelCalibrated = calculateLightLevel(lightLevelRaw);

      if (lightLevelCalibrated < 0)
        lightLevelCalibrated = 0;

      if (lightLevelCalibrated > 100)
        lightLevelCalibrated = 100;

      lightPRSensorReadingHasBeenTaken = true;

      if (secondsToMilliseconds(lightPRSensorReadingIntervalInSeconds) > delayAfterTurningLightPRSensorOn)
      {
        turnLightPRSensorOff();
      }
    }
  }
  /*else
  {
    if (isDebugMode)
    {
      Serial.println("Sensor reading is not due");
      
      Serial.print("  Last soil moisture sensor reading time: ");
      Serial.print(millisecondsToSecondsWithDecimal(lastLightPRSensorReadingTime));
      Serial.println(" seconds");
      
      Serial.print("  Last soil moisture sensor reading interval: ");
      Serial.print(lightPRSensorReadingIntervalInSeconds);
      Serial.println(" seconds");
    
      int timeLeftUntilNextReading = lastLightPRSensorReadingTime + secondsToMilliseconds(lightPRSensorReadingIntervalInSeconds) - millis();
      Serial.print("  Time left until next soil moisture sensor reading: ");
      Serial.print(millisecondsToSecondsWithDecimal(timeLeftUntilNextReading));
      Serial.println(" seconds");
    }
  }*/
}

double getAverageLightPRSensorReading()
{
  int readingSum  = 0;
  int totalReadings = 10;

  for (int i = 0; i < totalReadings; i++)
  {
    int reading = analogRead(lightPRSensorPin);

    readingSum += reading;
  }

  double averageReading = readingSum / totalReadings;

  return averageReading;
}

double calculateLightLevel(int lightPRSensorReading)
{
  return map(lightPRSensorReading, darkCalibrationValue, brightCalibrationValue, 0, 100);
}

/* Reading interval */
void setupLightPRSensorReadingInterval()
{
  if (EEPROMFlagIsSet(lightPRSensorReadIntervalIsSetFlagAddress))
  {
    if (isDebugMode)
    	Serial.println("EEPROM read interval value has been set. Loading.");

    lightPRSensorReadingIntervalInSeconds = getLightPRSensorReadingInterval();
  }
  else
  {
    if (isDebugMode)
      Serial.println("EEPROM read interval value has not been set. Using defaults.");
  }
}

void setLightPRSensorReadingInterval(char* msg)
{
    int value = readInt(msg, 1, strlen(msg)-1);

    setLightPRSensorReadingInterval(value);
}

void setLightPRSensorReadingInterval(long newValue)
{
  if (isDebugMode)
  {
    Serial.print("Set sensor reading interval: ");
    Serial.println(newValue);
  }

  EEPROMWriteLong(lightPRSensorReadingIntervalAddress, newValue);

  EEPROMSetFlag(lightPRSensorReadIntervalIsSetFlagAddress);

  lightPRSensorReadingIntervalInSeconds = newValue; 

  serialOutputIntervalInSeconds = newValue;
  
  if (secondsToMilliseconds(newValue) <= delayAfterTurningLightPRSensorOn)
    turnLightPRSensorOn();
}

long getLightPRSensorReadingInterval()
{
  long value = EEPROMReadLong(lightPRSensorReadingIntervalAddress);

  if (value == 0
      || value == 255)
    return lightPRSensorReadingIntervalInSeconds;
  else
  {
    if (isDebugMode)
    {
      Serial.print("Read interval found in EEPROM: ");
      Serial.println(value);
    }

    return value;
  }
}

/* Calibration */
void setupCalibrationValues()
{
  if (EEPROMFlagIsSet(lightPRSensorIsCalibratedFlagAddress))
  {
    if (isDebugMode)
    	Serial.println("EEPROM calibration values have been set. Loading.");

    darkCalibrationValue = getDarkCalibrationValue();
    brightCalibrationValue = getBrightCalibrationValue();
  }
  /*else
  {
    if (isDebugMode)
      Serial.println("EEPROM calibration values have not been set. Using defaults.");
    
    //setDarkCalibrationValue(darkCalibrationValue);
    //setBrightCalibrationValue(brightCalibrationValue);
  }*/
}

void setDarkCalibrationValue(char* msg)
{
  int length = strlen(msg);

  if (length == 1)
    setDarkCalibrationValueToCurrent();
  else
  {
    int value = readInt(msg, 1, length-1);

//    Serial.println("Value:");
//    Serial.println(value);

    setDarkCalibrationValue(value);
  }
}

void setDarkCalibrationValueToCurrent()
{
  lastLightPRSensorReadingTime = 0;
  takeLightPRSensorReading();
  setDarkCalibrationValue(lightLevelRaw);
}

void setDarkCalibrationValue(int newValue)
{
  if (isDebugMode)
  {
    Serial.print("Setting dry soil moisture sensor calibration value: ");
    Serial.println(newValue);
  }

  darkCalibrationValue = newValue;
  
  EEPROMWriteLong(darkCalibrationValueAddress, newValue); // Must divide by 4 to make it fit in eeprom

  EEPROMSetFlag(lightPRSensorIsCalibratedFlagAddress);
}

void setBrightCalibrationValue(char* msg)
{
  int length = strlen(msg);

  if (length == 1)
    setBrightCalibrationValueToCurrent();
  else
  {
    int value = readInt(msg, 1, length-1);

//    Serial.println("Value:");
//    Serial.println(value);

    setBrightCalibrationValue(value);
  }
}

void setBrightCalibrationValueToCurrent()
{
  lastLightPRSensorReadingTime = 0;
  takeLightPRSensorReading();
  setBrightCalibrationValue(lightLevelRaw);
}

void setBrightCalibrationValue(int newValue)
{
  if (isDebugMode)
  {
    Serial.print("Setting wet soil moisture sensor calibration value: ");
    Serial.println(newValue);
  }

  brightCalibrationValue = newValue;

  EEPROMWriteLong(brightCalibrationValueAddress, newValue);
  
  EEPROMSetFlag(lightPRSensorIsCalibratedFlagAddress);
}

void reverseLightCalibrationValues()
{
  if (isDebugMode)
    Serial.println("Reversing soil moisture sensor calibration values");

  int tmpValue = darkCalibrationValue;

  darkCalibrationValue = brightCalibrationValue;

  brightCalibrationValue = tmpValue;

  if (EEPROM.read(lightPRSensorIsCalibratedFlagAddress) == 99)
  {
    setBrightCalibrationValue(brightCalibrationValue);
    setDarkCalibrationValue(darkCalibrationValue);
  }
}

int getDarkCalibrationValue()
{
  int value = EEPROMReadLong(darkCalibrationValueAddress);

  if (value < 0
      || value > ANALOG_MAX)
    return darkCalibrationValue;
  else
  {
    int dryLightPRSensorValue = value;
  
    if (isDebugMode)
    {
      Serial.print("Dry calibration value found in EEPROM: ");
      Serial.println(dryLightPRSensorValue);
    }

    return dryLightPRSensorValue;
  }
}

int getBrightCalibrationValue()
{
  int value = EEPROMReadLong(brightCalibrationValueAddress);

  if (value < 0
      || value > ANALOG_MAX)
    return brightCalibrationValue;
  else
  {
    if (isDebugMode)
    {
      Serial.print("Wet calibration value found in EEPROM: ");
      Serial.println(value);
    }
  }

  return value;
}

void restoreDefaultLightPRSensorSettings()
{
  restoreDefaultCalibrationSettings();
  restoreDefaultLightPRSensorReadingIntervalSettings();
}

void restoreDefaultLightPRSensorReadingIntervalSettings()
{
  EEPROMRemoveFlag(lightPRSensorReadIntervalIsSetFlagAddress);

  lightPRSensorReadingIntervalInSeconds = 5;
  serialOutputIntervalInSeconds = 5;

  setLightPRSensorReadingInterval(lightPRSensorReadingIntervalInSeconds);
}

void restoreDefaultCalibrationSettings()
{
  EEPROMRemoveFlag(lightPRSensorIsCalibratedFlagAddress);

  darkCalibrationValue = (reverseLightPRSensor ? 0 : ANALOG_MAX);
  brightCalibrationValue = (reverseLightPRSensor ? ANALOG_MAX : 0);

  setDarkCalibrationValue(darkCalibrationValue);
  setBrightCalibrationValue(brightCalibrationValue);
}
