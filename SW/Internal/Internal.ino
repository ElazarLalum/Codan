#include <Servo.h>

Servo servo;

void setup() {
  // put your setup code here, to run once:
  pinMode(4,INPUT_PULLUP);
  pinMode(5,OUTPUT);
  servo.attach(3);
  servo.write(27);
  digitalWrite(5,0);
}

void loop() {
  // put your main code here, to run repeatedly:
  int input = digitalRead(4);
  if(input){
    digitalWrite(5,1);
    delay(150);
    servo.write(162);
    delay(4000);
    servo.write(27);
    delay(250);
    digitalWrite(5,0);
  }
  delay(20);
}
