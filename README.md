# ruchebac2017 

# Partie capteur température/hygrométrie:
<b> Etapes à suivre durant le projet: </b> </p>
</p>
Cette partie à pour but de prendre des mesures de la température ainsi que de l'humidité de chacune des ruches.
Les données prisent par les capteurs devrons être renvoyées à l'IHM ( interface homme machine).</p>
Pour ce faire, nous avons besoin d'un capteur d'une grande précision et si possible, qu'il puisse faire les deux mesures.
Voici le composant qui sera utilisé lors de notre pojet :</p>

https://cdn-shop.adafruit.com/datasheets/1899_HTU21D.pdf
https://learn.adafruit.com/adafruit-htu21d-f-temperature-humidity-sensor/overview
</p> Voici le programme final de la partie température/hygrometrie du projet: </p>


#include <Wire.h>



#define HTU21DF_I2CADDR       0x40
#define HTU21DF_READTEMP      0xE3
#define HTU21DF_READHUM       0xE5
#define HTU21DF_WRITEREG       0xE6
#define HTU21DF_READREG       0xE7
#define HTU21DF_RESET       0xFE



void setup() {
Serial.begin(9600);
Serial.println("Adafruit test..");
Wire.begin();
reset();
Wire.beginTransmission(0x40);
Wire.write(0xE7);
Wire.endTransmission(); 
Wire.requestFrom(HTU21DF_I2CADDR, 1);
if(Wire.read() == 0x2)
{
  Serial.println("HTU21d ok !");
 }
 else 
 {
  Serial.println("ERROR...");
  } 



}

void reset()
{
  Wire.beginTransmission(0x40);
 Wire.write(0xFE);
 Wire.endTransmission();
 delay(15);
}
void Temperature() {

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
}
void Humidity() {

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
  hum *= 1/0.9925785124; 
  hum += 6.9250595041;
 Serial.print("Humidite :"); 
 Serial.println(hum);
}
  
 void loop()
 {


Temperature();
delay(500);
Humidity();
delay(500);
}
