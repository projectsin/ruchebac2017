//#include <SoftwareSerial.h>
#include <GSM.h>

#define PINNUMBER "1234"


class SmsSender {
  public:
  GSM gsmAccess;
  GSM_SMS sms;
  
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
};

#ifndef Morse_h
#define Morse_h

class XbeeSender {
  #include <SoftwareSerial.h>
  SoftwareSerial xbee(2, 3);
  xbee.begin(9600);
  public:
  void sendData(String data){
    xbee.print(data);
    Serial.println(data);
  }
  
};

#endif


void setup() {
  Serial.begin(9600);
}

void loop() {
}

