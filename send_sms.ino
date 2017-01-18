//#include <SoftwareSerial.h>
#include <GSM.h>

#define PINNUMBER "1234"

//SoftwareSerial xbee(2, 3);

GSM gsmAccess;
GSM_SMS sms;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  send_sms("Test4");
  //xbee.begin(9600);

  //sendData("Test xbee");
}

void loop() {
}

/*void sendData(String data){ //test
  xbee.print(data);
  Serial.println(data);
}*/

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
