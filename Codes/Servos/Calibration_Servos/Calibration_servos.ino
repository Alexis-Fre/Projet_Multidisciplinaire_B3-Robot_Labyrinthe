#include <Servo.h>
Servo myServo;
void setup() {
  myServo.attach(9);
  myServo.writeMicroseconds(1500); // Signal d'arrêt standard
}
void loop(){
  myServo.writeMicroseconds(1500);
  delay(1000);
}