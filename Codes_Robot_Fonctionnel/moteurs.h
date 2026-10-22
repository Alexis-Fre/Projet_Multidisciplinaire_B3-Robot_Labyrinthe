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
    servoGauche.writeMicroseconds(1620); 
    servoDroite.writeMicroseconds(1150);
  } 
  else if (correction_droite) {
    servoGauche.writeMicroseconds(1900);
    servoDroite.writeMicroseconds(1380); 
  } 
  else {
    servoDroite.writeMicroseconds(1150); 
    servoGauche.writeMicroseconds(1900); 
  }
}

void demarrer_tourner_droite() {
  // Couple réaugmenté pour un pivot plus net sans frottement sur l'angle
  servoGauche.writeMicroseconds(1580); 
  servoDroite.writeMicroseconds(1580); 
}

void demarrer_tourner_gauche() {
  servoGauche.writeMicroseconds(1420); 
  servoDroite.writeMicroseconds(1420); 
}

#endif