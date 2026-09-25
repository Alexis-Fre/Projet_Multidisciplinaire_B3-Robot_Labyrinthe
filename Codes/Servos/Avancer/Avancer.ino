#include <Servo.h>

Servo servoGauche;  // create Servo object to control a servo
Servo servoDroite;

void setup() {
  servoGauche.attach(10);  // attaches the servo on pin 9 to the Servo object
  servoDroite.attach(9);
  Serial.begin(9600);
}

void loop() {
  servoDroite.write(0);
  servoGauche.write(180);
}
