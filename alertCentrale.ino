//#include <GSM.h>
#define PINNUMBER "1234"
#include <SoftwareSerial.h> //En mega > Serial1

/*GSM gsmAccess;
GSM_SMS sms;
GSMVoiceCall vcs;*/

SoftwareSerial xbee(2, 3); //En mega > Serial1

char numbersOperators[] = {"0611511932"};

boolean activatedAlert = true; //Modifiable via l'ihm
int idRuche = 1; 
String packet = "!Alert#Vol#1";
String subPacket;

void setup() {
    Serial.begin(9600);
    xbee.begin(9600);
    boolean notConnected = true;
    /*while (notConnected) {
        if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
            notConnected = false;
        } else {
            Serial.println("Not connected");
            delay(1000);
        }
    }*/
}
void loop() {
    while(Serial.available()){
      if(Serial.readString() == "reset"){
        Serial.println("Reset alerte");
        xbee.print("reset");
      }
    }
    checkAlert();
}

void checkAlert(){
    while(xbee.available()){
        if(activatedAlert){
            packet = xbee.readString();
            Serial.println(packet);
            if(packet.substring(0,7) == "!Alert#"){
              subPacket = packet.substring(7,10);
              if(subPacket == "Vol"){
                sendSms("Alerte > Vol (id=" + packet.substring(11) + ")");
                //callGsm();
              }
      
              if(subPacket == "Tem") sendSms("Alerte > Température (" + packet.substring(19) + "°C)");
              if(subPacket == "Hyg") sendSms("Alerte > Hygrométrie (" + packet.substring(19) + "%)");
              if(subPacket == "Bat") sendSms("Alerte > Niveau de batterie (" + packet.substring(16) + "%)");
             } 
        }
    }
    delay(200);
    
}

/*void callGsm(){
   String remoteNumber = "0611511932"; 
   char charbuffer[20];
   if (remoteNumber.length() < 20) {
      remoteNumber.toCharArray(charbuffer, 20);
      if (vcs.voiceCall(charbuffer)) {
         while (Serial.read() != '\n' && (vcs.getvoiceCallStatus() == TALKING));
         vcs.hangCall();
      }
        remoteNumber = "";
    } else {
       Serial.println("That's too long for a phone number. I'm forgetting it");
       remoteNumber = "";
    }
}*/

void sendSms(String msg){
  Serial.println(msg);
  xbee.flush();
  for(char n : numbersOperators){
     /*sms.beginSMS(n);
     sms.print(msg);
     sms.endSMS();*/
  }
}
