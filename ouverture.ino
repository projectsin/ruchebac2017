#define detect 2
int etatDetect;

void setup() {
  Serial.begin(9600);
  pinMode(detect, INPUT);

}

void loop() {
  etatDetect = digitalRead(detect);
  Serial.println(etatDetect);
  delay(200);

}
