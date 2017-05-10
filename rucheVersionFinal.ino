/*
  Données(envoyé):
  0 !V{ID}#
  1 !H{ID};{arg}#
  2 !T{ID};{arg}#
  3 !B{ID};{arg}#
  Données(reçu):
  A1
  A0
  ?
*/

//léo
#include <Wire.h>
//---

//Define emilien
#include <HX711.h> //inclure la bibliothèque -> Emilien

#define S0 2 //définir broche de sélection multiplexeur 
#define S1 3  //définir broche de sélection multiplexeur 
#define DOUT A2
#define CLK A1
float tableau[] = {46500 , 45100 , 44500 , 42000}; //valeur de calibration
int pointeur [4] = {0, 1, 3, 2}; //définie l'ordre des capteurs pour éviter les conflits (binaire réfléchie)
float tare [5];
float val;
float val_final;

HX711 pesee (DOUT, CLK);
//---

#define bat A0
#define detect 4

#define HTU21DF_I2CADDR       0x40
#define HTU21DF_READTEMP      0xE3
#define HTU21DF_READHUM       0xE5
#define HTU21DF_WRITEREG       0xE6
#define HTU21DF_READREG       0xE7
#define HTU21DF_RESET       0xFE

boolean sVol = true, sHygro = true, sTemp = true, sBat = true;
String text;
boolean sendAlert = true;

int etatDetect = 1, idRuche = 1; //Variable a modifié

int bits[] = {8, 9, 10, 11, 12, 13};

int intervalleHygrometrie[] = {5, 20};
int intervalleTemperature[] = {5, 20};
int intervalleMasse[] = {5, 20};

long instant = millis();

int id = 0;

#include <SoftwareSerial.h>
SoftwareSerial xbee(2, 3);

void setup() {
  Serial.begin(9600);
  xbee.begin(9600);

  for (int index = 0 ; index <= 5 ; index++) {
    pinMode(bits[index], INPUT_PULLUP);
  }

  idRuche = getId();

  //Emilien setup
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);

  for (int count = 0; count < 4 ; count++)
  {
    digitalWrite(S0, bitRead(pointeur[count], 0));
    digitalWrite(S1, bitRead(pointeur [count], 1));
    val = pesee.get_value(10);
    tare[pointeur[count]] = val;
    Serial.print("Tare :");

    Serial.println(val);
  }
  //---

  //Léo setup
  reset();
  Wire.begin();
  Wire.beginTransmission(0x40);
  Wire.write(0xE7);
  Wire.endTransmission();
  Wire.requestFrom(HTU21DF_I2CADDR, 1);
  if (Wire.read() == 0x2)
  {
    Serial.println("HTU21d ok !");
  }
  else
  {
    Serial.println("ERROR...");
  }
  //---
}

void loop() {
  delay(100);

  checkAlert();

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
  while (Serial.available()) {
    if (Serial.readString() == "t") {
      sendXbee(protocolBatterie());
    }
  }
  //Temperature();
  //Humidity();

}

void parseData(String data) {
  if (data.startsWith("?")) {
    delay(idRuche * 3000);
    sendXbee(protocol());
  } else {
    if (data.startsWith("A1")) resetAlert();
    sendAlert = data.startsWith("A1");
    Serial.println((sendAlert) ? "Activé" : "Désactivé");
    xbee.flush();
  }
}

void checkAlert() {
  if (!sendAlert)
    return;

  if (millis() - instant < 10000) {
    instant = millis();
    float humidity = getHumidity();
    float temperature = getTemperature();
    float batterie = getBattery();

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
  return "!B" + String(idRuche) + ";" + String(getBattery()) + "#";
}
String protocol() { //maxi
  return "D" + String(idRuche) + ";"  + String(getMasse()) + ";" + String(getTemperature()) + ";" + String(getHumidity()) + "#";
}

float getMasse() {
  return random(5, 20);
}

float getTemperature() {
  return random(5, 20);
}

float getHumidity() {
  return random(5, 20);
}

byte getBattery() {
  return random(5, 100);
}

//Partie emilien

float calcul_masse()
{
  val = 0;
  for (byte count = 0; count < 4 ; count++)
  {
    digitalWrite(S0, bitRead(pointeur[count], 0));
    digitalWrite(S1, bitRead(pointeur[count], 1));
    //
   // Serial.print (pointeur[count]);
    //Serial.print("-->");
    val = val + (pesee.get_value(10) - tare[pointeur[count]]) / tableau[pointeur[count]];
  }
  Serial.println (val);
  Serial.print ("Mesure masse : ");
  return val;
}

//--- Fin emilien

void reset()
{
  Wire.beginTransmission(0x40);
  Wire.write(0xFE);
  Wire.endTransmission();
  delay(15);
}

float Temperature() {

  Wire.beginTransmission(HTU21DF_I2CADDR);
  Wire.write(HTU21DF_READTEMP);
  Wire.endTransmission();




  delay(50);



  //Wire.begin();
  Wire.requestFrom(HTU21DF_I2CADDR, 3);
  while (!Wire.available()) {}


  uint16_t t = Wire.read();
  t <<= 8;
  t |= Wire.read();

  uint8_t crc = Wire.read();

  float temp = t;
  temp *= 175.72;
  temp /= 65536;
  temp -= 46.85;

  Serial.print("Temperature :");
  Serial.print(String(temp) + "   ");
  return temp;
}
float Humidity() {

  Wire.beginTransmission(HTU21DF_I2CADDR);
  Wire.write(HTU21DF_READHUM);
  Wire.endTransmission();



  delay(50);



  //Wire.begin();
  Wire.requestFrom(HTU21DF_I2CADDR, 3);
  while (!Wire.available()) {}

  uint16_t h = Wire.read();
  h <<= 8;
  h |= Wire.read();

  //uint8_t crc = Wire.read();

  float hum = h;
  hum *= 125;
  hum /= 65536;
  hum -= 6;
  hum *= 1 / 0.9925785124;
  hum += 6.9250595041;
  Serial.print("Humidite :");
  Serial.println(hum);
  return hum;
}

int getId() {
  if ( id == 0) {
    byte dizaine;
    byte unit;

    for (int index = 0 ; index <= 1 ; index++)
      bitWrite(dizaine, index, !digitalRead(bits[index + 4]));

    for (int index = 0 ; index <= 3 ; index++)
      bitWrite(unit, index, !digitalRead(bits[index]));

    return id = 10 * dizaine + unit;
  } return id;
}


