#include <SoftwareSerial.h>

SoftwareSerial xbee(2, 3); // RX, TX

//protocole d'envoie id;masse;temperature;hygrometrie/

byte id = 1; //TODO : definir ensuite depuis l"exterieur

void setup()  {
  Serial.begin(9600);
  xbee.begin(9600);
}

void loop()  {
  while (xbee.available()) {
    if(xbee.read() == 'P') 
      sendData();
  }
 
}

void sendData() {
  send(String(id) + ";" + String(getMasse()) + ";" + String(getTemperature()) + ";" + String(getHygrometrie()) + "/";
}
       
void send(String data) {
  xbee.print(data);
}  
       
short getMasse() {
  return 0;
}
       
short getTemperature() {
  return 0; 
}
       
short getHygrometrie() {
  return 0; 
}
       

