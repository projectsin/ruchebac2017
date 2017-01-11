#include <SoftwareSerial.h>

SoftwareSerial xbee(2, 3); // RX, TX

void setup()  {
  Serial.begin(9600);
  xbee.begin(9600);
}

void loop()  {
  while (Serial.available()) {
    String data = Serial.readString();
    xbee.print(data);
    Serial.println("Envoie -> " + data);
  }

  while (xbee.available()) {
    Serial.println("Reception <- " + xbee.readString());
  }
 
}
