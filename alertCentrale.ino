boolean activatedAlert = true; //Modifiable via l'ihm

void setup() {
    Serial.begin(9600);
    Serial1.begin(9600);
}
void loop() {
    checkAlert();
}

void sendAlertToOperator(){
    //Envois sms et appel opérateur
}

void checkAlert(){
    while(Serial1.available()){
        if(Serial1.read() == "Alerte" && activatedAlert == true){
            sendAlertToOperator();
            activatedAlert = false;
        }
    }
}
  
