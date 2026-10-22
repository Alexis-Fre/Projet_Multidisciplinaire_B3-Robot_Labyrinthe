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
    // CORRECTION DOUCE vers la gauche (on ralentit juste un peu la roue gauche)
    servoGauche.writeMicroseconds(1550); // (Anciennement 1510)
    servoDroite.writeMicroseconds(1420);
  } 
  else if (correction_droite) {
    // CORRECTION DOUCE vers la droite (on ralentit juste un peu la roue droite)
    servoGauche.writeMicroseconds(1580);
    servoDroite.writeMicroseconds(1450); // (Anciennement 1490)
  } 
  else {
    // Ligne droite
    servoDroite.writeMicroseconds(1420); 
    servoGauche.writeMicroseconds(1580); 
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