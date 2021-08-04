int counter_gauge = 0;
int timeout = 400;

long int read_strain_gauge() {
      long int sensorValue;
      counter_gauge = 0;
      clear_serial();
      BTSerial.write(123);
      wait();
      sensorValue = BTSerial.read();
      wait();
      sensorValue = (sensorValue<<8)|BTSerial.read();
      wait();
      sensorValue = (sensorValue<<8)|BTSerial.read();
      wait();
      sensorValue = (sensorValue<<8)|BTSerial.read();
      return sensorValue;
}

void wait(){
      while (BTSerial.available() < 1  && counter_gauge <= timeout){
      counter_gauge ++;
      delay(10);
      }
}

void clear_serial(){
  while (BTSerial.available() > 0){
    char t = BTSerial.read();
  }
}
