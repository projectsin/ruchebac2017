#include <Thread.h>
#include <Wire.h>
#include <HX711.h> //inclure la bibliothèque -> Emilien

#define S0 2 //définir broche de sélection multiplexeur 
#define S1 3  //définir broche de sélection multiplexeur 
#define DOUT A1
#define CLK A2
float tableau[] = {46500 , 45100 , 44500 , 42000}; //valeur de calibration
int pointeur [4] = {0, 1, 3, 2}; //défini l'ordre des capteurs pour éviter les conflits (binaire réfléchi)
float tare [5];
float val;
float val_final;

HX711 pesee (DOUT, CLK);

#define bat A0
#define detect 6

#define HTU21DF_I2CADDR       0x40
#define HTU21DF_READTEMP      0xE3
#define HTU21DF_READHUM       0xE5
#define HTU21DF_WRITEREG       0xE6
#define HTU21DF_READREG       0xE7
#define HTU21DF_RESET       0xFE

boolean sVol = true, sHygro = true, sTemp = true, sBat = true;
String text;
boolean sendAlert = true;

int etatDetect = 1;

int hiveId;

int bits[] = {8, 9, 10, 11, 12, 13};

int intervalleHygrometrie[] = {5, 20};
int intervalleTemperature[] = {5, 20};
int intervalleMasse[] = {5, 20};

long instant = millis();

#include <SoftwareSerial.h>
SoftwareSerial xbee(5, 4);

void setup() {
  Serial.begin(1200);
  xbee.begin(9600);

  for (int index = 0 ; index <= 5 ; index++)
    pinMode(bits[index], INPUT_PULLUP);

  hiveId = getId();

  setupMass();
  setupWire();
}


void setupMass() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  for (int count = 0; count < 4 ; count++) {
    digitalWrite(S0, bitRead(pointeur[count], 0));
    digitalWrite(S1, bitRead(pointeur [count], 1));
    val = pesee.get_value();
    tare[pointeur[count]] = val;
  }
  delay(100);
  Serial.println("Mass : OK");
}

void setupWire() {
  Wire.begin();
  Wire.beginTransmission(0x40);
  Wire.write(0xE7);
  Wire.endTransmission();
  Wire.requestFrom(HTU21DF_I2CADDR, 1);
  Serial.println("Wire : OK");
}

void loop() {
  //checkAlert();
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
    delay(hiveId * 3000);
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

  if (millis() - instant > 60000) {
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

  etatDetect = digitalRead(detect);
  if (sVol && etatDetect == 0) {
    sendXbee(protocolVol());
    sVol = false;
  }
}


void sendXbee(String data) {
  xbee.print(data);
  Serial.println("Xbee > " + data);
}

void resetAlert() {
  sVol = true, sHygro = true, sTemp = true, sBat = true;
}

String protocolVol() {
  return "!V" + String(hiveId) + "#";
}
String protocolHygrometrie() {
  sHygro = false;
  return "!H" + String(hiveId) + ";" + String(getHumidity()) + "#";
}
String protocolTemperature() {
  sTemp = false;
  return "!T" + String(hiveId) + ";" + String(getTemperature()) + "#";
}
String protocolBatterie() {
  sBat = false;
  return "!B" + String(hiveId) + ";" + String(getBattery()) + "#";
}
String protocol() { //maxi
  return "D" + String(hiveId) + ";"  + String(getMasse()) + ";" + String(getTemperature()) + ";" + String(getHumidity()) + "#";
}

float getBattery() {
  return random(5, 100);
  //return (float(BatteryValue)*5)/1023*2;
}

//Partie emilien

float getMasse() {
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

float getTemperature() {
  Wire.beginTransmission(HTU21DF_I2CADDR);
  Wire.write(HTU21DF_READTEMP);
  Wire.endTransmission();
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
  return temp;
}

float getHumidity() {
  Wire.beginTransmission(HTU21DF_I2CADDR);
  Wire.write(HTU21DF_READHUM);
  Wire.endTransmission();
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
  return hum;
}

int getId() {
  byte dizaine;
  byte unit;

  for (int index = 0 ; index <= 1 ; index++)
    bitWrite(dizaine, index, !digitalRead(bits[index + 4]));

  for (int index = 0 ; index <= 3 ; index++)
    bitWrite(unit, index, !digitalRead(bits[index]));

  return 10 * dizaine + unit;
}
