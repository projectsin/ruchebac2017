/*
Données(envoyé):
0 !V{ID} 
1 !H{ID};{arg}
2 !T{ID};{arg}
3 !B{ID};{arg}
  
Données(reçu):
on
off

*/

#define detect 2
#include <SoftwareSerial.h>

const int timerConst = 5; //en seconde

boolean sVol = true, sHygro = true, sTemp = true, sBat = true;
String text;
boolean sendAlert = true;

int etatDetect = 1, hygrometrie = 15, temperature = 15, masse = 10, batterie = 20, idRuche = 1; //Variable a modifié
int timer; //En seconde

int intervalleHygrometrie[] = {5,20};
int intervalleTemperature[] = {5,20};
int intervalleMasse[] = {5,20};

SoftwareSerial xbee(2, 3);

void setup() {
  Serial.begin(9600);
  Serial.flush(); //maxi
  xbee.begin(9600);
  pinMode(detect, INPUT);
  timer = timerConst;
}

void loop() {
  checkAlert();
  checkReset();
  commands();
  while(xbee.available()){
    if(xbee.readString().startsWith("?")){
        delay(idRuche * 3000);
        xbee.println(protocol());
    }
  }
}

void checkAlert(){
  delay(1000);
  if(!sendAlert) return;
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
  
  //etatDetect = 1/*digitalRead(detect)*/;
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
    if(xbee.readString().equals("on")){
      sendAlert = true;
      resetAlert();     
    }
    if(xbee.readString().equals("off")){
      sendAlert = false;
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

void commands(){
  while(Serial.available()){//4
    text = Serial.readString();
    String subText = text.substring(0,3);
    int valeur = text.substring(4).toInt();
    if(subText == "tem"){
      temperature = valeur;
    }
    if(subText == "hyg"){
      hygrometrie = valeur;
    }
    if(subText == "bat"){
      batterie = valeur;
    }
    if(subText == "vol"){
      etatDetect = valeur;
    }
  }
}

String protocolVol(){
  return "!V" + String(idRuche);
}
String protocolHygrometrie(){
  sHygro = false;
  return "!H" + String(idRuche) + ";" + String(hygrometrie);
}
String protocolTemperature(){
  sTemp = false;
  return "!T" + String(idRuche) + ";" + String(temperature); 
}
String protocolBatterie(){
  sBat = false;
  return "!B" + String(idRuche) + ";" + String(batterie); 
}
String protocol() { //maxi
  return "D" + String(idRuche) + ";"  + String(masse) + ";" + String(temperature) + ";" + String(hygrometrie) + "#";
}


