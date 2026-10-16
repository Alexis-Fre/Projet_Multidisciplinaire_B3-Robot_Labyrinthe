#include <Wire.h>
#include "Adafruit_VL6180X.h"
#include "moteurs.h" // Inclusion de ton fichier de fonctions

#define nb_capteurs 3
#define SEUIL_MUR 50 // Distance en mm en dessous de laquelle on considère qu'il y a un mur

#define CAPTEUR_GAUCHE 2 // Index des capteurs basé sur le schéma
#define CAPTEUR_FACE   1
#define CAPTEUR_DROITE 0

Adafruit_VL6180X capteurs[nb_capteurs] = {
  Adafruit_VL6180X(), Adafruit_VL6180X(), Adafruit_VL6180X()
};

const int PIN_CE[nb_capteurs] = { 2, 3, 4 };
const uint8_t ADRESSES_I2C[nb_capteurs] = { 0x30, 0x31, 0x32 };

struct DonneesCapteur {
  float lux;
  uint8_t distance;
  uint8_t statut;
};

DonneesCapteur valeurs_capteurs[nb_capteurs];

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(1);

  init_moteurs(); // Appel de la fonction depuis moteurs.h

  for (int i = 0 ; i < nb_capteurs ; i++) {
    pinMode(PIN_CE[i], OUTPUT);
    digitalWrite(PIN_CE[i], LOW);
  }
  delay(10);

  for (int i = 0; i < nb_capteurs; i++) {
    digitalWrite(PIN_CE[i], HIGH);
    delay(100);
    if (capteurs[i].begin()) {
       capteurs[i].setAddress(ADRESSES_I2C[i]);
    }
  }
  Serial.println("Robot Pret !");
  delay(2000); // Pause avant de démarrer les moteurs
}

void loop() {
  lire_capteurs(VL6180X_ALS_GAIN_5);

  uint8_t dist_gauche = valeurs_capteurs[CAPTEUR_GAUCHE].distance;
  uint8_t dist_face   = valeurs_capteurs[CAPTEUR_FACE].distance;
  uint8_t dist_droite = valeurs_capteurs[CAPTEUR_DROITE].distance;

  // --- LOGIQUE DE NAVIGATION ---
  if (dist_face < SEUIL_MUR) {
    arreter(); // On stoppe le robot avant de tourner
    delay(100); 

    if (dist_droite > SEUIL_MUR) {
      tourner_droite();
    } 
    else if (dist_gauche > SEUIL_MUR) {
      tourner_gauche();
    } 
    else {
      demi_tour();
    }
  } 
  else {
    // La voie est libre en face
    if (dist_droite < 40) {
      avancer(true, false);  // Correction vers la gauche
    }
    else if (dist_gauche < 40) {
      avancer(false, true);  // Correction vers la droite
    }
    else {
      avancer(false, false); // Avancer tout droit
    }
  }
  
  // Petite pause pour ne pas saturer l'I2C pendant que le robot avance
  delay(50);
}

void lire_capteurs(uint8_t gain_lux) {
  for (int i = 0; i < nb_capteurs; i++) {
    valeurs_capteurs[i].lux = capteurs[i].readLux(gain_lux);
    uint8_t distance_brute = capteurs[i].readRange();
    valeurs_capteurs[i].statut = capteurs[i].readRangeStatus();
    
    if (valeurs_capteurs[i].statut == VL6180X_ERROR_NONE) {
      valeurs_capteurs[i].distance = distance_brute;
    } else {
      valeurs_capteurs[i].distance = 255; 
    }
  }
}
