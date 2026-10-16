#include <Servo.h>

Servo servoGauche;  // Créer un objet par servo (Gauche)
Servo servoDroite;  // Créer un objet par servo (Droite)

void setup() {
  servoGauche.attach(10);  // Définir le Pin par servo
  servoDroite.attach(9);   // Définir le Pin par servo
  Serial.begin(9600);
}

/*
Informations sur les valeurs dans <servox>.write(angle)
La vitesse des servomotteur s'écris de façon angulaire avec le .write()
Plus la valeur approche 90° le moins vite le servo tourne
Donc 0° équivaut à tourner à la vitesse maximal dans un sens et 180° inversement
*/
void loop() {
  servoDroite.write(0); // Tourné à la vitesse maximal (vers l'avant)
  servoGauche.write(180); // La valeur est à l'opposé car les moteurs ne sont pas dans le même sens lorsqu'ils sont monté sur le chassis
}
