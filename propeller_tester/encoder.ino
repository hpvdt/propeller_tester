
// IMPORTANT NOTE
// Assumes that motor side always leads ahead of propeller

void setupEncoders() {
  // Set up encoders for use
  pinMode(motorEnc, INPUT);
  pinMode(propEnc, INPUT);

  attachInterrupt(digitalPinToInterrupt(motorEnc), motorInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(propEnc), propInterrupt, RISING);

  // Get phase from EEPROM
  EEPROM.get(PHASE_DIFF_LOCATION, steadyStatePhaseDif);
}

void motorInterrupt() {
  unsigned long temp = micros();

  // Record only if the minimum time has passed
  if (holdOffPeriod < (temp - motorSideLast)) {
    motorSideLast = temp;
  }
}

void propInterrupt() {
  unsigned long temp = micros();

  // Exit if there is too soon of a repetition
  if (holdOffPeriod > (temp - motorSideLast)) {
    return;  
  }

  // Estimate rotational period and record the start of this interrupt
  rotationalPeriod = (temp - propSideLast) * numberTicks;
  propSideLast = temp;

  // Determine phase difference
  // Find it using the time between the notches and dividing
  // by the estimated rotational period to get a value in
  // radians. Multiplied by 1000 to keep 3 decimal places 
  // and not need to use floats which are much slower.
  
  temp = propSideLast - motorSideLast;
  temp = temp * 1000;
  temp = temp / rotationalPeriod;
  temp = temp - steadyStatePhaseDif;
  phase = temp / 1000.0;
}

float findTorque() {
  float workingPhase = phase; // Avoid issues from an updated phase value
  float output = workingPhase;

  // Do some math here, get equations later

  return output;
}
