int counter = 0;
int timeout = 300;
long int read_strain_gauge() {
  
      long int sensorValue;
      
      BTSerial.write(1);
      delay(250);
     
      wait();
      sensorValue = BTSerial.read();
      
      wait();
      sensorValue = (sensorValue<<8)|BTSerial.read();
      
      wait();
      sensorValue = (sensorValue<<8)|BTSerial.read();
      
      wait();
      sensorValue = (sensorValue<<8)|BTSerial.read();

      clear_serial();
      return sensorValue;
}

void wait(){
      while (BTSerial.available() == 0){
      counter ++;
      delay(10);
      }
}

void clear_serial(){
  while (BTSerial.available() > 0){
    char t = BTSerial.read();
  }
}
