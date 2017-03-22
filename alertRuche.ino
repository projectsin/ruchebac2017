/*

Données(envoyé):
0 !Alert#Vol#{ID} 
1 !Alert#Hygrometrie#{Valeur}
2 !Alert#Temperature#{Valeur}
3 !Alert#Batterie#{Niveau de batterie}
4 !Alert#Mieller#ID-Ruche <- Chute de masse avec détecteur d'ouverture sur 1
  
Données(reçu):
reset

*/

#define detect 2
#include <SoftwareSerial.h>

const String protocolAlert = "!Alert#";
const int timerConst = 5; //en seconde

boolean sVol = true, sHygro = true, sTemp = true, sBat = true;

int etatDetect = 1, hygrometrie = 3, temperature = 15, masse = 10, batterie = 2, idRuche = 1; //Variable a modifié
int timer; //En seconde

int intervalleHygrometrie[] = {5,20};
int intervalleTemperature[] = {5,20};
int intervalleMasse[] = {5,20};

SoftwareSerial xbee(2, 3);

void setup() {
  Serial.begin(9600);
  xbee.begin(9600);
  pinMode(detect, INPUT);
  timer = timerConst;
}

void loop() {
  checkAlert();
  checkReset();
}

void checkAlert(){
  delay(1000);
  timer--;
  if(timer <= 0){
    if(hygrometrie < intervalleHygrometrie[0] || hygrometrie > intervalleHygrometrie[1]){
      if(sHygro) sendXbee(protocolHygrometrie());
    }
    if(temperature < intervalleTemperature[0] || temperature > intervalleTemperature[1]){
      if(sTemp) sendXbee(protocolTemperature());       
    }
    if(batterie < 5){
      if(sBat) sendXbee(protocolBatterie());
    }
    timer = timerConst;
  }
  
  etatDetect = 0/*digitalRead(detect)*/;
  if(sVol && etatDetect == 0){
    sendXbee(protocolVol());
    sVol = false;
  }
  if(!sVol && etatDetect == 1){
    sVol = true;
  }
  Serial.println(etatDetect);
}

void checkReset(){
  while(xbee.available()){
    if(xbee.readString().equals("reset")){
      resetAlert();     
    }
  }
}

void sendXbee(String str){
    xbee.print(str);
    Serial.println("Envois > "+str);
    delay(2000);
}

void resetAlert(){
  sVol = true, sHygro = true, sTemp = true, sBat = true;
}

String protocolVol(){
  return protocolAlert + "Vol#" + String(idRuche);
}
String protocolHygrometrie(){
  sHygro = false;
  return protocolAlert + "Hygrometrie#" + String(hygrometrie);
}
String protocolTemperature(){
  sTemp = false;
  return protocolAlert + "Temperature#" + String(temperature); 
}
String protocolBatterie(){
  sBat = false;
  return protocolAlert + "Batterie#" + String(batterie); 
}
