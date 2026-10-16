#include <Servo.h>

Servo myServo;   // Assigner un objet Servo nommé myServo

void setup() {
  myServo.attach(9);   // Définir le Pin PWM du servo sur la carte (le Pin doit pouvoir faire du PWM qui est indiqué par `~` sur la carte
}
void loop(){
  myServo.writeMicroseconds(1500);   // Signal d'arrêt standard, il faut donc une fois que ce programme tourne ajuster la visse à l'arrière du servo pour ne plus qu'il tourne
  delay(1000);
}
