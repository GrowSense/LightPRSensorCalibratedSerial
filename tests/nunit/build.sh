echo "Starting build for project"
echo "Dir: $PWD"

DIR=$PWD

xbuild src/LightPRSensorCalibratedSerial.sln /p:Configuration=Release
