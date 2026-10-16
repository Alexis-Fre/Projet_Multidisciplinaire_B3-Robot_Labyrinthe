#include <Servo.h>

Servo myServo1;   // Assigner un objet Servo nommé myServo
Servo myServo2;

void setup() {
  Serial.begin(9600);
  myServo1.attach(9);   // Définir le Pin PWM du servo sur la carte (le Pin doit pouvoir faire du PWM qui est indiqué par `~` sur la carte
  myServo2.attach(10);
}
void loop(){
  myServo1.writeMicroseconds(1500);   // Signal d'arrêt standard, il faut donc une fois que ce programme tourne ajuster la visse à l'arrière du servo pour ne plus qu'il tourne
  myServo2.writeMicroseconds(1500);
  Serial.println("Arret");
  delay(1000);
}
