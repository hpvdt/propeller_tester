#include <EEPROM.h> //Needed to record user settings

////////////////////////////////
// Calibration configuration
const int samplesNeeded = 1000; // Samples that will be collected before an average is found
const int updateSpacing = 100; // Update the user every so often with progress
const unsigned int holdOffPeriod = 10000; // Holdoff after successive encoder events
const int PHASE_DIFF_LOCATION = 20; // Location of phase difference in EEPROM

// Encoder pins
const byte motorEnc = 2;
const byte propEnc = 3;

// Constants
const byte numberTicks = 2; // Number of encoder pulses per rotation

// Variables used in interrupts
volatile unsigned long motorSideLast, propSideLast; // Record previous interrupt times
volatile unsigned long rotationalPeriod = 100000000; // Rotational period (times 1000) in us

// Calibration variables
int samplesTaken = 0;
unsigned long cummulativeTotal = 0;
int updateMarker = updateSpacing; // Marker for when to output next progress update

void setup() {
  Serial.begin(9600);
  
  // Set up encoders for use
  pinMode(motorEnc, INPUT);
  pinMode(propEnc, INPUT);

  Serial.println(F("HPVDT Propeller Shaft Phase Calibration"));
  Serial.println(F("Allow shaft to reach steady state speed, UNLOADED\n"));
  Serial.println(F("When steady-state is reached, send any character to begin calibration\n"));

  // Wait for data to continue
  while (Serial.available()) Serial.read(); //Clear anything in RX buffer
  while (Serial.available() == 0) delay(10); //Wait for user to press key
  while (Serial.available()) Serial.read(); //Clear anything in RX buffer

  // Inform user of configuration
  Serial.print(F("STARTING CALIBRATION. "));
  Serial.print(samplesNeeded);
  Serial.println(F(" samples will be averaged."));

  // Attach interrupts for encoders
  attachInterrupt(digitalPinToInterrupt(motorEnc), motorInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(propEnc), propInterrupt, RISING);
}

void loop() {
  
  // Wait until the required number of samples are acquired
  if (samplesTaken >= samplesNeeded) {
    // Disable any interrupts
    detachInterrupt(digitalPinToInterrupt(motorEnc));
    detachInterrupt(digitalPinToInterrupt(propEnc));

    // Calculated average and store it to EEPROM
    cummulativeTotal = cummulativeTotal / samplesTaken;
    EEPROM.put(PHASE_DIFF_LOCATION, cummulativeTotal);

    // Inform user of completion and results
    Serial.println(F("Complete calibration cycle!"));
    Serial.print(F("Collected "));
    Serial.print(samplesTaken);
    Serial.print(F(" samples, average phase of "));
    Serial.print(float(cummulativeTotal)/1000.0, 4);
    Serial.println(F(" radians recorded."));
    Serial.print(F("Result saved to EEPROM at address "));
    Serial.println(PHASE_DIFF_LOCATION);

    Serial.println("\nCalibration complete, please reprogram with testing program.");
    Serial.println("System will now quit.");  

    while(1) {
      delay(1000); // Hold microcontroller here until restarted/reprogrammed
    }
  }

  // Provide update to user periodically
  if (samplesTaken >= updateMarker) {
    Serial.print(F("Completed "));
    Serial.print(samplesTaken);
    Serial.print(F(" / "));
    Serial.println(samplesNeeded);

    updateMarker = updateSpacing + updateMarker; // Move to next increment
  }

  delay(1);
}


// Interrupts are basically the same as the actual tester code.
// Just some minor additioons to increment the cummulative sum and sample count.

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

  // Increase cumulative total and sample count
  cummulativeTotal = cummulativeTotal + temp; 
  samplesTaken++;
}
