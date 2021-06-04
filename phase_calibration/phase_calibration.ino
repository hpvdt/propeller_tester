#include <EEPROM.h> //Needed to record user settings

// Encoder pins
const byte motorEnc = 2;
const byte propEnc = 3;

// Constants
unsigned long steadyStatePhaseDif = 0; // Radians times 1000, set once in setup
const byte numberTicks = 2; // Number of encoder pulses per rotation
const unsigned int holdOffPeriod = 10000; // Holdoff after successive events
const int PHASE_DIFF_LOCATION = 20; // Location of phase difference in EEPROM

// Variables used in interrupts
volatile unsigned long motorSideLast, propSideLast;
volatile unsigned long rotationalPeriod = 100000000; // Rotational period (times 1000) in us
volatile float phase; // Phase in radians


////////////////////////////////
// Calibration configuration
const int sampleCount = 1000;
int samplesTaken = 0;


void setup() {
  Serial.begin(9600);
  
  // Set up encoders for use
  pinMode(motorEnc, INPUT);
  pinMode(propEnc, INPUT);

  Serial.println("HPVDT Propeller Shaft Phase Calibration");
  Serial.println("Allow shaft to reach steady state speed, UNLOADED\n");
  Serial.println("When steady-state is reached, send any character to begin calibration");

  // Wait for data to continue
  while (Serial.available()) Serial.read(); //Clear anything in RX buffer
  while (Serial.available() == 0) delay(10); //Wait for user to press key
  while (Serial.available()) Serial.read(); //Clear anything in RX buffer
  
  attachInterrupt(digitalPinToInterrupt(motorEnc), motorInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(propEnc), propInterrupt, RISING);
}

void loop() {

  if (samplesTaken == sampleCount) {
    detachInterrupt(digitalPinToInterrupt(motorEnc));
    detachInterrupt(digitalPinToInterrupt(propEnc));
  }

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

  samplesTaken++;
}
