#include <Wire.h>
#include <EEPROM.h> //Needed to record user settings
#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_NAU8702

NAU7802 loadCell; //Create instance of the NAU7802 class
const byte loadCellInteruptPin = 6;

// Use the first library example to read values and do a two point calibration

// Locations in EEPROM of values
const int LOCATION_CALIBRATION_FACTOR = 0; //Float, requires 4 bytes of EEPROM
const int LOCATION_ZERO_OFFSET = 10; //Must be more than 4 away from previous spot. Long, requires 4 bytes of EEPROM

void setupLoadCell();
float updateThrust();
