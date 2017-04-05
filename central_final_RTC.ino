#include <SoftwareSerial.h>
#include <Wire.h>
#include <DS3231.h>
SoftwareSerial XBEE(3, 2); // Affectation des broches du micro-contrôleur



// Définition des variables
DS3231 clock;
RTCDateTime dt;
boolean isAlarm = false;


String message;

String Str_chaine1;
String Str_chaine2;
String Str_chaine3;
String Str_chaine4;

int id_ruche;

long val_masse;
long val_temperature;
long val_humidity;

boolean etat_mesure;
int index_val1;
int index_val2;
int index_val3;
int index_val4;


void setup()
{
  XBEE.begin(9600);
  Serial.begin(9600);
  Serial.flush();
  Serial.println("Initialize DS3231");;
  clock.begin();
  clock.setDateTime(__DATE__, __TIME__);
  clock.enableOutput(false);
  attachInterrupt(0, declenchement, FALLING);
  pinMode(2,INPUT_PULLUP);
}

void loop()
{
  val_masse = 0;
  val_temperature = 0;
  val_humidity = 0;
  clock.setAlarm1(0, 0, 0, 30, DS3231_MATCH_S);
  dt = clock.getDateTime();
  Serial.println(clock.dateFormat("d-m-Y H:i:s - l", dt));
  if (isAlarm == 1)
  {
    Serial.println("Heure de declenchement ! --> envoie de la demande"); //A retirer plus tard
    Serial.println("Mesure"); //A retirer plus tard
    //demande_mesures(); //A ajouter plus tard
    isAlarm = 0;
    //lire_message(); //A ajouter plus tard
    //ecrire_console(); //A ajouter plus tard
  }
  Serial.print("--> "); //A retirer plus tard
  }


void demande_mesures()  {

  Serial.println("Send ?"); //A retirer plus tard
  XBEE.println("?");

}



void lire_message()
{
  message = "";
  while (XBEE.available() != 0)  {
    char c = XBEE.read();
    if (c == '#') {
      decode_message(); //A retirer plus tard
      //new_decode_message //A ajouter plus tard
      Serial.println(message); //debug
      message = "";
    } else {
      message = String(message + c);
    }
  }
}

void decode_message() //A retirer plus tard
{
  if (message.startsWith("D"))
  {
    message = message.substring(1);

    index_val1 = message.indexOf(';');
    index_val2 = message.indexOf(';', index_val1 + 1);
    index_val3 = message.indexOf(';', index_val2 + 1);
    index_val4 = message.indexOf(';', index_val3 + 1);

    Str_chaine1 = message.substring(index_val1 + 1);
    Str_chaine2 = message.substring(index_val2 + 1);
    Str_chaine3 = message.substring(index_val3 + 1);
    Str_chaine4 = message.substring(index_val4 + 1);

    id_ruche = Str_chaine4.toInt();
    val_masse = Str_chaine1.toInt();
    val_temperature = Str_chaine2.toInt();
    val_humidity = Str_chaine3.toInt();
  }
}
void ecrire_console() //A changer plus tard
{
  Serial.println("--> Receiving data <--");
  Serial.print("-Hive number : n*");
  Serial.println(id_ruche);
  Serial.print("-Mass : ");
  Serial.print(val_masse);
  Serial.println("Kg");
  Serial.print("-Temperature : ");
  Serial.print(val_temperature);
  Serial.println("*C");
  Serial.print("-Humidity : ");
  Serial.print(val_humidity);
  Serial.println("%");
  Serial.println("");
}

void declenchement()
{
  isAlarm = true;
}

//void new_decode_message()
//{
//if (data.startsWith("D")) {
//data = data.substring(1);

//String id = getValue(data, ';', 0);
//String mass = getValue(data, ';', 1);
// String temperature = getValue(data, ';', 2);
// String humidity = getValue(data, ';', 3);
//}
