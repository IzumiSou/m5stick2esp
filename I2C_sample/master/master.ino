#include <Wire.h>
#define I2C_Slave 0x08

void setup() {
  Wire.begin();
  Serial.begin(115200);
}

byte x = 0;

void loop() {

  //master -> slave
  Wire.beginTransmission(8);
  Wire.write("x is ");
  Wire.write(x);
  Wire.endTransmission();
  x++;

  //slave -> master
  Wire.requestFrom(8, 6);  // request 6 bytes from slave device #8

  while (Wire.available()) {  // slave may send less than requested
    char c = Wire.read();     // receive a byte as character
    Serial.print(c);          // print the character
  }

  delay(500);
}
