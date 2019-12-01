#ifndef SOILMOISTURESENSOR_H_
#define SOILMOISTURESENSOR_H_

extern int lightLevelCalibrated;
extern int lightLevelRaw;

extern unsigned long lastLightPRSensorReadingTime;
extern long lightPRSensorReadingIntervalInSeconds;
extern int lightPRSensorReadIntervalIsSetFlagAddress;

extern int darkCalibrationValue;
extern int brightCalibrationValue;

extern bool lightPRSensorIsOn;
extern unsigned long lastSensorOnTime;
extern int delayAfterTurningSensorOn;
extern bool lightPRSensorReadingHasBeenTaken;

void setupLightPRSensor();

void setupCalibrationValues();

void setupLightPRSensorReadingInterval();

void turnLightPRSensorOn();

void turnLightPRSensorOff();

void takeLightPRSensorReading();

double getAverageLightPRSensorReading();

double calculateLightLevel(int lightPRSensorReading);

void setLightPRSensorReadingInterval(char* msg);
void setLightPRSensorReadingInterval(long readInterval);

long getLightPRSensorReadingInterval();

void setDarkCalibrationValue(char* msg);

void setDarkCalibrationValueToCurrent();

void setDarkCalibrationValue(int darkCalibrationValue);

void setBrightCalibrationValue(char* msg);

void setBrightCalibrationValueToCurrent();

void setBrightCalibrationValue(int brightCalibrationValue);

void reverseLightCalibrationValues();

int getDarkCalibrationValue();

int getBrightCalibrationValue();

void restoreDefaultLightPRSensorSettings();
void restoreDefaultLightPRSensorReadingIntervalSettings();
void restoreDefaultCalibrationSettings();
#endif
/* LIGHTPRSENSOR */
