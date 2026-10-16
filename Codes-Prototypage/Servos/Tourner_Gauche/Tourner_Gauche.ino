#include <Servo.h>

Servo servoGauche;
Servo servoDroite;

// --- VALEUR À AJUSTER ---
#define TEMPS_ROTATION_TEST 470
// ------------------------

void setup() {
  servoGauche.attach(10);
  servoDroite.attach(9);
  
  // Arrêt initial
  servoGauche.writeMicroseconds(1500) ;
  servoDroite.writeMicroseconds(1500) ;
  delay(2000); // Laisse 2 secondes pour poser le robot par terre
  
  // Exécution du virage à gauche
  servoGauche.writeMicroseconds(1400) ; // Gauche recule doucement
  servoDroite.writeMicroseconds(1400) ; // Droite avance doucement
  delay(TEMPS_ROTATION_TEST);
  
  // Arrêt définitif
  servoGauche.writeMicroseconds(1500) ;
  servoDroite.writeMicroseconds(1500) ;
}

void loop() {
  // On ne fait rien en boucle
}