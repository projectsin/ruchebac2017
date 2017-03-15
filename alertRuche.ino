#define detect 2
#include <SoftwareSerial.h>
int etatDetect;
int hygrometrie, temperature, masse;
int intervalleHygrometrie[] = {5,20};
int intervalleTemperature[] = {5,20};
int intervalleMasse[] = {5,20};

boolean sendAlert = true;

SoftwareSerial xbee(2, 3);

void setup() {
  Serial.begin(9600);
  pinMode(detect, INPUT);
}

void loop() {
  etatDetect = digitalRead(detect);
  if(sendAlert && etatDetect == 0){
    xbee.print("AlerteVol");
    sendAlert = false;
  }
  if(!sendAlert && etatDetect == 1){
    sendAlert = true;
  }
  Serial.println(etatDetect);
  delay(200);

}
