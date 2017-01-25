#include <Arduino.h>
#include "SmsSender.h"
#include <GSM.h>

#define PINNUMBER "1234"

GSM gsmAccess;
GSM_SMS sms;
void SmsSender::send_sms(String msg)
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