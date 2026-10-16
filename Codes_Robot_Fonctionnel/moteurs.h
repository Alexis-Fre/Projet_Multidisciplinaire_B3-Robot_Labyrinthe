#ifndef MOTEURS_H
#define MOTEURS_H

#include <Servo.h>

Servo servoGauche;
Servo servoDroite;

// Temps de rotation calibrés (ms)
#define TEMPS_ROTATION_DROITE 325
#define TEMPS_ROTATION_GAUCHE 470
#define TEMPS_DEMI_TOUR       650

// Signal d'arrêt absolu
#define SIGNAL_ARRET 1500

void init_moteurs() {
  servoGauche.attach(10); // Pin PWM servo gauche
  servoDroite.attach(9);  // Pin PWM servo droite

  // Arrêt parfait au démarrage (1500 µs)
  servoGauche.writeMicroseconds(SIGNAL_ARRET);
  servoDroite.writeMicroseconds(SIGNAL_ARRET);
}

void arreter() {
  servoGauche.writeMicroseconds(SIGNAL_ARRET);
  servoDroite.writeMicroseconds(SIGNAL_ARRET);
}

void avancer(bool correction_gauche = false, bool correction_droite = false) {
  if (correction_gauche) {
    // Ralentit la roue gauche (se rapproche de 1500 µs)
    servoGauche.writeMicroseconds(1560); 
    servoDroite.writeMicroseconds(1400);
  } 
  else if (correction_droite) {
    servoGauche.writeMicroseconds(1600);
    // Ralentit la roue droite (se rapproche de 1500 µs)
    servoDroite.writeMicroseconds(1460); 
  } 
  else {
    // Vitesse lente et constante
    // Droite < 1500 µs = avance | Gauche > 1500 µs = avance
    servoDroite.writeMicroseconds(1400); 
    servoGauche.writeMicroseconds(1600); 
  }
}

void tourner_droite() {
  servoGauche.writeMicroseconds(1600); // Gauche avance
  servoDroite.writeMicroseconds(1600); // Droite recule
  delay(TEMPS_ROTATION_DROITE);
  arreter();
}

void tourner_gauche() {
  servoGauche.writeMicroseconds(1400); // Gauche recule
  servoDroite.writeMicroseconds(1400); // Droite avance
  delay(TEMPS_ROTATION_GAUCHE);
  arreter();
}

void demi_tour() {
  servoGauche.writeMicroseconds(1600); // Pivote sur place
  servoDroite.writeMicroseconds(1600);
  delay(TEMPS_DEMI_TOUR);
  arreter();
}

#endif