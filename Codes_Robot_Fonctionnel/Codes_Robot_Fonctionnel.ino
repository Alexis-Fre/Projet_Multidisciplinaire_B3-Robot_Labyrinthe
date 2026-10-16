#include <Wire.h>
#include "Adafruit_VL6180X.h"
#include "moteurs.h"

#define nb_capteurs 3
#define SEUIL_MUR 100

#define CAPTEUR_GAUCHE 2
#define CAPTEUR_FACE   1
#define CAPTEUR_DROITE 0

Adafruit_VL6180X capteurs[nb_capteurs] = {
  Adafruit_VL6180X(), Adafruit_VL6180X(), Adafruit_VL6180X()
};

const int PIN_CE[nb_capteurs] = { 2, 3, 4 };
const uint8_t ADRESSES_I2C[nb_capteurs] = { 0x30, 0x31, 0x32 };

struct DonneesCapteur {
  uint8_t distance;
  uint8_t statut;
};

DonneesCapteur valeurs_capteurs[nb_capteurs];

enum EtatRobot {
  AVANCER,
  TOURNER_DROITE,
  TOURNER_GAUCHE,
  DEMI_TOUR
};

EtatRobot etatActuel = AVANCER;
unsigned long tempsDebutVirage = 0;

// Gestion du temps pour cadencer les mesures I2C sans bloquer
unsigned long derniereLectureCapteurs = 0;
const unsigned long INTERVALLE_CAPTEURS = 30; // Lecture toutes les 30 ms

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(1);

  init_moteurs();

  for (int i = 0; i < nb_capteurs; i++) {
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
  arreter();
  delay(2000);
}

void loop() {
  unsigned long tempsActuel = millis();

  switch (etatActuel) {

    case AVANCER:
      {
        // On ne lit les capteurs que toutes les 30ms pour ne pas saturer l'I2C
        if (tempsActuel - derniereLectureCapteurs >= INTERVALLE_CAPTEURS) {
          derniereLectureCapteurs = tempsActuel;

          lire_capteurs();

          uint8_t dist_gauche = valeurs_capteurs[CAPTEUR_GAUCHE].distance;
          uint8_t dist_face   = valeurs_capteurs[CAPTEUR_FACE].distance;
          uint8_t dist_droite = valeurs_capteurs[CAPTEUR_DROITE].distance;

          Serial.print("G: "); Serial.print(dist_gauche);
          Serial.print(" | F: "); Serial.print(dist_face);
          Serial.print(" | D: "); Serial.println(dist_droite);

          if (dist_face < SEUIL_MUR) {
            arreter();

            if (dist_droite > SEUIL_MUR && dist_droite > dist_gauche) {
              Serial.println("ACTION : TOURNER A DROITE");
              demarrer_tourner_droite();
              tempsDebutVirage = tempsActuel;
              etatActuel = TOURNER_DROITE;
            } else if (dist_gauche > SEUIL_MUR) {
              Serial.println("ACTION : TOURNER A GAUCHE");
              demarrer_tourner_gauche();
              tempsDebutVirage = tempsActuel;
              etatActuel = TOURNER_GAUCHE;
            } else if (dist_droite > dist_gauche + 25) {
              Serial.println("ACTION : VIRAGE SERRE A DROITE");
              demarrer_tourner_droite();
              tempsDebutVirage = tempsActuel;
              etatActuel = TOURNER_DROITE;
            } else if (dist_gauche > dist_droite + 25) {
              Serial.println("ACTION : VIRAGE SERRE A GAUCHE");
              demarrer_tourner_gauche();
              tempsDebutVirage = tempsActuel;
              etatActuel = TOURNER_GAUCHE;
            } else {
              Serial.println("ACTION : VRAI CUL-DE-SAC -> DEMI-TOUR");
              bool sensDroite = (dist_droite >= dist_gauche);
              demarrer_demi_tour(sensDroite);
              tempsDebutVirage = tempsActuel;
              etatActuel = DEMI_TOUR;
            }
          } 
          else {
            // --- BLOC RETROUVÉ : ORDRE D'AVANCER ---
            if (dist_droite < 50) {
              avancer(true, false);  // Correction gauche
            } else if (dist_gauche < 50) {
              avancer(false, true);  // Correction droite
            } else {
              avancer(false, false); // Tout droit
            }
          }
        }
        break;
      }

    case TOURNER_DROITE:
      if (tempsActuel - tempsDebutVirage >= TEMPS_ROTATION_DROITE) {
        arreter();
        etatActuel = AVANCER;
      }
      break;

    case TOURNER_GAUCHE:
      if (tempsActuel - tempsDebutVirage >= TEMPS_ROTATION_GAUCHE) {
        arreter();
        etatActuel = AVANCER;
      }
      break;

    case DEMI_TOUR:
      if (tempsActuel - tempsDebutVirage >= TEMPS_DEMI_TOUR) {
        arreter();
        etatActuel = AVANCER;
      }
      break;
  }
}

void lire_capteurs() {
  for (int i = 0; i < nb_capteurs; i++) {
    uint8_t distance_brute = capteurs[i].readRange();
    valeurs_capteurs[i].statut = capteurs[i].readRangeStatus();

    if (valeurs_capteurs[i].statut == VL6180X_ERROR_NONE) {
      valeurs_capteurs[i].distance = distance_brute;
    } else {
      valeurs_capteurs[i].distance = 255;
    }
  }
}