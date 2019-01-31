#ifndef SOILMOISTURESENSOR_H_
#define SOILMOISTURESENSOR_H_

extern int soilMoistureLevelCalibrated;
extern int soilMoistureLevelRaw;

extern long lastLightPRSensorReadingTime;
extern long lightPRSensorReadingIntervalInSeconds;
extern int lightPRSensorReadIntervalIsSetFlagAddress;

extern int darkCalibrationValue;
extern int brightCalibrationValue;

extern bool lightPRSensorIsOn;
extern long lastSensorOnTime;
extern int delayAfterTurningSensorOn;
extern bool lightPRSensorReadingHasBeenTaken;

void setupLightPRSensor();

void setupCalibrationValues();

void setupLightPRSensorReadingInterval();

void turnLightPRSensorOn();

void turnLightPRSensorOff();

void takeLightPRSensorReading();

double getAverageLightPRSensorReading();

double calculateSoilMoistureLevel(int lightPRSensorReading);

void setEEPROMIsCalibratedFlag();

void setLightPRSensorReadingInterval(char* msg);
void setLightPRSensorReadingInterval(long readInterval);

long getLightPRSensorReadingInterval();

void setEEPROMLightPRSensorReadingIntervalIsSetFlag();
void removeEEPROMLightPRSensorReadingIntervalIsSetFlag();

void setDarkCalibrationValue(char* msg);

void setDarkCalibrationValueToCurrent();

void setDarkCalibrationValue(int darkCalibrationValue);

void setBrightCalibrationValue(char* msg);

void setBrightCalibrationValueToCurrent();

void setBrightCalibrationValue(int brightCalibrationValue);

void reverseSoilMoistureCalibrationValues();

int getDarkCalibrationValue();

int getBrightCalibrationValue();

void setEEPROMIsCalibratedFlag();

void removeEEPROMIsCalibratedFlag();

void restoreDefaultLightPRSensorSettings();
void restoreDefaultLightPRSensorReadingIntervalSettings();
void restoreDefaultCalibrationSettings();
#endif
/* SOILMOISTURESENSOR_H_ */
