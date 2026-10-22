#ifndef MOTEURS_H
#define MOTEURS_H

#include <Servo.h>

Servo servoGauche;
Servo servoDroite;

#define SIGNAL_ARRET 1500

void init_moteurs() {
  servoGauche.attach(10);
  servoDroite.attach(9);
  servoGauche.writeMicroseconds(SIGNAL_ARRET);
  servoDroite.writeMicroseconds(SIGNAL_ARRET);
}

void arreter() {
  servoGauche.writeMicroseconds(SIGNAL_ARRET);
  servoDroite.writeMicroseconds(SIGNAL_ARRET);
}

void avancer(bool correction_gauche = false, bool correction_droite = false) {
  if (correction_gauche) {
    // CORRECTION DOUCE vers la gauche (on ralentit la roue gauche)
    servoGauche.writeMicroseconds(1550); 
    servoDroite.writeMicroseconds(1420);
  } 
  else if (correction_droite) {
    // CORRECTION DOUCE vers la droite (on ralentit la roue droite)
    // On conserve la nouvelle puissance augmentée de la roue gauche
    servoGauche.writeMicroseconds(1600);
    servoDroite.writeMicroseconds(1450); 
  } 
  else {
    // Ligne droite : Roue gauche boostée à 1600 pour contrer la dérive à gauche
    servoDroite.writeMicroseconds(1420); 
    servoGauche.writeMicroseconds(1600); 
  }
}

void demarrer_tourner_droite() {
  servoGauche.writeMicroseconds(1550);
  servoDroite.writeMicroseconds(1550);
}

void demarrer_tourner_gauche() {
  servoGauche.writeMicroseconds(1450);
  servoDroite.writeMicroseconds(1450);
}

#endif