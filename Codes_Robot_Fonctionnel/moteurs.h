#ifndef MOTEURS_H
#define MOTEURS_H

#include <Servo.h>

Servo servoGauche;
Servo servoDroite;

// Temps de rotation ajustés pour la vitesse adoucie (à affiner au besoin)
#define TEMPS_ROTATION_DROITE 600
#define TEMPS_ROTATION_GAUCHE 850
#define TEMPS_DEMI_TOUR       1200

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
    // Le robot est trop à droite, on veut aller à gauche.
    // On ralentit FORTEMENT la roue gauche (presque à l'arrêt à 1510)
    servoGauche.writeMicroseconds(1510); 
    servoDroite.writeMicroseconds(1420);
  } 
  else if (correction_droite) {
    // Le robot est trop à gauche, on veut aller à droite.
    // On ralentit FORTEMENT la roue droite (presque à l'arrêt à 1490)
    servoGauche.writeMicroseconds(1630);
    servoDroite.writeMicroseconds(1490); 
  } 
  else {
    // Ordre d'aller "tout droit" (même si c'est imparfait mécaniquement)
    servoDroite.writeMicroseconds(1420); 
    servoGauche.writeMicroseconds(1580); 
  }
}

// Virages et demi-tours adoucis (impulsion réduite à +/- 50 µs du point neutre)
void demarrer_tourner_droite() {
  servoGauche.writeMicroseconds(1550); // Gauche avance doucement
  servoDroite.writeMicroseconds(1550); // Droite recule doucement
}

void demarrer_tourner_gauche() {
  servoGauche.writeMicroseconds(1450); // Gauche recule doucement
  servoDroite.writeMicroseconds(1450); // Droite avance doucement
}

void demarrer_demi_tour(bool vers_droite = true) {
  if (vers_droite) {
    servoGauche.writeMicroseconds(1550);
    servoDroite.writeMicroseconds(1550);
  } else {
    servoGauche.writeMicroseconds(1450);
    servoDroite.writeMicroseconds(1450);
  }
}

#endif