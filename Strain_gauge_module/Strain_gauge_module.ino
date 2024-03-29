#include <Wire.h>
#include <HX711.h>

HX711 gauge;


long int sensorValue = 10;
const int ledPin = 2;
int ack = 0;

void setup() {
  Serial.begin(9600);//Baudrate of bluetooth modules
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
    for (int i =1; i<=2;i++){
    digitalWrite(ledPin, HIGH); // LED ON
    delay(100);
    digitalWrite(ledPin, LOW); // LED OFF
    delay(100);
  } 
  delay(1000);
  gauge.begin(5,6);
  gauge.set_gain(64);
  gauge.set_scale();
  gauge.tare(40);
  torqueZero = gauge.get_value(40);
  for (int i =1; i<=3;i++){
    digitalWrite(ledPin, HIGH); // LED ON
    delay(100);
    digitalWrite(ledPin, LOW); // LED OFF
     delay(100);
  } 
  
}

void loop() {
  
  if (Serial.available() > 0)
  {
    // Checks whether data is comming from the serial port
    ack = Serial.read(); // Reads the data from the serial port
    digitalWrite(ledPin, HIGH); // LED ON
    delay(50);
    digitalWrite(ledPin, LOW); // LED OFF
  }

  if (ack == 123) {
    sensorValue = gauge.get_value(40);
    sensorValue = (sensorValue-torqueZero)/10;
    Serial.write(byte(sensorValue>>24));
    Serial.write(byte(sensorValue>>16));
    Serial.write(byte(sensorValue>>8));
    Serial.write(byte(sensorValue));
    digitalWrite(ledPin, HIGH); // LED ON
    delay(50);
    digitalWrite(ledPin, LOW); // LED OFF
    delay(50);
    digitalWrite(ledPin, HIGH); // LED ON
    delay(50);
    digitalWrite(ledPin, LOW); // LED OFF
    ack = 0;
    Serial.flush();
    clear_serial();
  }
}

void clear_serial(){
  while (Serial.available() > 0){
    char t = Serial.read();
  }
}
