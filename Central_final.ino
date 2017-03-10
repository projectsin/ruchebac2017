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
unsigned long val_masse;                //Valeurs
unsigned long val_temperature;
unsigned long val_hygrometrie;
boolean etat_mesure;
int val_1;
int val_2;
int val_3;
int val_4;
int index_val1;
int index_val2;
int index_val3;
int index_val4;

void setup()
{
  XBEE.begin(9600);
  Serial.begin(9600);
  Serial.flush();          
}

void demande_mesures()  {
  id_ruche = 1;
 
}

void code_message()
{
  Str_chaine1 = "DM"+String(id_ruche);
  Str_message = '$'+Str_chaine1;
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
  if(Str_message.startsWith("$"))                     
  {
    index_val1 = Str_message.indexOf('#');
    index_val2 = Str_message.indexOf('#',index_val1+1);
    index_val3 = Str_message.indexOf('#',index_val2+1);
    index_val4 = Str_message.indexOf('#',index_val3+1);
    Str_chaine1 = Str_message.substring(index_val1+1);
    id_ruche = Str_chaine1.toInt();
    Str_chaine2 = Str_message.substring(index_val2+1);
    val_masse = Str_chaine2.toInt();
    Str_chaine3 = Str_message.substring(index_val3+1);
    val_temperature = Str_chaine3.toInt();
    Str_chaine4 = Str_message.substring(index_val4+1);
    val_hygrometrie = Str_chaine4.toInt();
  }
}

void loop()
{
  val_masse = 0;
  val_temperature = 0;
  val_hygrometrie = 0;
  demande_mesures();
  code_message();
  ecrire_message();
  delay(10000);
  lire_message();
  decode_message();
  Serial.print("Numero de ruche : ");
  Serial.print(id_ruche);
  Serial.print("  Masse : ");
  Serial.print(val_masse);
  Serial.print(" Temperature : ");
  Serial.print(val_temperature);
  Serial.print(" Hygrometrie : ");
  Serial.println(val_hygrometrie);
}


