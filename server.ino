#include <GSM.h>
#include <MsTimer2.h>

#define PINNUMBER "1234"

char remoteNumber[20] = "0651769265";

char server[] = "88.162.164.62";
int port = 2000;

#define GPRS_APN       "free"
#define GPRS_LOGIN     "free"
#define GPRS_PASSWORD  ""

String toSend;
long updateTime = millis();
boolean connectionActive = false;

GSMClient client;
GPRS gprs;
GSM gsmAccess;
GSMVoiceCall vcs;
GSM_SMS sms;

String xbeeData = "";
String clientData = "";

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  connectWithInternet();

  MsTimer2::set(30000, ask);
  MsTimer2::start();
}

void connectClient() {
  Serial.println("Connecting to server...");
  if (client.connect(server, port)) {
    client.println("H");
    Serial.println("Connected to server..");
    connectionActive = true;
  }  else {
    Serial.println("Not connected to server...");
    connectionActive = false;
  }
}

void loop() {
  if ((millis() - updateTime) > (connectionActive ? 5 * 60000 : 60000)) {
    checkConnection();
    updateTime = millis();
  }


  if (Serial1.available() != 0)  {
    char c = Serial1.read();
    //Serial.println("Receive char -> " + String(c)); //debug mode

    if (c == '#') {
      parseHiveData(xbeeData);
      Serial.println("Receive X -> " + xbeeData); //debug mode
      xbeeData = "";
    } else {
      xbeeData += String(c);
    }
  }

  if (client.available() != 0)  {
    char c = client.read();
    Serial.println("Receive char (client) -> " + String(c)); //debug mode
    if (c == '#') {
      Serial.println("Receive (client) -> " + clientData); //debug mode
      parseClientData(clientData);
      clientData = "";
    } else
      clientData += String(c);
  }
}

void checkConnection() {
  if (client.connected()) {
    if (toSend.length() != 0)
      client.print(toSend);
  } else {
    connectClient();
  }
}

void parseClientData(String data) {
  switch (data.charAt(0)) {
    case 'N':
      data.substring(1).toCharArray(remoteNumber, 20);
      break;
    case 'A':
      Serial1.println(data + "#");
      Serial.println("Envois xbee > " + data + "#");
      break;
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
    client.print("D" + data);
  } else if (data.startsWith("!")) { //PROTOCOL : !'CHAR'ID;ARG
    switch (data.charAt(1)) {
      case 'T':
        sendSms("Alerte > la temperature de la ruche " + getValue(data.substring(2), ';', 0) + " est de " +  getValue(data.substring(2), ';', 1), true);
        break;
      case 'V':
        sendSms("Alerte > la ruche " + getValue(data.substring(2), ';', 0) + " subis un vol", true);
        callGsm();
        break;
      case 'H':
        sendSms("Alerte > la hygrometrie de la ruche " + getValue(data.substring(2), ';', 0) + " est de " +  getValue(data.substring(2), ';', 1), true);
        break;
      case 'B':
        sendSms("Alerte > la batterie de la ruche " + getValue(data.substring(2), ';', 0) + " est de " +  getValue(data.substring(2), ';', 1) + "%", true);
        break;
    }
  }
}



void ask() {
  Serial1.println("?#");
  Serial.println("Asking...");
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

void connectWithInternet() {
  if ((gsmAccess.begin(PINNUMBER) == GSM_READY) & (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) {
    Serial.println("Sim cart connected");
    connectClient();
  } else
    Serial.println("Not connected");
}

void connectWithoutInternet() {
  if ((gsmAccess.begin(PINNUMBER) == GSM_READY)) {
    Serial.println("Sim cart connected without internet");
  } else
    Serial.println("Not connected");
}

void sendSms(String message, boolean call) {
  connectWithoutInternet();
  sms.beginSMS(remoteNumber);
  sms.print(message);
  sms.endSMS();
  if (call)
    callGsm();
  connectWithInternet();
}
void callGsm() {
  if (vcs.voiceCall(remoteNumber)) {
    vcs.hangCall();
  }
}






