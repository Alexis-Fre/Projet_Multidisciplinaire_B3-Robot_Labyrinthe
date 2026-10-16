#include <Servo.h>

Servo servoGauche;
Servo servoDroite;

// --- VALEUR À AJUSTER ---
#define TEMPS_ROTATION_TEST 325 
// ------------------------

void setup() {
  Serial.begin(9600);
  servoGauche.attach(10);
  servoDroite.attach(9);

  // ** En µs **
  // Arrêt initial
  Serial.println("Arrêt des moteurs");
  
  servoGauche.writeMicroseconds(1500);
  servoDroite.writeMicroseconds(1500);
  delay(2000); // Laisse 2 secondes pour poser le robot par terre
  
  //// Exécution du virage à droite
  Serial.println("Tourner à droite");
  servoGauche.writeMicroseconds(1600);
  servoDroite.writeMicroseconds(1600);
  delay(TEMPS_ROTATION_TEST);
  
  //// Arrêt définitif
  Serial.println("Arrêt définitif des moteurs");
  servoGauche.writeMicroseconds(1500);
  servoDroite.writeMicroseconds(1500);

}

void loop() {}