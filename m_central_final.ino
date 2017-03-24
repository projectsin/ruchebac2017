#include <SoftwareSerial.h>
SoftwareSerial XBEE(2, 3);

// Affectation des broches du micro-contrôleur



// Définition des variables

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

long time = millis();

void setup()
{
  XBEE.begin(9600);
  Serial.begin(9600);
  Serial.flush();
}

void loop()
{
  val_masse = 0;
  val_temperature = 0;
  val_humidity = 0;

  demande_mesures();

  lire_message();
}


void demande_mesures()  {
  if (millis() - time > 15000) {
    Serial.println("Send ?");
    XBEE.println("?");
    time = millis();
  }
}



void lire_message()
{
  message = "";
  while (XBEE.available() != 0)  {
    char c = XBEE.read();
    if (c == '#') {
      decode_message();
      Serial.println(message); //debug
      message = "";
    } else {
      message = String(message + c);
    }
  }
}

void decode_message()
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
}


