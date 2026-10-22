#include <Wire.h>
#include "Adafruit_VL6180X.h"
#include "moteurs.h"

#define nb_capteurs 3
#define SEUIL_MUR 80 

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
  TOURNER_GAUCHE
};

EtatRobot etatActuel = AVANCER;
unsigned long tempsDebutVirage = 0;

unsigned long derniereLectureCapteurs = 0;
const unsigned long INTERVALLE_CAPTEURS = 30; 

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

  // On effectue les lectures I2C dans TOUS les états, toutes les 30ms
  if (tempsActuel - derniereLectureCapteurs >= INTERVALLE_CAPTEURS) {
    derniereLectureCapteurs = tempsActuel;
    lire_capteurs();

    uint8_t dist_gauche = valeurs_capteurs[CAPTEUR_GAUCHE].distance;
    uint8_t dist_face   = valeurs_capteurs[CAPTEUR_FACE].distance;
    uint8_t dist_droite = valeurs_capteurs[CAPTEUR_DROITE].distance;

    switch (etatActuel) {

      case AVANCER:
        {
          Serial.print("G: "); Serial.print(dist_gauche);
          Serial.print(" | F: "); Serial.print(dist_face);
          Serial.print(" | D: "); Serial.println(dist_droite);

          if (dist_face < SEUIL_MUR) {
            arreter();
            
            if (dist_droite > dist_gauche) {
              Serial.println("ACTION : DEBUT VIRAGE DROITE");
              demarrer_tourner_droite();
              tempsDebutVirage = tempsActuel;
              etatActuel = TOURNER_DROITE;
            } else {
              Serial.println("ACTION : DEBUT VIRAGE GAUCHE");
              demarrer_tourner_gauche();
              tempsDebutVirage = tempsActuel;
              etatActuel = TOURNER_GAUCHE;
            }
          } 
          else {
            if (dist_droite < 45) {
              avancer(true, false);  // Correction gauche
            } else if (dist_gauche < 45) {
              avancer(false, true);  // Correction droite
            } else {
              avancer(false, false); // Tout droit
            }
          }
          break;
        }

      case TOURNER_DROITE:
        {
          // Affiche ce que voit le capteur avant PENDANT qu'il tourne
          Serial.print("SCAN VIRAGE DROITE - Face : "); Serial.println(dist_face);
          
          // Condition de fin : on a tourné au moins 300ms (pour dégager le mur) 
          // ET la voie s'est libérée devant (> 150mm)
          if (tempsActuel - tempsDebutVirage > 300 && dist_face > 150) {
            Serial.println("ALIGNEMENT REUSSI -> AVANCER");
            arreter();
            etatActuel = AVANCER;
          }
          break;
        }

      case TOURNER_GAUCHE:
        {
          Serial.print("SCAN VIRAGE GAUCHE - Face : "); Serial.println(dist_face);
          
          if (tempsActuel - tempsDebutVirage > 300 && dist_face > 150) {
            Serial.println("ALIGNEMENT REUSSI -> AVANCER");
            arreter();
            etatActuel = AVANCER;
          }
          break;
        }
    }
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