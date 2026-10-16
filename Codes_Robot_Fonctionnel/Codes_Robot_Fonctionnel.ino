#include <Wire.h>
#include "Adafruit_VL6180X.h"
#include "moteurs.h" // Inclusion de ton fichier de fonctions

#define nb_capteurs 3
#define SEUIL_MUR 100 // Distance en mm en dessous de laquelle on considère qu'il y a un mur

#define CAPTEUR_GAUCHE 2 // Index des capteurs basé sur le schéma
#define CAPTEUR_FACE   1
#define CAPTEUR_DROITE 0

Adafruit_VL6180X capteurs[nb_capteurs] = {
  Adafruit_VL6180X(),
  Adafruit_VL6180X(),
  Adafruit_VL6180X()
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

  init_moteurs(); 

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
  Serial.println("Moteurs arreter");
  arreter();
  delay(5000); // Pause avant de démarrer les moteurs
}

void loop() {
  lire_capteurs(VL6180X_ALS_GAIN_1_67);

  uint8_t dist_gauche = valeurs_capteurs[CAPTEUR_GAUCHE].distance;
  uint8_t dist_face   = valeurs_capteurs[CAPTEUR_FACE].distance;
  uint8_t dist_droite = valeurs_capteurs[CAPTEUR_DROITE].distance;

  // Affichage des distances mesurées
  Serial.print("G: "); Serial.print(dist_gauche);
  Serial.print(" mm | F: "); Serial.print(dist_face);
  Serial.print(" mm | D: "); Serial.print(dist_droite);
  Serial.print(" mm  ==>  ACTION : ");

  // --- LOGIQUE DE NAVIGATION AVEC DÉBOGAGE ---
  if (dist_face < SEUIL_MUR) {
    arreter(); 
    delay(100); 

    if (dist_droite > SEUIL_MUR) {
      Serial.println("TOURNER A DROITE");
      tourner_droite();
    } 
    else if (dist_gauche > SEUIL_MUR) {
      Serial.println("TOURNER A GAUCHE");
      tourner_gauche();
    } 
    else {
      Serial.println("CUL-DE-SAC -> DEMI-TOUR");
      demi_tour();
    }
  } 
  else {
    // La voie est libre en face
    if (dist_droite < 40) {
      Serial.println("AVANCER (Correction legere vers la gauche)");
      avancer(true, false);  
    }
    else if (dist_gauche < 40) {
      Serial.println("AVANCER (Correction legere vers la droite)");
      avancer(false, true);  
    }
    else {
      Serial.println("AVANCER TOUT DROIT");
      avancer(false, false); 
    }
  }
  
  // Petite pause pour ne pas saturer le moniteur série et l'I2C
  delay(10);
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
