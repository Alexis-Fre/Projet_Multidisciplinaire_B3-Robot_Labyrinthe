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
    // La roue droite est à 1330 (rapide), on ralentit la gauche pour corriger
    servoGauche.writeMicroseconds(1570); 
    servoDroite.writeMicroseconds(1330);
  } 
  else if (correction_droite) {
    // La roue gauche est à 1700 (rapide), on ralentit la droite pour corriger
    servoGauche.writeMicroseconds(1700);
    servoDroite.writeMicroseconds(1430); 
  } 
  else {
    // LIGNE DROITE V2 RAPIDE : 
    // Écart proportionnel pour contrer la dérive (Gauche pousse plus fort)
    servoDroite.writeMicroseconds(1330); 
    servoGauche.writeMicroseconds(1700); 
  }
}

void demarrer_tourner_droite() {
  servoGauche.writeMicroseconds(1560); 
  servoDroite.writeMicroseconds(1560); 
}

void demarrer_tourner_gauche() {
  servoGauche.writeMicroseconds(1440); 
  servoDroite.writeMicroseconds(1440); 
}

#endif