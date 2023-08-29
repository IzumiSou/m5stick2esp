#include <Wire.h>
#define I2C_Slave 0x08

void setup() {
  Wire.begin(0x08);
  Wire.onRequest(requestEvent); //slave -> master
  Wire.onReceive(receiveEvent); //master -> slave
  Serial.begin(115200);
}

void loop() {
  delay(100);
}

void requestEvent() {  //slave -> master
  Wire.write("hello ");
}

void receiveEvent(int howMany) { //master -> slave
  while (1 < Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  int x = Wire.read();
  Serial.println(x);
}