#include "display.h"
#include "encoder.h"
#include "loadcell.h"

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
float rpm, power, torque, thrust;

// Rolling Average Variables
const byte numberOfSamplesNeeded = 10;
byte currentSample = 0;
const int samplePeriod = 100; // Period between sampling in ms
float rpmC, powerC, torqueC, thrustC; // Store cummulation of the samples for averaging later

void setup() {
  Serial.begin(115200); // Setup serial first

  Serial.println(F("HPVDT Propeller Test Rig"));

  setupLoadCell();
  displaySetup();
  setupEncoders();

  Serial.println(F("Propeller tester set up. YOU MAY START THE MOTOR.\n"));
  Serial.println("\nRPM | TORQUE | POWER | THRUST"); // Data headers
  
  delay(1000); // Brief delay to show header info before datastream
}


void loop() {

  // Get encoder dependant variables in quick succession
  rpm = rotationalPeriod;
  torque = findTorque();
  thrust = updateThrust();
  
  // Update cummulated results
  torqueC = torqueC + torque;
  thrustC = thrustC + thrust;
  rpm = 6283185.3 / rpm; // Convert rotational period (us) to rate (rad/s)
  rpmC = rpmC + rpm;

  currentSample++;

  // Have we collected enough samples?
  if (currentSample >= numberOfSamplesNeeded) {
    
    // Determine average of all readings
    torque = torqueC / currentSample;
    thrust = thrustC / currentSample;
    rpm = rpmC / currentSample; // Note: this is still in rad/s
    
    power = torque * rpm; // Get average power
    
    rpm = 9.549296586 * rpm; // Convert rad/s to RPM
    
    // Update displayed values
    displayFloat(rpm,rpmDisp,2);
    displayFloat(power,powDisp,2);
    displayFloat(torque,torDisp,2);
    displayFloat(thrust,thrDisp,2);

    outputDataSerial();

    // Reset values for next sample cycle
    currentSample = 0;
    torqueC = 0.0;
    thrustC = 0.0;
    rpmC = 0.0;
  }
  
  delay(samplePeriod);
}

void outputDataSerial() {
  // Output data over serial
  Serial.print(rpm, 2);
  Serial.write('\t');
  Serial.print(torque, 2);
  Serial.write('\t');
  Serial.print(power, 2);
  Serial.write('\t');
  Serial.print(thrust, 2);
  Serial.write('\n');
}
