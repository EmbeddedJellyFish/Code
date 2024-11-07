#include <Arduino.h>

HardwareSerial mySerial(2);  

void setup() {

  mySerial.begin(9600, SERIAL_8N1, 16, 17); 

  Serial.begin(9600);

}

void loop() {
  if (mySerial.available()) {
    Serial.print("\nHello");
    delay(1000);
  }
}
