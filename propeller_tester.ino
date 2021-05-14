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
float rpm, power, torque;

const unsigned int startPause = 10; // Pause from power on to calibration (seconds)

void setup() {
  Serial.begin(115200); // Setup serial first

  Serial.println(F("HPVDT Propeller Test Rig"));
  Serial.println(F("AXLE MUST BE STATIONARY FOR CALIBRATION"));
  Serial.print(F("You have "));
  Serial.print(startPause);
  Serial.println(F(" seconds to stop it before calibration begins.\n"));
  delay(startPause * 1000);
  Serial.println(F("STARTING CALIBRATIONS. AXLE SHOULD BE STILL!\n"));

  setupLoadCell();
  displaySetup();
  setupEncoders();

  Serial.println(F("Propeller tester set up. YOU MAY START THE MOTOR.\n"));
}


void loop() {

  updateThrust();
  
  // Get encoder dependant variables in quick succession
  rpm = rotationalPeriod;
  torque = findTorque();
  
  rpm = 6283185.3 / rpm; // Convert rotational period (us) to rate (rad/s)
  power = torque * rpm;
  rpm = 9.549296586 * rpm; // Convert rad/s to RPM
  
  // Update displayed values
  displayFloat(rpm,rpmDisp,2);
  displayFloat(power,powDisp,2);
  displayFloat(torque,torDisp,2);
  displayFloat(thrust,thrDisp,2);
  
  outputDataSerial();
  delay(25);
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
