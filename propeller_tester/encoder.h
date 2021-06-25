#include <EEPROM.h> //Needed to record user settings

// Shaft Constants
const float Jm = 57.9402017;    // J * Shear Modulus (Nm^2)
const float shaftLength = 0.315;  // Shaft length in meters
// Jm needs to be precalculated since using a large range of numbers can lead
// to inacccurate results on microcontrollers. J is generally on the order of
// 10^10 while J is 10^-9, shaft length and phase are around 10^0.

// Encoder constants
const byte motorEnc = 2;                // Pin for motor side encoder
const byte propEnc = 3;                 // Pin for propeller end encoder
unsigned long steadyStatePhaseDif = 0;  // Radians times 1000, set once in setup
const int PHASE_DIFF_LOCATION = 20;     // Location of phase difference in EEPROM
const byte numberTicks = 2;             // Number of encoder pulses per rotation
const unsigned int holdOffPeriod = 10000; // Holdoff after successive events (us)

// Variables used in interrupts
volatile unsigned long motorSideLast, propSideLast;
volatile unsigned long rotationalPeriod = 100000000; // Rotational period (times 1000) in us
volatile float phase; // Phase in radians

void setupEncoders();
void motorInterrupt();
void propInterrupt();
float findTorque();
