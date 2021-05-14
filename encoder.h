// Encoder pins
const byte motorEnc = 2;
const byte propEnc = 3;

// Constants
const unsigned long steadyStatePhaseDif = 0; // Radians times 1000
const byte numberTicks = 2; // Number of encoder pulses per rotation
const unsigned int holdOffPeriod = 10000; // Holdoff after successive events

// Variables used in interrupts
volatile unsigned long motorSideLast, propSideLast;
volatile unsigned long rotationalPeriod = 100000000; // Rotational period (times 1000) in us
volatile float phase; // Phase in radians

void setupEncoders();
void motorInterrupt();
void propInterrupt();
