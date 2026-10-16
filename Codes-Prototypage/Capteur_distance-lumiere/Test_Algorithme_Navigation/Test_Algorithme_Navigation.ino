#include <Wire.h>
#include "Adafruit_VL6180X.h"

#define nb_capteurs 3
#define SEUIL_MUR 50 // Distance en mm en dessous de laquelle on considère qu'il y a un mur

// Index des capteurs basé sur ton schéma de positionnement
#define CAPTEUR_GAUCHE 2
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

  // Éteindre les capteurs
  for (int i = 0 ; i < nb_capteurs ; i++) {
    pinMode(PIN_CE[i], OUTPUT);
    digitalWrite(PIN_CE[i], LOW);
  }
  delay(10);

  // Initialisation et changement d'adresses
  for (int i = 0; i < nb_capteurs; i++) {
    digitalWrite(PIN_CE[i], HIGH);
    delay(100);
    if (capteurs[i].begin()) {
       capteurs[i].setAddress(ADRESSES_I2C[i]);
    }
  }
  Serial.println("Initialisation terminee. Debut de la simulation...");
}

void loop() {
  lire_capteurs(VL6180X_ALS_GAIN_5);

  // Extraction des distances pour alléger la lecture des conditions
  uint8_t dist_gauche = valeurs_capteurs[CAPTEUR_GAUCHE].distance;
  uint8_t dist_face   = valeurs_capteurs[CAPTEUR_FACE].distance;
  uint8_t dist_droite = valeurs_capteurs[CAPTEUR_DROITE].distance;

  // Affichage compact sur une seule ligne
  Serial.print("G: "); Serial.print(dist_gauche);
  Serial.print(" mm | F: "); Serial.print(dist_face);
  Serial.print(" mm | D: "); Serial.print(dist_droite);
  Serial.print(" mm  ==>  ACTION : ");

  // --- LOGIQUE DE NAVIGATION ---
  
  if (dist_face < SEUIL_MUR) {
    // Il y a un mur juste en face, le robot doit tourner
    if (dist_droite > SEUIL_MUR) {
      Serial.println("TOURNER A DROITE");
    } 
    else if (dist_gauche > SEUIL_MUR) {
      Serial.println("TOURNER A GAUCHE");
    } 
    else {
      Serial.println("CUL-DE-SAC -> DEMI-TOUR");
    }
  } 
  else {
    // La voie est libre en face
    // Ajout d'une correction de trajectoire si on dévie trop près d'un mur latéral
    if (dist_droite < 40) {
      Serial.println("AVANCER (Correction legere vers la gauche)");
    }
    else if (dist_gauche < 40) {
      Serial.println("AVANCER (Correction legere vers la droite)");
    }
    else {
      Serial.println("AVANCER TOUT DROIT");
    }
  }

  delay(400); // Ralentir la boucle pour faciliter la lecture du moniteur série
}

void lire_capteurs(uint8_t gain_lux) {
  for (int i = 0; i < nb_capteurs; i++) {
    valeurs_capteurs[i].lux = capteurs[i].readLux(gain_lux);
    uint8_t distance_brute = capteurs[i].readRange();
    valeurs_capteurs[i].statut = capteurs[i].readRangeStatus();
    
    if (valeurs_capteurs[i].statut == VL6180X_ERROR_NONE) {
      valeurs_capteurs[i].distance = distance_brute;
    } else {
      // Si aucune détection (vide ou erreur), on met la distance au maximum (255)
      valeurs_capteurs[i].distance = 255; 
    }
  }
}
