#include <SoftwareSerial.h>
SoftwareSerial XBEE(2,3);

// Affectation des broches du micro-contrôleur



// Définition des variables

String Str_message;       // chaîne de caractère transmise ( reçue ou émise )
String Str_chaine1;
String Str_chaine2;
String Str_chaine3;
String Str_chaine4;
int id_ruche;
unsigned long val_temperature;           //Valeurs
unsigned long val_hygrometrie;
unsigned long val_masse;
boolean etat_mesure;


void setup()
{
  XBEE.begin(9600);
  Serial.begin(9600);
  Serial.flush();          
}

void lire_mesures()
{
  id_ruche = 1;
  val_masse = random(10,70);
  val_temperature = random(0,35);
  val_hygrometrie = random(0,100);
}

void code_message()
{
  Str_chaine1 = '#'+String(id_ruche);
  Str_chaine2 = '#'+String(val_masse);
  Str_chaine3 = '#'+String(val_temperature);
  Str_chaine4 = '#'+String(val_hygrometrie);
  Str_message = '$'+Str_chaine1+Str_chaine2+Str_chaine3+Str_chaine4;
}

void ecrire_message()
{
  XBEE.println(Str_message);
}

void lire_message()                         
{
  Str_message = "";    
  while (XBEE.available() != 0)          
    {
     Str_message = String(Str_message+char(XBEE.read()));               
    }
}

void decode_message()                                 
{
  if(Str_message.startsWith("$DM1"))                     
  {
    etat_mesure = 1;
  }
}

void loop()
{
  lire_message();
  decode_message();
  Serial.println(etat_mesure);
  if  (etat_mesure==1)
  {
    lire_mesures();
    code_message();
    ecrire_message();
    etat_mesure = 0;
  }
  delay (5000);
}


