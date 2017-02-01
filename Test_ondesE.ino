
#include <SoftwareSerial.h>

SoftwareSerial xbee(2, 3); // RX, TX

void setup()  {
  Serial.begin(9600);
  xbee.begin(9600);
}

void loop()  {
  while (Serial.available()) {
    String data = Serial.readString();
    xbee.print("DM_R1");
    Serial.println("DM_R1");
    
  }
  while (xbee.available()) {
    Serial.println("Reception <- " + xbee.readString());
    
  }
  }

