#ifndef MOTEURS_H
#define MOTEURS_H

#include <Servo.h>

Servo servoGauche;
Servo servoDroite;

// Valeur en millisecondes pour faire un virage de 90° (À CALIBRER PHYSIQUEMENT)
#define TEMPS_ROTATION_90 500
#define TEMPS_DEMI_TOUR 1000

void init_moteurs() {
  servoGauche.attach(10); // Définir le Pin par servo
  servoDroite.attach(9);  // Définir le Pin par servo

  // Arrêt par défaut au démarrage
  servoGauche.write(90);
  servoDroite.write(90);
}

void avancer(bool correction_gauche = false, bool correction_droite = false) {
  if (correction_gauche) {
    servoGauche.write(170); // Ralentit légèrement la roue gauche
    servoDroite.write(0);
  } else if (correction_droite) {
    servoGauche.write(180);
    servoDroite.write(10); // Ralentit légèrement la roue droite
  } else {
    servoDroite.write(0); // Tourne à la vitesse maximale (vers l'avant)
    servoGauche.write(
        180); // Valeur opposée car les moteurs sont montés inversés
  }
}

void arreter() {
  servoGauche.write(90);
  servoDroite.write(90);
}

void tourner_droite() {
  servoGauche.write(180); // Gauche avance
  servoDroite.write(180); // Droite recule
  delay(TEMPS_ROTATION_90);
  arreter();
}

void tourner_gauche() {
  servoGauche.write(0); // Gauche recule
  servoDroite.write(0); // Droite avance
  delay(TEMPS_ROTATION_90);
  arreter();
}

void demi_tour() {
  servoGauche.write(180);
  servoDroite.write(180);
  delay(TEMPS_DEMI_TOUR);
  arreter();
}

#endif
