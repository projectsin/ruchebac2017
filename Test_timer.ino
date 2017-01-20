#include <MsTimer2.h>

boolean output = HIGH;

void allumer() {
  digitalWrite(14, output == HIGH ? LOW : HIGH);
  digitalWrite(16,  output == HIGH ? LOW : HIGH);
  output =  output == HIGH ? LOW : HIGH;
}
void setup() {
pinMode (14, OUTPUT);
pinMode (16, OUTPUT);
MsTimer2::set(43200000, allumer); //43200000ms = 12 heures
  MsTimer2::start();
}

void loop() {
  
}
