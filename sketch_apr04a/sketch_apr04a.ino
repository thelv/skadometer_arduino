void setup(){
    Serial.begin(115200);
    Serial.print("14CORE | TEST CODE FOR ESP32 BLINK");
    pinMode(13, OUTPUT);
}
void loop(){
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
    Serial.println(millis()+1);
}
