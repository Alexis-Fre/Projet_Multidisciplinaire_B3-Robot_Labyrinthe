#include <Servo.h>

Servo servoGauche;  
Servo servoDroite;  

void setup() {
  servoGauche.attach(10);  
  servoDroite.attach(9);   
  Serial.begin(9600);
}

void loop() {
  // Avancer doucement en microsecondes
  servoDroite.writeMicroseconds(1400); // Droite avance
  servoGauche.writeMicroseconds(1600); // Gauche avance
}