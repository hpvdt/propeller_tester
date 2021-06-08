void setupLoadCell() {
  Wire.begin();
  
  if (loadCell.begin() == false) {
    Serial.println(F("Load cell not detected. Please check wiring. Freezing..."));
    while (1);
  }
  
  Serial.println(F("Load cell detected!"));

  // Configure how data is collected
  loadCell.setIntPolarityHigh(); //Set Int pin to be high when data is ready (default)
  loadCell.setGain(NAU7802_GAIN_128); //Gain can be set to 1, 2, 4, 8, 16, 32, 64, or 128. (128 used since that is what it is calibrated at by default)
  loadCell.setSampleRate(NAU7802_SPS_80); //Sample rate can be set to 10, 20, 40, 80, or 320Hz (Needs to be high for zero calibration)
  loadCell.calibrateAFE(); //Does an internal calibration.

  // Get scale calibration data from EEPROM
  // (Use "Example2" from the NAU7805 library to calibrate)
  // Note that library calibrates at a gain of 128!

  readSystemSettings(); //Load zeroOffset and calibrationFactor from EEPROM
  Serial.print("Calibration factor: ");
  Serial.println(loadCell.getCalibrationFactor());

  // Tare scale every time to account for varying static load between setups
  Serial.println(F("Setup scale with no weight on it. Press a key when ready."));
  while (Serial.available()) Serial.read(); //Clear anything in RX buffer
  while (Serial.available() == 0) delay(10); //Wait for user to press key
  loadCell.calculateZeroOffset(64); //Zero or Tare the scale. Average over 64 readings. 
  Serial.print(F("Zero offset: "));
  Serial.println(loadCell.getZeroOffset());
  
  if (loadCell.getCalibrationFactor() < 0.1) {

    Serial.println(F("Place known weight on scale. Press a key when weight is in place and stable."));
    while (Serial.available()) Serial.read(); // Clear anything in RX buffer
    while (Serial.available() == 0) delay(10); // Wait for user to press key

    Serial.print(F("Please enter the weight, without units, currently sitting on the scale (for example '4.25'): "));
    while (Serial.available()) Serial.read(); // Clear anything in RX buffer
    while (Serial.available() == 0) delay(10); // Wait for user to press key
    
    //Read user input
    float weightOnScale = Serial.parseFloat();
    loadCell.calculateCalibrationFactor(weightOnScale, 64); //Tell the library how much weight is currently on it
    Serial.print(F("\nForce factor: "));
    Serial.println(loadCell.getCalibrationFactor(), 2);

    recordSystemSettings();
  }
} 

float updateThrust() {

  //  Serial.print("Reading: ");
  //  Serial.print(loadCell.getReading());
  //  Serial.print("\t");
  //  Serial.println(loadCell.getWeight(), 2);

  // Check for interrupt pin
  //  if (digitalRead(loadCellInteruptPin) == HIGH) {
  //    return(loadCell.getWeight());
  //  }

  return (-(loadCell.getWeight()));
}

void recordSystemSettings(void) {
  //Get various values from the library and commit them to NVM
  EEPROM.put(LOCATION_CALIBRATION_FACTOR, loadCell.getCalibrationFactor());
  EEPROM.put(LOCATION_ZERO_OFFSET, loadCell.getZeroOffset());
}

//Reads the current system settings from EEPROM
//If anything looks weird, reset setting to default value​
void readSystemSettings(void) {
  float settingCalibrationFactor; //Value used to convert the load cell reading to lbs or kg
  long settingZeroOffset; //Zero value that is found when scale is tared
  
  //Look up the calibration factor
  EEPROM.get(LOCATION_CALIBRATION_FACTOR, settingCalibrationFactor);
  if (settingCalibrationFactor == 0xFFFFFFFF)
  {
    settingCalibrationFactor = 0; //Default to 0
    EEPROM.put(LOCATION_CALIBRATION_FACTOR, settingCalibrationFactor);
  }
  
  //Look up the zero tare point
  EEPROM.get(LOCATION_ZERO_OFFSET, settingZeroOffset);
  if (settingZeroOffset == 0xFFFFFFFF)
  {
    settingZeroOffset = 1000L; //Default to 1000 so we don't get inf
    EEPROM.put(LOCATION_ZERO_OFFSET, settingZeroOffset);
  }
  
  //Pass these values to the library
  loadCell.setCalibrationFactor(settingCalibrationFactor);
  loadCell.setZeroOffset(settingZeroOffset);
}
