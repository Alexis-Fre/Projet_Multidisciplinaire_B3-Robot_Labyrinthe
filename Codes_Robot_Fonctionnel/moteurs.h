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
    // CORRECTION DOUCE : On ralentit un peu la roue gauche (1570 au lieu de 1650)
    // La roue droite continue de pousser fort (1380)
    servoGauche.writeMicroseconds(1570); 
    servoDroite.writeMicroseconds(1380);
  } 
  else if (correction_droite) {
    // CORRECTION DOUCE : On ralentit un peu la roue droite (1430 au lieu de 1380)
    servoGauche.writeMicroseconds(1650);
    servoDroite.writeMicroseconds(1430); 
  } 
  else {
    // VITESSE V2 : Augmentation d'environ 40-50µs par rapport à la V1
    // (L'écart compensatoire pour rouler droit est conservé)
    servoDroite.writeMicroseconds(1380); 
    servoGauche.writeMicroseconds(1650); 
  }
}

void demarrer_tourner_droite() {
  // Rotations accélérées pour dégager les murs plus vite
  servoGauche.writeMicroseconds(1580);
  servoDroite.writeMicroseconds(1580);
}

void demarrer_tourner_gauche() {
  // Rotations accélérées pour dégager les murs plus vite
  servoGauche.writeMicroseconds(1420);
  servoDroite.writeMicroseconds(1420);
}

#endif