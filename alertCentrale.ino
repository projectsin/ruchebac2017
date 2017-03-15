#include <GSM.h>
#define PINNUMBER "1234"

GSM gsmAccess;
GSM_SMS sms;
GSMVoiceCall vcs;

boolean activatedAlert = true; //Modifiable via l'ihm

void setup() {
    Serial.begin(9600);
    Serial1.begin(9600);
    boolean notConnected = true;
    while (notConnected) {
        if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
            notConnected = false;
        } else {
            Serial.println("Not connected");
            delay(1000);
        }
    }
}
void loop() {
    checkAlert();
}

void sendAlertToOperator(){
    callGsm();
}

void checkAlert(){
    while(Serial1.available()){
        if(Serial1.readString() == "Alerte" && activatedAlert == true){
            sendAlertToOperator();
            activatedAlert = false;
        }
    }
}

void callGsm(){
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
}
  
