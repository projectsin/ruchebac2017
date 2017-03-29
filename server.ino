#include <GSM.h>
#include <MsTimer2.h>

#define PINNUMBER "1234"
char numbersOperators[] = {"0611511932"};

char server[] = "88.162.164.62";
int port = 2000;

#define GPRS_APN       "free"
#define GPRS_LOGIN     "free"
#define GPRS_PASSWORD  ""


GSMClient client;
GPRS gprs;
GSM gsmAccess;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  boolean notConnected = true;

  while (notConnected) {
    if ((gsmAccess.begin(PINNUMBER) == GSM_READY) & (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY))
      notConnected = false;
    else
      Serial.println("Not connected");
  }

  Serial.println("Connecting...");

  if (client.connect(server, port))
    Serial.println("Connected..");
  else
    Serial.println("Not connected");

  MsTimer2::set(60000, ask);
  MsTimer2::start();
}

void loop() {
  String data;
  while (Serial1.available() != 0)  {
    char c = Serial1.read();
    if (c == '#') {
      parseHiveData(data);
      Serial.println("Receive -> " + message); //debug mode
    } else
      data = String(message + c);
  }
}

void parseHiveData(String data) {
  if (data.startsWith("D")) {
    data = data.substring(1);

    String id = getValue(data, ';', 0);
    String mass = getValue(data, ';', 1);
    String temperature = getValue(data, ';', 2);
    String humidity = getValue(data, ';', 3);

    Serial.println("Hive " + id + " mass = " + mass + " temperature = " + temperature + " humidity = " + humidity);
  } else if (data.startsWith("!") { //PROTOCOL : !'CHAR'ID;ARG
  switch (data.charAt(1)) {
      case 'T':
        sendSms("Alerte > la temperature de la ruche " + getValue(data.substring(2), ';', 0) + " est de " +  getValue(data.substring(2), ';', 1));
        break;
      case 'V'
          sendSms("Alerte > la ruche " + getValue(data.substring(2), ';', 0) + " subis un vol");
        callGsm();
        break;
      case 'H'
          sendSms("Alerte > l'hygrometrie de la ruche " + getValue(data.substring(2), ';', 0) + " est de " +  getValue(data.substring(2), ';', 1));
        break;
      case 'B':
        sendSms("Alerte > la batterie de la ruche " + getValue(data.substring(2), ';', 0) + " est de " +  getValue(data.substring(2), ';', 1) + "%");
        break;
    }
  }
}

void ask() {
  Serial1.print("?");
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void sendSms(String sms) {
  Serial.println("Send sms > " + sms);
  for (char number : numbersOperators) {
    sms.beginSMS(number);
    sms.print(sms);
    sms.endSMS();
  }
}

void callGsm() {
  String remoteNumber = numbersOperators[0;
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

}
