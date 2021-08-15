#include "display.h"
#include "encoder.h"
#include "loadcell.h"
#include "gauge.h"

/* Propeller Tester Code
 *  Calculates and outputs the following data from a propeller
 *    - Rotational speed
 *    - Shaft torque using two encoders
 *    - Shaft power
 *    - Thrust
 */

// Display indices
const byte rpmDisp = 0;
const byte torDisp = 1;
const byte powDisp = 2;
const byte thrDisp = 3;

// Temporary variables for data from encoders (to avoid interrupts changing them)
float rpm, power,thrust;
long int torque;

// Rolling Average Variables
byte currentSample = 0;
const int samplePeriod = 500; // Period between sampling in ms
float rpmC, powerC, torqueC, thrustC; // Store cummulation of the samples for averaging later

void setup() {
  BTSerial.begin(9600);
  Serial.begin(115200); // Setup serial first
  Serial.println(F("HPVDT Propeller Test Rig"));

  setupLoadCell();
  setupEncoders();

  Serial.println(F("Propeller tester set up. YOU MAY START THE MOTOR.\n"));
  Serial.println("\nRPM | TORQUE | POWER | THRUST"); // Data headers
  
  delay(500); // Brief delay to show header info before datastream
}


void loop() {

  // Get encoder dependant variables in quick succession
  rpm = rotationalPeriod;
  torque = read_strain_gauge();
  torque = torque/5681.82;//Strain gauge factor
  thrust = updateThrust();
  // Update cummulated results
  rpm = 6283185.3 / rpm; // Convert rotational period (us) to rate (rad/s)
  power = torque * rpm; // Get average power
  rpm = 9.549296586 * rpm; // Convert rad/s to RPM

  outputDataSerial();
  
  delay(samplePeriod);
}

void outputDataSerial() {
  // Output data over serial
  Serial.print(rpm, 2);
  Serial.write('\t');
  Serial.print(torque);
  Serial.write('\t');
  Serial.print(power, 2);
  Serial.write('\t');
  Serial.print(thrust, 2);
  Serial.write('\n');
}
