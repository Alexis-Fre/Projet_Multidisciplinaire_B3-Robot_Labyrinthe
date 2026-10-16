#include <Servo.h>

Servo servoGauche;
Servo servoDroite;

// --- VALEUR À AJUSTER ---
#define TEMPS_DEMI_TOUR_TEST 650 
// ------------------------

void setup() {
  servoGauche.attach(10);
  servoDroite.attach(9);
  
  // Arrêt initial
  servoGauche.writeMicroseconds(1500) ;
  servoDroite.writeMicroseconds(1500) ;
  delay(2000); // Laisse 2 secondes pour poser le robot par terre
  
  // Exécution du demi-tour (Rotation prolongée vers la droite)
  servoGauche.writeMicroseconds(1600) ; 
  servoDroite.writeMicroseconds(1600) ; 
  delay(TEMPS_DEMI_TOUR_TEST);
  
  // Arrêt définitif
  servoGauche.writeMicroseconds(1500) ;
  servoDroite.writeMicroseconds(1500) ;
}

void loop() {
  // On ne fait rien en boucle
}