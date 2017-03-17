/*

Données:
!Alert#Vol#{ID}
!Alert#Hygrometrie#{Valeur}
!Alert#Temperature#{Valeur}
!Alert#Batterie#{Niveau de batterie}
!Alert#Mieller#ID-Ruche <- Chute de masse avec détecteur d'ouverture sur 1

*/

#define detect 2
#include <SoftwareSerial.h>

const String protocolAlert = "!Alert#";
const int timerConst = 5; //en seconde

int etatDetect = 1, hygrometrie = 10, temperature = 10, masse = 10, batterie = 10, idRuche = 1; //Variable a modifié
int timer; //En seconde

int intervalleHygrometrie[] = {5,20};
int intervalleTemperature[] = {5,20};
int intervalleMasse[] = {5,20};

boolean sendAlert = true;

SoftwareSerial xbee(2, 3);

void setup() {
  Serial.begin(9600);
  pinMode(detect, INPUT);
  timer = timerConst;
}

void loop() {
  checkAlert();
}

void checkAlert(){
  delay(1000);
  timer--;
  if(timer <= 0){
    if(hygrometrie < intervalleHygrometrie[0] || hygrometrie > intervalleHygrometrie[1]) xbee.print(protocolHygrometrie());
    if(temperature < intervalleTemperature[0] || temperature > intervalleTemperature[1]) xbee.print(protocolTemperature());
    if(batterie < 5) xbee.print(protocolBatterie());
    timer = timerConst;
  }
  
  etatDetect = digitalRead(detect);
  if(sendAlert && etatDetect == 0){
    xbee.print(protocolVol());
    sendAlert = false;
  }
  if(!sendAlert && etatDetect == 1){
    sendAlert = true;
  }
  Serial.println(etatDetect);
}

String protocolVol(){
  return protocolAlert + "Vol#" + String(idRuche);
}
String protocolHygrometrie(){
  return protocolAlert + "Hygrometrie#" + String(hygrometrie);
}
String protocolTemperature(){
  return protocolAlert + "Temperature#" + String(temperature); 
}
String protocolBatterie(){
  return protocolAlert + "Batterie#" + String(batterie); 
}
