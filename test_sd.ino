/*SD Card --> BUS SPI:
  MOSI -> pin 11, uno
  MISO -> pin 12
  SCK -> pin 13
  CS -> pin 53 */

#include <Wire.h>
#include <DS3231.h>
#include <SD.h>
#include <SPI.h>
DS3231 clock;
RTCDateTime dt;
File fileSD;


void setup() {
  Serial.begin(9600);
  Serial.flush();
  Serial.println("Initialize DS3231");
  clock.begin();
  clock.setDateTime(__DATE__, __TIME__); // La RTC prend l'heure de l'ordinateur
  pinMode(19, INPUT_PULLUP);
  if (!SD.begin(53)) {
    Serial.println("initialization failed");
    return;
  }
  Serial.println("initialization done");
}


void loop() {
  fileSD = SD.open("fileSD.txt", FILE_WRITE);
  int masse = random(101, 200);
  int temp = random(10, 70);
  int hygro = random(0, 100);
  dt = clock.getDateTime(); //Prend l'heure
  if (SD.exists("fileSD.txt")) {
    ;
  } else {
    fileSD.print("Date");
    fileSD.print(",");
    fileSD.print("Masse (Kg)");
    fileSD.print(",");
    fileSD.print("Temperature (*C)");
    fileSD.print(",");
    fileSD.println("Hygrometrie (/100)");
  }
  if (fileSD) {
    Serial.println("ecriture");
    //fileSD.print(",");  Ecriture de "," dans le fichier. Ce caractère permet de séparer les données pour ranger les données dans le tableur
    fileSD.print(clock.dateFormat("d-m-Y H:i:s - l", dt));
    fileSD.print(",");
    fileSD.print(masse);
    fileSD.print(",");
    fileSD.print(temp);
    fileSD.print(",");
    fileSD.println(hygro);
    fileSD.println(" ");
    fileSD.close();
    Serial.println("done");
  } else {
    Serial.println(" ");
  }




}


