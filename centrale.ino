#include <GSM.h>
#define PINNUMBER "1234"

GSM gsmAccess;
GSM_SMS sms;
GSMVoiceCall vcs;

boolean alert = false;

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

  while (Serial.available()) {
    String data = Serial.readString();
    Serial1.print("DM_R1");
    Serial.println("DM_R1");
  }
  while (Serial1.available()) {
    Serial.println("Reception <- " + Serial1.readString());
  }
}
void sendSms(String msg)
{
  sms.beginSMS("0611511932");
  sms.print(msg);
  sms.endSMS();
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

String protocol(String id, String masse, String temperature, String humidity) {
  return "D" + id + ";"  + masse + ";" + temperature + ";" + humidity;
}
