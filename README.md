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

</p>

#define HTU21DF_I2CADDR       0x40 </P>
#define HTU21DF_READTEMP      0xE3 </p>
#define HTU21DF_READHUM       0xE5 </p>
#define HTU21DF_WRITEREG       0xE6 </p>
#define HTU21DF_READREG       0xE7 </p>
#define HTU21DF_RESET       0xFE </P>

</p>

void setup() { </p>
Serial.begin(9600); </p>
Serial.println("Adafruit test.."); </p>
Wire.begin(); </p>
reset(); </P>
Wire.beginTransmission(0x40); </p>
Wire.write(0xE7); </p>
Wire.endTransmission(); </p>
Wire.requestFrom(HTU21DF_I2CADDR, 1); </p>
if(Wire.read() == 0x2) </p>
{ </p>
  Serial.println("HTU21d ok !"); </p>
 } </p>
 else </p>
 { </p>
  Serial.println("ERROR..."); </p>
  } </p>

</p>

} </p>
 </p>
void reset() </p>
{ </p>
  Wire.beginTransmission(0x40); </p>
 Wire.write(0xFE);</p>
 Wire.endTransmission();</p>
 delay(15);</p>
}</p>
void Temperature() {</p>
</p>
  Wire.beginTransmission(HTU21DF_I2CADDR); </p>
  Wire.write(HTU21DF_READTEMP); </p>
  Wire.endTransmission();</p>
  
</p>
</p>
  
  delay(50); </p>
     
   <p>

  //Wire.begin(); </p>
  Wire.requestFrom(HTU21DF_I2CADDR, 3);</p>
while (!Wire.available()) {}</p>


 uint16_t t = Wire.read();</p>
  t <<= 8;</p>
  t |= Wire.read();</p>
</p>
  uint8_t crc = Wire.read();</p>
</p>
  float temp = t;</p>
  temp *= 175.72;</p>
  temp /= 65536;</p>
  temp -= 46.85;</p>
</p>
  Serial.print("Temperature :");</p>
  Serial.print(String(temp) + "   ");</p>
}</p>
void Humidity() {</p>
</p>
   Wire.beginTransmission(HTU21DF_I2CADDR); </p>
  Wire.write(HTU21DF_READHUM); </p>
  Wire.endTransmission();</p>
  </p>
  </p>
  
  delay(50); </p>
  </p>
  </p>
  
  //Wire.begin(); </p>
  Wire.requestFrom(HTU21DF_I2CADDR, 3); </p>
  while (!Wire.available()) {} </p>
</p>
  uint16_t h = Wire.read();</p>
  h <<= 8;</p>
  h |= Wire.read();</p>

 //uint8_t crc = Wire.read();</p>
</p>
  float hum = h;</p>
  hum *= 125;</p>
  hum /= 65536;</p>
  hum -= 6;</p>
  hum *= 1/0.9925785124; </p>
  hum += 6.9250595041;</p>
 Serial.print("Humidite :"); </p>
 Serial.println(hum);</p>
}</p>
  </p>
 void loop()</p>
 {</p>

</p>
Temperature();</p>
delay(500);</p>
Humidity();</p>
delay(500);</p>
}</p>
