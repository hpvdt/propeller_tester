void setupLoadCell() {
  Wire.begin();

  if (loadCell.begin() == false) {
    Serial.println(F("Load cell not detected. Please check wiring. Freezing..."));
    while (1);
  }
  Serial.println(F("Load cell detected!"));

  // Configure how data is collected
  loadCell.setIntPolarityHigh(); //Set Int pin to be high when data is ready (default)
  loadCell.setGain(NAU7802_GAIN_2); //Gain can be set to 1, 2, 4, 8, 16, 32, 64, or 128.
  loadCell.setSampleRate(NAU7802_SPS_40); //Sample rate can be set to 10, 20, 40, 80, or 320Hz

  // Used to set up conversion to N
  loadCell.setCalibrationFactor(settingCalibrationFactor);
  loadCell.setZeroOffset(settingZeroOffset);
  
  loadCell.calibrateAFE(); //Does an internal calibration.
  // Recommended after power up, gain changes, sample rate changes, or channel changes.
}

void updateThrust() {
  // Check for interrupt pin
  if (digitalRead(loadCellInteruptPin) == HIGH) {
    thrust = loadCell.getWeight();
  }
}
  
