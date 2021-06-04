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
  loadCell.setSampleRate(NAU7802_SPS_40); //Sample rate can be set to 10, 20, 40, 80, or 320Hz

  loadCell.calibrateAFE(); //Does an internal calibration.
  // Recommended after power up, gain changes, sample rate changes, or channel changes.

  // Get scale calibration data from EEPROM
  // (Use "Example2" from the NAU7805 library to calibrate)
  // Note that library calibrates at a gain of 128!
  float settingCalibrationFactor; //Value used to convert the load cell reading to lbs or kg
  long settingZeroOffset; //Zero value that is found when scale is tared
  EEPROM.get(LOCATION_CALIBRATION_FACTOR, settingCalibrationFactor);
  EEPROM.get(LOCATION_ZERO_OFFSET, settingZeroOffset);

  // Used to set up conversion to N
  loadCell.setCalibrationFactor(settingCalibrationFactor);
  loadCell.setZeroOffset(settingZeroOffset);  

  Serial.print(F("Zero offset: "));
  Serial.println(loadCell.getZeroOffset());
  Serial.print(F("Force factor: "));
  Serial.println(loadCell.getCalibrationFactor(), 2);
}

float updateThrust() {
  
//  Serial.print("Reading: ");
//  Serial.print(loadCell.getReading());
//  Serial.print("\t");
//  Serial.println(loadCell.getWeight(), 2);

    // Check for interrupt pin
//  if (digitalRead(loadCellInteruptPin) == HIGH) {
//    thrust = loadCell.getWeight();
//  }

  return(-(loadCell.getWeight()));
}
  
