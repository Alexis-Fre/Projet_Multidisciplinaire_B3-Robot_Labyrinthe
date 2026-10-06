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
  
  // float lux = capteur_1.readLux(VL6180X_ALS_GAIN_5); // Lecture de la lumière mais permet de mesurer la distance aussi. 
                                                        // Il faut une valeur de gain faible pour une meilleur estimation de la distance avec une luminosité haute et inversement
  // Variable de gain :
  // VL6180X_ALS_GAIN_1
  // VL6180X_ALS_GAIN_1_25
  // VL6180X_ALS_GAIN_1_67
  // VL6180X_ALS_GAIN_2_5
  // VL6180X_ALS_GAIN_5
  // VL6180X_ALS_GAIN_10
  // VL6180X_ALS_GAIN_20
  // VL6180X_ALS_GAIN_40

  // Serial.print("Lux: "); Serial.println(lux);
  
  // uint8_t range = capteur_1.readRange(); // Lecture de la distance (entre 0 et 200)
  // uint8_t status = capteur_1.readRangeStatus(); // Permet d'avoir des erreurs de déjà renseigné

  // if (status == VL6180X_ERROR_NONE) {
  //  Serial.print("Range: "); Serial.println(range);
  // }

  // Some error occurred, print it out!
  
  // if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
  //   Serial.println("System error");
  // }
  // else if (status == VL6180X_ERROR_ECEFAIL) {
  //   Serial.println("ECE failure");
  // }
  // else if (status == VL6180X_ERROR_NOCONVERGE) {
  //   Serial.println("No convergence");
  // }
  // else if (status == VL6180X_ERROR_RANGEIGNORE) {
  //   Serial.println("Ignoring range");
  // }
  // else if (status == VL6180X_ERROR_SNR) {
  //   Serial.println("Signal/Noise error");
  // }
  // else if (status == VL6180X_ERROR_RAWUFLOW) {
  //   Serial.println("Raw reading underflow");
  // }
  // else if (status == VL6180X_ERROR_RAWOFLOW) {
  //   Serial.println("Raw reading overflow");
  // }
  // else if (status == VL6180X_ERROR_RANGEUFLOW) {
  //   Serial.println("Range reading underflow");
  // }
  // else if (status == VL6180X_ERROR_RANGEOFLOW) {
  //   Serial.println("Range reading overflow");
  // }
  // delay(50);
}
