VERSION=$(cat version.txt)
BUILD_NUMBER=$(cat buildnumber.txt)

FULL_VERSION="$VERSION-$BUILD_NUMBER"

SOURCE_FILE="src/LightPRSensorCalibratedSerial/Common.h"

sed -i "s/#define VERSION .*/#define VERSION \"$FULL_VERSION\"/" $SOURCE_FILE
