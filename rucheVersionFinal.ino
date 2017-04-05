/*

  Données(envoyé):
  0 !V{ID}
  1 !H{ID};{arg}
  2 !T{ID};{arg}
  3 !B{ID};{arg}

  Données(reçu):
  on
  off
  ?

*/

#include <SoftwareSerial.h>
#include <HX711.h> //inclure la bibliothèque -> Emilien

#define S0 2 //définir broche de sélection multiplexeur -> Emilien 
#define S1 3  //définir broche de sélection multiplexeur -> Emilien
#define bat A0
#define detect 4

#include <SoftwareSerial.h>

boolean sVol = true, sHygro = true, sTemp = true, sBat = true;
String text;
boolean sendAlert = true;

int etatDetect = 1, batterie = 20, idRuche = 1; //Variable a modifié

int intervalleHygrometrie[] = {5, 20};
int intervalleTemperature[] = {5, 20};
int intervalleMasse[] = {5, 20};

long instant = millis();

HX711 scale (A4, A5); //défini les broches d'entrée de données -> Emilien

SoftwareSerial xbee(2, 3);

void setup() {
  Serial.begin(9600);
  xbee.begin(9600);
  pinMode(detect, INPUT);
  pinMode(S0, OUTPUT); //emilien
  pinMode(S1, OUTPUT); //emilien
  scale.set_scale(118200); //la valeur obtenue par le programme de calibration -> emilien
  scale.tare(); //emilien

  idRuche = getId();
}

void loop() {
  checkAlert();
  //checkReset(); //a remettre

  String data = "";
  while (xbee.available()) {
    char c = xbee.read();
    Serial.println("Receive char ->" + c);
    if (c == '#') {
      Serial.println("Receive complete ->" + c);
      Serial.println("Receive char ->" + data);
      parseData(data);
    } else {
      data += c;
    }
  }
}

void parseData(String data) {
  if (data.startsWith("?")) {
    delay(idRuche * 3000);
    sendXbee(protocol());
  } else {
    sendAlert = data.startsWith("on");
  }
}

void checkAlert() {
  if (!sendAlert)
    return;

  if (millis() - instant < 10000) {
    instant = millis();
    float humidity = getHumidity();
    float temperature = getTemperature();

    if (humidity < intervalleHygrometrie[0] || humidity > intervalleHygrometrie[1]) {
      if (sHygro)
        sendXbee(protocolHygrometrie());
    }
    if (temperature < intervalleTemperature[0] || temperature > intervalleTemperature[1]) {
      if (sTemp)
        sendXbee(protocolTemperature());
    }
    if (batterie < 5) {
      if (sBat)
        sendXbee(protocolBatterie());
    }
  }

  //etatDetect = 1/*digitalRead(detect)*/;
  if (sVol && etatDetect == 0) {
    sendXbee(protocolVol());
    sVol = false;
  }
  if (!sVol && etatDetect == 1) {
    sVol = true;
  }
}


void sendXbee(String data) {
  xbee.print(data);
  Serial.println("Envois > " + data);
}

void resetAlert() {
  sVol = true, sHygro = true, sTemp = true, sBat = true;
}

String protocolVol() {
  return "!V" + String(idRuche) + "#";
}
String protocolHygrometrie() {
  sHygro = false;
  return "!H" + String(idRuche) + ";" + String(getHumidity()) + "#";
}
String protocolTemperature() {
  sTemp = false;
  return "!T" + String(idRuche) + ";" + String(getTemperature()) + "#";
}
String protocolBatterie() {
  sBat = false;
  return "!B" + String(idRuche) + ";" + String(batterie) + "#";
}
String protocol() { //maxi
  return "D" + String(idRuche) + ";"  + String(getMasse()) + ";" + String(getTemperature()) + ";" + String(getHumidity()) + "#";
}

int getBat() {
  return analogRead(bat);
}

//Partie emilien

void masseLoop() {
  for (byte count = 0; count < 3 ; count++) { //incrémente les pattes du multiplexeur de 0 à 3
    digitalWrite(S0, bitRead(count, 0));
    digitalWrite(S1, bitRead(count, 1));

    float reading = getMasse();
    Serial.print ("valeurSEL"); //afficher dans la console la broche sélectionné par le multi

    Serial.print(bitRead(count, 1));
    Serial.print(bitRead(count, 0));
    Serial.print(' ');
    Serial.println(reading);

    delay (1000);
  }
}

float getMasse() {
  return 0;
}

float getTemperature() {
  return 0;
}

float getHumidity() {
  return 0;
}

int getId() {
  return 1;
}


