#include <Arduino.h>

int cnt = 0;
int myTime;
void isr() {
    cnt++;
}

void setup() {
Serial.begin(9600);
delay(3000);
Serial.println("Start:");
pinMode(13, INPUT);
pinMode(7, OUTPUT);
attachInterrupt(13, isr, RISING);
myTime = micros();

}

void loop() {
    digitalWrite(7, !digitalRead(7));
    if(cnt>=15000) {
        int duration = micros()-myTime;
        float frequency = 15/duration;
        Serial.printf("15000 Impulse; Dauer: %5d ms; Frequenz: %5.3f Hz\n", duration, frequency);
        cnt = 0;
        myTime = micros();

    }


}