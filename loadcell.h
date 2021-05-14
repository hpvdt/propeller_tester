#include <Wire.h>
#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_NAU8702

NAU7802 loadCell; //Create instance of the NAU7802 class
const byte loadCellInteruptPin = 6;

float thrust = 0.0;
const float settingCalibrationFactor = 1.0; //Value used to convert the load cell reading to N
const long settingZeroOffset = 0; //Zero value that is found when scale is tared
// Use the first library example to read values and do a two point calibration

void setupLoadCell();
void updateThrust();
