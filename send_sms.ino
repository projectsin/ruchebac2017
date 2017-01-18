#include <GSM.h>

#define PINNUMBER "1234"

GSM gsmAccess;
GSM_SMS sms;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  send_sms("Test2");
}

void loop() {
 if(sms.available()){
  Serial.println("Message reçu !");
 }
 delay(1000);
}

void send_sms(String msg){
  boolean notConnected = true;
  Serial.println("Connection...");
  while (notConnected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
      notConnected = false;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }
  Serial.println("Envoie...");
  sms.beginSMS("0611511932");
  sms.print(msg);
  sms.endSMS();
  Serial.println("Message envoyé !");
}
