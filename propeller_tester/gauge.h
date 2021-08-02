#include <Wire.h>


#include <SoftwareSerial.h>
SoftwareSerial BTSerial(A1,A2);

long int read_strain_gauge();
void wait();
void clear_serial();
