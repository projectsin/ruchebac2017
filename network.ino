#include <SoftwareSerial.h>

SoftwareSerial xbee(2, 3); // RX, TX
char c = 'A';
int  pingPong = 1;

void setup()  {
  Serial.begin(9600);
  Serial.println( "Arduino started sending bytes via XBee" );

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
