#include <Wire.h>  // Bibliothèque I2C
#include "Adafruit_VL6180X.h"  // Bibliothèque / Driver pour le capteur de distance / lumière

#define nb_capteurs 3

// Tableau pour déclarer les objet I2C des 3 capteurs
Adafruit_VL6180X capteurs[nb_capteurs] = {
  Adafruit_VL6180X(),
  Adafruit_VL6180X(),
  Adafruit_VL6180X()
};


// Broches CE des capteur sur l'arduino
const int PIN_CE[nb_capteurs] = { 2, 3, 4 };

// Adresse I2C attribuer
const uint8_t ADRESSES_I2C[nb_capteurs] = { 0x30, 0x31, 0x32 };

// Stockage des données des capteurs
struct DonneesCapteur {
  float lux;
  uint8_t distance;
  uint8_t statut;
};

// Tableau de stockage global
DonneesCapteur valeurs_capteurs[nb_capteurs];

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    delay(1);
  }

  // Éteindre les capteur pour après configurer leurs adresses
  for (int i = 0 ; i < nb_capteurs ; i++) {
    pinMode(PIN_CE[i], OUTPUT);
    digitalWrite(PIN_CE[i], LOW);
  }
  delay(10);

  // configurer les adresses I2C des capteurs
  for (int i = 0; i < nb_capteurs; i++) {
    digitalWrite(PIN_CE[i], HIGH);
    delay(100);

    if (!capteurs[i].begin()) { // Tester si le capteur répond
      Serial.print("Capteur "); Serial.print(i + 1);
      Serial.println(" NON DETECTÉ ! ");
    } else {
       Serial.print("Capteur "); Serial.print(i + 1);
       Serial.println(" DETECTÉ ! Changement de l'adresse I2C...");
       capteurs[i].setAddress(ADRESSES_I2C[i]); // Changer l'adresse I2C
    }
  }
}

void loop() {
  // Met à jour les données des 3 capteurs
    lire_capteurs(VL6180X_ALS_GAIN_5);

    // Affichage des résultats pour le moniteur série
    for (int i = 0; i < nb_capteurs; i++) {
      Serial.print("--- Capteur "); Serial.print(i + 1); Serial.println(" ---");
      
      if (valeurs_capteurs[i].statut == VL6180X_ERROR_NONE) {
        Serial.print("Distance: "); Serial.print(valeurs_capteurs[i].distance); Serial.println(" mm");
        Serial.print("Lux: "); Serial.println(valeurs_capteurs[i].lux);
      } else {
        Serial.print("Erreur de lecture. Code : "); 
        Serial.println(valeurs_capteurs[i].statut);
        if  ((valeurs_capteurs[i].statut >= VL6180X_ERROR_SYSERR_1) && (valeurs_capteurs[i].statut <= VL6180X_ERROR_SYSERR_5)) {
          Serial.println("System error");
        }
        else if (valeurs_capteurs[i].statut == VL6180X_ERROR_ECEFAIL) {
          Serial.println("ECE failure");
        }
        else if (valeurs_capteurs[i].statut == VL6180X_ERROR_NOCONVERGE) {
          Serial.println("No convergence");
        }
        else if (valeurs_capteurs[i].statut == VL6180X_ERROR_RANGEIGNORE) {
          Serial.println("Ignoring range");
        }
        else if (valeurs_capteurs[i].statut == VL6180X_ERROR_SNR) {
          Serial.println("Signal/Noise error");
        }
        else if (valeurs_capteurs[i].statut == VL6180X_ERROR_RAWUFLOW) {
          Serial.println("Raw reading underflow");
        }
        else if (valeurs_capteurs[i].statut == VL6180X_ERROR_RAWOFLOW) {
          Serial.println("Raw reading overflow");
        }
        else if (valeurs_capteurs[i].statut == VL6180X_ERROR_RANGEUFLOW) {
          Serial.println("Range reading underflow");
        }
        else if (valeurs_capteurs[i].statut == VL6180X_ERROR_RANGEOFLOW) {
          Serial.println("Range reading overflow");
        }
      }
    }
    
    Serial.println();
    delay(150); // Pause pour éviter de saturer le bus I2C
}

void lire_capteurs(uint8_t gain_lux) {
  for (int i = 0; i < nb_capteurs; i++) {
    // 1. Lecture de la luminosité avec le gain choisi
    valeurs_capteurs[i].lux = capteurs[i].readLux(gain_lux);
    
    // 2. Lecture de la distance
    uint8_t distance_brute = capteurs[i].readRange();

    // 3. Lecture du statut (erreurs éventuelles)
    valeurs_capteurs[i].statut = capteurs[i].readRangeStatus();
    
    // 4. Lecture de la distance uniquement s'il n'y a pas d'erreur
    if (valeurs_capteurs[i].statut == VL6180X_ERROR_NONE) {
      valeurs_capteurs[i].distance = distance_brute;
    } else {
      // Valeur arbitraire de sécurité (ex: 255) pour indiquer 
      // à l'algorithme qu'il manque une donnée de distance.
      valeurs_capteurs[i].distance = 255; 
    }
  }
}

