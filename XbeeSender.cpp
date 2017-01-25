#include <Arduino.h>
#include "XbeeSender.h"
#include <SoftwareSerial.h>

SoftwareSerial xbee(2, 3);
String data = "test xbee";

void SmsSender::send_xbee()
{
  xbee.begin(9600);
  xbee.print(data);
}