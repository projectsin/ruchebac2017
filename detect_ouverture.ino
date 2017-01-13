#define PIN_REED_SENSOR 2
int pinReedSensor;
int oldPinReedSensor;
void setup() {
    Serial.begin(9600);
    Serial.println("*** Setup ***");
    pinMode(PIN_REED_SENSOR, INPUT);
    oldPinReedSensor = digitalRead(PIN_REED_SENSOR);
}
void loop() {
    pinReedSensor = digitalRead(PIN_REED_SENSOR);
    if (pinReedSensor != oldPinReedSensor) {
        Serial.print("Reed = ");
        Serial.println(pinReedSensor);
        oldPinReedSensor = pinReedSensor;
    }
}
