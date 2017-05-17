/*SD Card --> BUS SPI:
  MOSI -> pin 11
  MISO -> pin 12
  SCK -> pin 13
  CS -> pin 4 */


#include <SD.h>
#include <SPI.h>

File fileSD;


void setup() {
  Serial.begin(9600);
  if (!SD.begin(4)) {
    Serial.println("initialization failed");
    return;
  }
  Serial.println("initialization done");
}

/*Pour plus tard #SauvegardeSD
    Serial.println(t.year);
    Serial.println(t.month);
    Serial.println(t.day);
    Serial.println(t.DayOfWeek);
    Serial.println(t.hour);
    Serial.println(t.minute); */

void loop() {
  fileSD = SD.open("fileSD.txt", FILE_WRITE);
   int masse = random(101,200);
  int temp = random(10, 70);
   int hygro = random(0, 100);

  if (fileSD) {
    Serial.println("ecriture");
    fileSD.print(",");
    fileSD.print("Date / Heure");
    fileSD.print(t.year);
    fileSD.print(";");
    fileSD.println(t.month);
    fileSD.print(";");
    fileSD.println(t.day);
    fileSD.print(";");
    fileSD.println(t.hour);
    fileSD.print(":");
    fileSD.println(t.minute);
    fileSD.print(","); //Ecriture de "," dans le fichier. Ce caractère permet de séparer les données pour ranger les données dans le tableur
    fileSD.print("Masse : ");
    fileSD.print(masse);
    fileSD.print(",");
    fileSD.print("temperature : ");
    fileSD.print(temp);
    fileSD.print(",");
     fileSD.print("hygrometrie : ");
    fileSD.println(hygro);
    //fileSD.print(",");
    fileSD.close();
    Serial.println("done");
  } else {
    Serial.println(" ");
  }



  
  }


