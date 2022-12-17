// C++ Code
//
const int ledPin = 8;
const int buttonPin = 2; //Button to perform interrupt
volatile float RPM;
int ledtoggle = LOW; //LED state

//variables to keep track of the timing of recent interrupts

unsigned long timeNow = 0;
volatile int period;
volatile book printRPM = false;
volatile long timeLast = 1000;

void button_ISR (){

//Set time
timeNow = millis();
period = timeNow - timeLast;
timeLast = timeNow;

//Set RPM
RPM = 60000.0/period; // Immediate RPM 

//Print RPM
printRPM = true;

//Set LED toggle
  ledToggle = !ledToggle;
  digitalWrite*ledPin, ledToggle);
}

void setup (){
  
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(buttonPin), button_ISR, RISING);
}

void loop(){
  
  // If stops turning, RPM set back to 0
  
  if (timeNow - timeLast > 5000) {
    RPM = 0;
  }
  
  if (print RPM == true) {
    Serial.println("RPM");
    Serial.print(RPM);
  }
  
}
