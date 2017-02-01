#include <SoftwareSerial.h>

SoftwareSerial xbee(2, 3); // RX, TX

void setup()  {
  Serial.begin(9600);
  xbee.begin(9600);
}

void loop()  {


  while (Serial.available()) {

  }
  
  while (xbee.available()) {
    String read = xbee.readString();
    if (read.equals("DM_R1")) {
      xbee.print("test");
      Serial.println("Envoie -> test ");
    }
    Serial.println("Reception <- " + read);

  }
}

