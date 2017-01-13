#include <GSM.h>

#define PINNUMBER ""

GSM gsmAccess; // include a 'true' parameter for debug enabled
GSM_SMS sms;

char remoteNumber[20]= "12125551212";  
char txtMsg[200]="Test";

void setup()
{
  Serial.begin(9600);
}

void loop()
{
// nothing to see here
}

void sendSMS(char number, char msg){
  remoteNumber[20] = number;
  txtMsg[200] = msg;
  boolean notConnected = true;
  while(notConnected)
  {
    if(gsmAccess.begin(PINNUMBER)==GSM_READY)
      notConnected = false;
    else
    {
      delay(1000);
    }
  }
  sms.beginSMS(remoteNumber);
  sms.print(txtMsg);
  sms.endSMS();  
}
