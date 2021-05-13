#include "display.h"

#include <Wire.h>
#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_NAU8702

NAU7802 loadCell; //Create instance of the NAU7802 class

void setup() {
  Serial.begin(9600);
  Serial.println("Qwiic Scale Example");

  Wire.begin();

  if (loadCell.begin() == false)
  {
    Serial.println("Scale not detected. Please check wiring. Freezing...");
    while (1);
  }
  Serial.println("Scale detected!");
  
  displaySetup();
}


void loop() {
  displayInt(random(10000), random(2) + 1);
  displayFloat(millis()/1000.0,3,1); // Time running

  delay(25);
  
}
