boolean activatedAlert = true; //Modifiable via l'ihm

void setup() {
    Serial.begin(9600);
}
void loop() {
    while(Serial1.available()){
      if(Serial1.read() == "Alerte" && activatedAlert == true){
         sendAlertToOperator();
         activatedAlert = false;
      }
    }
}

void sendAlertToOperator(){
    
}
