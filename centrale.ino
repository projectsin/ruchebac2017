#include <GSM.h>
#define PINNUMBER "1234"

GSM gsmAccess;
GSM_SMS sms;

boolean alert = false;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
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
  boolean notConnected = true;
  while (notConnected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
      notConnected = false;
    } else {
      delay(1000);
    }
  }
  sms.beginSMS("0611511932");
  sms.print(msg);
  sms.endSMS();
}

String protocol(String id, String masse, String temperature, String humidity) {
  return "D" + id + ";"  + masse + ";" + temperature + ";" + humidity;
}
